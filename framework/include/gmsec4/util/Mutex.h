/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Mutex.h
 *
 * @brief This file contains a multi-platform abstraction implementation of a recursive Mutex class.
 */

#ifndef GMSEC_API_UTIL_MUTEX_H
#define GMSEC_API_UTIL_MUTEX_H


#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
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
 * @brief This is the basic implementation of the cross-platform mutex
 */
class GMSEC_API Mutex
{
public:
	/**
	 * @fn Mutex()
	 *
	 * @brief Basic constructor
	 *
	 * @throw An Exception is thrown if the Mutex cannot be created.
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
	 * @throw An Exception is thrown if the Mutex cannot be locked.
	 */
	void CALL_TYPE enterMutex();


	/**
	 * @fn leaveMutex()
	 *
	 * @brief Releases the mutex. If the mutex has been "entered" multiple times, only one
	 * release is required.
	 *
	 * @throw An Exception is thrown if the Mutex cannot be unlocked/released.
	 */
	void CALL_TYPE leaveMutex();


private:
	friend class gmsec::api::internal::MutexBuddy;

	// Declared, but not implemented
	Mutex(const Mutex &);
	Mutex &operator=(const Mutex &);

	gmsec::api::internal::InternalMutex* m_internal;

	// Add padding to ensure binary compatibility
	//
#if defined(_M_IX86) || defined(__i386__) || defined(__i386) || defined(__i486__) || defined(__i486) || defined(i386)
	// 32-bit architecture
	#ifdef WIN32
		char padding[8];    // windows
	#else
		char padding[28];   // non-windows
	#endif
#else
	// 64-bit architecture
	#ifdef WIN32
		char padding[16];   // windows
	#else
		char padding[48];   // non-windows
	#endif
#endif

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
	void enter();


	/**
	 * @fn leave()
	 *
	 * @brief This function manually releases the mutex.
	 */
	void leave();


private:
	// Declared, but not implemented
	AutoMutex();
	AutoMutex(const AutoMutex&);
	AutoMutex& operator= (const AutoMutex&);

	gmsec::api::internal::InternalAutoMutex* m_internal;

	// Add padding to ensure binary compatibility
	//
#if defined(_M_IX86) || defined(__i386__) || defined(__i386) || defined(__i486__) || defined(__i486) || defined(i386)
	// 32-bit architecture
	char padding[4];
#else
	// 64-bit architecture
	char padding[8];
#endif
};


} // namespace util
} // namespace api
} // namespace gmsec

#endif
