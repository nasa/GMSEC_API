/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**  @file gmsec_websphere.h
 *
 *   @author Matthew Handy
 *   @date February 20, 2009
 *
 *   @brief This file contains the definitions and methods for dynaminc runtime loading of this dll.
 *
 */

#ifndef GMSEC_WEBSPHERE_H
#define GMSEC_WEBSPHERE_H

#ifdef WIN32

	#include <windows.h>

	#ifdef GMSEC_WEBS_EXPORTS
		#define GMSEC_WEBS_API __declspec(dllexport)
	#else
		#define GMSEC_WEBS_API __declspec(dllimport)
	#endif

	/* disable invalid STL waring for windows targets */
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else	// All other platforms make this macro invisible

	#define GMSEC_WEBS_API

#endif

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Create a WebSphere connection.
	 */
	GMSEC_WEBS_API void createConnection(const gmsec::api::Config* config, gmsec::api::internal::ConnectionInterface** connIf , gmsec::api::Status* status);

#ifdef __cplusplus
}	// extern "C"
#endif


//	GMSEC_LIBROOTNAME is defined in the building Makefile or mak/dsp file.
#ifdef GMSEC_LIBROOTNAME

	//	This makes a "" string from the given symbol.
	#define makeString(s) (#s)

	//	This evaluates the symbol before making a string out of it.
	#define makeStringFromValue(s) makeString(s)

	//	That way, I can make a string which has the value of the macro,
	//	as opposed to a string which has the value of the macro name.
	//	No more "" in Makefiles and/or mak/dsw files.
	#define WEBSPHERE_VERSION_STRING makeStringFromValue(GMSEC_LIBROOTNAME)

#else

	//	Or, in the case of the original 6.5 release, this.
	#define WEBSPHERE_VERSION_STRING "gmsec_websphere7"

#endif


#endif	// GMSEC_WEBSPHERE_H
