/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_UTIL_CXX_H
#define GMSEC_API5_UTIL_CXX_H

//TODO: Is this still needed??
#ifdef WIN32
    #define GMSEC_CXX_11_AVAILABLE 1
#else
    #define GMSEC_CXX_11_AVAILABLE (__cplusplus >= 201103L)
#endif


#ifdef __linux__
	// Deduce the version of GCC
	#define GCC_VERSION (__GNUC__ * 10000 \
	                     + __GNUC_MINOR__ * 100 \
	                     + __GNUC_PATCHLEVEL__)
#endif

#endif
