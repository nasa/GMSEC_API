/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalAtomics.h
 *
 *  @brief This file contains the definition for a series of Atomic elements.
 *
**/

#ifndef GMSEC_API_INTERNAL_ATOMICS_H
#define GMSEC_API_INTERNAL_ATOMICS_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>

namespace gmsec {
namespace api {
namespace internal {

GMSEC_I32 GMSEC_API syncAddAndFetch(volatile GMSEC_I32* p, GMSEC_I32 n);

}
}
}

#endif
