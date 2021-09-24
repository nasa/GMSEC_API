/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_OPENDDS_H
#define GMSEC_OPENDDS_H

#ifdef WIN32

	// Macro to support linking on MS Windows

	#ifdef GMSEC_OPENDDS_EXPORTS
		#define GMSEC_OPENDDS_API __declspec(dllexport)
	#else
		#define GMSEC_OPENDDS_API __declspec(dllimport)
	#endif

	// disable invalid STL waring for windows targets
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else	// All other platforms make this macro invisible

	#define GMSEC_OPENDDS_API

#endif


#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <gmsec4/internal/ConnectionInterface.h>


#ifdef __cplusplus
extern "C"
{
#endif

	// Create an OpenDDS connection.
	GMSEC_OPENDDS_API void createConnection(const gmsec::api::Config* cfg, gmsec::api::internal::ConnectionInterface** connIf, gmsec::api::Status* status);

#ifdef __cplusplus
}	// extern "C"
#endif


// The name of the OpenDDS middleware wrapper library.

//	GMSEC_LIBROOTNAME is defined in the building Makefile or mak/dsp file.
#ifdef GMSEC_LIBROOTNAME

	//	This makes a "" string from the given symbol.
	#define makeString(s) (#s)
	//	This evaluates the symbol before making a string out of it.
	#define makeStringFromValue(s) makeString(s)
	//	That way, I can make a string which has the value of the macro,
	//	as opposed to a string which has the value of the macro name.
	//	No more "" in Makefiles and/or mak/dsw files.
	#define OPENDDS_LIBROOTNAME makeStringFromValue(GMSEC_LIBROOTNAME)

#else

	#define OPENDDS_LIBROOTNAME "gmsec_opendds"

#endif

//	GMSEC_LIBVERSION is defined in the building Makefile or mak/dsp file.
#ifdef GMSEC_LIBVERSION

	//	This makes a "" string from the given symbol.
#define makeString(s) (#s)
//	This evaluates the symbol before making a string out of it.
#define makeStringFromValue(s) makeString(s)
//	That way, I can make a string which has the value of the macro,
//	as opposed to a string which has the value of the macro name.
//	No more "" in Makefiles and/or mak/dsw files.
#define OPENDDS_VERSION_STRING makeStringFromValue(GMSEC_LIBVERSION)

#else

#define OPENDDS_VERSION_STRING "gmsec_opendds"

#endif

#endif
