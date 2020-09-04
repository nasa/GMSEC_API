/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConnectionManagerEventCallback.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Connection;
import gov.nasa.gsfc.gmsec.api.Status;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnectionManager;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnMgrEventCallback;


/**
 * This abstract class is for receiving connection-related events using a callback.
 * A user created class, derived from this class, can be passed to
 * {@link ConnectionManager#registerEventCallback(Connection.ConnectionEvent, ConnectionManagerEventCallback)}
 * to have user code executed asynchronously when an event occurs in the connection object.
 * <p>
 * Please note that because users are able to create their own ConnectionManagerEventCallback class, reentrancy
 * is not guaranteed unless if they implement their own reentrancy rules.
 * Also note that because a ConnectionManagerEventCallback can be registered to multiple connections,
 * it can be run concurrently among those connections.
 * <p>
 * Example callback class:
 * <pre>{@code
 * public class MyEventCallback extends ConnectionManagerEventCallback
 * {
 *     public void onEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent event)
 *     {
 *         System.out.println(status.get());
 *     }
 * }
 * }</pre>
 * <p>
 * Example callback registration:
 * <pre>{@code
 * connMgr.registerEventCallback(Connection.ConnectionEvent.ALL_EVENTS, new MyEventCallback());
 * }</pre>
 *
 * @see ConnectionManager#registerEventCallback(Connection.ConnectionEvent, ConnectionManagerEventCallback)
*/
public abstract class ConnectionManagerEventCallback
{
	private JNIConnMgrEventCallback m_jniConnMgrEventCallback = null;
	private JNIConnectionManager    m_jniConnMgr = null;


	/**
	 * This method is for internal GMSEC API use only.
	 * @param cb ConnectionManagerEventCallback object to reference for acquiring internal JNIConnMgrEventCallback.
	 * @return Internal JNIConnMgrEventCallback object.
	 */
	public static JNIConnMgrEventCallback getInternal(ConnectionManagerEventCallback cb)
	{
		return (cb == null ? null : cb.m_jniConnMgrEventCallback);
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @return Internal JNIConnectionManager object.
	 */
	public JNIConnectionManager getConnectionManager()
	{
		return m_jniConnMgr;
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jconnMgr Internal JNIConnectionManager object.
	 */
	public void setConnectionManager(JNIConnectionManager jconnMgr)
	{
		m_jniConnMgr = jconnMgr;
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 */
	protected ConnectionManagerEventCallback()
	{
		m_jniConnMgrEventCallback = new JNIConnMgrEventCallback(this);
	}


	/**
	 * This method is called in response to a event after a call to
	 * {@link ConnectionManager#registerEventCallback(Connection.ConnectionEvent, ConnectionManagerEventCallback)}.
	 * <p>
	 * Please note that if a callback is registered to multiple connections, onEvent() can be invoked 
	 * concurrently from the different connection threads.
	 *
	 * @param connMgr A reference to the ConnectionManager object that is reporting the event.
	 * @param status  A reference to a status object that contains information regarding the event.
	 * @param event   the event that led the callback to be summoned.
	 * <p>
	 * Note: <b>DO NOT DESTROY</b> the ConnectionManager that is passed into this function
	 * by the API. It is owned by the API and does not need to be managed by the client program.
	 */
	public abstract void onEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent event);
}
