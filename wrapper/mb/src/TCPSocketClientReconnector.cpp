/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "TCPSocketClientReconnector.h"

#include <gmsec5/util/Log.h>
#include <gmsec5/util/TimeUtil.h>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace gmsec::api5;
using namespace gmsec::api5::util;

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
	m_reconnectorThread()
{
}



TCPSocketClientReconnector::~TCPSocketClientReconnector()
{
	m_isShuttingDown.set(true);
	m_shouldBeConnected.set(false);
	m_isConnected.set(false);

	//DMW - The delay below is lame, however at this time it is offered as
	//      a token to allow for the TCPReconnector thread to terminate/die
	//      before the TSCR object is destroyed.
	TimeUtil::millisleep(500);
}


bool TCPSocketClientReconnector::isConnected()
{
	return m_isConnected.get();
}


Status TCPSocketClientReconnector::connect(const std::string& server, int port)
{
	m_port   = port;
	m_server = server;

	Status result = TCPSocketClient::connect(server, port);

	m_isConnected.set(!result.hasError());

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

	// Start the reconnector thread once we're connected
	m_reconnectorThread.reset(new StdThread(&runReconnectorThread, m_sharedReconnector));
	m_reconnectorThread->start();

	return result;
}


Status TCPSocketClientReconnector::read(char * &buffer, GMSEC_I32& len)
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


Status TCPSocketClientReconnector::write(const char *buffer, GMSEC_I32 len)
{
	Status result;

	if (m_isConnected.get())
	{
		result = TCPSocketClient::write(buffer, len);
	}
	else
	{
		result.set(CONNECTION_ERROR, CONNECTION_LOST, "Connection to the server was lost");
	}

	return result;
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

	result = TCPSocketClient::connect(m_server, m_port);
	m_isConnected.set(!result.hasError());

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
			write(value, static_cast<int>(i->first.length())+1);
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
// DMW: write failures can occur; let's not be so aggressive with a reconnect
void TCPReconnector::run(void)
{
	if (!reconnector)
	{
		return;
	}

	// Allow other thread to run
	StdThread::yield();

	bool haveError = false;
	int  failures  = 0;

	while (!reconnector->m_isShuttingDown.get())
	{
		TimeUtil::millisleep(100);

		if (reconnector->m_shouldBeConnected.get())
		{
			if (reconnector->m_isConnected.get())
			{
				Status result = reconnector->write("O", 1); // Check the server is okay
				if (result.hasError())
				{
					++failures;

					if (failures >= 10)
					{
						if (!haveError)
						{
							GMSEC_INFO << "Attempting to reconnect...";
							haveError = true;
						}
						if (haveError && !reconnector->m_isShuttingDown.get())
						{
							reconnector->internalReconnect();
						}

						failures = 0;
					}
				}
				else if (haveError)
				{
					GMSEC_INFO << "Reconnected!";
					haveError = false;
					failures  = 0;
				}
			}
			else
			{
				// Add small delay... if indeed we are shutting down, let's avoid logging the 'reconnect' message
				TimeUtil::millisleep(200);

				if (!haveError && reconnector->m_shouldBeConnected.get() && !reconnector->m_isShuttingDown.get())
				{
					GMSEC_INFO << "Attempting to reconnect...";
					haveError = true;
				}
				if (haveError && !reconnector->m_isShuttingDown.get())
				{
					reconnector->internalReconnect();
				}
			}
		}
	}
}


void TCPSocketClientReconnector::runReconnectorThread(SharedTCPReconnector shared)
{
	shared->run();
}
