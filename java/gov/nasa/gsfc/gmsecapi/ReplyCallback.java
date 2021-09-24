/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/**
 * @interface ReplyCallback
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This interface is for received reply from asynchronous requests. 
 * A user created class, implemented with this interface, can be passed into 
 * %Request() to have user code executed asynchronously
 * when a message is received or an error occurs.
 * 
 * Example ReplyCallback class:
 * @code
 * class ReqReplyCallback implements ReplyCallback
 * {
 *	public void OnReply(Connection conn, Message request, Message reply) {
 *		GMSEC_String prtMsg = new GMSEC_String();
 * 		request.ToXML(prtMsg);
 *		System.out.println(prtMsg.Get());
 * 		reply.ToXML(prtMsg);
 *		System.out.println(prtMsg.Get());
 *		// Do not destroy the message here
 *	}
 *  public void OnError(Connection conn, Message request, Status result) {
 *  	System.out.println(result.Get());
 *  }
 * }
 * @endcode
 *
 * Example ReplyCallback registration:
 * @code
 * Message req = new Message();
 *  // create request here
 *  
 * result = conn.Request(req,timeout,new ReqReplyCallback());
 * if( result.isError() )
 *	// handle error
 * @endcode
 *
 * @sa Connection::Request(Message request, int timeout, ReplyCallback cb)
 */
@Deprecated
public interface ReplyCallback extends ErrorCallback
{
	/**
	 * @brief This function is called by the API in response to a message, from either the DispatchMsg()
	 * call or inside the Auto-dispatcher after a StartAutoDispatch() call. A class that implements
	 * Callback needs to be registered with a connection, using Subscribe() or Request() in order
	 * to be called for a particular subject registration pattern.
	 * 
	 * @note <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
	 * They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * the gmsec::Message, it must be cloned with Connection::CloneMessage().
	 * 
	 * @param conn - connection on which the message was recieved
	 * @param request - the sent request message
	 * @param reply - the recieved reply message
	 * 
	 */
	public void OnReply(Connection conn, Message request, Message reply);

}
