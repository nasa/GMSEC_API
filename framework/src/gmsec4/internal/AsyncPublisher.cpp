/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file AsyncPublish.cpp
 *
 *  @brief This file contains a class for the Asynchronous Publisher
 *
 */

#include <gmsec4/internal/AsyncPublisher.h>

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api::util;


namespace gmsec {
namespace api {
namespace internal {


AsyncPublisher::AsyncPublisher(ConnectionInterface* connIf, 
							   BoundedQueue<MessagePublishTask>* asyncQueue)
	: m_connIf(connIf),
	  m_asyncQueue(asyncQueue),
	  m_alive(false),
	  m_hardKill(false),
	  m_shutdownLatch(1)
{
	m_alive.set(true);
}


AsyncPublisher::~AsyncPublisher()
{
	m_hardKill.set(true);

	//Just in case the thread wasn't properly terminated before cleanup
	m_shutdownLatch.await(10000);
}


void AsyncPublisher::start(StdSharedPtr<AsyncPublisher> shared)
{
	shared->run();
}


bool AsyncPublisher::stop(unsigned int timeout_ms)
{
	if (!m_alive.get())
	{
		GMSEC_WARNING << "AsyncPublisher::stop: not running";
		return false;
	}

	m_alive.set(false);

	return m_shutdownLatch.await(timeout_ms);
}


void AsyncPublisher::run()
{
	GMSEC_INFO << "Asynchronous Publisher thread starting";

	MessagePublishTask mpt;

	//Stay alive as long as the "keep alive" flag is set and queue isn't empty,
	//but only if the "hard kill" flag is not also set
	while ((m_alive.get() || !m_asyncQueue->empty()) && !m_hardKill.get())
	{
		if (m_asyncQueue->poll(500, mpt))
		{
			if (mpt.msg)
			{
				const Message* message = mpt.msg;
				const Config*  config  = mpt.config;

				try
				{
					m_connIf->mwPublish(*message, *config);
				}
				catch (Exception& e)
				{
					GMSEC_ERROR << "Asynchronous Publisher could not publish message. Error status: "
					            << e.what();
				}

				delete mpt.msg;
				delete mpt.config;
			}
		}
	}

	m_shutdownLatch.countDown();

	GMSEC_INFO << "Asynchronous Publisher thread ending";
}

}//internal
}//api
}//gmsec
