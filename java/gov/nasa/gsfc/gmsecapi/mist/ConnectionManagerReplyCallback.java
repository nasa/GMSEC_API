/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConnectionManagerReplyCallback.java
 *
 *  @brief This file contains the abstract base class for asynchronous request-reply callbacks.
**/

package gov.nasa.gsfc.gmsecapi.mist;

import gov.nasa.gsfc.gmsecapi.Message;


/** @class ConnectionManagerReplyCallback
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This class is the abstract base class for received replies from asynchronous request messages.
 * A user created class, derrived from this class, can be passed into
 * %Request() to have user code executed asynchronously when a reply is received or when an error occurs.
 * Please note that because users are able to create their own ConnectionManagerReplyCallback class,
 * reentrancy is not guaranteed unless they implement their own reentrancy rules.
 * Also note that because a ConnectionManagerReplyCallback can be registered to multiple connections,
 * it can be run concurrently amongst those connections.  Because of this, the use of a gmsec::util::AutoMutex
 * is * suggested to enforce thread safety.
 *
 * Example callback class:
 * @code
 * class MyReplyCallback : public ConnectionManagerReplyCallback
 * {
 *    public void OnReply(ConnectionManager* connMgr, Message* request, Message* reply)
 *    {
 *        GMSEC_String xml = new GMSEC_String();
 *        request.ToXML(xml);
 *        System.out.println(xml);
 *        reply.ToXML(xml);
 *        System.out.println(xml);
 *    }
 *    public void OnError(ConnectionManager connMgr, Message request, Status result)
 *    {
 *        System.err.println(result.Get());
 *    }
 * };
 * @endcode
 *
 * Example ConnectionManagerReplyCallback registration:
 * @code
 * MyReplyCallback cb = new MyReplyBack();
 * Message request = new Message();
 *
 * // TBD: create request here
 *
 * result = connMgr.Request(request, timeout, cb);
 * if (result.IsError())
 *     // handle error
 * @endcode
 */
@Deprecated
public interface ConnectionManagerReplyCallback extends ConnectionManagerErrorCallback
{
	/** @fn OnReply(ConnectionManager connMgr, Message request, Message reply)
	 *
	 * @brief This function is called by the API in response to a reply recieved from a request,
	 * from within the Request call. A class derrived from ConnectionManagerReplyCallback needs
	 * to be passed into the Request() call.
	 *
	 * Please note that if a callback is registered to multiple connections, OnReply can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this funtion is:
	 * @code public void OnReply(ConnectionManager connMgr, Message request, Message reply) @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager, or the Messages that are passed into this function by
	 * the API.  They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * a Message, the message must be cloned with ConnectionManager.CloneMessage().
	 *
	 * @param connMgr - connection manager on which the message was received
	 * @param request - the pending request
	 * @param reply - the received reply
	 *
	 * @sa ConnectionManager.Request(Message request, int timeout, ConnectionManagerReplyCallback cb)
	*/
	public void OnReply(ConnectionManager connMgr, Message request, Message reply);
}
