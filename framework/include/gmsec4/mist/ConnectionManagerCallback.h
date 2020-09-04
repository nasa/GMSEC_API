/*
 * Copyright 2007-2018 United States Government as represented by the
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
 *     virtual void CALL_TYPE onMessage(ConnectionManager& connMgr, const Message& msg)
 *     {
 *         gmsec::util::AutoMutex lock(myMutex);
 *
 *         cout << msg.toXML() << endl;
 *     }
 *
 * private:
 *     gmsec::util::Mutex myMutex;
 * };
 * @endcode
 *
 * Example callback registration:
 * @code
 * int main(int argc, char** argv)
 * {
 *     MyCallback* cb = new MyCallback;
 *
 *     try {
 *         Config config(argc, argv);
 *         ConnectionManager connMgr(config);
 *
 *         connMgr.subscribe("GMSEC.MISSION.SAT.>", cb);
 *
 *          ...
 *     }
 *     catch (Exception& e) {
 *         cerr << e.what() << endl;
 *     }
 * }
 * @endcode
*/
class GMSEC_API ConnectionManagerCallback
{
public:
	// base class needs virtual destructor
	virtual ~ConnectionManagerCallback() { }


	/** @fn OnMessage(ConnectionManager& connMgr, const Message& msg)
	 * @brief This function is called by the API in response to a message, from either the dispatch()
	 * call or inside the Auto-dispatcher after a startAutoDispatch() call. A class derrived from
	 * ConnectionManagerCallback needs to be registered with a connection, using subscribe() in order
	 * to be called for a particular subject registration pattern.
	 *
	 * Please note that if a ConnectionManagerCallback is registered to multiple connections, onMessage
	 * can be invoked concurrently from different connection threads.
	 *
	 *   The prototype for this funtion is:
	 *   @code virtual void CALL_TYPE onMessage(ConnectionManager& connMgr, const Message& msg) @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager or Message that is passed into this function
	 * by the API.
	 * They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * the Message, a copy must be made.
	 *
	 * @param conn - connection manager on which the message was received
	 * @param msg - the received message
	 *
	 * @sa ConnectionManager::subscribe(const char* subject, ConnectionManagerCallback* cb) @n
	 *	   ConnectionManager::dispatch(const Message& msg) @n
	 *     ConnectionManager::startAutoDispatch() @n
	 */
	virtual void CALL_TYPE onMessage(ConnectionManager& connMgr, const Message& msg) = 0;
};

} // namespace mist
} // namespace api 
} // namespace gmsec

#endif
