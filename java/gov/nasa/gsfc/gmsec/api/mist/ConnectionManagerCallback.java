/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConnectionManagerCallback.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnectionManager;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnMgrCallback;


/**
 * This abstract class is for receiving messages using a callback.
 * A user created class, derived from this class, can be passed to 
 * {@link ConnectionManager#subscribe(String, ConnectionManagerCallback)}
 * and to have user code executed asynchronously when a message is received.
 * <p>
 * Note that because users are able to create their own ConnectionManagerCallback class,
 * reentrancy is not guaranteed unless if reentrancy rules are specified.
 *
 * In addition, if a ConnectionManagerCallback is registered to multiple connections,
 * onMessage() can be invoked concurrently from different connection manager threads.
 * Users are encouraged to employ the use of synchronization to enforce thread safety.
 * <p>
 * Example ConnectionManagerCallback class:
   <pre>{@code
   class PublishCallback extends ConnectionManagerCallback
   {
       public void onMessage(ConnectionManager connMgr, Message msg)
       {
           System.out.println("Received Message:\n" + msg.toXML());
           // Do not clean up the Connection Manager or store the message
       }
   }
   }</pre>
 * <p>
 * Example ConnectionManagerCallback registration:
   <pre>{@code
   try
   {
       SubscriptionInfo info = connMgr.subscribe("GMSEC.TEST.PUBLISH", new PublishCallback());
       ...
   }
   catch (GMSEC_Exception e)
   {
       // handle error
   }
   }</pre>
 *
 * @see ConnectionManager#subscribe(String, ConnectionManagerCallback)
 * @see ConnectionManager#unsubscribe(SubscriptionInfo)
 * @see ConnectionManager#startAutoDispatch()
 * @see ConnectionManager#dispatch(Message)
 */
public abstract class ConnectionManagerCallback
{
	private JNIConnMgrCallback   m_jniConnMgrCallback = null;
	private JNIConnectionManager m_jniConnMgr = null;


	/**
	 * This method is for internal GMSEC API use only.
	 * @param cb ConnectionManagerCallback object to reference for acquiring internal JNIConnMgrCallback.
	 * @return Internal JNIConnMgrCallback object.
	 */
	public static JNIConnMgrCallback getInternal(ConnectionManagerCallback cb)
	{
		return (cb == null ? null : cb.m_jniConnMgrCallback);
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
	protected ConnectionManagerCallback()
	{
		m_jniConnMgrCallback = new JNIConnMgrCallback(this);
	}


	/**
	 * This method is called by the API in response to a message, from either the
	 * dispatch() call or inside the auto-dispatcher after a startAutoDispatch() call. A
	 * class derived from ConnectionManagerCallback needs to be registered with a ConnectionManager, using
	 * subscribe() in order to be called for a particular subject registration pattern.
	 * <p>
	 * If a ConnectionManagerCallback is registered to multiple connections, onMessage()
	 * can be invoked concurrently from different connection threads.
	 * <p>
	 * <b>DO NOT STORE or CHANGE STATE</b> of the ConnectionManager object; it should only be used within
	 * the scope of the callback method.
	 * <p>
	 * <b>DO NOT STORE</b> the Message object for use beyond the scope of the callback. Otherwise, make a
	 * copy of the Message object.
	 *
	 * @param connMgr Connection manager on which the message was received.
	 * @param msg The received message.
	 */
	public abstract void onMessage(ConnectionManager connMgr, Message msg);
}
