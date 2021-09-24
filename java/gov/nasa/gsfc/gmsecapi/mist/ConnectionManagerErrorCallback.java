/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConnectionManagerErrorCallback.java
 *
 *  @brief This file contains the abstract base class for error callbacks.
 **/

package gov.nasa.gsfc.gmsecapi.mist;

import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;


/** @interface ConnectionManagerErrorCallback
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This class is the abstract base class for error callbacks.
 * A user created class, derrived from this class, can be passed into %RegisterErrorCallback() to have
 * user code executed asynchronously when an error occurs in the connection object.
 * Please note that because users are able to create their own ConnectionManagerErrorCallback class, reentrancy
 * is not guarunteed unless if they implement their own reentrancy rules.
 * Also note that because a ConnectionManagerErrorCallback can be registered to multiple connections,
 * it can be run concurrently amongst those connections.  Because of this, the use of a gmsec::util::AutoMutex is
 * suggested to enforce thread safety.
 *
 * Example callback class:
 * @code
 * public class MyErrorCallback implements ConnectionManagerErrorCallback
 * {
 *     public void OnError(ConnectionManager connMgr, Message msg, Status status, String event)
 *     {
 *         System.out.println(status.Get());
 *     }
 * }
 * @endcode
 *
 * Example callback registration:
 * @code
 * MyErrorCallback cb = new MyErrorCallback();
 * result = connMgr.RegisterErrorCallback(GMSEC_CONNECTION_REQUEST_TIMEOUT, cb);
 * if (result.isError())
 *     //handle error
 * @endcode
 *
 * @sa ConnectionManager.RegisterErrorCallback(String event, ConnectionManagerErrorCallback cb)
 */
@Deprecated
public interface ConnectionManagerErrorCallback
{
	/** @fn OnError(ConnectionManager connMgr, Message msg, Status status)
	 * This function is called in response to a error after a call to %RegisterErrorCallback().
	 *
	 * Please note that if a callback is registered to multiple connections, OnError can be invoked concurrently
	 * from the different connection threads.
	 *
	 * The prototype for this funtion is:
	 * @code
	 * public void OnError(ConnectionManager connMgr, Message msg, Status status, String event)
	 * @endcode
	 *
	 * @note <b>DO NOT DESTROY</b> the ConnectionManager, Message, or Status that is passed into this function
	 * by the API.  They are owned by the API and do not need to be managed by the client program.  Also, they can
	 * not be stored by the client program beyond the scope of this callback function.  In order to store
	 * the Message, the message must be cloned with ConnectionManager.CloneMessage().
	 *
	 * @note Message is not returned in all uses of the callback; if it is not used, msg will be set to null.
	 *
	 * @sa ConnectionManager.RegisterErrorCallback(String event, ConnectionManagerErrorCallback cb)
	 */
	public void OnError(ConnectionManager connMgr, Message msg, Status status);
}
