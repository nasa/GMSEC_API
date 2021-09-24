/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API_UTIL_CXX_H
#define GMSEC_API_UTIL_CXX_H

#ifdef WIN32
    #define GMSEC_CXX_11_AVAILABLE 1
#else
    #define GMSEC_CXX_11_AVAILABLE (__cplusplus >= 201103L)
#endif

#endif
