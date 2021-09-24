/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/**
 * @interface ErrorCallback
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This interface is for received error callbacks. 
 * A user created class, implemented with this interface, can be passed into 
 * %RegisterErrorCallback() to have user code executed when the identified error
 * occurs. Please refer to the GMSEC API User's Guide for error string identifiers.
 * 
 * Example ErrorCallback class:
 * @code
 * class DispatchErrorCallback implements ErrorCallback
 * {
 *	public void OnError(Connection conn, Message msg, Status status) {
 *		System.out.println(status.Get());
 *		// Do not destroy the message here
 *	}
 * }
 * @endcode
 *
 * Example ErrorCallback registration:
 * @code
 * result = conn.RegisterErrorCallback("CONNECTION_DISPATCHER_ERROR",new DispatchErrorCallback());
 * if( result.isError() )
 *	// handle error
 * @endcode
 *
 * @sa Connection::RegisterErrorCallback(String error, ErrorCallback cb)
 */
@Deprecated
public interface ErrorCallback
{
	/**
	 * @brief This function is called by the API in response to a error.
	 * 
	 * @note <b>DO NOT DESTROY</b> the Connection, Message, or Status that is passed into this function by the API.
	 * They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * the gmsec::Message, it must be cloned with Connection::CloneMessage().
	 * 
	 * @param conn - connection related to the error
	 * @param msg - the message related to the error (if available - otherwise null)
	 * @param status - the status containing error details
	 * 
	 */
	public void OnError(Connection conn, Message msg, Status status);

}
