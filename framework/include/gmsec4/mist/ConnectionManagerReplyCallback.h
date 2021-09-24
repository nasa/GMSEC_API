/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ConnectionManagerReplyCallback.h
 *
 *  @brief This file contains the abstract base class for asynchronous request-reply callbacks.
 */

#ifndef CONNECTION_MANAGER_REPLYCALLBACK_H
#define CONNECTION_MANAGER_REPLYCALLBACK_H

#include <gmsec4/mist/ConnectionManagerEventCallback.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
// Forward declaration
class Message;

namespace mist
{
// Forward declaration
class ConnectionManager;


/**
 * @class ConnectionManagerReplyCallback
 *
 * @brief This class is the abstract base class for received replies from asynchronous request messages.
 * A user created class, derived from this class, can be passed into
 * %request() to have user code executed asynchronously when a reply is received or when an error occurs.
 *
 * Note that because users are able to create their own ConnectionManagerReplyCallback class,
 * reentrancy is not guaranteed unless they implement their own reentrancy rules.
 *
 * In addition, if a ConnectionManagerReplyCallback is registered to multiple Connection Manager objects,
 * onReply() can be invoked concurrently from different threads. Use of a gmsec::util::AutoMutex is
 * suggested to enforce thread safety.
 *
 * Example Callback class:
 * @code
   class MyReplyCallback : public ConnectionManagerReplyCallback
   {
    public:
        virtual void CALL_TYPE onReply(ConnectionManager& connMgr, const Message& request, const Message& reply)
        {
            cout << request.toXML() << endl;
            cout << reply.toXML()   << endl;
        }
        virtual void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& result, const char* event)
        {
            cout << result->get() << endl;
        }
   };
 * @endcode
 *
 * Example ConnectionManagerReplyCallback registration:
 * @code
   MyReplyCallback* cb = new MyReplyCack;
   Message reqMsg("GMSEC.MISSION.SAT.REQ.DIR", Message::REQUEST);
   int timeout = 1000;
   try {
       connMgr->request(reqMsg, timeout, cb);
   }
   catch (Exception& e) {
       cout << e.what() << endl;
   }
 * @endcode
*/
class GMSEC_API ConnectionManagerReplyCallback : public ConnectionManagerEventCallback
{
public:
	// base class needs virtual destructor
	virtual ~ConnectionManagerReplyCallback() { }


	/**
	 * @fn onReply(ConnectionManager& connMgr, const Message& request, const Message& reply)
	 *
	 * @brief This method is called by the API in response to a reply recieved from a request,
	 * from within the Request call. A class derived from gmsec::mist::ConnectionManagerReplyCallback needs
	 * to be passed into the request() call.
	 *
	 * Note that if a callback is registered to multiple connections, onReply can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this method is:
	 * @code virtual void CALL_TYPE onReply(ConnectionManager& connMgr, const Message& request, const Message& reply) @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * @note <b>DO NOT STORE or CHANGE STATE</b> of the ConnectionManager object that is passed to the callback
	 * method.
	 *
	 * @note<b>DO NOT STORE</b> the Message objects for use beyond the scope of the callback. Otherwise,
	 * make a copy of the Message object(s).
	 *
	 * @param connMgr - connection manager on which the message was received
	 * @param request - the pending request
	 * @param reply - the received reply
	 *
	 * @sa ConnectionManager::request(const Message& request, GMSEC_I32 timeout, ConnectionManagerReplyCallback *cb, GMSEC_I32 republish_ms = 0)
	*/
	virtual void CALL_TYPE onReply(ConnectionManager& connMgr, const Message& request, const Message& reply) = 0;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
