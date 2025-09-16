/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalAtomics.cpp
 *
 *  @brief This file contains the definition for a series of Atomic elements.
 */

#include <gmsec5/internal/InternalAtomics.h>

#include <gmsec5/util/cxx.h>
#include <gmsec5/util/Mutex.h>

#include <stddef.h>   // for size_t

#if defined (WIN32)
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <libkern/OSAtomic.h>
#endif


using namespace gmsec::api5::util;

namespace gmsec
{
namespace api5
{
namespace internal
{

GMSEC_I32 syncAddAndFetch(volatile GMSEC_I32* p, GMSEC_I32 n)
{
	GMSEC_I32 o;

#if defined(__APPLE__)

	o = __atomic_add_fetch(p, n, __ATOMIC_SEQ_CST);

#elif GCC_VERSION >= 40100

	o = __sync_add_and_fetch(p, n);

#elif defined(WIN32)

	o = InterlockedExchangeAdd(p, n) + n;

#endif

	return o;
}


}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec
