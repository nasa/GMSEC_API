/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef gmsec_bolt_h
#define gmsec_bolt_h

/*
 * C interface for creating and destroying Bolt server connections.
 */

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <gmsec4/internal/ConnectionInterface.h>

#include <stdio.h>

#ifndef GMSEC_LIBROOTNAME
#define GMSEC_LIBROOTNAME gmsec_bolt
#endif


#ifdef WIN32

#ifdef GMSEC_MW_EXPORTS
#define GMSEC_MW_API __declspec(dllexport)
#else
#define GMSEC_MW_API __declspec(dllimport)
#endif

#else /* WIN32 */

#define GMSEC_MW_API

#endif /* WIN32 */



namespace gmsec_bolt {


const int DEFAULT_PORT = 9100;


} // namespace gmsec_bolt


extern "C" {

/**
 * @brief Create a Bolt connection.
 */
GMSEC_MW_API void createConnection(const gmsec::api::Config* cfg, gmsec::api::internal::ConnectionInterface** connIf, gmsec::api::Status* status);

}



//	GMSEC_LIBROOTNAME is defined in the building Makefile or mak/dsp file.
#ifdef GMSEC_LIBROOTNAME
//	This makes a "" string from the given symbol.
#define makeString(s) (#s)
//	This evaluates the symbol before making a string out of it.
#define makeStringFromValue(s) makeString(s)
//	That way, I can make a string which has the value of the macro,
//	as opposed to a string which has the value of the macro name.
//	No more "" in Makefiles and/or mak/dsw files.
#define GMSEC_MW_VERSION_STRING makeStringFromValue(GMSEC_LIBROOTNAME)
#endif



#endif /* gmsec_bolt_h */

