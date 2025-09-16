/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file CountDownLatch.h
 *
 *  @brief This file contains a cross-platform CountDownLatch that functions the same
 *	as the Java class of the same name.
 */

#ifndef GMSEC_API5_UTIL_COUNTDOWN_LATCH_H
#define GMSEC_API5_UTIL_COUNTDOWN_LATCH_H

#include <gmsec5/util/Condition.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace util
{

/**
 * @class CountDownLatch
 *
 * @brief This file contains a cross-platform CountDownLatch that functions the same
 * as the Java class of the same name.
 */
class GMSEC_API CountDownLatch
{
public:
	/**
	 * @fn CountDownLatch(int count)
	 *
	 * @brief Constructs a %CountDownLatch object initialized with the given count value.
	 *
	 * @param count - starting count value
	 */
	CountDownLatch(int count);


	/**
	 * @fn ~CountDownLatch()
	 * @brief Destructor.
	 */
	~CountDownLatch();


	/**
	 * @fn await()
	 *
	 * @brief Force the current thread to wait until the latch has counted down to zero.
	 */
	void CALL_TYPE await();


	/**
	 * @fn await(int milliTimeout)
	 *
	 * @brief Force the current thread to wait until the latch has counted down to zero,
	 * or the specified timeout period (in milliseconds) has elapsed.
	 *
	 * @return Returns true of the latch count is zero, or false if a timeout occurred.
	 */
	bool CALL_TYPE await(int milliTimeout);


	/**
	 * @fn countdown()
	 *
	 * @brief Decrement the count of the latch, releasing all waiting threads if the count reaches zero.
	 */
	void CALL_TYPE countDown();


	/**
	 * @fn getCount()
	 *
	 * @brief Returns the current latch count.
	 *
	 * @returns The remaining latch count associated with the %CountDownLatch.
	 */
	int CALL_TYPE getCount();

private:
	int count;
	gmsec::api5::util::Condition condition;
};

}
}
}

#endif
