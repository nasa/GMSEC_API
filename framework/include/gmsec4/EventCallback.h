/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file EventCallback.h
 *
 *  @brief This file contains the abstract base class for event callbacks.
 *
**/

#ifndef GMSEC_API_EVENTCALLBACK_H
#define GMSEC_API_EVENTCALLBACK_H

#include <gmsec4/Connection.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Status;


/** @class EventCallback
 * @brief This class is the abstract base class for event callbacks.
 * A user created class, derrived from this class, can be passed into %registerEventCallback() to have
 * user code executed asynchronously when an event occurs in the connection object.
 * Please note that because users are able to create their own EventCallback class, reentrancy is not
 * guaranteed unless if they implement their own reentrancy rules.
 * Also note that because an EventCallback can be registered to multiple connections, it can be run
 * concurrently amongst those connections.  Because of this, the use of a gmsec::api::util::AutoMutex is
 * suggested to enforce thread safety.
 *
 * Example callback class:
 * @code
 * class MyEventCallback : public EventCallback
 * {
 * public:
 *     virtual void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
 *     {
 *         std::cout << status.get() << std::endl;
 *     }
 * };
 * @endcode
 *
 * Example callback registration:
 * @code
 * MyEventCallback cb;
 * conn->registerEventCallback(Connection::REQUEST_TIMEOUT, &cb);
 * @endcode
 *
 * @sa Connection::registerEventCallback()
*/
class GMSEC_API EventCallback
{
public:
	virtual ~EventCallback() {}


	/** @fn onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
	 * This function is called in response to a error after a call to %registerEventCallback().
	 *
	 * Please note that if an EventCallback is registered to multiple connections, onEvent() can be invoked
	 * concurrently from the different connection threads.
	 *
	 *   The prototype for this funtion is:
	 *   @code
	 *   virtual void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
	 *   @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the Connection that is passed into this function by the API.
	 * It is owned by the API and does not need to be managed by the client program. References to
	 * the Connection object, the Status object and the event string should not be stored by the
	 * client program beyond the scope of this callback function.
	 *
	 * @param conn   - A reference to the Connection object that is reporting the event
	 * @param status - A reference to a status object that contains information regarding the event
	 * @param event  - the event that led the callback to be summoned
	 *
	 * @sa Connection::registerEventCallback()
	 */
	virtual void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event) = 0;
};

} // namespace api
} // namespace gmsec

#endif
