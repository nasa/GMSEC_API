/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_INTERNAL_MUTEX_BUDDY_H
#define GMSEC_API5_INTERNAL_MUTEX_BUDDY_H

#include <gmsec5/internal/util/InternalMutex.h>

#include <gmsec5/util/Mutex.h>


namespace gmsec
{
namespace api5
{
namespace internal
{


class MutexBuddy
{
public:
	static InternalMutex& getInternal(const gmsec::api5::util::Mutex& m)
	{
		return *m.m_internal;
	}
};


}  // namespace internal
}  // namespace api5
}  // namespace gmsec

#endif
