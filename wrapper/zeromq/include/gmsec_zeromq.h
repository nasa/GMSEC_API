/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* @file gmsec_zeromq.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#ifndef GMSEC_ZEROMQ_H
#define GMSEC_ZEROMQ_H

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <zmq.h>


// Use an extra level of indirection to expand integer macros before concatenating them
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifdef WIN32

	#include <windows.h>

	#ifdef GMSEC_ZEROMQ_EXPORTS
		#define GMSEC_ZEROMQ_API __declspec(dllexport)
	#else
		#define GMSEC_ZEROMQ_API __declspec(dllimport)
	#endif

	/* disable invalid STL waring for windows targets */
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else // All other platforms make this macro invisible

	#define GMSEC_ZEROMQ_API

#endif


//Constants
//#define ZEROMQ_VERSION_STRING "gmsec_zeromq" STR(ZEROMQ_VERSION_MAJOR) STR(ZEROMQ_VERSION_MINOR) STR(ZEROMQ_VERSION_PATCH)
#define ZEROMQ_REPLY_ADDRESS "ZMQ-REPLY-ADDRESS"


// GMSEC_LIBROOTNAME is defined in the building Makefile or mak/dsp file.
#ifdef GMSEC_LIBROOTNAME
	// This makes a "" string from the given symbol.
	#define makeString(s) (#s)
	// This evaluates the symbol before making a string out of it.
	#define makeStringFromValue(s) makeString(s)
	// That way, I can make a string which has the value of the macro,
	// as opposed to a string which has the value of the macro name.
	// No more "" in Makefiles and/or mak/dsw files.
	#define ZEROMQ_VERSION_STRING makeStringFromValue(GMSEC_LIBROOTNAME)
#else
	// Or, in the case of the original 6.5 release, this.
	#define ZEROMQ_VERSION_STRING "gmsec_zeromq"
#endif


#ifdef __cplusplus
extern "C"
{
#endif

GMSEC_ZEROMQ_API void createConnection(const gmsec::api::Config* config, gmsec::api::internal::ConnectionInterface** connIf , gmsec::api::Status* status);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
