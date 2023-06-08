/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalAtomics.h
 *
 *  @brief This file contains the definition for a series of Atomic elements.
 */

#ifndef GMSEC_API5_INTERNAL_ATOMICS_H
#define GMSEC_API5_INTERNAL_ATOMICS_H

#include <gmsec5_defs.h>

#include <gmsec5/util/wdllexp.h>

namespace gmsec {
namespace api5 {
namespace internal {

GMSEC_API GMSEC_I32 CALL_TYPE syncAddAndFetch(volatile GMSEC_I32* p, GMSEC_I32 n);

}
}
}

#endif
