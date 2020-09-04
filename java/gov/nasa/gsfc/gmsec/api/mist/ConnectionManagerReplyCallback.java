/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ConnectionManagerReplyCallback.java
 *
 *  @brief This file contains the abstract base class for asynchronous request-reply callbacks.
**/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnectionManager;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnMgrReplyCallback;


/**
 * @class ConnectionManagerReplyCallback
 *
 * @brief This class is the abstract base class for received replies from asynchronous request messages.
 * A user created class, derrived from this class, can be passed into
 * %request() to have user code executed asynchronously when a reply is received or when an error occurs.
 * Please note that because users are able to create their own ConnectionManagerReplyCallback class,
 * reentrancy is not guaranteed unless they implement their own reentrancy rules.
 * Also note that because a ConnectionManagerReplyCallback can be registered to multiple connections,
 * it can be run concurrently amongst those connections.
 *
 * Example callback class:
 * @code
 * class MyReplyCallback : public ConnectionManagerReplyCallback
 * {
 *    public void onReply(ConnectionManager connMgr, Message request, Message reply)
 *    {
 *        System.out.println("Request Message:\n" + request.toXML());
 *        System.out.println("Reply Message:\n" + reply.toXML());
 *    }
 *
 *    public void onEvent(ConnectionManager connMgr, Status status, String event)
 *    {
 *        System.err.println(status.get());
 *    }
 * };
 * @endcode
 *
 * Example ConnectionManagerReplyCallback registration:
 * @code
 * try
 * {
 *     Message request = new Message("GMSEC.MISSION.SAT.MY.REQUEST", Message.MessageKind.REQUEST);
 *
 *     // ... add fields to request message
 *
 *     connMgr.request(request, timeout, new MyReplyCallback());
 *
 *     ...
 * }
 * catch (GMSEC_Exception e)
 * {
 *     // handle error
 * }
 * @endcode
*/
public abstract class ConnectionManagerReplyCallback extends ConnectionManagerEventCallback
{
	private JNIConnMgrReplyCallback m_jniConnMgrReplyCallback = null;
	private JNIConnectionManager    m_jniConnMgr = null;


	public static JNIConnMgrReplyCallback getInternal(ConnectionManagerReplyCallback cb)
	{
		return (cb == null ? null : cb.m_jniConnMgrReplyCallback);
	}


	public JNIConnectionManager getConnectionManager()
	{
		return m_jniConnMgr;
	}


	public void setConnectionManager(JNIConnectionManager jconnMgr)
	{
		m_jniConnMgr = jconnMgr;
	}


	protected ConnectionManagerReplyCallback()
	{
		m_jniConnMgrReplyCallback = new JNIConnMgrReplyCallback(this);
	}


	/**
	 * @fn onReply(ConnectionManager connMgr, Message request, Message reply)
	 *
	 * @brief This function is called by the API in response to a reply recieved from a request,
	 * from within the Request call. A class derrived from ConnectionManagerReplyCallback needs
	 * to be passed into the Request() call.
	 *
	 * Please note that if a callback is registered to multiple connections, OnReply can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this funtion is:
	 * @code public void CALL_TYPE onReply(ConnectionManager connMgr, Message request, Message reply) @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager, nor the Messages that are passed into this function by
	 * the API.  They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * a Message, the message must be copied.
	 *
	 * @param connMgr - connection manager on which the message was received
	 * @param request - the pending request message
	 * @param reply - the received reply message
	 *
	 * @sa ConnectionManager.request(Message request, int timeout, ConnectionManagerReplyCallback cb)
	*/
	public abstract void onReply(ConnectionManager connMgr, Message request, Message reply);
}
