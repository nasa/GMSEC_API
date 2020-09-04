/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Callback.java
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNICallback;
import gov.nasa.gsfc.gmsec.api.jni.JNIConnection;


/**
 * This abstract class is for receiving messages using a callback. 
 * <p>
 * A user created class, derived from this class, can be passed into 
 * {@link Connection#subscribe(String, Callback)} to have user code
 * executed asynchronously when a message is received.
 * <p>
 * Example Callback class:
 * <pre>{@code
 * class PublishCallback extends Callback
 * {
 *     public void onMessage(Connection conn, Message msg)
 *     {
 *         System.out.println(msg.toXML());
 *
 *         // Do not destroy the message or the connection
 *     }
 * }
 * }</pre>
 * <p>
 * Example Callback registration:
 * <pre>{@code
 * SubscriptionInfo info = conn.subscribe("GMSEC.TEST.PUBLISH", new PublishCallback());
 * }</pre>
 *
 * @see Connection#subscribe(String, Callback)
 */
public abstract class Callback
{
	private JNICallback   m_jniCallback = null;
	private JNIConnection m_jniConnection = null;


	/**
	 * This method is for internal GMSEC API use only.
	 * @param cb Callback object to reference for acquiring internal JNICallback
	 * @return Internal JNICallback object
	 */
	public static JNICallback getInternal(Callback cb)
	{
		return (cb == null ? null : cb.m_jniCallback);
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @return Internal JNIConnection object
	 */
	public JNIConnection getConnection()
	{
		return m_jniConnection;
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jconn Internal JNIConnection object
	 */
	public void setConnection(JNIConnection jconn)
	{
		m_jniConnection = jconn;
	}


	/**
	 * Constructor.
	 */
	protected Callback()
	{
		m_jniCallback = new JNICallback(this);
	}


	/**
	 * This method is called by the API in response to a message, from either the
	 * dispatch() call or inside the auto-dispatcher after a startAutoDispatch() call. A
	 * class derived from Callback needs to be registered with a connection, using subscribe()
	 * in order to be called for a particular subject registration pattern.
	 * <p>
	 * Please note that if a Callback is registered to multiple connections, onMessage() can be
	 * invoked concurrently from different connection threads.
	 * 
	 * @param conn Connection on which the message was received
	 * @param msg  The received message
	 */
	public abstract void onMessage(Connection conn, Message msg);
}
