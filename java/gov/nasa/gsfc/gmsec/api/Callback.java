/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Callback.java
 *
 * @brief An interface for receiving messages via a callback.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNICallback;
import gov.nasa.gsfc.gmsec.api.jni.JNIConnection;


/**
 * @class Callback
 *
 * @brief This interface is for received message callbacks. 
 * A user created class, that extends this class, can be passed into 
 * %subscribe() and %request() to have user code executed asynchronously
 * when a message is received.
 * 
 * Example Callback class:
 * @code
 * class PublishCallback extends Callback
 * {
 *     public void onMessage(Connection conn, Message msg)
 *     {
 *         System.out.println(msg.toXML());
 *
 *         // Do not destroy the message
 *     }
 * }
 * @endcode
 *
 * Example Callback registration:
 * @code
 * SubscriptionInfo info = conn.subscribe("GMSEC.TEST.PUBLISH", new PublishCallback());
 * @endcode
 *
 * @sa Connection::subscribe(String subject, Callback cb)
 */
public abstract class Callback
{
	private JNICallback   m_jniCallback = null;
	private JNIConnection m_jniConnection = null;


	public static JNICallback getInternal(Callback cb)
	{
		return (cb == null ? null : cb.m_jniCallback);
	}


	public JNIConnection getConnection()
	{
		return m_jniConnection;
	}


	public void setConnection(JNIConnection jconn)
	{
		m_jniConnection = jconn;
	}


	protected Callback()
	{
		m_jniCallback = new JNICallback(this);
	}


	/**
	 * @fn void onMessage(Connection conn, Message msg)
	 *
	 * @brief This function is called by the API in response to a message, from either the
	 * dispatchMsg() call or inside the auto-dispatcher after a startAutoDispatch() call. A
	 * class derrived from Callback needs to be registered with a connection, using subscribe()
	 * in order to be called for a particular subject registration pattern.
	 *
	 * Please note that if a Callback is registered to multiple connections, onMessage() can be
	 * invoked concurrently from different connection threads.
	 * 
	 * @param conn - connection on which the message was recieved
	 * @param msg - the recieved message
	 */
	public abstract void onMessage(Connection conn, Message msg);
}
