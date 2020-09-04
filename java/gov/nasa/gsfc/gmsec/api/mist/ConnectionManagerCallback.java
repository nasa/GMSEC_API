/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ConnectionManagerCallback.java
 *
 *  @brief This file contains the abstract base class for received message callbacks
**/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnectionManager;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnMgrCallback;


/**
 * @class ConnectionManagerCallback
 *
 * @brief This abstract class is for received message callbacks. 
 * A user created class, extending this class, can be passed into 
 * %subscribe() and to have user code executed asynchronously
 * when a message is received.
 * 
 * Example ConnectionManagerCallback class:
 * @code
 * class PublishCallback extends ConnectionManagerCallback
 * {
 *     public void onMessage(ConnectionManager connMgr, Message msg)
 *     {
 *         System.out.println("Received Message:\n" + msg.toXML());
 *         // Do not destroy the message here
 *     }
 * }
 * @endcode
 *
 * Example ConnectionManagerCallback registration:
 * @code
 * try
 * {
 *     SubscriptionInfo info = connMgr.subscribe("GMSEC.TEST.PUBLISH", new PublishCallback());
 *
 *     ...
 * }
 * catch (GMSEC_Exception e)
 * {
 *     // handle error
 * }
 * @endcode
 *
 * @sa ConnectionManager.subscribe(String subject, ConnectionManagerCallback cb)
 * @sa ConnectionManager.unsubscribe(SubscriptionInfo info)
 * @sa ConnectionManager.startAutoDispatcher()
 * @sa ConnectionManager.dispatchMsg(Message msg)
 */
public abstract class ConnectionManagerCallback
{
	private JNIConnMgrCallback   m_jniConnMgrCallback = null;
	private JNIConnectionManager m_jniConnMgr = null;


	public static JNIConnMgrCallback getInternal(ConnectionManagerCallback cb)
	{
		return (cb == null ? null : cb.m_jniConnMgrCallback);
	}


	public JNIConnectionManager getConnectionManager()
	{
		return m_jniConnMgr;
	}


	public void setConnectionManager(JNIConnectionManager jconnMgr)
	{
		m_jniConnMgr = jconnMgr;
	}


	protected ConnectionManagerCallback()
	{
		m_jniConnMgrCallback = new JNIConnMgrCallback(this);
	}


	/**
	 * @fn onMessage(ConnectionManager connMgr, Message msg)
	 *
	 * @brief This function is called by the API in response to a message, from either the dispatchMsg()
	 * call or inside the auto-dispatcher after a startAutoDispatch() call.
	 *
	 * gmsec.mist.ConnectionManagerCallback needs to be registered with a ConnectionManager, using
	 * Subscribe() in order to be called for a particular subject registration pattern.
	 *
	 * Please note that if a ConnectionManagerCallback is registered to multiple connections, onMessage
	 * can be invoked concurrently from different connection threads.
	 *
	 *   The prototype for this funtion is:
	 *   @code void onMessage(ConnectionManager connMgr, Message msg) @endcode
	 *
	 * @param connMgr - connection manager on which the message was received
	 * @param msg - the received message
	 *
	 * @sa ConnectionManager.subscribe(String subject, ConnectionManagerCallback cb)
	 * @sa ConnectionManager.unsubscribe(SubscriptionInfo info)
	 * @sa ConnectionManager.startAutoDispatcher()
	 * @sa ConnectionManager.dispatchMsg(Message msg)
	 */
	public abstract void onMessage(ConnectionManager connMgr, Message msg);
}
