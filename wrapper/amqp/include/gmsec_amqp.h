/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* @file gmsec_amqp.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#ifndef GMSEC_AMQP_H
#define GMSEC_AMQP_H

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <proton/version.h>


// Use an extra level of indirection to expand integer macros before concatenating them
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifdef WIN32

	#include <windows.h>

	#ifdef GMSEC_AMQP_EXPORTS
		#define GMSEC_AMQP_API __declspec(dllexport)
	#else
		#define GMSEC_AMQP_API __declspec(dllimport)
	#endif

	/* disable invalid STL waring for windows targets */
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else	// All other platforms make this macro invisible

	#define GMSEC_AMQP_API

#endif


#define AMQP_VERSION_STRING "gmsec_amqp1 (Qpid Proton version " \
	STR(PN_VERSION_MAJOR) "." STR(PN_VERSION_MINOR) "." STR(PN_VERSION_POINT) ")"


#ifdef __cplusplus
extern "C"
{
#endif

GMSEC_AMQP_API void createConnection(const gmsec::api::Config* config,
                                                    gmsec::api::internal::ConnectionInterface** connIf , gmsec::api::Status* status);

#ifdef __cplusplus
}	// extern "C"
#endif


#endif // GMSEC_AMQP_H
