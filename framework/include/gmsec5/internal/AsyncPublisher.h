/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file AsyncPublisher.h
 *
 * @brief This file defines the Asynchronous Publisher Thread used by InternalConnection to dispatch publish calls.
 */

#ifndef GMSEC_API5_INTERNAL_ASYNC_PUBLISHER_H
#define GMSEC_API5_INTERNAL_ASYNC_PUBLISHER_H

#include <gmsec5/internal/InternalConnection.h>

#include <gmsec5/util/Atomics.h>
#include <gmsec5/util/BoundedQueue.h>
#include <gmsec5/util/CountDownLatch.h>


namespace gmsec
{
namespace api5
{
namespace internal
{
	// Forward declaration(s)
	class ConnectionInterface;


class AsyncPublisher
{
public:
	AsyncPublisher(ConnectionInterface* connIf, gmsec::api5::util::BoundedQueue<MessagePublishTask>* asyncQueue);

	// Destructor turns off thread operations
	~AsyncPublisher();

	// This function starts the AsyncPublisher thread;
	static void start(gmsec::api5::util::StdSharedPtr<AsyncPublisher> shared);


	// This function stops the AsyncPublisher thread
	bool stop(int timeout_ms);

private:
	// The main processing function for the AsyncPublisher.
	void run();

	ConnectionInterface*                                 m_connIf;
	gmsec::api5::util::BoundedQueue<MessagePublishTask>* m_asyncQueue;
	gmsec::api5::util::AtomicBoolean                     m_alive;
	gmsec::api5::util::AtomicBoolean                     m_hardKill;
	gmsec::api5::util::CountDownLatch                    m_shutdownLatch;
};

} //end namespace internal
} //end namespace api5
} //end namespace gmsec

#endif
