/*
 * Copyright 2007-2021 United States Government as represented by the
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
 * Note that because users are able to create their own Callback class, reentrancy is not
 * guaranteed unless if reentrancy rules are specified.
 * <p>
 * In addition, if a Callback is registered to multiple connections, onMessage() can be
 * invoked concurrently from different connection threads. Users are encouraged to employ
 * the use of synchronization to enforce thread safety.
 * <p>
 * Example Callback class:
 * <pre>{@code
   class PublishCallback extends Callback
   {
       public void onMessage(Connection conn, Message msg)
       {
           System.out.println(msg.toXML());
           // Do not destroy the connection or store the message
       }
   }
   }</pre>
 * <p>
 * Example Callback registration:
 * <pre>{@code
   SubscriptionInfo info = conn.subscribe("GMSEC.TEST.PUBLISH", new PublishCallback());
   }</pre>
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
	 * If a Callback is registered to multiple connections, onMessage() can be
	 * invoked concurrently from different connection threads.
	 * <p>
	 * <b>DO NOT DESTROY or CHANGE STATE</b> of the Connection object that is passed to the callback method,
	 * nor store it for use beyond the scope of the callback method.
	 * <p>
	 * <b>DO NOT STORE</b> the Message object for use beyond the scope of the callback. Otherwise, make a
	 * copy of the Message object.
	 * 
	 * @param conn Connection on which the message was received
	 * @param msg  The received message
	 */
	public abstract void onMessage(Connection conn, Message msg);
}
