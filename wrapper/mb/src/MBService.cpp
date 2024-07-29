/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "MBService.h"

#include "ConnectionMgr.h"
#include "MBResourceData.h"

#include <gmsec5/util/Log.h>
#include <gmsec5/util/StdThread.h>

#include <memory>
#include <cstdlib>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;

using namespace gmsec_messagebus;


MBService::MBService(int port, const Config& config)
	:
	m_port(port),
	m_msgFactory(config),
	m_callbackLookup(new CallbackLookup()),
	MBR_instance(new MBResourceData()),
	m_allowExit(false)
{
}


MBService::~MBService()
{
	delete m_callbackLookup;
	delete MBR_instance;
}


void MBService::run()
{
	Status status;

	// open the server socket
	status = m_sock.connect(m_port);
	if (status.hasError())
	{
		GMSEC_ERROR << "Unable to open server socket: " << status.get();
		return;
	}

	// accept new connections and wait for new ones
	while (true)
	{
		// wait for the new connection
		GMSEC_INFO << "waiting for client";
		TCPSocketServer *tsChild = new TCPSocketServer();
		m_sock.accept(tsChild);

		tsChild->setDebug(false);

		// start a new connection manager with the new client
		startNewMgr(tsChild);
	}

}


void MBService::stop()
{
	if (m_allowExit)
	{
		GMSEC_WARNING << "received exit command";
		std::exit(0);
	}
	else
	{
		GMSEC_WARNING << "ignoring exit command";
	}
}


void MBService::startNewMgr(TCPSocketServer* newSocket)
{
	// allocate the new manager and start it
	ManagedConnectionMgr* managed = new ManagedConnectionMgr;
	managed->shared.reset(new ConnectionMgr(newSocket, this, m_msgFactory));
	managed->thread = new StdThread(&runConnMgr, managed->shared);

	managed->thread->start();

	// clean finished managers
	for (ConnectionMgrList::iterator it = m_connMgrs.begin(); it != m_connMgrs.end();)
	{
		ManagedConnectionMgr* mgr = *it;

		if (mgr->shared->isRunning() == false)
		{
			mgr->shared->shutdown();

			mgr->shared.reset();

			mgr->thread->join();

			delete mgr->thread;

			delete mgr;

			it = m_connMgrs.erase(it);
		}
		else
		{
			++it;
		}
	}

	m_connMgrs.push_back(managed);
}


void MBService::addCallback(const char* subject, Callback* cb)
{
	m_callbackLookup->addCallback(subject, cb);
}


void MBService::removeCallback(const char* subject, Callback* cb)
{
	m_callbackLookup->removeCallback(subject, cb);
}


void MBService::dispatchMsg(const Message& msg)
{
	m_callbackLookup->dispatchMsg(msg);
}


size_t MBService::getCallbackCount(const char* subject)
{
	return m_callbackLookup->getCallbackCount(subject);
}


MBResourceData* MBService::getMBResourceDataInstance()
{
	return MBR_instance;
}


Mutex& MBService::getCallbackLookupMutex()
{
	return m_callLkpLock;
}


void MBService::setExitable(bool flag)
{
	m_allowExit = flag;
}


void MBService::runConnMgr(SharedConnectionMgr shared)
{
	shared->run();
}
