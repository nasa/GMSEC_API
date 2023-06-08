/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ReplyCallback.h
 *
 *  @brief This file contains the abstract base class for asynchronous request-reply callbacks.
 */

#ifndef GMSEC_API5_REPLYCALLBACK_H
#define GMSEC_API5_REPLYCALLBACK_H

#include <gmsec5/EventCallback.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Connection;
	class Message;


/**
 * @class ReplyCallback
 *
 * @brief This class is the abstract base class for received replies from asynchronous request messages.
 * A user created class, derived from this class, can be passed into
 * %request() to have user code executed asynchronously when a reply is received or when an error occurs.
 *
 * Note that because users are able to create their own ReplyCallback class,
 * reentrancy is not guaranteed unless they implement their own reentrancy rules.
 *
 * In addition, if a ReplyCallback is registered to multiple Connection objects,
 * onReply() can be invoked concurrently from different threads. Use of a AutoMutex is
 * suggested to enforce thread safety.
 *
 * Example Callback class:
 * @code
   class MyReplyCallback : public ReplyCallback
   {
    public:
        virtual void onReply(Connection& conn, const Message& request, const Message& reply)
        {
            std::cout << request.toXML() << std::endl;
            std::cout << reply.toXML()   << std::endl;
        }
        virtual void onEvent(Connection& conn, const Status& result, Connection::Event event)
        {
            std::cout << result->get() << std::endl;
        }
   };
 * @endcode
 *
 * Example ReplyCallback registration:
 * @code
   MyReplyCallback* cb = new MyReplyCallback;
   Message reqMsg("GMSEC.MISSION.SAT.REQ.DIR", Message::REQUEST);
   int timeout = 1000;
   try {
       conn->request(reqMsg, timeout, cb);
   }
   catch (GmsecException& e) {
       std::cout << e.what() << std::endl;
   }
 * @endcode
*/
class GMSEC_API ReplyCallback : public EventCallback
{
public:
	// base class needs virtual destructor
	virtual ~ReplyCallback() {}


	/**
	 * @fn onReply(Connection& conn, const Message& request, const Message& reply)
	 *
	 * @brief This method is called by the API in response to a reply recieved from a request,
	 * from within the Request call. A class derived from ReplyCallback needs
	 * to be passed into the request() call.
	 *
	 * Note that if a callback is registered to multiple connections, onReply can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this method is:
	 * @code virtual void onReply(Connection& conn, const Message& request, const Message& reply) @endcode
	 *
	 * @note <b>DO NOT STORE or CHANGE STATE</b> of the Connection object that is passed to the callback
	 * method.
	 *
	 * @note<b>DO NOT STORE</b> the Message objects for use beyond the scope of the callback. Otherwise,
	 * make a copy of the Message object(s).
	 *
	 * @param conn - Connection on which the message was received
	 * @param request - the pending request
	 * @param reply - the received reply
	 *
	 * @sa Connection::request(const Message& request, GMSEC_I32 timeout, ReplyCallback *cb, GMSEC_I32 republish_ms = 0)
	*/
	virtual void CALL_TYPE onReply(Connection& conn, const Message& request, const Message& reply) = 0;
};

} // namespace api5
} // namespace gmsec

#endif
