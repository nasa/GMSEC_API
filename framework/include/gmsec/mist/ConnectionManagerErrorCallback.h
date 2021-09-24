/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConnectionManagerErrorCallback.h
 *
 *  @brief This file contains the abstract base class for error callbacks.
**/

#ifndef CONNECTION_MANAGER_ERRORCALLBACK_H
#define CONNECTION_MANAGER_ERRORCALLBACK_H

#include <gmsec/util/wdllexp.h>

namespace gmsec
{
class Message;
class Status;

namespace mist
{
class ConnectionManager;

/** @class ConnectionManagerErrorCallback
 *
 * @brief This class is the abstract base class for error callbacks.
 * A user created class, derrived from this class, can be passed into %RegisterErrorCallback() to have
 * user code executed asynchronously when an error occurs in the connection object.
 * Please note that because users are able to create their own ConnectionManagerErrorCallback class, reentrancy
 * is not guaranteed unless if they implement their own reentrancy rules.
 * Also note that because a ConnectionManagerErrorCallback can be registered to multiple connections,
 * it can be run concurrently amongst those connections.  Because of this, the use of a gmsec::util::AutoMutex is
 * suggested to enforce thread safety.
 *
 * Example callback class:
 * @code
 * class MyErrorCallback : public ConnectionManagerErrorCallback
 * {
 * public:
 *     virtual void CALL_TYPE OnError(ConnectionManager* connMgr, Message* msg, Status* status, const char* event)
 *     {
 *         cout << status->Get() << endl;
 *     }
 * }
 * @endcode
 *
 * Example callback registration:
 * @code
 * MyErrorCallback* cb = new MyErrorCallback;
 * result = connMgr->RegisterErrorCallback(GMSEC_CONNECTION_REQUEST_TIMEOUT, cb);
 * if (result.isError())
 *     //handle error
 * @endcode
 *
 * @sa ConnectionManager::RegisterErrorCallback(const char* event, ConnectionManagerErrorCallback* cb)
*/
class GMSEC_API ConnectionManagerErrorCallback
{
public:
	// base class needs virtual destructor
	virtual ~ConnectionManagerErrorCallback() { }


	/** @fn OnError(ConnectionManager* connMgr, Message* msg, Status* status, const char* event)
	 * This function is called in response to a error after a call to %RegisterErrorCallback().
	 *
	 * Please note that if a callback is registered to multiple connections, OnError can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this funtion is:
	 * @code
	 * virtual void CALL_TYPE OnError(ConnectionManager* connMgr, Message* msg, Status* status, const char* event)
	 * @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager, Message, or Status that is passed into this function
	 * by the API.  They are owned by the API and do not need to be managed by the client program.  Also, they can
	 * not be stored by the client program beyond the scope of this callback function.  In order to store
	 * the gmsec::Message, the message must be cloned with ConnectionManager::CloneMessage().
	 *
	 * @note Message is not returned in all uses of the callback; if it is not used, msg will be set to NULL.
	 *
	 * @sa ConnectionManager::RegisterErrorCallback(const char* event, ConnectionManagerErrorCallback* cb)
	 */
	virtual void CALL_TYPE OnError(ConnectionManager* connMgr, Message* msg, Status* status, const char* event) = 0;
};

} // namespace mist

} // namespace gmsec

#endif
