/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file Deprecated.h
 *
 * @brief Contains macros which can be used to mark classes, methods, and functions
 *        as deprecated.
 */

#ifndef GMSEC_DEPRECATED_H
#define GMSEC_DEPRECATED_H

#ifdef __GNUC__

        #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

        #if GCC_VERSION > 40500
                #define GMSEC_DEPRECATED __attribute__((deprecated("API 3.x has been deprecated; use API 4.x instead.")))
        #else
                #define GMSEC_DEPRECATED __attribute__((deprecated()))
        #endif

#elif defined(_MSC_VER)

	#define GMSEC_DEPRECATED __declspec(deprecated("API 3.x has been deprecated; use API 4.x instead."))

#else

	#pragma message("WARNING: API 3.x has been deprecated; use API 4.x instead.")
	#define GMSEC_DEPRECATED

#endif

#endif
