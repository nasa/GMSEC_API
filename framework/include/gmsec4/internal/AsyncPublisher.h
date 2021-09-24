/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file AsyncPublisher.h
 *
 *  @brief This file defines the Asynchronous Publisher Thread used by
 *  BaseConnection to dispatch publish calls.
 */

#ifndef GMSEC_API_INTERNAL_ASYNC_PUBLISHER_H
#define GMSEC_API_INTERNAL_ASYNC_PUBLISHER_H

#include <gmsec4/internal/InternalConnection.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/BoundedQueue.h>
#include <gmsec4/util/CountDownLatch.h>


namespace gmsec {
namespace api {
namespace internal {

// Forward declaration(s)
class ConnectionInterface;


class AsyncPublisher
{
public:
	AsyncPublisher(ConnectionInterface* connIf,
	               gmsec::api::util::BoundedQueue<MessagePublishTask>* asyncQueue);

	// Destructor turns off thread operations
	~AsyncPublisher();

	// This function starts the AsyncPublisher thread;
	static void start(gmsec::api::util::StdSharedPtr<AsyncPublisher> shared);


	// This function stops the AsyncPublisher thread
	bool stop(unsigned int timeout_ms);

private:
	// The main processing function for the AsyncPublisher.
	void run();

	ConnectionInterface*                                m_connIf;
	gmsec::api::util::BoundedQueue<MessagePublishTask>* m_asyncQueue;
	gmsec::api::util::AtomicBoolean                     m_alive;
	gmsec::api::util::AtomicBoolean                     m_hardKill;
	gmsec::api::util::CountDownLatch                    m_shutdownLatch;
};

}//internal
}//api
}//gmsec


#endif
