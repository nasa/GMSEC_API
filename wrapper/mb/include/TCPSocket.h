/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_MB_TCPSOCKET_H
#define GMSEC_MB_TCPSOCKET_H


#include <gmsec5/util/Mutex.h>

#include <gmsec5/Status.h>


#include <stdio.h>

#if defined (WIN32)
#include <Ws2tcpip.h>
//#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#define END_OF_TRANS 99


namespace gmsec_messagebus
{

/**
 * @brief This is the abstract base class for socket commuications
 */
class TCPSocket
{
public:
#ifdef WIN32
	typedef SOCKET           Socket;
	typedef SOCKADDR         SockAddr;
	typedef SOCKADDR_STORAGE SockAddrIn;
#else
	typedef int              Socket;
	typedef sockaddr         SockAddr;
	typedef sockaddr_storage SockAddrIn;
#endif

private:
	gmsec::api5::util::Mutex m_sockMutex;

	gmsec::api5::util::Mutex m_readMutex;
	gmsec::api5::util::Mutex m_writeMutex;

	gmsec::api5::util::Mutex m_sockApiCallCountMutex;
	gmsec::api5::util::Mutex m_needModeChangeMutex;

	unsigned short           m_needModeChange;
	unsigned short           m_sockApiCallCount;

protected:
	int m_eToWrite;
	int m_eToRead;
	int m_isDebug;
	int m_pLen;

	gmsec::api5::Status fillAddress(SockAddr* saddr, const size_t saddrlen, const int port, const char* address = 0);

	/** @fn getLastSocketError()
	 *  return a status with the last error
	 */
	gmsec::api5::Status getLastSocketError();

public:  // seriously!!!
	Socket    m_sock;
	SockAddr* m_pAddr;
	bool      m_isConnected;


public:
	/**
	 * @fn TCPSocket()
	 * default constructor
	 */
	TCPSocket();


	virtual ~TCPSocket();


	/** @fn disconnect();
	 * close the socket connection
	 */
	gmsec::api5::Status disconnect();


	/**
	 * @fn read(char*& buffer, GMSEC_I32& len);
	 * read data from a socket
	 * buffer is set to a new memory segment pointing to
	 * the data receiveed
	 * len is set to the length of the data received in bytes
	 * this function assumes a format of (len)':'(data)
	 * ie "5:12345" and returns "12345" inside buffer and
	 * sets len = 5
	 */
	gmsec::api5::Status read(char*& buffer, GMSEC_I32& len);


	/**
	 * @fn write(const char* buffer, GMSEC_I32 len);
	 * write data to a socket.
	 * buffer contains the data that is to be sent.
	 * len is  the length of the data to be sent in bytes
	 * this function assumes a format of (len)':'(data)
	 * ie "5:12345"
	 */
	gmsec::api5::Status write(const char* buffer, GMSEC_I32 len);


	/**
	 * @fn SetDebug(bool debug)
	 * set the debuf flag to display/(or not) debug information
	 */
	void setDebug(bool debug);
};

}  // end namespace gmsec_messagebus

#endif
