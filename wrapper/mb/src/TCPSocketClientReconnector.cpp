/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "TCPSocketClientReconnector.h"

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace gmsec::api;
using namespace gmsec::api::util;

using namespace gmsec_messagebus;


static const int IS_DEBUG = 0;


TCPSocketClientReconnector::TCPSocketClientReconnector(unsigned nreconnectors, ConfigData *config_data, Mutex *config_data_mutex)
	:
	m_nreconnectors(nreconnectors),
	m_shouldBeConnected(false),
	m_isConnected(false),
	m_isShuttingDown(false),
	m_port(0),
	m_config_data(config_data),
	m_config_data_mutex(config_data_mutex),
	m_sharedReconnector(new TCPReconnector(this)),
	m_reconnectorThread(new StdThread(&runReconnectorThread, m_sharedReconnector))
{
	m_reconnectorThread->start();
}



TCPSocketClientReconnector::~TCPSocketClientReconnector()
{
	m_isShuttingDown.set(true);

	disconnect();
}


bool TCPSocketClientReconnector::isConnected()
{
	return m_isConnected.get();
}


Status TCPSocketClientReconnector::connect(int port, const char * server)
{
	m_port   = port;
	m_server = server;

	Status result = TCPSocketClient::connect(port, server);

	m_isConnected.set(!result.isError());

// HP-UX and Solaris will block, others won't
#if !defined(__hpux) && !defined(__sun)
	// Since there are bugs with using a socket time out, only do it
	// when there are redundant servers (note that using redundant servers
	// experimental at this time)
	if (m_nreconnectors > 0)
	{
		struct timeval timeout = {1, 0};
		setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (const char *) &timeout, sizeof(timeout));
	}
#endif

	m_shouldBeConnected.set(true);

	return result;
}


Status TCPSocketClientReconnector::read(char * &buffer, int &len)
{
	if (m_isConnected.get())
	{
		return TCPSocketClient::read(buffer, len);
	}
	else
	{
		Status result;
		result.set(CONNECTION_ERROR, CONNECTION_LOST, "Connection to the server was lost");
		return result;
	}
}


Status TCPSocketClientReconnector::write(const char *buffer, int len)
{
	if (m_isConnected.get())
	{
		return TCPSocketClient::write(buffer, len);
	}
	else
	{
		Status result;
		result.set(CONNECTION_ERROR, CONNECTION_LOST, "Connection to the server was lost");
		return result;
	}
}


void TCPSocketClientReconnector::reconnect()
{
	disconnect();
	m_shouldBeConnected.set(true);
}


Status TCPSocketClientReconnector::internalReconnect()
{
	if (IS_DEBUG)
	{
		GMSEC_DEBUG << "Reconnector:" << this << ":InternalReconnect starting";
	}

	Status result = TCPSocketClient::disconnect();
	m_isConnected.set(false);

	result = TCPSocketClient::connect(m_port, m_server.c_str());
	m_isConnected.set(!result.isError());

// HP-UX and Solaris will block, others won't
#if !defined(__hpux) && !defined(__sun)
	// Since there are bugs with using a socket time out, only do it
	// when there are redundant servers (note that using redundant servers
	// experimental at this time)
	if (m_nreconnectors > 0)
	{
		struct timeval sstimeout = {1, 0};
		setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (const char *) &sstimeout, sizeof(sstimeout));
	}
#endif

	if (m_isConnected.get())
	{
		AutoMutex hold(*m_config_data_mutex);
		for (ConfigData::iterator i = m_config_data->begin(); i != m_config_data->end(); ++i)
		{
			const char* value = i->first.c_str();
			write(value, i->first.length()+1);
		}
	}

	if (IS_DEBUG)
	{
		GMSEC_DEBUG << "Reconnector:" << this << ":InternalReconnect returning";
	}

	return result;
}


Status TCPSocketClientReconnector::disconnect()
{
	m_shouldBeConnected.set(false);

	// fixme
	TimeUtil::millisleep(200);

	Status result = TCPSocketClient::disconnect();

	m_isConnected.set(false);

	return result;
}


TCPReconnector::TCPReconnector(TCPSocketClientReconnector* r)
	:
	reconnector(r)
{
}


TCPReconnector::~TCPReconnector()
{
}


// REW: note that this (silly) code write an OK message 10 times a second!
void TCPReconnector::run(void)
{
	if (!reconnector)
	{
		return;
	}

	bool haveError = false;

	while (!reconnector->m_isShuttingDown.get())
	{
		if (reconnector->m_shouldBeConnected.get())
		{
			if (reconnector->m_isConnected.get())
			{
				Status result = reconnector->write("O", 1); // Check that server is okay
				if (result.isError())
				{
					if (!haveError)
					{
						GMSEC_INFO << "Attempting to reconnect...";
						haveError = true;
					}
					reconnector->internalReconnect();
				}
				else if (haveError)
				{
					GMSEC_INFO << "Reconnected!";
					haveError = false;
				}
			}
			else
			{
				if (!haveError)
				{
					GMSEC_INFO << "Attempting to reconnect...";
					haveError = true;
				}
				reconnector->internalReconnect();
			}
		}

		TimeUtil::millisleep(100);
	}
}


void TCPSocketClientReconnector::runReconnectorThread(SharedTCPReconnector shared)
{
	shared->run();
}
