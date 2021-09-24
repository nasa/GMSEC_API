/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalAutoDispatcher.cpp
**/

#include <gmsec4/internal/InternalAutoDispatcher.h>

#include <gmsec4/internal/Dispatcher.h>
#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/TicketMutex.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


InternalAutoDispatcher::InternalAutoDispatcher(InternalConnection* connection, TicketMutex& readMutex, TicketMutex& writeMutex)
	: m_iConn(connection),
	  m_readMutex(readMutex),
	  m_writeMutex(writeMutex),
	  m_dispatcherRunning(false),
	  m_dispatcher(NULL),
	  m_dispatchShared(NULL)
{
	if (!m_iConn)
	{
		throw Exception(DISPATCHER_ERROR, INVALID_CONNECTION, "InternalConnection object is NULL");
	}
}


InternalAutoDispatcher::~InternalAutoDispatcher()
{
	// we do NOT delete m_iConn, for we do not own it.
	m_dispatcherRunning.set(false);
}


bool InternalAutoDispatcher::start()
{
	if (m_dispatchShared.get() == NULL)
	{
		//... create thread and launch
		m_dispatchShared.reset(new DispatchShared(m_iConn));

		m_dispatcher.reset(new StdThread(&runDispatcher, m_dispatchShared));

		m_dispatcherRunning.set(true);
		m_dispatcher->start();

		if (!m_dispatchShared->await(10000))
		{
			GMSEC_ERROR << "Timed out when initializing Auto-Dispatcher";
			m_dispatcherRunning.set(false);
		}

		return m_dispatcherRunning.get();
	}

	GMSEC_WARNING << "Auto-Dispatcher is already running!";
	return false;
}


bool InternalAutoDispatcher::stop(bool waitForCompletion)
{
	AutoTicket holdRead(m_readMutex);
	AutoTicket holdWrite(m_writeMutex);

	//... if currently running
	DispatchShared* tmp = m_dispatchShared.get();

	if (tmp)
	{
		int tries = (waitForCompletion ? 3 : 1);
		do
		{
			const int timeout_ms = (waitForCompletion ? 3000 : 1);

			holdRead.leave();
			holdWrite.leave();

			if (tmp->shutdown(timeout_ms))
			{
				holdRead.enter();
				holdWrite.enter();

				m_dispatchShared.reset();
				m_dispatcher.reset();
				m_dispatcherRunning.set(false);
			}
			else if (waitForCompletion)
			{
				GMSEC_WARNING << "Failed to shutdown Auto-Dispatcher after " << timeout_ms << " ms";
			}
		} while (m_dispatcherRunning.get() && --tries > 0);

		return !m_dispatcherRunning.get();
	}

	GMSEC_WARNING << "Auto-Dispatcher is not running!";
	return false;
}


bool InternalAutoDispatcher::isRunning()
{
	return m_dispatcherRunning.get();
}
