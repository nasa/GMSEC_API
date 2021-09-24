/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file EventCallback.java
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNIConnection;
import gov.nasa.gsfc.gmsec.api.jni.JNIEventCallback;


/**
 * This class is the abstract base class for event callbacks.
 * A user created class, derived from this class, can be passed to
 * {@link Connection#registerEventCallback(Connection.ConnectionEvent, EventCallback)}
 * to have user code executed asynchronously when an event occurs in the connection object.
 * <p>
 * Note that because users are able to create their own EventCallback class,
 * reentrancy is not guaranteed unless if they implement their own reentrancy rules.
 * <p>
 * In addition, if an EventCallback is registered to multiple connections, onEvent() can be
 * invoked concurrently from different connection threads. Users are encouraged to employ
 * the use of synchronization to enforce thread safety.
 *
 * @see Connection#registerEventCallback(Connection.ConnectionEvent, EventCallback)
 */
public abstract class EventCallback
{
	private JNIEventCallback m_jniEventCallback = null;
	private JNIConnection    m_jniConnection = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param cb Object to reference for acquiring internal JNIEventCallback
	 * @return Internal JNIEventCallback object.
	 */
	public static JNIEventCallback getInternal(EventCallback cb)
	{
		return (cb == null ? null : cb.m_jniEventCallback);
	}


	/** 
	 * This method is for internal GMSEC API use only.
	 * @return Returns the internal JNIConnection object.
	 */
	public JNIConnection getConnection()
	{
		return m_jniConnection;
	}


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param jconn An internal JNIConnection object.
	 */
	public void setConnection(JNIConnection jconn)
	{
		m_jniConnection = jconn;
	}


	/**
	 * Constructor.
	 */
	protected EventCallback()
	{
		m_jniEventCallback = new JNIEventCallback(this);
	}


	/**
	 * Callback method that is called by the API in response to an event.
	 * <p>
	 * If an EventCallback is registered to multiple connections, onEvent() can
	 * be invoked concurrently from the different connection threads.
	 * <p>
	 * <b>DO NOT DESTROY or CHANGE STATE</b> of the Connection object that is passed to the callback method,
	 * nor store it for use beyond the scope of the callback method.
	 * <p>
	 * <b>DO NOT STORE</b> the Status object for use beyond the scope of the callback. Otherwise, make a
	 * copy of the Status object.
	 * 
	 * @param conn   Connection related to the error.
	 * @param status The status containing error details.
	 * @param event  The event that caused the callback to be summoned.
	 */
	public abstract void onEvent(Connection conn, Status status, Connection.ConnectionEvent event);
}
