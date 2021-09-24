
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Callback.h
 *
 *  @brief This file contains the abstract base class for received message callbacks
**/

#ifndef gmsec_Callback_h
#define gmsec_Callback_h

#include <gmsec/util/wdllexp.h>

namespace gmsec
{

class Connection;
class Message;

/** @class Callback
 * @brief This class is the abstract base class for received message callbacks.
 * A user created class, derrived from this class, can be passed into
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
 *	virtual void CALL_TYPE OnMessage(Connection *conn, Message *msg)
 *	{
 *		const char *tmp;
 *		msg->ToXML(tmp);
 *		cout << tmp << endl;
 *	}
 * };
 * @endcode
 *
 * Example Callback registration:
 * @code
 * PublishCallback cb;
 * result = conn->Subscribe("gmsec.*.publish",&cb);
 * if( result.isError() )
 *	// handle error
 * @endcode
 *
 * @sa Connection::Subscribe(const char *subject, Callback *cb) @n
 *     Connection::Request(Message *request, GMSEC_I32 timeout, Callback *cb)
*/
class GMSEC_API Callback
{
public:

	// base class needs virtual destructor
	virtual ~Callback() { }

	/** @fn OnMessage(Connection *conn, Message *msg)
	 * @brief This function is called by the API in response to a message, from either the DispatchMsg()
	 * call or inside the Auto-dispatcher after a StartAutoDispatch() call. A class derrived from
	 * gmsec::Callback needs to be registered with a connection, using Subscribe() or Request() in order
	 * to be called for a particular subject registration pattern.
	 *
	 * Please note that if a Callback is registered to multiple connections, OnMessage can be invoked
	 * concurrently from different connection threads.
	 *
	 *   The prototype for this funtion is:
	 *   @code virtual void CALL_TYPE OnMessage(Connection *conn, Message *msg) @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
	 * They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * the gmsec::Message, it must be cloned with Connection::CloneMessage().
	 *
	 * @param conn - connection on which the message was recieved
	 * @param msg - the recieved message
	 *
	 * @sa Connection::Subscribe(const char *subject, Callback *cb) @n
	 *     Connection::Request(Message *request, GMSEC_I32 timeout, Callback *cb) @n
	 *	   Connection::DispatchMsg(Message *  msg) @n
	 *     Connection::StartAutoDispatch() @n
	 *	   Connection::CloneMessage(Message * in, Message *& out) @n
	 */
	virtual void CALL_TYPE OnMessage(Connection *conn, Message *msg) = 0;
};

} // namespace gmsec

#endif /* gmsec_Callback_h */
