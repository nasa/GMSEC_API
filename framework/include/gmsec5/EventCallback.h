/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file EventCallback.h
 *
 *  @brief This file contains the abstract base class for event callbacks.
 */

#ifndef GMSEC_API5_EVENTCALLBACK_H
#define GMSEC_API5_EVENTCALLBACK_H

#include <gmsec5/util/wdllexp.h>

#include <gmsec5/Connection.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Message;
	class Status;


/**
 * @class EventCallback
 *
 * @brief This class is the abstract base class for event callbacks.
 * A user created class, derived from this class, can be passed into %registerEventCallback() to have
 * user code executed asynchronously when an event (typically an error) occurs in the connection object.
 *
 * Note that because users are able to create their own EventCallback class, reentrancy
 * is not guaranteed unless if they implement their own reentrancy rules.
 *
 * In addition, if a EventCallback is registered to multiple Connection objects,
 * onEvent() can be invoked concurrently from different threads. Use of a AutoMutex is
 * suggested to enforce thread safety.
 *
 * Example callback class:
 * @code
   class MyEventCallback : public EventCallback
   {
   public:
       virtual void onEvent(Connection& conn, const Status& status, Connection::Event event)
       {
           std::cout << status->get() << std::endl;
       }
   }
 * @endcode
 *
 * Example callback registration:
 * @code
   MyEventCallback* cb = new MyEventCallback;
   conn->registerEventCallback(Connection::REQUEST_TIMEOUT, cb);
 * @endcode
 *
 * @sa Connection::registerEventCallback(Connection::Event event, EventCallback* cb)
*/
class GMSEC_API EventCallback
{
public:
	// base class needs virtual destructor
	virtual ~EventCallback() { }


	/**
	 * @fn onEvent(Connection& conn, const Status& status, Connection::Event event)
	 *
	 * @brief This method is called in response to a event after a call to %registerEventCallback().
	 *
	 * If a callback is registered to multiple connections, onEvent() can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this method is:
	 * @code
	 * virtual void onEvent(Connection& conn, const Status& status, Connection::Event event)
	 * @endcode
	 *
	 * @note <b>DO NOT STORE or CHANGE STATE</b> of the Connection object that is passed to the callback
	 * method.
	 *
	 * @note <b>DO NOT STORE</b> the Status object for use beyond the scope of the callback. Otherwise,
	 * make a copy of the Status object.
	 *
	 * @param conn   - A reference to the Connection object that is reporting the event
	 * @param status - A reference to a status object that contains information regarding the event
	 * @param event  - the event that led the callback to be summoned
	 *
	 * @sa Connection::registerEventCallback(Connection::Event event, EventCallback* cb)
	 */
	virtual void CALL_TYPE onEvent(Connection& conn, const Status& status, gmsec::api5::Connection::Event event) = 0;
};

} // namespace api5
} // namespace gmsec

#endif
