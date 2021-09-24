/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalMutex.h
 *
 * @brief This file contains a multi-platform abstraction implementation of a recursive Mutex class.
 */
#ifndef GMSEC_API_INTERNAL_MUTEX_H
#define GMSEC_API_INTERNAL_MUTEX_H


// This prevents having to include windows.h
//------------------------------------------
#ifdef WIN32
typedef void * HANDLE;
#else
#include <pthread.h>
#endif


namespace gmsec
{
namespace api
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

	friend class gmsec::api::internal::InternalCondition;

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
	InternalAutoMutex(gmsec::api::util::Mutex& m, bool acquire = true);

	~InternalAutoMutex();

	void enter();

	void leave();

private:
	InternalAutoMutex();
	InternalAutoMutex(const InternalAutoMutex&);
	InternalAutoMutex& operator= (const InternalAutoMutex&);

	gmsec::api::util::Mutex& mutex;
	bool                     owned;
};


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
