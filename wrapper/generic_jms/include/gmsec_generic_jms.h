/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**  @file gmsec_generic_jms.h
 *
 *   @brief This file contains the definitions and methods for dynamic runtime loading of this dll.
 */

#ifndef GMSEC_GENERIC_JMS_H
#define GMSEC_GENERIC_JMS_H

#ifdef WIN32

	#include <windows.h>

	#ifdef GMSEC_MW_EXPORTS
		#define GMSEC_MW_API __declspec(dllexport)
	#else
		#define GMSEC_MW_API __declspec(dllimport)
	#endif

	/* disable invalid STL waring for windows targets */
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else

	#define GMSEC_MW_API

#endif


//	GMSEC_LIBROOTNAME is defined in the building Makefile
#ifdef GMSEC_LIBROOTNAME
//	This makes a "" string from the given symbol.
#define makeString(s) (#s)
//	This evaluates the symbol before making a string out of it.
#define makeStringFromValue(s) makeString(s)
//	That way, I can make a string which has the value of the macro,
//	as opposed to a string which has the value of the macro name.
//	No more "" in Makefiles and/or mak/dsw files.
#define JMS_VERSION_STRING makeStringFromValue(GMSEC_LIBROOTNAME)
#else
#define GMSEC_LIBROOTNAME "gmsec_generic_jms"
#define JMS_VERSION_STRING "gmsec_generic_jms"
#endif


#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>


#ifdef __cplusplus
extern "C"
{
#endif

GMSEC_MW_API void createConnection(const gmsec::api::Config* cfg,
                                                  gmsec::api::internal::ConnectionInterface** connIf , gmsec::api::Status* status);

#ifdef __cplusplus
}	// extern "C"
#endif


#endif // GMSEC_GENERIC_JMS_H
