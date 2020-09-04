/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Callback.h
 *
 *  @brief This file contains the abstract base class for received message callbacks
**/

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


/** @class Callback
 * @brief This class is the abstract base class for received message callbacks.
 * A user created class, derived from this class, can be passed into
 * %Subscribe() and %Request() to have user code executed asynchronously
 * when a message is received.
 * Please note that because users are able to create their own Callback class, reentrancy is not
 * guarunteed unless if reentrancy rules are specified.
 * Also note that because a Callback can be registered to multiple connections, it can be run
 * concurrently amongst those connections.  Because of this, the use of a gmsec::util::AutoMutex
 * is suggested to enforce thread safety.
 *
 * Example Callback class:
 * @code
 * class PublishCallback : public Callback
 * {
 *	public:
 *	    virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg)
 *	    {
 *	    	std::cout << msg.toXML() << std::endl;
 *	    }
 * };
 * @endcode
 *
 * Example Callback registration:
 * @code
 * PublishCallback cb;
 * conn->subscribe("GMSEC.*.PUBLISH", &cb);
 * @endcode
 *
 * @sa Connection::subscribe(const char* subject, Callback* cb)
*/
class GMSEC_API Callback
{
public:
	// base class needs virtual destructor
	virtual ~Callback() {}


	/** @fn onMessage(Connection& conn, const Message& msg)
	 * @brief This function is called by the API in response to a message, from either the dispatchMsg()
	 * call or inside the Auto-dispatcher after a startAutoDispatch() call. A class derrived from
	 * gmsec::api::Callback needs to be registered with a connection, using subscribe() or request() in order
	 * to be called for a particular subject registration pattern.
	 *
	 * Please note that if a Callback is registered to multiple connections, onMessage() can be invoked
	 * concurrently from different connection threads.
	 *
	 *   The prototype for this funtion is:
	 *   @code virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg) @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
	 * They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * the Message, it must be copied.
	 *
	 * @param conn - connection on which the message was received
	 * @param msg - the received message
	 *
	 * @sa Connection::subscribe() @n
	 *     Connection::request() @n
	 *	   Connection::dispatchMessage() @n
	 *     Connection::startAutoDispatch()
	 */
	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg) = 0;
};

} // namespace api
} // namespace gmsec

#endif
