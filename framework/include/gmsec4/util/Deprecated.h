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

#ifndef GMSEC_API_UTIL_DEPRECATED_H
#define GMSEC_API_UTIL_DEPRECATED_H

#if !defined(GMSEC_DEPRECATED)
	#ifdef __GNUC__

		#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

		#if GCC_VERSION > 40500 || __APPLE__ == 1
			#define GMSEC_DEPRECATED __attribute__((deprecated("The method or function you are attempting to use has been deprecated.  Refer to the API documentation for more details.")))
		#else
			#define GMSEC_DEPRECATED __attribute__((deprecated()))
		#endif

		#if GCC_VERSION > 40600 || __APPLE__ == 1
			#define GMSEC_DISABLE_DEPRECATED_WARNINGS \
				_Pragma("GCC diagnostic push") \
				_Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")

			#define GMSEC_ENABLE_DEPRECATED_WARNINGS \
				_Pragma("GCC diagnostic pop")
		#else
			#define GMSEC_DISABLE_DEPRECATED_WARNINGS
			#define GMSEC_ENABLE_DEPRECATED_WARNINGS
		#endif

	#elif defined(_MSC_VER)

		#define GMSEC_DEPRECATED __declspec(deprecated("The method or function you are attempting to use has been deprecated.  Refer to the API documentation for more details."))

		#define GMSEC_DISABLE_DEPRECATED_WARNINGS
		#define GMSEC_ENABLE_DEPRECATED_WARNINGS

	#else

		#define GMSEC_DEPRECATED
		#define GMSEC_DISABLE_DEPRECATED_WARNINGS
		#define GMSEC_ENABLE_DEPRECATED_WARNINGS

	#endif
#endif

#endif
