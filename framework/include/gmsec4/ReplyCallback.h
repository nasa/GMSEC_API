/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ReplyCallback.h
 *
 *  @brief This file contains the abstract base class for asynchronous request-reply callbacks
 */

#ifndef GMSEC_API_REPLYCALLBACK_H
#define GMSEC_API_REPLYCALLBACK_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/EventCallback.h>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Connection;
class Message;


/**
 * @class ReplyCallback
 *
 * @brief This class is the abstract base class for received replies from asynchronous request messages.
 * A user created class, derived from this class, can be passed into %request() to have user code
 * executed asynchronously when a reply is received or when an error occurs.
 *
 * Note that because users are able to create their own ReplyCallback class, reentrancy is not
 * guaranteed unless if they implement their own reentrancy rules.
 *
 * In addition, if a ReplyCallback is registered to multiple Connection objects,
 * onReply() can be invoked concurrently from different threads. Use of a gmsec::util::AutoMutex is
 * suggested to enforce thread safety.
 *
 * Example ReplyCallback class:
   @code
   class ReqReplyCallback : public ReplyCallback
   {
   public:
       virtual void CALL_TYPE onReply(Connection& conn, const Message& request, const Message& reply)
       {
           std::cout << "Request:\n" << request.toXML() << "\n"
                     << "Reply:\n"   << reply.toXML()   << std::endl;
       }
  
       virtual void CALL_TYPE onEvent(Connection& conn, const Status& status, const char* event)
       {
           std::cout << "Status: " << status.get() << "\nEvent: " << event << std::endl;
       }
   };
   @endcode
 *
 * Example ReplyCallback registration:
   @code
   ReqReplyCallback cb;
   Message request("GMSEC.MY.REQUEST", Message::REQUEST);
   // Add message fields
   conn->request(request, timeout, &cb);
   @endcode
 *
 * @sa Connection::request()
 */
class GMSEC_API ReplyCallback : public gmsec::api::EventCallback
{
public:
	/**
	 * @fn onReply(Connection& conn, const Message& request, const Message& reply)
	 *
	 * @brief This method is called by the API in response to a reply recieved from a request,
	 * from within the request() call. A class derived from gmsec::api::ReplyCallback needs to be
	 * passed into the request() call.
	 *
	 * If a ReplyCallback is registered to multiple connections, onReply() can be
	 * invoked concurrently from the different connection threads.
	 *
	 * The prototype for this method is:
	 * @code virtual void CALL_TYPE onReply(Connection* conn, const Message& request, const Message& reply) @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY or CHANGE STATE</b> of the Connection object that is passed to the callback method,
	 * nor store it for use beyond the scope of the callback method.
	 *
	 * @note<b>DO NOT STORE</b> the Message objects for use beyond the scope of the callback. Otherwise,
	 * make a copy of the Message object(s).
	 *
	 * @param conn - connection on which the message was received
	 * @param request - the pending request message
	 * @param reply - the received reply message
	 *
	 * @sa Connection::request()
	 */
	virtual void CALL_TYPE onReply(Connection& conn, const Message& request, const Message& reply) = 0;
};

} // namespace api
} // namespace gmsec

#endif
