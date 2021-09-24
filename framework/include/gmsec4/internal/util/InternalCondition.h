/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalCondition.h
 *
 *  @brief This file contains a class for cross platform condition variable support
 */

#ifndef GMSEC_API_UTIL_INTERNAL_CONDITION_H
#define GMSEC_API_UTIL_INTERNAL_CONDITION_H


#include <gmsec4/util/TimeUtil.h>


#if defined (WIN32)
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif


namespace gmsec
{
namespace api
{
	namespace util
	{
		// Forward declaration(s)
		class Mutex;
	}

namespace internal
{


class InternalCondition
{
public:
	InternalCondition();


	InternalCondition(gmsec::api::util::Mutex& mutex);


	~InternalCondition();


	gmsec::api::util::Mutex& getMutex();


	int wait();


	int wait(long millis);


	int waitUntil(const GMSEC_TimeSpec& t);


	void signal(int reason);


	void broadcast(int reason);

private:
	// Defined, but not implemented
	InternalCondition(const InternalCondition&);
	InternalCondition& operator=(const InternalCondition&);

	inline int getStatus() const { return status; }
	inline void setStatus(int s) { status = s; }


	volatile int             status;
	bool                     ownsMutex;
	gmsec::api::util::Mutex* mutex;

#if defined (WIN32)
	// Number of waiting threads.
	int waiters_count_;

	CRITICAL_SECTION waiters_count_lock_;

	// Semaphore used to queue up threads waiting for the condition to
	// become signaled.
	HANDLE sema_;

	// An auto-reset event used by the broadcast/signal thread to wait
	// for all the waiting thread(s) to wake up and be released from the
	// semaphore.
	HANDLE waiters_done_;

	// Keeps track of whether we were broadcasting or signaling.  This
	// allows us to optimize the code if we're just signaling.
	int was_broadcast_;

#else
	pthread_cond_t cv;
#endif
};


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
