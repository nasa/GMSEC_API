/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_MB_H
#define GMSEC_MB_H

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Status.h>


#ifdef WIN32

	#ifdef GMSEC_MB_EXPORTS
		#define GMSEC_MB_API __declspec(dllexport)
	#else
		#define GMSEC_MB_API __declspec(dllimport)
	#endif

	/* disable invalid STL waring for windows targets */
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else	// All other platforms make this macro invisible

	#define GMSEC_MB_API

#endif


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn CreateConnection(const gmsec::api::Config* config, gmsec::api::internal::ConnectionInterface** connIf)
	 * This function wraps the MBConnection constructor with the standard connection creation function prototype.
	 */
	GMSEC_MB_API void createConnection(const gmsec::api::Config* config, gmsec::api::internal::ConnectionInterface** connIf , gmsec::api::Status* status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
