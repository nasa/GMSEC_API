/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConnectionManagerCallback.h
 *
 *  @brief This file contains the abstract base class for received message callbacks
**/

#ifndef CONNECTION_MANAGER_CALLBACK_H
#define CONNECTION_MANAGER_CALLBACK_H

#include <gmsec/Callback.h>

#include <gmsec/util/wdllexp.h>

namespace gmsec
{
class Message;

namespace mist
{
class ConnectionManager;

/** @class ConnectionManagerCallback
 * @brief This class is the abstract base class for received message callbacks.
 * A user created class, derrived from this class, can be passed into
 * %Subscribe() to have user code executed asynchronously when a message is received.
 * Please note that because users are able to create their own Callback class, reentrancy is not
 * guaranteed unless if reentrancy rules are specified.
 * Also note that because a ConnectionManagerCallback can be registered to multiple Connection
 * Managers, it can be run concurrently amongst those Connection Managers.  Because of this,
 * the use of a gmsec::util::AutoMutex is suggested to enforce thread safety.
 *
 * Example ConnectionManagerCallback class:
 * @code
 * class MyCallback : public ConnectionManagerCallback
 * {
 * public:
 *     virtual void CALL_TYPE OnMessage(ConnectionManager* connMgr, Message* msg)
 *     {
 *         gmsec::util::AutoMutex lock(myMutex);
 *
 *         const char* tmp = 0;
 *         msg->ToXML(tmp);
 *         cout << tmp << endl;
 *     }
 *
 * private:
 *     gmsec::util::Mutex myMutex;
 * };
 * @endcode
 *
 * Example callback registration:
 * @code
 * MyCallback* cb = new MyCallback;
 * result = connMgr->Subscribe("GMSEC.SAT.MISSION.>", cb);
 * if (result.IsError())
 *     // handle error
 * @endcode
 *
 * @sa ConnectionManager::Subscribe(const char *subject, Callback *cb) @n
*/
class GMSEC_API ConnectionManagerCallback
{
public:
	// base class needs virtual destructor
	virtual ~ConnectionManagerCallback() { }


	/** @fn OnMessage(ConnectionManager* connMgr, Message* msg)
	 * @brief This function is called by the API in response to a message, from either the DispatchMsg()
	 * call or inside the Auto-dispatcher after a StartAutoDispatch() call. A class derrived from
	 * gmsec::ConnectionManagerCallback needs to be registered with a connection, using Subscribe() in order
	 * to be called for a particular subject registration pattern.
	 *
	 * Please note that if a ConnectionManagerCallback is registered to multiple connections, OnMessage
	 * can be invoked concurrently from different connection threads.
	 *
	 *   The prototype for this funtion is:
	 *   @code virtual void CALL_TYPE OnMessage(ConnectionManager* connMgr, Message* msg) @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager or Message that is passed into this function
	 * by the API.
	 * They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * the gmsec::Message, it must be cloned with ConnectionManager::CloneMessage().
	 *
	 * @param conn - connection manager on which the message was recieved
	 * @param msg - the recieved message
	 *
	 * @sa ConnectionManager::Subscribe(const char* subject, ConnectionManagerCallback* cb) @n
	 *	   ConnectionManager::DispatchMsg(Message*  msg) @n
	 *     ConnectionManager::StartAutoDispatch() @n
	 *	   ConnectionManager::CloneMessage(Message* in, Message*& out) @n
	 */
	virtual void CALL_TYPE OnMessage(ConnectionManager* connMgr, Message* msg) = 0;
};

} // namespace mist

} // namespace gmsec

#endif
