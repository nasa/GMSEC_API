/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Dispatcher.h
 *
 * @brief Perform asynchronous, automated delivery of messages off the message
 * queue to callback functions
 */

#ifndef GMSEC_API_INTERNAL_DISPATCHER_H
#define GMSEC_API_INTERNAL_DISPATCHER_H


#include <gmsec4/util/CountDownLatch.h>

#include <gmsec4/internal/InternalConnection.h>


namespace gmsec {
namespace api {
namespace internal {


/**	@class DispatchShared
 *
 *  @brief Type that holds state of interface between BaseConnection and
 *  Dispatcher thread.
 */
class DispatchShared
{
public:
	DispatchShared(InternalConnection* connection);


	~DispatchShared();


	/** @fn run()
	 * Overload the thread run method to perform the dispatch
	 * task.  The init() method must be called prior to this
	 * method, else the method will fail.  If error are encountered
	 * during the running of this method, the dispatcher global
	 * status code can be checked to determine the last error. To
	 * terminate this method, the destroy() method must be called.
	 *
	 * NOTE: The messages genereted within this method are
	 *		 cleaned up within this method and should not be
	 *		 freed by the client callbacks.
	 */
	void run();


	/** @fn startup(int timeout_ms)
	 * Timeout the startup of this object.
	 */
	bool await(int timeout_ms);


	/** @fn shutdown(int timeout_ms)
	 * Set a flag so that the run loop completes.
	 */
	bool shutdown(int timeout_ms);


private:
	// Declared, but not implemented.
	DispatchShared(const DispatchShared &);
	DispatchShared &operator=(const DispatchShared &);

	gmsec::api::util::AtomicBoolean running;

	// Reference to the connection the dispatcher is working on
	InternalConnection* m_connection;

	// CountDownLatch to signal starting thread that the dispatcher has
	// started up properly.
	gmsec::api::util::CountDownLatch m_startupLatch;

	// Latch through which to indicate completion.
	gmsec::api::util::CountDownLatch m_shutdownLatch;

};


/**
 * @function Dispatcher
 * @brief Entry point for the Dispatcher thread.
 */
void runDispatcher(gmsec::api::util::StdSharedPtr<DispatchShared> shared);


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
