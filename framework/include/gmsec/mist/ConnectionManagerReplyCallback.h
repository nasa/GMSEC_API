/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConnectionManagerReplyCallback.h
 *
 *  @brief This file contains the abstract base class for asynchronous request-reply callbacks.
**/

#ifndef CONNECTION_MANAGER_REPLYCALLBACK_H
#define CONNECTION_MANAGER_REPLYCALLBACK_H

#include <gmsec/mist/ConnectionManagerErrorCallback.h>

#include <gmsec/util/wdllexp.h>

namespace gmsec
{
class Message;

namespace mist
{
class ConnectionManager;

/** @class ConnectionManagerReplyCallback
 * @brief This class is the abstract base class for received replies from asynchronous request messages.
 * A user created class, derrived from this class, can be passed into
 * %Request() to have user code executed asynchronously when a reply is received or when an error occurs.
 * Please note that because users are able to create their own ConnectionManagerReplyCallback class,
 * reentrancy is not guaranteed unless they implement their own reentrancy rules.
 * Also note that because a ConnectionManagerReplyCallback can be registered to multiple connections,
 * it can be run concurrently amongst those connections.  Because of this, the use of a gmsec::util::AutoMutex
 * is * suggested to enforce thread safety.
 *
 * Example Callback class:
 * @code
 * class MyReplyCallback : public ConnectionManagerReplyCallback
 * {
 *  public:
 *      virtual void CALL_TYPE OnReply(ConnectionManager* connMgr, Message* request, Message* reply)
 *      {
 *          const char *tmp;
 *          request->ToXML(tmp);
 *          cout << tmp << endl;
 *          reply->ToXML(tmp);
 *          cout << tmp << endl;
 *      }
 *      virtual void CALL_TYPE OnError(ConnectionManager* connMgr, Message* request, Status* result)
 *      {
 *          cout << result->Get() << endl;
 *      }
 * };
 * @endcode
 *
 * Example ConnectionManagerReplyCallback registration:
 * @code
 * MyReplyCallback* cb = new MyReplyCack;
 * Message* request = NULL;
 *
 * // TBD: create request here
 *
 * result = conn->Request(request, timeout, cb);
 * if (result.isError())
 *     // handle error
 * @endcode
*/
class GMSEC_API ConnectionManagerReplyCallback : public ConnectionManagerErrorCallback
{
public:
	// base class needs virtual destructor
	virtual ~ConnectionManagerReplyCallback() { }


	/** @fn OnReply(ConnectionManager* connMgr, Message* request, Message* reply)
	 *
	 * @brief This function is called by the API in response to a reply recieved from a request,
	 * from within the Request call. A class derrived from gmsec::mist::ConnectionManagerReplyCallback needs
	 * to be passed into the Request() call.
	 *
	 * Please note that if a callback is registered to multiple connections, OnReply can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this funtion is:
	 * @code virtual void CALL_TYPE OnReply(ConnectionManager* connMgr, Message* request, Message* reply) @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager, or the Messages that are passed into this function by
	 * the API.  They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * a gmsec::Message, the message must be cloned with ConnectionManager::CloneMessage().
	 *
	 * @param connMgr - connection manager on which the message was received
	 * @param request - the pending request
	 * @param reply - the received reply
	 *
	 * @sa ConnectionManager::Request(Message *request, GMSEC_I32 timeout, ConnectionManagerReplyCallback *cb, GMSEC_I32 republish_ms = 0)
	*/
	virtual void CALL_TYPE OnReply(ConnectionManager* connMgr, Message* request, Message* reply) = 0;
};

} // namespace mist

} // namespace gmsec

#endif
