/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/**
 * @interface Callback
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This interface is for received message callbacks. 
 * A user created class, implemented with this interface, can be passed into 
 * %Subscribe() and %Request() to have user code executed asynchronously
 * when a message is received.
 * 
 * Example Callback class:
 * @code
 * class PublishCallback implements Callback
 * {
 *	public void OnMessage(Connection conn, Message msg) {
 *		GMSEC_String prtMsg = new GMSEC_String();
 * 		msg.ToXML(prtMsg);
 *		System.out.println(prtMsg.Get());
 *		// Do not destroy the message here
 *	}
 * }
 * @endcode
 *
 * Example Callback registration:
 * @code
 * result = conn.Subscribe("GMSEC.TEST.PUBLISH",new PublishCallback());
 * if( result.isError() )
 *	// handle error
 * @endcode
 *
 * @sa Connection::Subscribe(String subject, Callback cb) @n
 *     Connection::Request(Message request, int timeout, Callback cb)
 */
@Deprecated
public interface Callback
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
	 * @param msg - the recieved message
	 * 
	 */
	public void OnMessage(Connection conn, Message msg);
} //  class Callback
