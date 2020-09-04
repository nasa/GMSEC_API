/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ConnectionManagerEventCallback.h
 *
 *  @brief This file contains the abstract base class for event callbacks.
**/

#ifndef CONNECTION_MANAGER_EVENTCALLBACK_H
#define CONNECTION_MANAGER_EVENTCALLBACK_H

#include <gmsec4/Connection.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
// Forward declarations
class Message;
class Status;

namespace mist
{
// Forward declaration
class ConnectionManager;


/** @class ConnectionManagerEventCallback
 *
 * @brief This class is the abstract base class for MIST event callbacks.
 * A user created class, derrived from this class, can be passed into %registerEventCallback() to have
 * user code executed asynchronously when an event (typically an error) occurs in the connection object.
 * Please note that because users are able to create their own ConnectionManagerEventCallback class, reentrancy
 * is not guaranteed unless if they implement their own reentrancy rules.
 * Also note that because a ConnectionManagerEventCallback can be registered to multiple connections,
 * it can be run concurrently amongst those connections.  Because of this, the use of a gmsec::util::AutoMutex is
 * suggested to enforce thread safety.
 *
 * Example callback class:
 * @code
 * class MyEventCallback : public ConnectionManagerEventCallback
 * {
 * public:
 *     virtual void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
 *     {
 *         cout << status->get() << endl;
 *     }
 * }
 * @endcode
 *
 * Example callback registration:
 * @code
 * MyEventCallback* cb = new MyEventCallback;
 * connMgr->registerEventCallback(Connection::REQUEST_TIMEOUT, cb);
 * @endcode
 *
 * @sa ConnectionManager::registerEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback* cb)
*/
class GMSEC_API ConnectionManagerEventCallback
{
public:
	// base class needs virtual destructor
	virtual ~ConnectionManagerEventCallback() { }


	/**
	 * @fn onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
	 * This function is called in response to a event after a call to %registerEventCallback().
	 *
	 * Please note that if a callback is registered to multiple connections, onEvent() can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this funtion is:
	 * @code
	 * virtual void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
	 * @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager that is passed into this function
	 * by the API.  It is owned by the API and does not need to be managed by the client program.  Also,
	 * the ConnectionManager object should not be stored by the client program beyond the scope of this
	 * callback function.
	 *
	 * @param conn   - A reference to the ConnectionManager object that is reporting the event
	 * @param status - A reference to a status object that contains information regarding the event
	 * @param event  - the event that led the callback to be summoned
	 *
	 * @sa ConnectionManager::registerEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback* cb)
	 */
	virtual void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event) = 0;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
