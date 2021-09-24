
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file gmsec_void.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#ifndef _gmsec_void_h_
#define _gmsec_void_h_

#ifdef WIN32
#ifdef GMSEC_VOID_EXPORTS
#define GMSEC_VOID_API __declspec(dllexport)
#else
#define GMSEC_VOID_API __declspec(dllimport)
#endif
/* disable invalid STL waring for windows targets */
#pragma warning ( disable : 4251 )
#pragma warning ( disable : 4786 )
#else	// All other platforms make this macro invisible
#define GMSEC_VOID_API
#endif

#include <gmsec_cpp.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @fn CreateConnection(Config *cfg, Connection **conn)
 * This function wraps the VoidConnection constructor with the standard
 * connection creation function prototype.
 *
 * @sa ConnectionFactory::Create(Config *cfg, Connection *&conn)
 */
GMSEC_VOID_API gmsec::Status
*CreateConnection(gmsec::Config *cfg, gmsec::Connection **conn);

/** @fn DestroyConnection(Connection *conn)
 * This function wraps the VoidConnection destructor with the standard
 * connection deletion function prototype.
 *
 * @sa ConnectionFactory::Destroy(Connection *conn)
 */
GMSEC_VOID_API void
DestroyConnection(gmsec::Connection *conn);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_void_h */

