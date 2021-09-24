
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file connection_factory.h
 *
 *  @brief This file contains functions that are global to the C API, such as connection creation functions.
 *
 *	Example use:
 *	@code
 *	// Output the GMSEC API version
 *	printf("%s\n",GetAPIVersion());
 *	@endcode
 *
 *
**/

#ifndef gmsec_c_connection_factory_h
#define gmsec_c_connection_factory_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn gmsec_GetAPIVersion()
	 *
	 * @brief This function will return the version string for the current GMSEC API.
	 *
	 * @return the current GMSEC API version string
	 */
	GMSEC_DEPRECATED GMSEC_API const char * CALL_TYPE gmsec_GetAPIVersion();

	/** @fn gmsec_CreateConnection(GMSEC_CONNECTION_TYPE connectionType, GMSEC_CONFIG_OBJECT config, GMSEC_CONNECTION_OBJECT *conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function is used to create a connection when a connection type identifier is available.
	 * @note <b>Connection creation by middleware ID has been deprecated, please use CreateConnectionForConfig()</b>
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] connectionType - type
	 * @param[in] config - Config Handle
	 * @param[out] conn - Connection Handle
	 * @param[out] status - operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_CreateConnection(GMSEC_CONNECTION_TYPE connectionType, GMSEC_CONFIG_OBJECT config, GMSEC_CONNECTION_OBJECT *conn, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_CreateConnectionForType(GMSEC_CONNECTION_TYPE connectionType, GMSEC_CONFIG_OBJECT config, GMSEC_CONNECTION_OBJECT *conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function is used to create a connection when a connection type identifier is available.
	 * @note <b>Connection creation by middleware ID has been deprecated, please use CreateConnectionForConfig()</b>
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] connectionType - type
	 * @param[in] config - Config Handle
	 * @param[out] conn - Connection Handle
	 * @param[out] status - operation status
	 *
	 * @sa gmsec_DestroyConnection()
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_CreateConnectionForType(GMSEC_CONNECTION_TYPE connectionType, GMSEC_CONFIG_OBJECT config, GMSEC_CONNECTION_OBJECT *conn, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_CreateConnectionForConfig(GMSEC_CONFIG_OBJECT config, GMSEC_CONNECTION_OBJECT *conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function is used to create a connection, although the connection type
	 * is inside the Config as an item named "connectionType" and
	 * whose value is the root library name for a middleware.
	 *
	 * For example, the icsswb can be designated by the following:
	 * @code
	 * GMSEC_STATUS_OBJECT result = NULL;
	 * GMSEC_CONNECTION_OBJECT conn = NULL;
	 * GMSEC_CONFIG_OBJECT cfg = NULL;
	 *
	 * // create status
	 * gmsec_CreateStatus(&result);
	 *
	 * // create config
	 * gmsec_CreateConfig(&cfg,result);
	 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
	 *	//handle error
	 *
	 * // add connectionType to config
	 * gmsec_ConfigAddValue(cfg,"connectionType","gmsec_icsswb",result);
	 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
	 *	//handle error
	 *
	 * // create connection
	 * gmsec_CreateConnectionForConfig(cfg, &conn, result);
	 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
	 *	//handle error
	 *
	 * // do something
	 *
	 * // destroy connection
	 * gmsec_DestroyConnection(conn,result);
	 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
	 *	//handle error
	 * @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] config - Config Handle
	 * @param[out] conn - Connection Handle
	 * @param[out] status - operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_CreateConnectionForConfig(GMSEC_CONFIG_OBJECT config, GMSEC_CONNECTION_OBJECT *conn, GMSEC_STATUS_OBJECT status);


	/** @fn DestroyConnection(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function is used to destroy a connection
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in,out] conn - on input a handle to the Connection.  On output an empty Connection
	 * @param[out] status - operation status
	 *
	 * @sa gmsec_CreateConnection()
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_DestroyConnection(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_c_connection_factory_h */
