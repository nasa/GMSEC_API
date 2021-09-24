/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file InternalAtomics.cpp
 *
 *  @brief This file contains the definition for a series of Atomic elements.
 *
**/

#include <gmsec/internal/InternalAtomics.h>
#include <gmsec/util/Mutex.h>

#if defined (WIN32)
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <libkern/OSAtomic.h>
#endif


static gmsec::util::Mutex& get_sync_mutex(size_t s)
{
	const unsigned COUNT = 7;
	static gmsec::util::Mutex mutexi[COUNT];
	return mutexi[s % COUNT];
}


#if defined(__GNUC__) && !defined(__APPLE__)
/*
Not all operations are supported by all target processors. If a particular operation
cannot be implemented on the target processor, a warning will be generated and a
call an external function will be generated. The external function will carry the
same name as the builtin, with an additional suffix `_n' where n is the size of the
data type.
*/
GMSEC_I32 __sync_add_and_fetch_4(GMSEC_I32 *p, GMSEC_I32 n)
{
	gmsec::util::AutoMutex hold(get_sync_mutex(size_t(p)));
	*p += n;
	return *p;
}
#endif /* __GNUC__ */


GMSEC_I32 gmsec::internal::syncAddAndFetch(volatile GMSEC_I32* p, GMSEC_I32 n)
{
	GMSEC_I32 o;

#ifdef __GNUC__
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif
#endif

#if defined(__APPLE__)

    o = __atomic_add_fetch(p, n, __ATOMIC_SEQ_CST);

#elif GCC_VERSION >= 40100 && !defined(__hppa)

	o = __sync_add_and_fetch(p, n);

#elif defined(WIN32)

	o = InterlockedExchangeAdd(p, n) + n;

#else

	gmsec::util::AutoMutex hold(get_sync_mutex(size_t(p)));
	*p += n;
	o = *p;

#endif

	return o;
}
