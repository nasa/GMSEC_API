/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





package gov.nasa.gsfc.gmsecapi;


/**
 * @class ConnectionFactory
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is the class for creating and destroying connections.
 *
 */
@Deprecated
public class ConnectionFactory
{
	// Do not allow instantiations of this class.
	private ConnectionFactory() {}

	/**
	 * @brief The create function returns a Connection object associated with the determined
	 * connection type but does not actually establish the connection.  The Connection
	 * object method connect() is required to establish the software bus connection.
	 *
	 * The advantage of using a create function, or factory class is that the parameters can
	 * be stored in a configuration file, environmental variable, or passed by the command
	 * line parameter without additional coding.  This allows for easy configuration of
	 * which middleware implemenation to use.
	 *
	 * @param cfg - A Config container specifying named parameters and values parameter strings.  The config
	 *              must specify a parameter or enumeration for the connection type.
	 * @param conn - the created connection object, if the create was successful
	 * @return Status - the resulting status of the operation
	 */
	public static Status Create(Config cfg, Connection conn)
	{
		if (cfg == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FACTORY_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Config is null");
		}
		if (conn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FACTORY_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection is null");
		}

		return gov.nasa.gsfc.gmsecapi.jni.JNIConnectionFactory.Create(cfg, conn);
	}


	/**
	 * @brief Allow for the cleanup of ConnectionFactory created Connections
	 * @param conn - the connection object to be destroyed
	 * @return Status - the resulting status of the operation
	 */
	public static Status Destroy(Connection conn)
	{
		if (conn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FACTORY_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection is null");
		}

		return gov.nasa.gsfc.gmsecapi.jni.JNIConnectionFactory.Destroy(conn);
	}


	/** 
	 * @brief This function returns a string containing the description of the current API version
	 * @return String - the version string
	 */
	public static String GetAPIVersion()
	{
		return gov.nasa.gsfc.gmsecapi.ApiVersion.GMSEC;
	}
}
