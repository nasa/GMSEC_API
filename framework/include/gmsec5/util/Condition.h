/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Condition.h
 *
 * @brief This file contains a class for cross platform condition variable support.
 */

#ifndef GMSEC_API5_UTIL_CONDITION_H
#define GMSEC_API5_UTIL_CONDITION_H


#include <gmsec5/util/TimeUtil.h>
#include <gmsec5/util/wdllexp.h>


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
namespace api5
{
	namespace internal
	{
		class InternalCondition;
	}

namespace util
{
	class Mutex;


/**
 * @class Condition
 *
 * @brief A class for cross platform condition variable support.
 */
class GMSEC_API Condition
{
public:

	/**
	 * @brief Basic subset of reasons that can be returned while
	 * awaiting for a result from the %Condition. Users can define
	 * additional custom reason(s) as needed.
	 */
	enum ConditionReason
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
	 * @throw A GmsecException is thrown if the Condition cannot be created.
	 */
	Condition();


	/**
	 * @fn Condition()
	 *
	 * @brief Standard constructor. Initializes the internal platform dependent
	 * modules with an externally managed mutex.
	 *
	 * @throw A GmsecException is thrown if the Condition cannot be created.
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
	Mutex& CALL_TYPE getMutex();


	/**
	 * @fn wait()
	 *
	 * @brief The function will hold at this point until the thread is signaled.
	 */
	int CALL_TYPE wait();


	/**
	 * @fn wait(long millis)
	 *
	 * @brief The function will hold at this point until the thread is signaled or
	 * the time specified has expired.  If millis is not positive, timeout immediately.
	 *
	 * @param millis - The length of time in milliseconds to wait for the thread to
	 * be signalled before continuing.
	 */
	int CALL_TYPE wait(long millis);


	/**
	 * @fn waitUntil(const GMSEC_TimeSpec& t)
	 *
	 * @brief The function will cause the calling thread to wait until
	 * the condition is signalled or the time specified has been reached.
	 *
	 * @param t - The absolute timeout.
	 */
	int CALL_TYPE waitUntil(const GMSEC_TimeSpec& t);


	/**
	 * @fn signal(int reason)
	 *
	 * @brief This function signals any waiting threads to continue with their
	 * activities. 
	 *
	 * Note: Only one thread should be responsible for signalling threads 
	 * waiting on the Condition. 
	 */
	void CALL_TYPE signal(int reason);


	/**
	 * @fn broadcast(int reason)
	 *
	 * @brief This function signals all threads that are waiting on the condition
	 * variable to continue with their activities.
	 * 
	 * Note: Only one thread should be responsible for signalling threads 
	 * waiting on the Condition. 
	 */
	void CALL_TYPE broadcast(int reason);

private:
	// Defined, but not implemented
	Condition(const Condition&);
	Condition& operator=(const Condition&);

	gmsec::api5::internal::InternalCondition* m_internal;
};


} // namespace util
} // namespace api5
} // namespace gmsec


#endif
