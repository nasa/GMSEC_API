/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ReplyCallback.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.jni.JNIConnection;
import gov.nasa.gsfc.gmsec.api5.jni.JNIReplyCallback;


/**
 * This is the abstract base class for receiving reply messages from asynchronous requests. 
 * A user created class, derived from this class, can be passed to 
 * {@link Connection#request(Message, int, ReplyCallback, int)}
 * to have user code executed asynchronously when a message is received or an event occurs.
 * <p>
 * Note that because users are able to create their own ReplyCallback class, reentrancy is not
 * guaranteed unless if they implement their own reentrancy rules.
 * <p>
 * In addition, if a ReplyCallback is registered to multiple connections, onReply() can be
 * invoked concurrently from different connection threads. Users are encouraged to employ
 * the use of synchronization to enforce thread safety.
 * <p>
 * Example ReplyCallback class:
   <pre>{@code
   class ReqReplyCallback extends ReplyCallback
   {
       public void onReply(Connection conn, Message request, Message reply)
       {
           System.out.println(request.toXML());
           System.out.println(reply.toXML());
           // Do not destroy the connection or store the messages!
       }
       public void onEvent(Connection conn, Status status, Connection.Event event)
       {
           System.out.println(status.get());
           // Do not destroy the connection or store the status!
       }
   }
   }</pre>
 * Example ReplyCallback registration:
   <pre>{@code
   try
   {
       Message req = conn.getMessageFactory().createMessage("REQ.DIR");
       // include additional fields into Request message...
       conn.request(req, timeout, new ReqReplyCallback(), Gmsec.REQUEST_REPUBLISH_NEVER);
   }
   catch (IllegalArgumentException | GmsecException e)
   {
       // handle error
   }
   }</pre>
 *
 * @see Connection#request(Message, int, ReplyCallback, int)
 */
public abstract class ReplyCallback extends EventCallback
{
	private JNIReplyCallback m_jniReplyCallback = null;
	private JNIConnection    m_jniConnection = null;


	//! @cond
	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param cb Object to reference for acquiring internal JNIReplyCallback
	 * @return Internal JNIReplyCallback object.
	 */
	public static JNIReplyCallback getInternal(ReplyCallback cb)
	{
		return (cb == null ? null : cb.m_jniReplyCallback);
	}


	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @return Returns the internal JNIConnection object.
	 */
	public JNIConnection getConnection()
	{
		return m_jniConnection;
	}


	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param jconn The internal JNIConnection object.
	 */
	public void setConnection(JNIConnection jconn)
	{
		super.setConnection(jconn);
		m_jniConnection = jconn;
	}
	//! @endcond


	/**
	 * Constructor.
	 */
	protected ReplyCallback()
	{
		m_jniReplyCallback = new JNIReplyCallback(this);
	}


	/**
	 * This method is called by the API in response to a reply received from a request,
	 * from within the request() call. A class derived from ReplyCallback needs to be passed
	 * into the request() call.
	 * <p>
	 * If a ReplyCallback is registered to multiple connections, onReply() can
	 * be invoked concurrently from the different connection threads.
	 * <p>
	 * <b>DO NOT DESTROY or CHANGE STATE</b> of the Connection object that is passed to the callback method,
	 * nor store it for use beyond the scope of the callback method.
	 * <p>
	 * <b>DO NOT STORE</b> the Message objects for use beyond the scope of the callback. Otherwise,
	 * make a copy of the Message object(s).
	 * 
	 * @param conn    Connection on which the message was received
	 * @param request The sent request message
	 * @param reply   The received reply message
	 */
	public abstract void onReply(Connection conn, Message request, Message reply);
}
