/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "TCPSocket.h"

#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/util/Log.h>

#include <sstream>

#include <cerrno>
#include <cstring>   // for strerror


#if defined (WIN32)
	#include <process.h>
	#include <io.h>
#endif

#define UNIX_SOCKET_TIMEOUT_OR_BROKEN_PIPE 11
#define UNIX_SOCKET_TIMEOUT_OR_NOFILE 2

#define MAX_BUF 100

using namespace gmsec::api;
using namespace gmsec::api::util;

using namespace gmsec_messagebus;


TCPSocket::TCPSocket()
	:
	m_needModeChange(0),
	m_sockApiCallCount(0),
	m_eToWrite(0),
	m_eToRead(0),
	m_isDebug(0),
	m_pLen(0),
	m_sock(-1),
	m_pAddr(NULL),
	m_isConnected(false)
{
	//TODO: Constructor should accept a domain, so we can determine
	//      whether to define an IPv4 or IPv6 interface.
	//      For now, assume IPv4.

	m_pAddr = reinterpret_cast<SockAddr*>(new SockAddrIn);

	//m_pAddr.sin_port = 0;
	//m_pAddr.sin_family = 0;
}


TCPSocket::~TCPSocket()
{
	delete m_pAddr;
}


Status TCPSocket::disconnect()
{
	Status status;

	if (m_sock != -1)
	{
#ifdef WIN32
		closesocket(m_sock);
#else
		close(m_sock);
#endif
	}

	if (m_isDebug)
	{
		GMSEC_DEBUG << "Closed connection\n\n";
	}

	return status;
}


Status TCPSocket::read(char*& buffer, int& len)
{
	Status status;
	char   rcvBuff[MAX_BUF] = {0};
	int    rcvIndex = -1;
	int    inMsgSize;
	int    n;

	buffer = 0;
	len    = 0;

	AutoMutex hold(m_readMutex);

	// extract the message len from the header
	// start the loop that reads the header containing the length
	// of the data
	do
	{
		++rcvIndex;

		n = recv(m_sock, rcvBuff + rcvIndex, 1, 0);

		status = getLastSocketError();

		if (m_isDebug)
		{
			if (n > 0)
			{
				fprintf(stderr, "R: line: %d  sock: %d   n: %d  index: %d   data --:%c\n",
				    __LINE__, m_sock, n, rcvIndex, rcvBuff[rcvIndex]);
			}
		}

		if (m_eToRead && m_eToWrite)
		{
			n = 0;
		}

		if (n < 0)
		{
			m_eToRead = 1;
			return status;
		}
		else if (n == 0)
		{
			// unix sockets do not detect the difference between
			// a broken pipe and a timeout
			// on a read but return a value of 0 bytes
			// read as opposed to -1 when an error code is
			// detected
			m_eToRead = 1;
			status.set(NO_ERROR_CLASS, CUSTOM_ERROR_CODE, "Broken pipe or timeout", UNIX_SOCKET_TIMEOUT_OR_BROKEN_PIPE);
			return status;
		}

		status.reset();

	} while (rcvBuff[rcvIndex] != ':' && rcvIndex < MAX_BUF-1);


	// the length of the data is contained in rcvBuff
	if (rcvIndex >= MAX_BUF)
	{
		status.set(OTHER_ERROR, OTHER_ERROR_CODE, "message too large");
	}

	// prepare the content of rcvBuff
	// to be parsed into an integer to determine the
	// length of the data
	rcvBuff[rcvIndex] = '\0';

	{
		GMSEC_I32 tmp = 0;
		if (!StringUtil::stringParseI32(rcvBuff, tmp))
		{
			status.set(OTHER_ERROR, OTHER_ERROR_CODE, "invalid data size");
			m_eToRead = 1;
			return status;
		}
		inMsgSize = tmp;
	}

	if (m_isDebug)
	{
		fprintf(stderr, "R: line: %d  atoiParam: %s\n", __LINE__, rcvBuff);
	}

	if (inMsgSize <= 0)
	{
		status.set(OTHER_ERROR, OTHER_ERROR_CODE, "invalid data size");
		m_eToRead = 1;
		return status;
	}

	// prepare the return buffer with the length of the data
	len    = inMsgSize;
	buffer = new char[len];

	while (inMsgSize > 0)
	{
		n = recv(m_sock, buffer + (len - inMsgSize), inMsgSize, 0);

		if (n <= 0)
		{
			m_eToRead = 1;
			status = getLastSocketError();
			break;
		}

		inMsgSize -= n;
	}

	return status;
}


Status TCPSocket::write(const char * buffer, int len)
{
	const int TRIES = 3;
	char tBuff[MAX_BUF];
	Status status;

	AutoMutex hold(m_writeMutex);

	// pepare the header containing
	// the message length
	StringUtil::stringFormat(tBuff, MAX_BUF, "%d:", len);

	// send out the header
	int bytes_sent = 0;
	int buf_len = StringUtil::stringLength(tBuff);
	int tries = TRIES;

	while ((bytes_sent < buf_len) && (tries > 0))
	{
		int sent = send(m_sock, tBuff + bytes_sent, buf_len - bytes_sent, 0);

		if (sent <= 0)
		{
			GMSEC_VERBOSE << "wrote nothing- retrying up to " << tries << " times";
			--tries;
		}
		else
		{
			bytes_sent += sent;
			tries = TRIES;
		}
	}

	if (bytes_sent < buf_len)
	{
		int tmp = errno;
		m_eToWrite = 1;
		GMSEC_VERBOSE << "error writing header [error=" << strerror(tmp) << "]";
		return getLastSocketError();
	}

	// now send the rest of the message
	bytes_sent = 0;
	tries = TRIES;
	while ((bytes_sent < len) && (tries > 0))
	{
		int sent = send(m_sock, buffer + bytes_sent, len - bytes_sent, 0);

		if (sent <= 0)
		{
			GMSEC_VERBOSE << "wrote nothing- retrying up to " << tries << " times\n";
			--tries;
		}
		else
		{
			bytes_sent += sent;
			tries = TRIES;
		}
	}

	if (bytes_sent < len)
	{
		int tmp = errno;
		m_eToWrite = 1;
		GMSEC_WARNING << "error writing body [error=" << strerror(tmp) << "]";
		return getLastSocketError();
	}

	return status;
}


void TCPSocket::setDebug(bool debug)
{
	m_isDebug = debug;
}


Status TCPSocket::fillAddress(SockAddr* saddr, const size_t saddrlen, const int port, const char* address)
{
	Status status;

	memset(saddr, 0, saddrlen);

	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family   = (address == NULL ? AF_INET6 : AF_UNSPEC);
	hints.ai_flags    = (address == NULL ? AI_PASSIVE : 0);
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_addrlen  = 0;

	struct addrinfo* host_info = 0;
	std::stringstream portstr;
	portstr << port;

	if (getaddrinfo(address, portstr.str().c_str(), &hints, &host_info) != 0  ||
	    !host_info || !host_info->ai_addr || (host_info->ai_family != AF_INET && host_info->ai_family != AF_INET6))
	{
#ifdef WIN32
		status = getLastSocketError();
#else
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, gai_strerror(errno), errno);
#endif

		if (host_info) freeaddrinfo(host_info);

		return status;
	}

	memcpy(saddr, host_info->ai_addr, host_info->ai_addrlen);

	freeaddrinfo(host_info);

	return status;
}


Status TCPSocket::getLastSocketError()
{
	Status status;
#if defined (WIN32)
	int nRet = WSAGetLastError();

	switch (nRet)
	{
	case WSAEINTR:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Interrupted function call", nRet);
		break;

	case WSAEACCES:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE,
		           "Permission denied. An attempt was made to access a socket in a way forbidden by its access permissions.",
		           nRet
		          );
		break;

	case WSAEFAULT:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Bad address", nRet);
		break;

	case WSAEINVAL:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Invalid argument", nRet);
		break;

	case WSAEMFILE:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Too many open files or sockets", nRet);
		break;

	case WSAEWOULDBLOCK:
		status.set(MIDDLEWARE_ERROR, TIMEOUT_OCCURRED, "Resource temporarily unavailable", nRet);
		break;

	case WSAEINPROGRESS:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Operation now in progress", nRet);
		break;

	case WSAEALREADY:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Operation already in progress", nRet);
		break;

	case WSAENOTSOCK:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Socket operation on nonsocket", nRet);
		break;

	case WSAEDESTADDRREQ:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Destination address required", nRet);
		break;

	case WSAEMSGSIZE:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Message too long", nRet);
		break;

	case WSAEPROTOTYPE:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Protocol wrong type for socket", nRet);
		break;

	case WSAENOPROTOOPT:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Bad protocol option", nRet);
		break;

	case WSAEPROTONOSUPPORT:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Protocol not supported", nRet);
		break;

	case WSAESOCKTNOSUPPORT:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Socket type not supported", nRet);
		break;

	case WSAEOPNOTSUPP:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Operation not supported", nRet);
		break;

	case WSAEPFNOSUPPORT:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Protocol family not supported", nRet);
		break;

	case WSAEAFNOSUPPORT:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Address family not supported by protocol family", nRet);
		break;

	case WSAEADDRINUSE:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Address already in use", nRet);
		break;

	case WSAEADDRNOTAVAIL:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Cannot assign requested address", nRet);
		break;

	case WSAENETDOWN:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Network is down", nRet);
		break;

	case WSAENETUNREACH:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Network is unreachable", nRet);
		break;

	case WSAENETRESET:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Network dropped connection on reset", nRet);
		break;

	case WSAECONNABORTED:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Software caused connection abort", nRet);
		break;

	case WSAECONNRESET:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Connection reset by peer", nRet);
		break;

	case WSAENOBUFS:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "No buffer space available", nRet);
		break;

	case WSAEISCONN:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Socket is already connected", nRet);
		break;

	case WSAENOTCONN:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Socket is not connected", nRet);
		break;

	case WSAESHUTDOWN:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Cannot send after socket shutdown", nRet);
		break;

	case WSAETIMEDOUT:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Connection timed out", nRet);
		break;

	case WSAECONNREFUSED:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Connection refused", nRet);
		break;

	case WSAEHOSTDOWN:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Host is down", nRet);
		break;

	case WSAEHOSTUNREACH:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "No route to host", nRet);
		break;

	case WSAEPROCLIM:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Too many processes", nRet);
		break;

	case WSASYSNOTREADY:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Network subsystem is unavailable", nRet);
		break;

	case WSAVERNOTSUPPORTED:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Winsock.dll version out of range", nRet);
		break;

	case WSANOTINITIALISED:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Successful WSAStartup not yet performed", nRet);
		break;

	case WSAEDISCON:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Graceful shutdown in progress", nRet);
		break;

	case 10109: //WSATYPE_NOT_FOUND:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Class type not found", nRet);
		break;

	case WSAHOST_NOT_FOUND:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Host not found", nRet);
		break;

	case WSATRY_AGAIN:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Nonauthoritative host not found", nRet);
		break;

	case WSANO_RECOVERY:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "This is a nonrecoverable error", nRet);
		break;

	case WSANO_DATA:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "Valid name, no data record of requested type", nRet);
		break;

	default:
		status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, "No error information available", nRet);
		break;
	}

#else  // non-Windoze

	int error = errno;

	// the following if statements are to normalize the sockt error
	// codes returned by different UNIX operating systems
	if (error == UNIX_SOCKET_TIMEOUT_OR_BROKEN_PIPE || error == UNIX_SOCKET_TIMEOUT_OR_NOFILE)
	{
		error = TIMEOUT_OCCURRED;
	}

	status.set(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, strerror(errno), error);

#endif // #if defined (WIN32)

	return status;
}
