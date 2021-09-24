/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Condition.h
 *
 * @brief This file contains a class for cross platform condition variable support.
 */

#ifndef GMSEC_API_UTIL_CONDITION_H
#define GMSEC_API_UTIL_CONDITION_H


#include <gmsec4/util/TimeUtil.h>


/*

EXAMPLES

internal mutex example

{
	Condition condition;
	Mutex &mutex = condition.getMutex();

	mutex.enterMutex();

	int reason = condition.wait();
	printf("condition got %d\n", reason);

	mutex.leaveMutex();
}


external mutex example
{
	Mutex external;
	Condition condition(external);

	external.enterMutex();

	int reason = condition.wait();
	printf("condition got %d\n", reason);

	external.leaveMutex();
}


If the Mutex class is updated to keep track of its ownership, this can
be made more robust and use less code as:


internal mutex example with AutoMutex

{
	Condition condition;
	AutoMutex hold(condition.getMutex());

	int reason = condition.wait();
	printf("condition got %d\n", reason);
}


external mutex example with AutoMutex
{
	Mutex external;
	Condition condition(external);

	AutoMutex hold(external);

	int reason = condition.wait();
	printf("condition got %d\n", reason);
}

*/


namespace gmsec
{
namespace api
{
	namespace internal
	{
		class InternalCondition;
	}

namespace util
{
	class Mutex;


class GMSEC_API Condition
{
public:

	enum
	{
		INVALID,    ///< INVALID
		TIMEOUT,    ///< TIMEOUT
		USER        ///< USER
	};


	/**
	 * @fn Condition()
	 *
	 * @brief Standard constructor. Initializes the internal platform dependent
	 * modules.
	 *
	 * @throw An Exception is thrown if the Condition cannot be created.
	 */
	Condition();


	/**
	 * @fn Condition()
	 *
	 * @brief Standard constructor. Initializes the internal platform dependent
	 * modules with an externally managed mutex.
	 *
	 * @throw An Exception is thrown if the Condition cannot be created.
	 */
	Condition(Mutex& mutex);


	/**
	 * @fn ~Condition()
	 *
	 * @brief Standard deconstructor. Runs platform dependent tear-down code.
	 */
	~Condition();


	/**
	 * @fn getMutex()
	 *
	 * @brief Returns a reference to the mutex.
	 */
	Mutex& getMutex();


	/**
	 * @fn wait()
	 *
	 * @brief The function will hold at this point until the thread is signaled.
	 */
	int wait();


	/**
	 * @fn wait(long millis)
	 *
	 * @brief The function will hold at this point until the thread is signaled or
	 * the time specified has expired.  If millis is not positive, timeout immediately.
	 *
	 * @param millis - The length of time in milliseconds to wait for the thread to
	 * be signalled before continuing.
	 */
	int wait(long millis);


	/**
	 * @fn waitUntil(const GMSEC_TimeSpec& t)
	 *
	 * @brief The function will cause the calling thread to wait until
	 * the condition is signalled or the time specified has been reached.
	 *
	 * @param t - The absolute timeout.
	 */
	int waitUntil(const GMSEC_TimeSpec& t);


	/**
	 * @fn signal(int reason)
	 *
	 * @brief This function signals any waiting threads to continue with their
	 * activities. 
	 *
	 * Note: Only one thread should be responsible for signalling threads 
	 * waiting on the Condition. 
	 */
	void signal(int reason);


	/**
	 * @fn broadcast(int reason)
	 *
	 * @brief This function signals all threads that are waiting on the condition
	 * variable to continue with their activities.
	 * 
	 * Note: Only one thread should be responsible for signalling threads 
	 * waiting on the Condition. 
	 */
	void broadcast(int reason);

private:
	// Defined, but not implemented
	Condition(const Condition&);
	Condition& operator=(const Condition&);

	gmsec::api::internal::InternalCondition* m_internal;

	// Add padding to ensure binary compatibility
	//
#if defined(_M_IX86) || defined(__i386__) || defined(__i386) || defined(__i486__) || defined(__i486) || defined(i386)
	// 32-bit architecture
	#ifdef WIN32
		char padding[48];   // windows
	#else
		char padding[60];   // non-windows
	#endif
#else
	// 64-bit architecture
	#ifdef WIN32
		char padding[80];   // windows
	#else
		char padding[56];   // non-windows
	#endif
#endif
};


} // namespace util
} // namespace api
} // namespace gmsec


#endif
