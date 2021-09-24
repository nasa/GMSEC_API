
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Mutex.h
 *
 *  @brief This file contains a multi-platform abstraction implementation of a recursive Mutex class.
 *
 *
**/
#ifndef gmsec_util_Mutex_h
#define gmsec_util_Mutex_h


#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>

// This prevents having to include windows.h
//------------------------------------------
#ifdef WIN32
typedef void * HANDLE;
#else
#include <pthread.h>
#endif


namespace gmsec
{

namespace util
{
class Condition;
}

namespace util
{

/**	@class Mutex
 *
 *	@brief This is the basic implementation of the cross-platform mutex
 *
**/
class GMSEC_API Mutex
{
private:
	Mutex(const Mutex &);
	Mutex &operator=(const Mutex &);

	friend class gmsec::util::Condition;

#if defined (WIN32)
	HANDLE fHandle;
#else
	pthread_mutex_t fHandle ;
#endif

	int fDepth;

	/** @fn decrDepth()
	 * @brief Decrement the depth counter by 1.
	**/
	void CALL_TYPE decrDepth();

	/** @fn incrDepth()
	 * @brief Increment the depth counter.
	**/
	void CALL_TYPE incrDepth();

public:
	GMSEC_DEPRECATED Mutex();
	virtual ~Mutex();

	/**	@fn enterMutex()
	 *
	 *	@brief Waits until the Mutex is aquired and locks. This is a reentrant function.
	 *
	**/
	void CALL_TYPE enterMutex();

	/**	@fn leaveMutex()
	 *
	 *	@brief Releases the mutex. If the mutex has been "entered" multiple times, only one
	 *	release is required.
	 *
	**/
	void CALL_TYPE leaveMutex();
};



/**	@class AutoMutex
 *
 *	@brief This class implements the Resource Acquisition Is Initialization
 *	(RAII) pattern where the resource is ownership of a mutex.
**/
class GMSEC_API AutoMutex
{
public:
	GMSEC_DEPRECATED AutoMutex(Mutex &m, bool acquire = true);
	~AutoMutex();

	/**	@fn enter()
	 *
	 *	@brief If the mutex has been manually released, this function is used to reenter.
	 *
	**/
	void enter();

	/**	@fn leave()
	 *
	 *	@brief This function manually releases the mutex.
	 *
	**/
	void leave();


private:
	AutoMutex();
	AutoMutex& operator= (const AutoMutex &other);

	Mutex &mutex;
	bool owned;
};


} // namespace util
} // namespace gmsec

#endif /* gmsec_util_Mutex_h */
