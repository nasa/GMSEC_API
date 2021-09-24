/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "TCPSocketClient.h"

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::util;

using namespace gmsec_messagebus;


Status TCPSocketClient::connect(int port)
{
	return connect(port, "localhost");
}


Status TCPSocketClient::connect(int port, const char * server)
{
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

	status = fillAddress(m_pAddr, sizeof(SockAddrIn), port, server);

	if (status.isError())
	{
		return status;
	}

	m_sock = socket(m_pAddr->sa_family, SOCK_STREAM, 0);
	if (m_sock < 0)
	{
		return getLastSocketError();
	}


	// set Sockoption TCP_NODELAY which will turn off Nagle's algorithm, and send each
	// packet as soon as it gets it.
#ifdef WIN32
	#define OPT_TYPE const char*
#else
	#define OPT_TYPE const void*
#endif
	int t = 1;
	if (setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (OPT_TYPE) &t, sizeof(t)) < 0)
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

	if (::connect(m_sock, m_pAddr, sockaddr_len) < 0)
	{
		return getLastSocketError();
	}

	GMSEC_DEBUG << "Socket successfully created and connected.";

	return status;
}


void TCPSocketClient::setDebug(bool debug)
{
	TCPSocket::setDebug(debug);
}


Status TCPSocketClient::read(char * &buffer, int &len)
{
	return TCPSocket::read(buffer, len);
}


Status TCPSocketClient::write(const char *buffer, int len)
{
	return TCPSocket::write(buffer, len);
}


Status TCPSocketClient::disconnect()
{
	Status stat = TCPSocket::disconnect();

	if (stat.getClass() != NO_ERROR_CLASS)
	{
		GMSEC_WARNING << "Socket did not close successfully.";
	}
	else
	{
		GMSEC_DEBUG << "Socket closed successfully.";
	}
	return stat;
}
