/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Callback.h
 *
 *  @brief This file contains the abstract base class for received message callbacks
 */

#ifndef GMSEC_API_CALLBACK_H
#define GMSEC_API_CALLBACK_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Connection;
class Message;


/**
 * @class Callback
 *
 * @brief This class is the abstract base class for received message callbacks.
 * A user created class, derived from this class, can be passed into
 * %subscribe() to have user code executed asynchronously when a message is received.
 *
 * Note that because users are able to create their own Callback class, reentrancy is not
 * guaranteed unless if reentrancy rules are specified.
 *
 * In addition, if a Callback is registered to multiple connections, onMessage() can be
 * invoked concurrently from different connection threads. Use of a gmsec::util::AutoMutex
 * is suggested to enforce thread safety.
 *
 * Example Callback class:
   @code
   class PublishCallback : public Callback
   {
  	public:
  	    virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg)
  	    {
  	    	std::cout << msg.toXML() << std::endl;
  	    }
   };
   @endcode
 *
 * Example Callback registration:
   @code
   PublishCallback cb;
   conn->subscribe("GMSEC.*.PUBLISH", &cb);
   @endcode
 *
 * @sa Connection::subscribe(const char* subject, Callback* cb)
*/
class GMSEC_API Callback
{
public:
	// base class needs virtual destructor
	virtual ~Callback() {}


	/**
	 * @fn onMessage(Connection& conn, const Message& msg)
	 *
	 * @brief This method is called by the API in response to a message, from either
	 * the dispatch() call or inside the Auto-dispatcher after a startAutoDispatch() call.
	 * A class derived from Callback needs to be registered with a connection, using
	 * subscribe() in order to be called for a particular subject registration pattern.
	 *
	 * If a Callback is registered to multiple connections, onMessage() can be
	 * invoked concurrently from different connection threads.
	 *
	 * The prototype for this method is:
	 * @code virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg) @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY or CHANGE STATE</b> of the Connection object that is passed to
	 * the callback method, nor store it for use beyond the scope of the callback method.
	 * <p>
	 * @note <b>DO NOT STORE</b> the Message object for use beyond the scope of the callback.
	 * Otherwise, make a copy of the Message object.
	 *
	 * @param conn - connection on which the message was received
	 * @param msg - the received message
	 *
	 * @sa Connection::subscribe()
	 * @sa Connection::dispatchMessage()
	 * @sa Connection::startAutoDispatch()
	 */
	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg) = 0;
};

} // namespace api
} // namespace gmsec

#endif
