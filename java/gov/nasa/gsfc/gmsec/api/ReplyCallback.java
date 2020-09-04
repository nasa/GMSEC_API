/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ReplyCallback.java
 *
 * @brief Interface that can be used for receiving reply messages via a callback.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNIConnection;
import gov.nasa.gsfc.gmsec.api.jni.JNIReplyCallback;


/**
 * @class ReplyCallback
 *
 * @brief This interface is for received reply from asynchronous requests. 
 * A user created class, implemented with this interface, can be passed into 
 * %request() to have user code executed asynchronously
 * when a message is received or an error occurs.
 * 
 * Example ReplyCallback class:
 * @code
 * class ReqReplyCallback extends ReplyCallback
 * {
 *     public void onReply(Connection conn, Message request, Message reply)
 *     {
 *         System.out.println(request.toXML());
 *         System.out.println(reply.toXML());
 *
 *         // Do not destroy the messages!!!
 *     }
 *
 *     public void onEvent(Connection conn, Status status, Connection.ConnectionEvent event)
 *     {
 *         System.out.println(status.get());
 *     }
 * }
 * @endcode
 *
 * Example ReplyCallback registration:
 * @code
 * try
 * {
 *     Message req = new Message("GMSEC.MY.REQUEST", Message.MessageType.REQUEST);
 *
 *     // add fields to request here...
 *  
 *     conn.request(req, timeout, new ReqReplyCallback());
 * }
 * catch (IllegalArgumentException | GMSEC_Exception e)
 * {
 *     // handle error
 * }
 * @endcode
 *
 * @sa Connection::Request(Message request, int timeout, ReplyCallback cb)
 */
public abstract class ReplyCallback extends EventCallback
{
	private JNIReplyCallback m_jniReplyCallback = null;
	private JNIConnection    m_jniConnection = null;


	public static JNIReplyCallback getInternal(ReplyCallback cb)
	{
		return (cb == null ? null : cb.m_jniReplyCallback);
	}


	public JNIConnection getConnection()
	{
		return m_jniConnection;
	}


	public void setConnection(JNIConnection jconn)
	{
		m_jniConnection = jconn;
	}


	protected ReplyCallback()
	{
		m_jniReplyCallback = new JNIReplyCallback(this);
	}


	/**
	 * @fn void onReply(Connection conn, Message request, Message reply)
	 *
	 * @brief This function is called by the API in response to a reply recieved from a request,
	 * from within the request() call. A class derrived from ReplyCallback needs to be passed
	 * into the request() call.
	 *
	 * Please note that if a ReplyCallback is registered to multiple connections, onReply() can
	 * be invoked concurrently from the different connection threads.
	 *
	 * @note <b>DO NOT DESTROY</b> the Connection, or the Messages that are passed into this
	 * method.  They are owned by the API and do not need to be managed by the client program.
	 * Also, the Connection object should not be stored by the client program beyond the scope
	 * of this callback function.  The client program is permitted to make copies of the Messages.
	 * 
	 * @param conn    - connection on which the message was recieved
	 * @param request - the sent request message
	 * @param reply   - the received reply message
	 * 
	 */
	public abstract void onReply(Connection conn, Message request, Message reply);
}
