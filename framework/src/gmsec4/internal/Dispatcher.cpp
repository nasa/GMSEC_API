/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file Dispatcher.cpp
 *
 *  Generic implementation of the Connection dispatch mechanism.
 */

#include <gmsec4/internal/Dispatcher.h>

#include <gmsec4/internal/CallbackLookup.h>

#include <gmsec4/Connection.h>
#include <gmsec4/Callback.h>

#include <gmsec4/util/Log.h>

using namespace gmsec::api;
using namespace gmsec::api::util;


namespace gmsec {
namespace api {
namespace internal {


DispatchShared::DispatchShared(InternalConnection* connection)
	: m_connection(connection),
	  m_startupLatch(1),
	  m_shutdownLatch(1)
{
}


DispatchShared::~DispatchShared()
{
}


void DispatchShared::run()
{
	running.set(true);

	/* 
	 * DR1988 - Running must be set to true before any other calls in this method.
	 * This is because the system scheduler may switch threads before setting running
	 * to true, from which an error will occur. This leads to one of two things: a thread
	 * stuck in an infinite loop (because ShutDownAutodispatch sets running to false
	 * before this thread sets running to true), or a new thread being spawned which
	 * cannot connect to the middleware server.
	 */

	m_startupLatch.countDown();

	while (running.get())
	{
		m_connection->autoDispatch();
	}

	m_shutdownLatch.countDown();
}


bool DispatchShared::await(int timeout_ms)
{
	return m_startupLatch.await(timeout_ms);
}


bool DispatchShared::shutdown(int timeout_ms)
{
	if (!running.get())
	{
		GMSEC_VERBOSE << "DispatchShared::shutdown: not running";
		return false;
	}

	running.set(false);

	return m_shutdownLatch.await(timeout_ms);
}


void runDispatcher(StdSharedPtr<DispatchShared> shared)
{
	shared->run();
}


} // namespace internal
} // namespace api
} // namespace gmsec
