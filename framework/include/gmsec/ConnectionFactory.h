
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file ConnectionFactory.h
 *
 *  @brief This file contains the class responsible for creation/destruction of gmsec::Connection objects
 *
 *
**/

#ifndef gmsec_ConnectionFactory_h
#define gmsec_ConnectionFactory_h


#include <gmsec/Config.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/sysutil.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

class Connection;

/** @class ConnectionFactory
 * @brief This class contains static methods for the creation/destruction of connection objects
 *
 * This class is NOT an abstract base class, it will have complete implementation that
 * would call the appropriate Connection object constructor for the requested connection.
 *
 * The create function returns a Connection object associated with the determined connection
 * type but does not actually establish the connection. The Connection object method Connection::Connect()
 * is required to establish the software bus connection.
 *
*/
class GMSEC_API ConnectionFactory
{
public:

	/** @fn GetAPIVersion()
	 *
	 * @brief This function returns a string containing the description of the current API version
	 *
	 * @return the current GMSEC API version string
	 */
	GMSEC_DEPRECATED static const char * CALL_TYPE GetAPIVersion();

	/** @fn Create(GMSEC_CONNECTION_TYPE connectionType, Config *cfg, Connection *&conn)
	 *
	 * @brief This function is used to create a connection when a connection type identifier is available.
	 * @note <b>Connection creation by middleware ID has been deprecated, please use Create(Config *cfg, Connection *&conn)</b>
	 *
	 * @param connectionType - connection type identifier
	 * @param cfg - config object specifying connection paramters
	 * @param conn - out parameter, created connection
	 * @return status - result of the operation
	 */
	GMSEC_DEPRECATED static Status CALL_TYPE Create(GMSEC_CONNECTION_TYPE connectionType, Config *cfg, Connection *&conn);

	/** @fn Create(Config *cfg, Connection *&conn)
	 *
	 * @brief This function is used to create a connection, although the connection type
	  * is inside the Config as an item named "connectionType" and
	  * whose value is the root library name for a middleware.
	  *
	  * For example, the icsswb can be designated by the following:
	  * @code
	 * // create config
	 * Config cfg;
	 * // add connectionType to config
	 * result = cfg.AddValue("connectionType","gmsec_bolt");
	  * if( result.isError() )
	  *	//handle error
	  *
	  * // create connection
	 * result = ConnectionFactory::Create(cfg,&conn);
	  * if( result.isError() )
	  *	//handle error
	 *
	 * // do something
	 *
	 * // destroy connection
	 * result = ConnectionFactory::Destroy(conn);
	  * if( result.isError() )
	  *	//handle error
	  * @endcode
	 *
	 * @param cfg - config object specifying connection parameters
	 * @param conn - out parameter, created connection
	 * @return status - result of the operation
	 *
	 * @note Status results for any error will have a class code of GMSEC_STATUS_FACTORY_ERROR (DR 2046).
	 */
	GMSEC_DEPRECATED static Status CALL_TYPE Create(Config *cfg, Connection *&conn);

	/** @fn Destroy(Connection *conn)
	 * @brief This function is used to destroy a connection
	 *
	 * @param conn - connection to destroy
	 * @return status - result of the operation
	 *
	 * @note Status results for any error will have a class code of GMSEC_STATUS_FACTORY_ERROR (DR 2046).
	 */
	GMSEC_DEPRECATED static Status CALL_TYPE Destroy(Connection *conn);

	/** @fn shutdownAllMiddlewares()
	* @brief Calls shutdown routines for each middleware that has a shutdown routine
	*   registered.  If the status returned here is an error, it
	*   is not guaranteed that all shutdown routines were called.
	*/
	GMSEC_DEPRECATED static Status ShutdownAllMiddlewares();

	/** @fn shutdownMiddleware(name)
	* @brief Calls the shutdown routine for the middleware with the given name.
	* @param name A string representing the library name of the gmsec wrapper for
	*   the middleware to shutdown; e.g., "gmsec_mb".
	*/
	GMSEC_DEPRECATED static Status ShutdownMiddleware(const char *name);

};

} // namespace gmsec

#endif /* gmsec_ConnectionFactory_h */
