/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Mutex.h
 *
 * @brief This file contains a multi-platform abstraction implementation of a recursive Mutex class.
 */

#ifndef GMSEC_API5_UTIL_MUTEX_H
#define GMSEC_API5_UTIL_MUTEX_H

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	namespace internal
	{
		class InternalAutoMutex;
		class InternalMutex;
		class MutexBuddy;
	}

namespace util
{


/**
 * @class Mutex
 *
 * @brief This is the basic implementation of the cross-platform mutex.
 */
class GMSEC_API Mutex
{
public:
	/**
	 * @fn Mutex()
	 *
	 * @brief Basic constructor
	 *
	 * @throw A GmsecException is thrown if the Mutex cannot be created.
	 */
	Mutex();


	/**
	 * @fn ~Mutex()
	 *
	 * @brief Destructor
	 */
	virtual ~Mutex();


	/**
	 * @fn enterMutex()
	 *
	 * @brief Waits until the Mutex is aquired and locks. This is a reentrant function.
	 *
	 * @throw A GmsecException is thrown if the Mutex cannot be locked.
	 */
	void CALL_TYPE enterMutex();


	/**
	 * @fn leaveMutex()
	 *
	 * @brief Releases the mutex. If the mutex has been "entered" multiple times, only one
	 * release is required.
	 *
	 * @throw A GmsecException is thrown if the Mutex cannot be unlocked/released.
	 */
	void CALL_TYPE leaveMutex();


private:
	friend class gmsec::api5::internal::MutexBuddy;

	// Declared, but not implemented
	Mutex(const Mutex &);
	Mutex &operator=(const Mutex &);

	gmsec::api5::internal::InternalMutex* m_internal;
};



/**
 * @class AutoMutex
 *
 * @brief This class implements the Resource Acquisition Is Initialization
 * (RAII) pattern where the resource is ownership of a mutex.
 */
class GMSEC_API AutoMutex
{
public:
	/**
	 * @fn AutoMutex(Mutex& m, bool acquire = true)
	 *
	 * @brief Basic constructor.
	 *
	 * @param m - the Mutex to use.
	 * @param acquire - flag to indicate whether to lock the Mutex upon instantiation of the AutoMutex object; default is true.
	 */
	AutoMutex(Mutex& m, bool acquire = true);


	/**
	 * @fn ~AutoMutex()
	 *
	 * @brief Destructor
	 */
	~AutoMutex();


	/**
	 * @fn enter()
	 *
	 * @brief If the mutex has been manually released, this function is used to reenter.
	 */
	void CALL_TYPE enter();


	/**
	 * @fn leave()
	 *
	 * @brief This function manually releases the mutex.
	 */
	void CALL_TYPE leave();


private:
	// Declared, but not implemented
	AutoMutex();
	AutoMutex(const AutoMutex&);
	AutoMutex& operator= (const AutoMutex&);

	gmsec::api5::internal::InternalAutoMutex* m_internal;
};


} // namespace util
} // namespace api5
} // namespace gmsec

#endif
