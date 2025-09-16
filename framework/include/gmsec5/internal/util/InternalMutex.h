/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalMutex.h
 *
 * @brief This file contains a multi-platform abstraction implementation of a recursive Mutex class.
 */

#ifndef GMSEC_API5_INTERNAL_MUTEX_H
#define GMSEC_API5_INTERNAL_MUTEX_H


// This prevents having to include windows.h
//------------------------------------------
#ifdef WIN32
typedef void * HANDLE;
#else
#include <pthread.h>
#endif


namespace gmsec
{
namespace api5
{
	namespace util
	{
		class Mutex;
	}

namespace internal
{
	class InternalCondition;


/**
 * @class InternalMutex
 *
 * @brief This is the basic implementation of the cross-platform mutex
 */
class InternalMutex
{
public:
	InternalMutex();

	virtual ~InternalMutex();

	void enterMutex();

	void leaveMutex();

private:
	InternalMutex(const InternalMutex &);
	InternalMutex &operator=(const InternalMutex &);

	friend class gmsec::api5::internal::InternalCondition;

#ifdef WIN32
	HANDLE& getHandle();
#else
	pthread_mutex_t& getHandle();
#endif

	int  getDepth() const;
	void incrDepth();
	void decrDepth();


#if defined (WIN32)
	HANDLE          m_handle;
#else
	pthread_mutex_t m_handle;
#endif
	volatile int    m_depth;
};



/**
 * @class InternalAutoMutex
 *
 * @brief This class implements the Resource Acquisition Is Initialization
 * (RAII) pattern where the resource is ownership of a mutex.
 */
class InternalAutoMutex
{
public:
	InternalAutoMutex(gmsec::api5::util::Mutex& m, bool acquire = true);

	~InternalAutoMutex();

	void enter();

	void leave();

private:
	InternalAutoMutex();
	InternalAutoMutex(const InternalAutoMutex&);
	InternalAutoMutex& operator= (const InternalAutoMutex&);

	gmsec::api5::util::Mutex& mutex;
	bool                      owned;
};


} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
