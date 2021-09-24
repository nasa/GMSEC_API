
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*  wdllexp.h
 *
 *  The following ifdef block is the standard way of creating macros which make exporting
 *  from a DLL simpler. All files within this DLL are compiled with the GMSECAPI_EXPORTS
 *  symbol defined on the command line. this symbol should not be defined on any project
 *  that uses this DLL. This way any other project whose source files include this file see
 *  GMSEC_API functions as being imported from a DLL, wheras this DLL sees symbols
 *  defined with this macro as being exported.
 *
 *  THIS IS ONLY REQUIRED FOR WINDOWS PLATFORMS
 *
*/

#ifndef gmsec_util_wdllexp_h
#define gmsec_util_wdllexp_h

#ifdef WIN32

#ifdef GMSECAPI_EXPORTS
#define GMSEC_API __declspec(dllexport)
#define GMSEC_TEMPLATE
#define CALL_TYPE __cdecl
#else
#define GMSEC_API __declspec(dllimport)
#define GMSEC_TEMPLATE extern
#define CALL_TYPE __cdecl
#endif

/* disable _cdecl warning for windows targets, this warning comes up when the target
   program is set to something other than _cdecl for 'Calling Convention' compiler
   setting.    */
#pragma warning ( disable : 4007 )

/* disable invalid STL warning for windows targets */
#pragma warning ( disable : 4251 )
//disable the compile warning (C4786)---the identifier string
//exceeded the maximum allowable length (usually 255 chars)
//for compilation of debug version (for compilation of release
//version, there is no such warning).
#pragma warning ( disable : 4786 )

#else	// All other platforms make this macro invisible
#define GMSEC_API
#define GMSEC_TEMPLATE
#define CALL_TYPE
#endif	// WIN32

#endif /* gmsec_util_wdllexp_h */

