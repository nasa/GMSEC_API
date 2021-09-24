/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_TCPSOCKET_H
#define MB_TCPSOCKET_H


#include <gmsec4/util/Mutex.h>

#include <gmsec4/Status.h>


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
	gmsec::api::util::Mutex m_sockMutex;

	gmsec::api::util::Mutex m_readMutex;
	gmsec::api::util::Mutex m_writeMutex;

	gmsec::api::util::Mutex m_sockApiCallCountMutex;
	gmsec::api::util::Mutex m_needModeChangeMutex;

	unsigned short          m_needModeChange;
	unsigned short          m_sockApiCallCount;

protected:
	int m_eToWrite;
	int m_eToRead;
	int m_isDebug;
	int m_pLen;

	gmsec::api::Status fillAddress(SockAddr* saddr, const size_t saddrlen, const int port, const char* address = 0);

	/** @fn getLastSocketError()
	 *  return a status with the last error
	 */
	gmsec::api::Status getLastSocketError();

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


	/**
	 * @fn connect(int port);
	 * connect the socket using the specified 'port'
	 */
	gmsec::api::Status connect(int port);


	/** @fn disconnect();
	 * close the socket connection
	 */
	gmsec::api::Status disconnect();


	/**
	 * @fn read(char*& buffer, int& len);
	 * read data from a socket
	 * buffer is set to a new memory segment pointing to
	 * the data receiveed
	 * len is set to the length of the data received in bytes
	 * this function assumes a format of (len)':'(data)
	 * ie "5:12345" and returns "12345" inside buffer and
	 * sets len = 5
	 */
	gmsec::api::Status read(char*& buffer, int& len);


	/**
	 * @fn write(const char* buffer, int len);
	 * write data to a socket.
	 * buffer contains the data that is to be sent.
	 * len is  the length of the data to be sent in bytes
	 * this function assumes a format of (len)':'(data)
	 * ie "5:12345"
	 */
	gmsec::api::Status write(const char* buffer, int len);


	/**
	 * @fn SetDebug(bool debug)
	 * set the debuf flag to display/(or not) debug information
	 */
	void setDebug(bool debug);
};

}  // end namespace gmsec_messagebus

#endif
