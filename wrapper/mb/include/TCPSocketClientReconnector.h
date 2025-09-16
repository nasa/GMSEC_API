/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_TCP_SOCKET_CLIENT_RECONNECTOR_H
#define MB_TCP_SOCKET_CLIENT_RECONNECTOR_H

#include "TCPSocketClient.h"

#include <gmsec5/internal/ci_less.h>

#include <gmsec5/Status.h>

#include <gmsec5/util/Atomics.h>
#include <gmsec5/util/Mutex.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/StdUniquePtr.h>

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
	typedef std::map<std::string, char, gmsec::api5::internal::ci_less> ConfigData;


	TCPSocketClientReconnector(unsigned nreconnectors, ConfigData* config_data, gmsec::api5::util::Mutex* config_data_mutex);


	~TCPSocketClientReconnector();


	virtual gmsec::api5::Status connect(const std::string& server, int port);


	gmsec::api5::Status read(char*& buffer, GMSEC_I32& len);


	gmsec::api5::Status write(const char* buffer, GMSEC_I32 len);


	void reconnect();


	gmsec::api5::Status disconnect();


	bool isConnected();


private:
	// Defined, but not implemented
	TCPSocketClientReconnector(const TCPSocketClientReconnector &);
	TCPSocketClientReconnector &operator=(const TCPSocketClientReconnector &);


	gmsec::api5::Status internalReconnect();


	typedef gmsec::api5::util::StdSharedPtr<TCPReconnector>               SharedTCPReconnector;
	typedef gmsec::api5::util::StdUniquePtr<gmsec::api5::util::StdThread> ReconnectorThread;

	static void runReconnectorThread(SharedTCPReconnector shared);


	unsigned int                     m_nreconnectors;
	gmsec::api5::util::AtomicBoolean m_shouldBeConnected;
	gmsec::api5::util::AtomicBoolean m_isConnected;
	gmsec::api5::util::AtomicBoolean m_isShuttingDown;
	int                              m_port;
	std::string                      m_server;

	ConfigData*                      m_config_data;
	gmsec::api5::util::Mutex*        m_config_data_mutex;

	SharedTCPReconnector            m_sharedReconnector;
	ReconnectorThread               m_reconnectorThread;

	friend class TCPReconnector;
};

}  // end namespace gmsec_messagebus

#endif
