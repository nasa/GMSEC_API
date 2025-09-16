/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/*
 * @file gmsec_loopback.h
 * @brief This file provides a template for implementing a middleware wrapper.
 */

#ifndef GMSEC_LOOPBACK_H
#define GMSEC_LOOPBACK_H

#include <gmsec5/internal/ConnectionInterface.h>

#include <gmsec5/Config.h>
#include <gmsec5/Status.h>


// Use an extra level of indirection to expand integer macros before concatenating them
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifdef WIN32

	#include <windows.h>

	#ifdef GMSEC_LOOPBACK_EXPORTS
		#define GMSEC_LOOPBACK_API __declspec(dllexport)
	#else
		#define GMSEC_LOOPBACK_API __declspec(dllimport)
	#endif

	/* disable invalid STL waring for windows targets */
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else	// All other platforms make this macro invisible

	#define GMSEC_LOOPBACK_API

#endif


#define LOOPBACK_VERSION_STRING "gmsec_loopback"


#ifdef __cplusplus
extern "C"
{
#endif

GMSEC_LOOPBACK_API void createConnection(const gmsec::api5::Config* config,
                                         gmsec::api5::internal::ConnectionInterface** connIf ,
                                         gmsec::api5::Status* status);

#ifdef __cplusplus
}	// extern "C"
#endif


#endif // GMSEC_LOOPBACK_H
