/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_TCP_SOCKET_CLIENT_RECONNECTOR_H
#define MB_TCP_SOCKET_CLIENT_RECONNECTOR_H

#include "TCPSocketClient.h"

#include <gmsec4/internal/ci_less.h>

#include <gmsec4/Status.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <map>
#include <string>


namespace gmsec_messagebus
{

class TCPSocketClientReconnector;


class TCPReconnector
{
public:
	TCPReconnector(TCPSocketClientReconnector* t);

	~TCPReconnector();

	void run();

private:
	TCPSocketClientReconnector* reconnector;
};


class TCPSocketClientReconnector : public TCPSocketClient
{
public:
	typedef std::map<std::string, char, gmsec::api::internal::ci_less> ConfigData;


	TCPSocketClientReconnector(unsigned nreconnectors, ConfigData* config_data, gmsec::api::util::Mutex* config_data_mutex);


	~TCPSocketClientReconnector();


	virtual gmsec::api::Status connect(int port, const char* server);


	gmsec::api::Status read(char*& buffer, int& len);


	gmsec::api::Status write(const char* buffer, int len);


	void reconnect();


	gmsec::api::Status disconnect();


	bool isConnected();


private:
	// Defined, but not implemented
	TCPSocketClientReconnector(const TCPSocketClientReconnector &);
	TCPSocketClientReconnector &operator=(const TCPSocketClientReconnector &);


	gmsec::api::Status internalReconnect();


	typedef gmsec::api::util::StdSharedPtr<TCPReconnector>              SharedTCPReconnector;
	typedef gmsec::api::util::StdUniquePtr<gmsec::api::util::StdThread> ReconnectorThread;

	static void runReconnectorThread(SharedTCPReconnector shared);


	unsigned int                    m_nreconnectors;
	gmsec::api::util::AtomicBoolean m_shouldBeConnected;
	gmsec::api::util::AtomicBoolean m_isConnected;
	gmsec::api::util::AtomicBoolean m_isShuttingDown;
	int                             m_port;
	std::string                     m_server;

	ConfigData*                     m_config_data;
	gmsec::api::util::Mutex*        m_config_data_mutex;

	SharedTCPReconnector            m_sharedReconnector;
	ReconnectorThread               m_reconnectorThread;

	friend class TCPReconnector;
};

}  // end namespace gmsec_messagebus

#endif
