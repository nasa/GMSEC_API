/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.jni;

import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.gmsecAPI;

import gov.nasa.gsfc.gmsecapi.util.Log;


/**
 * @class ConnectionFactory

 * @brief This is the class for creating and destroying connections.
 *
 */
public class JNIConnectionFactory
{

	// Singleton class - static methods only
	private JNIConnectionFactory()
	{
	}

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
	public static Status Create(gov.nasa.gsfc.gmsecapi.Config cfg, gov.nasa.gsfc.gmsecapi.Connection conn)
	{
		if (cfg == null)
		{
			return new Status(
					gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
					gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
					"Null Config object");
		}
		if (conn == null)
		{
			return new Status(
					gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
					gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
					"Null Connection object");
		}
		JNIConfig jniCfg = new JNIConfig(cfg);
		JNIConnection jconn = new JNIConnection();
		Status result = new Status();
		try
		{
			long cPtr = gmsecJNI.ConnectionFactory_Create(JNIConfig.getCPtr(jniCfg), jniCfg, jconn);
			result = new JNIStatus(cPtr, true);

			if (result.IsError())
			{
				conn.setImpl(null);
			}
			else
			{
				jconn.setExternal(conn);
				conn.setImpl(jconn);
			}
		}
		catch (Exception e)
		{
			result.Set(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_INVALID_CONNECTION, "Can't load GMSEC JNI integration: " + e.getMessage());
			conn.setImpl(null);
		}
		return result;
	}

	/** 
	 * @brief This function returns a string containing the description of the current API version
	 * @return String - the version string
	 */
	public static String GetAPIVersion()
	{
		return gmsecJNI.ConnectionFactory_GetAPIVersion();
	}

	/**
	 * @brief Allow for the cleanup of ConnectionFactory created Connections
	 * @param conn - the connection object to be destroyed
	 * @return Status - the resulting status of the operation
	 */
	public static Status Destroy(Connection conn)
	{
		if (conn == null || conn.getImpl() == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Null Connection object");
		}

		JNIConnection jconn = conn.getImpl();

		long cPtr = gmsecJNI.ConnectionFactory_Destroy(JNIConnection.getCPtr(jconn), jconn);

		jconn.swigCPtr = 0;

		return new JNIStatus(cPtr, true);
	}
}
