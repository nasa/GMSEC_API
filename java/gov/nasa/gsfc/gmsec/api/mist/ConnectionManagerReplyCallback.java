/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConnectionManagerReplyCallback.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnectionManager;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnMgrReplyCallback;


/**
 * This abstract class is for receiving replies from asynchronous request messages.
 * A user created class, derived from this class, can be passed to
 * {@link ConnectionManager#request(Message, int, ConnectionManagerReplyCallback, int)}
 * to have user code executed asynchronously when a reply is received or when an error occurs.
 * <p>
 * Please note that because users are able to create their own ConnectionManagerReplyCallback class,
 * reentrancy is not guaranteed unless they implement their own reentrancy rules.
 * <p>
 * Also note that because a ConnectionManagerReplyCallback can be registered to multiple connections,
 * it can be run concurrently among those connections.
 * <p>
 * Example callback class:
 * <pre>{@code
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
 * }</pre>
 * <p>
 * Example ConnectionManagerReplyCallback registration:
 * <pre>{@code
 * try
 * {
 *     Message request = new Message("GMSEC.MISSION.SAT.MY.REQUEST", Message.MessageKind.REQUEST);
 *
 *     // ... add fields to request message
 *
 *     connMgr.request(request, timeout, new MyReplyCallback(), gmsecAPI.REQUEST_REPUBLISH_NEVER);
 *
 *     ...
 * }
 * catch (GMSEC_Exception e)
 * {
 *     // handle error
 * }
 * }</pre>
 *
 * @see ConnectionManager#request(Message, int, ConnectionManagerReplyCallback, int)
 * @see ConnectionManager#cancelRequest(ConnectionManagerReplyCallback)
 */
public abstract class ConnectionManagerReplyCallback extends ConnectionManagerEventCallback
{
	private JNIConnMgrReplyCallback m_jniConnMgrReplyCallback = null;
	private JNIConnectionManager    m_jniConnMgr = null;


	/**
	 * This method is for internal GMSEC API use only.
	 * @param cb ConnectionManagerReplyCallback object to reference for acquiring internal JNIConnMgrReplyCallback.
	 * @return Internal JNIConnMgrReplyCallback object.
	 */
	public static JNIConnMgrReplyCallback getInternal(ConnectionManagerReplyCallback cb)
	{
		return (cb == null ? null : cb.m_jniConnMgrReplyCallback);
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
	protected ConnectionManagerReplyCallback()
	{
		m_jniConnMgrReplyCallback = new JNIConnMgrReplyCallback(this);
	}


	/**
	 * This method is called by the API in response to a reply received from a request,
	 * from within the request(...) call. A class derrived from ConnectionManagerReplyCallback needs
	 * to be passed into the {@link ConnectionManager#request(Message, int, ConnectionManagerReplyCallback, int)} call.
	 * <p>
	 * Please note that if a callback is registered to multiple connections, onReply() can be invoked concurrently
	 * from the different connection threads.
	 * <p>
	 * Note: <b>DO NOT DESTROY</b> the ConnectionManager, nor the Messages that are passed into this function by
	 * the API. They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * a Message, the message must be copied.
	 *
	 * @param connMgr Connection manager on which the message was received.
	 * @param request The pending request message.
	 * @param reply The received reply message.
	*/
	public abstract void onReply(ConnectionManager connMgr, Message request, Message reply);
}
