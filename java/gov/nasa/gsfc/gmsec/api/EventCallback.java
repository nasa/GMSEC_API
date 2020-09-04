/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file EventCallback.java
 *
 * @brief Interface for receiving event information via a callback.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNIConnection;
import gov.nasa.gsfc.gmsec.api.jni.JNIEventCallback;


/**
 * @class EventCallback
 *
 * @brief This class is the abstract base class for event callbacks. A user created class,
 * derived from this class, can be passed into registerEventCallback() to have user code
 * executed asynchronously when an event occurs in the connection object. Please note that
 * because users are able to create their own EventCallback class, reentrancy is not guaranteed
 * unless if they implement their own reentrancy rules. Also note that because an EventCallback
 * can be registered to multiple connections, it can be run concurrently amongst those
 * connections.
 */
public abstract class EventCallback
{
	private JNIEventCallback m_jniEventCallback = null;
	private JNIConnection    m_jniConnection = null;


	public static JNIEventCallback getInternal(EventCallback cb)
	{
		return (cb == null ? null : cb.m_jniEventCallback);
	}


	public JNIConnection getConnection()
	{
		return m_jniConnection;
	}


	public void setConnection(JNIConnection jconn)
	{
		m_jniConnection = jconn;
	}


	protected EventCallback()
	{
		m_jniEventCallback = new JNIEventCallback(this);
	}


	/**
	 * @brief This function is called by the API in response to an event.
	 * 
	 * @note <b>DO NOT DESTROY</b> the Connection object that is passed into this method
	 * by the API.  It is owned by the API and does not need to be managed by the client
	 * program. Also, it should not be stored by the client program beyond the scope of
	 * this callback function.
	 * 
	 * @param conn   - connection related to the error
	 * @param status - the status containing error details
	 * @param event  - the event that caused the callback to be summoned
	 */
	public abstract void onEvent(Connection conn, Status status, Connection.ConnectionEvent event);
}
