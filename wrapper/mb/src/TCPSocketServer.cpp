/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "TCPSocketServer.h"

#include <gmsec4/util/Log.h>

using namespace gmsec::api;
using namespace gmsec::api::util;

using namespace gmsec_messagebus;


#ifdef WIN32
#define PRINTERROR(s)	\
fprintf(stderr,"\n%s: %d\n", s, WSAGetLastError())
#endif


// this method binds a server port with
// the specified value
Status TCPSocketServer::connect(int port)
{
	int one = 1;
	Status status;

	m_eToRead = 0;
	m_eToWrite = 0;

#ifdef WIN32
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	//
	// Initialize WinSock and check version
	//
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0)
	{
		return getLastSocketError();
	}
	else if (wsaData.wVersion != wVersionRequested)
	{
		GMSEC_WARNING << "Unable to load Windows Socket version 1.1";
	}
#endif

	status = fillAddress(m_pAddr, sizeof(SockAddrIn), port);

	if (status.isError())
	{
		return status;
	}

	/* create the socket */
	m_sock = socket(m_pAddr->sa_family, SOCK_STREAM, 0);

#ifdef WIN32
	if (m_sock == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return getLastSocketError();
	}
#else
	if (m_sock < 0)
	{
		return getLastSocketError();
	}
#endif

	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(one)) < 0)
	{
		return getLastSocketError();
	}

#ifdef WIN32
	#define OPT_TYPE const char*
#else
	#define OPT_TYPE const void*
#endif
	// set Sockoption TCP_NODELAY which will turn off Nagle's algorithm, and send each
	// packet as soon as it gets it.
	if (setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (OPT_TYPE) &one, sizeof(one)) < 0)
	{
		return getLastSocketError();
	}

	// Attempt to set Sockoption SO_SNDBUF to a larger buffer size; if it fails, we do not care.
	unsigned int size = 10*1024*1024;
	if (setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (OPT_TYPE) &size, sizeof(size)) < 0)
	{
		GMSEC_WARNING << "Unable to set socket SO_SNDBUF size to " << size << " bytes.";
	}

	// Attempt to set Sockoption SO_RCVBUF to a larger buffer size; if it fails, we do not care.
	if (setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (OPT_TYPE) &size, sizeof(size)) < 0)
	{
		GMSEC_WARNING << "Unable to set socket SO_RCVBUF size to " << size << " bytes.";
	}

	// Ideally we would use the size-of SockAddrIn, however not all OSes support this; hence we will
	// deduce the sockaddr size based on the INET domain in use.
	size_t sockaddr_len = (m_pAddr->sa_family == AF_INET6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in));

#ifdef WIN32
	if (bind(m_sock, m_pAddr, sockaddr_len) == SOCKET_ERROR)
	{
		PRINTERROR("bind()");
		closesocket(m_sock);
		return getLastSocketError();
	}
#else
	if (bind(m_sock, m_pAddr, sockaddr_len) < 0)
	{
		return getLastSocketError();
	}
#endif

	return status;
}


void TCPSocketServer::accept(TCPSocketServer* child)
{
	if (child == NULL)
		return;

	listen(m_sock, 5);

	child->m_sock = ::accept(m_sock, NULL, NULL);

#ifdef WIN32
	if (child->m_sock == INVALID_SOCKET)
	{
		printf("%p: CTCPSocketServer::Accept error=%ld\n", this, WSAGetLastError());
		return;
	}
#else
	if (child->m_sock < 0)
	{
		printf("%p: CTCPSocketServer::Accept error=%d\n", this, child->m_sock);
		return;
	}
#endif

#ifdef WIN32
	#define OPT_TYPE const char*
#else
	#define OPT_TYPE const void*
#endif
	// set Sockoption TCP_NODELAY which will turn off Nagle's algorithm, and send each
	// packet as soon as it gets it.
	int t = 1;
	if (setsockopt(child->m_sock, IPPROTO_TCP, TCP_NODELAY, (OPT_TYPE) &t, sizeof(int)) < 0)
	{
		return;
	}

    // Attempt to set Sockoption SO_SNDBUF to a larger buffer size; if it fails, we do not care.
    unsigned int size = 10*1024*1024;
    if (setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (OPT_TYPE) &size, sizeof(size)) < 0)
    {
        GMSEC_WARNING << "Unable to set socket SO_SNDBUF size to " << size << " bytes.";
    }

    // Attempt to set Sockoption SO_RCVBUF to a larger buffer size; if it fails, we do not care.
    if (setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (OPT_TYPE) &size, sizeof(size)) < 0)
    {
        GMSEC_WARNING << "Unable to set socket SO_RCVBUF size to " << size << " bytes.";
    }

	child->m_eToRead = 0;
	child->m_eToWrite = 0;
}


void TCPSocketServer::setDebug(bool debug)
{
	TCPSocket::setDebug(debug);
}


Status TCPSocketServer::read(char*& buffer, int& len)
{
	return TCPSocket::read(buffer, len);
}


Status TCPSocketServer::write(const char* buffer, int len)
{
	return TCPSocket::write(buffer, len);
}

Status TCPSocketServer::disconnect()
{
	return TCPSocket::disconnect();
}
