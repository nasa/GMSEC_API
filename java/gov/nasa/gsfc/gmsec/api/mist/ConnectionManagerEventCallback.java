/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ConnectionManagerEventCallback.java
 *
 *  @brief This file contains the abstract base class for event callbacks.
 **/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Connection;
import gov.nasa.gsfc.gmsec.api.Status;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnectionManager;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnMgrEventCallback;


/**
 * @class ConnectionManagerEventCallback
 *
 * @brief This class is the abstract base class for event callbacks.
 * A user created class, derrived from this class, can be passed into %registerEventCallback() to have
 * user code executed asynchronously when an event occurs in the connection object.
 * Please note that because users are able to create their own ConnectionManagerEventCallback class, reentrancy
 * is not guarunteed unless if they implement their own reentrancy rules.
 * Also note that because a ConnectionManagerEventCallback can be registered to multiple connections,
 * it can be run concurrently amongst those connections.
 *
 * Example callback class:
 * @code
 * public class MyEventCallback extends ConnectionManagerEventCallback
 * {
 *     public void onEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent event)
 *     {
 *         System.out.println(status.get());
 *     }
 * }
 * @endcode
 *
 * Example callback registration:
 * @code
 * connMgr.registerEventCallback(Connection.ConnectionEvent.ALL_EVENTS, new MyEventCallback());
 * @endcode
 *
 * @sa ConnectionManager.registerErrorCallback(Connection.ConnectionEvent event, ConnectionManagerEventCallback cb)
*/
public abstract class ConnectionManagerEventCallback
{
	private JNIConnMgrEventCallback m_jniConnMgrEventCallback = null;
	private JNIConnectionManager    m_jniConnMgr = null;


	public static JNIConnMgrEventCallback getInternal(ConnectionManagerEventCallback cb)
	{
		return (cb == null ? null : cb.m_jniConnMgrEventCallback);
	}


	public JNIConnectionManager getConnectionManager()
	{
		return m_jniConnMgr;
	}


	public void setConnectionManager(JNIConnectionManager jconnMgr)
	{
		m_jniConnMgr = jconnMgr;
	}


	protected ConnectionManagerEventCallback()
	{
		m_jniConnMgrEventCallback = new JNIConnMgrEventCallback(this);
	}


	/**
	 * @fn onEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent event)
	 *
	 * This function is called in response to a event after a call to %registerEventCallback().
	 *
	 * Please note that if a callback is registered to multiple connections, onEvent can be invoked 
	 * concurrently from the different connection threads.
	 *
	 * The prototype for this funtion is:
	 * @code
	 * public void onEvent(ConnectionManager connMgr, Status status, String event)
	 * @endcode
	 *
	 * @param connMgr - A reference to the ConnectionManager object that is reporting the event
	 * @param status  - A reference to a status object that contains information regarding the event
	 * @param event   - the event that led the callback to be summoned
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager that is passed into this function
	 * by the API.  It is owned by the API and does not need to be managed by the client program.
	 *
	 * @sa ConnectionManager.registerEventCallback(Connection.ConnectionEvent event, ConnectionManagerEventCallback cb)
	 */
	public abstract void onEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent event);
}
