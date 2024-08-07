/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_MB_TCP_SOCKET_CLIENT_H
#define GMSEC_MB_TCP_SOCKET_CLIENT_H

#include "TCPSocket.h"

#include <gmsec5/Status.h>

#include <sys/types.h>
#if defined (WIN32)
#include <winsock.h>
#else	// WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <string>


namespace gmsec_messagebus
{

/** @brief This is the abstract base class for client side socket commuications
**/
class TCPSocketClient : public  TCPSocket
{
public:
	/**
	 * @fn connect(int port, const char* server)
	 * connect to 'server' with the specified 'port'
	 */
	virtual gmsec::api5::Status connect(const std::string& server, int port);


	/**
	 * @fn setDebug(bool flag)
	 * set the debuf flag to display/(or not) debug information
	 */
	void setDebug(bool flag);


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
	 * @fn write(const char* buffer, GMSEC_I32 len)
	 * write data to a socket.
	 * buffer contains the data that is to be sent.
	 * len is  the length of the data to be sent in bytes
	 * this function assumes a format of (len)':'(data)
	 * ie "5:12345"
	 */
	gmsec::api5::Status write(const char* buffer, GMSEC_I32 len);


	/**
	 * @fn disconnect();
	 * close the socket connection
	 */
	gmsec::api5::Status disconnect();
};

}  // end namespace gmsec_messagebus

#endif
