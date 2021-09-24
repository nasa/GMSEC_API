/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file gmsec_activemq.h
 * Contains the definitions and functions for dynamic loading of
 * the Active MQ middleware wrapper.
 */

#ifndef GMSEC_ACTIVEMQ_H
#define GMSEC_ACTIVEMQ_H


#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <gmsec4/internal/ConnectionInterface.h>

#ifdef WIN32

	/**
	 * @def GMSEC_ACTIVEMQ_API
	 * Macro to support linking on MS Windows.
	 */
	#ifdef GMSEC_ACTIVEMQ_EXPORTS
		#define GMSEC_ACTIVEMQ_API __declspec(dllexport)
	#else
		#define GMSEC_ACTIVEMQ_API __declspec(dllimport)
	#endif

	/* disable invalid STL waring for windows targets */
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else	// All other platforms make this macro invisible

	#define GMSEC_ACTIVEMQ_API

#endif


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* @brief Create an Active MQ connection.
	*/
	GMSEC_ACTIVEMQ_API void createConnection(const gmsec::api::Config* cfg, gmsec::api::internal::ConnectionInterface** connIf, gmsec::api::Status* status);

#ifdef __cplusplus
}	// extern "C"
#endif


/**
* @def ACTIVEMQVERSIONSTRING
* The name of the Active MQ middleware wrapper library.
*/

//	GMSEC_LIBROOTNAME is defined in the building Makefile or mak/dsp file.
#ifdef GMSEC_LIBROOTNAME

	//	This makes a "" string from the given symbol.
	#define makeString(s) (#s)
	//	This evaluates the symbol before making a string out of it.
	#define makeStringFromValue(s) makeString(s)
	//	That way, I can make a string which has the value of the macro,
	//	as opposed to a string which has the value of the macro name.
	//	No more "" in Makefiles and/or mak/dsw files.
	#define ACTIVEMQVERSIONSTRING makeStringFromValue(GMSEC_LIBROOTNAME)

#else

	#define ACTIVEMQVERSIONSTRING "gmsec_activemq3"

#endif

#endif	// GMSEC_ACTIVEMQ_H
