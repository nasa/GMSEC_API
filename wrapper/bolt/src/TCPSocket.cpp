/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <bolt/TCPSocket.h>
#include <bolt/Log.h>
#include <bolt/util.h>

#include <gmsec4/internal/SystemUtil.h>


#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>		// <cerrno> promises nothing
#include <fcntl.h>
#endif

#ifdef __sun
#include <string.h>		// FD_ZERO() using memset 
#endif 


namespace bolt {


static i32 getID()
{
	static i32 counter = 0;
	i32 id = ++counter;
	return id;
}


static int initialize();



TCPSocket::TCPSocket ()
	:
	handle(0),
	connected(false),
	error(0),
	m_nagle(false),
	m_tcpBufferSize(0)
{
	initialize();

	tag = "TCPSocket[" + util::toString(getID()) + "] ";

	Options o;
	setOptions(o);
}


TCPSocket::~TCPSocket ()
{
	disconnect();

#ifdef WIN32
	WSACleanup();
#endif
}


Result TCPSocket::setOptions(const Options &options)
{
	Result result;

	connect_timeout_ms = options.getI32(opt::CONNECT_TIMEOUT_ms, 5000);
	m_nagle            = options.getFlag(opt::NAGLE, false);
	m_tcpBufferSize    = options.getI32(opt::TCP_BUF_SIZE, 0);

	return result;
}


Result TCPSocket::connect (const char *host, int port)
{
	Result result;

	AutoMutex hold(readMutex);

	if (connected)
	{
#if 1
		// will not happen if properly managed, but a bug remains
		GMSEC_WARNING << tag.c_str() << "already open";
#else
		result.set(ERR_STATE, "already open");
		return result;
#endif
	}

	struct addrinfo hints = { 0 };
	struct addrinfo *pai = 0;

	int flags = 0;
	int q = 0;
	bool isConnected = false;
	bool blocking = true;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	string sport = util::toString(i32(port));

	int code = getaddrinfo(host, sport.c_str(), &hints, &pai);
	if (code || !pai)
	{
		string text(host);
		text += ": ";
		text += gai_strerror(code);
		result.set(ERR_CONNECT, text);
		goto cleanup;
	}

	if (!handle)
	{
		result = open();
		if (result.isError())
			goto cleanup;
	}

#ifdef WIN32

	WSAEVENT e = 0;
	
	if (connect_timeout_ms > 0)
		e = WSACreateEvent();

	if (e)
	{
		// WSAEventSelect has side-effect of making handle non-blocking
		if (WSAEventSelect(handle, e, FD_CONNECT))
		{
			setError();
			warning("connect: WSAEventSelect failed");
		}
		else
			blocking = false;
	}

#else

	if (connect_timeout_ms > 0)
	{
		flags = fcntl(handle, F_GETFL, 0);
		if (flags < 0)
		{
			// stuck using blocking connect
		}
		else
		{
			// set to non-blocking to support timeout
			q = fcntl(handle, F_SETFL, flags|O_NONBLOCK);
			if (q)
			{
				setError();
				warning("connect: set non-blocking");
			}
			else
				blocking = false;
		}
	}

#endif

	{
		q = ::connect(handle, pai->ai_addr, pai->ai_addrlen);
		if (q < 0)
		{
			if (blocking)
				setError(result);
			else
				setError();
			GMSEC_VERBOSE << tag.c_str() << "connect: " << errorString.c_str() << " [" << error << ']';
		}
		else
		{
			isConnected = true;
			GMSEC_DEBUG << tag.c_str() << "connect succeeded";
		}
	}

#ifdef WIN32

	if (!blocking)
	{
		if (!isConnected)
		{
			q = WSAWaitForMultipleEvents(1, &e, 0, connect_timeout_ms, 0);

			if (q == WSA_WAIT_EVENT_0)
			{
				WSANETWORKEVENTS ne;
				if (!WSAEnumNetworkEvents(handle, e, &ne))
				{
					if (ne.lNetworkEvents & FD_CONNECT)
					{
						int code = ne.iErrorCode[FD_CONNECT_BIT];
						if (code)
						{
							setError(code);
							result.set(error, errorString);
						}
						else
						{
							isConnected = true;
						}
					}
					else
					{
						GMSEC_WARNING << tag.c_str() << "missing FD_CONNECT event";
					}
				}
				else
				{
					setError(result);
					warning("WSAEnumNetworkEvents");
				}
			}
			else if (q == WSA_WAIT_FAILED)
			{
				setError(result);
			}
			else
			{
				result.set(ERR_CONNECT, "wait for connection failed");
			}
		}

		// return to blocking
		WSAEventSelect(handle, e, 0);
	}

	if (e)
		WSACloseEvent(e);

#else
	if (!blocking)
	{
		// wait for connection to complete (or fail)
		struct timeval tv = { 0 };
		tv.tv_sec = connect_timeout_ms / 1000;
		tv.tv_usec = (connect_timeout_ms % 1000) * 1000;

		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(handle, &fds);

		q = select(handle+1, NULL, &fds, NULL, &tv);
		if (q < 0)
		{
			result.set(ERR_CONNECT, "select error");
			goto cleanup;
		}

		if (q == 1)
		{
			int localError = 0;
			socklen_t len = sizeof(localError);
			q = getsockopt(handle, SOL_SOCKET, SO_ERROR, &localError, &len);
			std::string localErrorString = "socket error: ";
			if (localError)
			{
				localErrorString += util::toString(i32(localError));
				
				localErrorString.append(", ").append(strerror(localError));
				
				result.set(ERR_CONNECT, localErrorString);
				goto cleanup;
			}
			else
				isConnected = true;
		}
		else
		{
			result.set(ERR_CONNECT, "Unable to connect");
			goto cleanup;
		}

		q = fcntl(handle, F_SETFL, (flags & ~O_NONBLOCK));
		if (q)
		{
			setError();
			warning("connect: reset blocking");
		}
	}
#endif

	if (isConnected)
	{
		if (!m_nagle)
		{
			q = 1;
			if (setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*) &q, sizeof(q)) < 0)
			{
				GMSEC_WARNING << tag.c_str() << "connect: unable to disable Nagle";
			}
		}

		if (m_tcpBufferSize != 0)
		{
			if (setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char*) &m_tcpBufferSize, sizeof(m_tcpBufferSize)) < 0)
			{
				GMSEC_WARNING << "Failed to set socket send buffer size to " << m_tcpBufferSize;

				int       buf_size_set = 0;
				socklen_t set_size     = sizeof(m_tcpBufferSize);

				getsockopt(handle, SOL_SOCKET, SO_SNDBUF, (char*) &buf_size_set, &set_size);
				GMSEC_DEBUG << "Actual send buffer size is " << buf_size_set;
			}

			if (setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char*) &m_tcpBufferSize, sizeof(m_tcpBufferSize)) < 0)
			{
				GMSEC_WARNING << "Failed to set socket recv buffer size to " << m_tcpBufferSize;

				int       buf_size_set = 0;
				socklen_t set_size     = sizeof(m_tcpBufferSize);

				getsockopt(handle, SOL_SOCKET, SO_RCVBUF, (char*) &buf_size_set, &set_size);
				GMSEC_WARNING << "Actual recv buffer size is " << buf_size_set;
			}
		}
	}

cleanup:

	if (pai)
		freeaddrinfo(pai);

	if (result.isError())
	{
		GMSEC_VERBOSE << tag.c_str() << "connect failed: " << result;
		release();
	}
	else if (isConnected)
	{
		GMSEC_VERBOSE << tag.c_str() << "connection established";
		connected = true;
	}

	return result;
}


Result TCPSocket::open()
{
	Result result;

	if (handle)
	{
		result.set(ERR_STATE, "already have handle");
		return result;
	}

	handle_t tmp = socket(AF_INET, SOCK_STREAM, 0);
GMSEC_DEBUG << tag.c_str() << "open: socket(AF_INET, SOCK_STREAM, 0) => " << tmp;
	if (tmp < 0)
		return setError(result);

	handle = tmp;

	return result;
}


Result TCPSocket::release()
{
	Result result;

	if (!handle)
	{
		result.set(ERR_STATE, "no handle");
		return result;
	}

	handle_t tmp = handle;
	handle = 0;

#ifdef WIN32
#define CLOSE_FUNCTION closesocket
#else
#define CLOSE_FUNCTION close
#endif

	int code = CLOSE_FUNCTION(tmp);
	if (code)
		setError(result);

	GMSEC_DEBUG << tag.c_str() << "close(handle) => " << code;

	if (result.isError())
	{
		GMSEC_WARNING << tag.c_str() << "release: close*=" << result;
	}

	return result;
}


Result TCPSocket::disconnect()
{
	Result result;

	AutoMutex hold(readMutex);

	if (!connected)
	{
		result.set(ERR_STATE, "not open");
		return result;
	}

	connected = false;

	release();

	return result;
}


int TCPSocket::doRead (char *buffer, int count, int timeout_ms)
{
	struct timeval tv = { 0 };
	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec = (timeout_ms % 1000) * 1000;

	AutoMutex hold(readMutex);

	if (!connected)
	{
		GMSEC_VERBOSE << tag.c_str() << "doRead: not open";
		return -1;
	}

	// fdread, fderr can be properly initialized state
	fd_set fdread;
	fd_set fderr;

	FD_ZERO(&fdread);
	FD_ZERO(&fderr);

	FD_SET(handle, &fdread);
	FD_SET(handle, &fderr);

	int q = select(handle+1, &fdread, 0, &fderr, &tv);
	if (q == 0) // timeout
		return q;

	if (q < 0)
	{
		setError();
		GMSEC_WARNING << tag.c_str() << "doRead: select=" << q << '=' << errorString.c_str();
		return q;
	}

	if (FD_ISSET(handle, &fderr))
	{
		setError("error on fd", ERR_IO);
		GMSEC_WARNING << tag.c_str() << "doRead: handle=" << handle << '=' << errorString.c_str();
		return getError();
	}

	int read = 0;

	// if we are here, this should always be true...
	if (FD_ISSET(handle, &fdread))
	{
		int flags = 0;
		read = recv(handle, buffer, count, flags);
		if (read <= 0)
		{
			if (read == 0)
			{
				setError(ERR_IO, "closed");
				disconnect();
			}
			else
				setError();
			GMSEC_VERBOSE << tag.c_str() << "doRead: recv=" << read << '=' << errorString.c_str();
			return -getError();
		}
	}
	else
	{
		GMSEC_WARNING << tag.c_str() << "doRead: ready but not found";
	}

	return read;
}


int TCPSocket::write (const char *buffer, int count)
{
	int flags = 0;
	if (!handle)
	{
		GMSEC_ERROR << tag.c_str() << "write: handle=" << handle;
	}

	int wrote = -1;
	bool tryAgain = false;
	int maxTries = 10;
	do {
		--maxTries;

		// fdwrite, fderr can be properly initialized state
		fd_set fdwrite;
		fd_set fderr;

		FD_ZERO(&fdwrite);
		FD_ZERO(&fderr);

		FD_SET(handle, &fdwrite);
		FD_SET(handle, &fderr);

		struct timeval tv = { 0, 100000 };   // 100ms

		int q = select(handle+1, 0, &fdwrite, &fderr, &tv);

		if (q == 0) // timeout
		{
			GMSEC_WARNING << tag.c_str() << "write: select=" << q;
			tryAgain = true;
			continue;
		}

		if (q < 0)
		{
			setError();
			GMSEC_WARNING << tag.c_str() << "write: select=" << q << '=' << errorString.c_str();
			tryAgain = true;
			continue;
		}

		if (FD_ISSET(handle, &fderr))
		{
			setError("error on fd", ERR_IO);
			GMSEC_WARNING << tag.c_str() << "write: handle=" << handle << '=' << errorString.c_str();
			tryAgain = true;
			continue;
		}

		// if we are here, this should always be true...
		if (FD_ISSET(handle, &fdwrite))
		{
			wrote = send(handle, buffer, count, flags);

			if (wrote < 0) {
#ifdef WIN32
				tryAgain = (WSAGetLastError() == WSAEWOULDBLOCK);
#else
				tryAgain = (errno == EWOULDBLOCK);
#endif
				if (tryAgain) {
					GMSEC_WARNING << tag.c_str() << "write=" << wrote << " reason=EWOULDBLOCK";
				}
			}
		}
	} while (wrote < 0 && tryAgain && maxTries > 0);

	if (wrote < 0) {
		setError();
		GMSEC_WARNING << tag.c_str() << "write: send=" << wrote << '=' << errorString.c_str();
	}
	return wrote;
}


int TCPSocket::getError () const
{
	return error;
}


string TCPSocket::getErrorString () const
{
	return errorString;
}


int TCPSocket::setError (const char *text, int code)
{
	this->errorString = text;

	if (!code)
		code = 1;
	error = code;

	return getError();
}


int TCPSocket::setError (int localError, bool setString)
{
#ifdef WIN32
	if (!localError)
		localError = WSAGetLastError();
#else
	if (!localError)
		localError = errno;
#endif

	this->error = localError;

	if (setString)
	{
		gmsec::api::util::SystemUtil::getErrorString(localError, errorString);
	}

	return getError();
}


Result TCPSocket::setError (Result &result)
{
	setError();
	int code = getError();
	if (!code)
		code = 1;
	result.set(code, getErrorString());
	return result;
}


int initialize()
{
	int code = 0;

#ifdef WIN32

	// call WSAStartup on Windows
	// Note that this use is not consistent with
	// http://msdn.microsoft.com/en-us/library/ms742213(VS.85).aspx
	// which say to call WSAStartup outside any DLL.
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;

	// Initialize WinSock and check version
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0 || wsaData.wVersion != wVersionRequested)
	{
		code = 1;
		GMSEC_WARNING << "TCPSocket.initialize: WSAStartup failed";
	}

#else /* WIN32 */

	// ignore SIGPIPE on UNIXes

	struct sigaction tmp;
	util::zeroBytes(&tmp, sizeof(tmp));
	tmp.sa_handler = SIG_IGN;
	code = sigaction(SIGPIPE, &tmp, 0);
	if (code)
	{
		string s;
		gmsec::api::util::SystemUtil::getErrorString(code, s);
		GMSEC_WARNING << "TCPSocket.initialize: sigaction=" << code << '=' << s.c_str();
	}

#endif /* WIN32 */

	return code;
}


int TCPSocket::warning(const char *s)
{
	if (error)
	{
		GMSEC_WARNING << tag.c_str() << s << " => " << errorString.c_str() << " [" << error << ']';
	}
	return error;
}


} // namespace bolt

