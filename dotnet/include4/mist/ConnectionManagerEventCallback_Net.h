/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CONNECTION_MANAGER_EVENT_CALLBACK_NET_H
#define GMSEC_API_CONNECTION_MANAGER_EVENT_CALLBACK_NET_H


#include <Connection_Net.h>


// C++ API native
#include <gmsec4/mist/ConnectionManagerEventCallback.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Status;

namespace MIST
{

// Forward declarations
ref class ConnectionManager;


/// <summary>
/// This class is the abstract base class for MIST event callbacks.
/// A user created class, derrived from this class, can be passed into %RegisterEventCallback() to have
/// user code executed asynchronously when an event (typically an error) occurs in the connection object.
/// Please note that because users are able to create their own ConnectionManagerEventCallback class, reentrancy
/// is not guaranteed unless if they implement their own reentrancy rules.
/// Also note that because a ConnectionManagerEventCallback can be registered to multiple connections,
/// it can be run concurrently amongst those connections.
/// </summary>
///
/// <seealso cref="ConnectionManager::registerEventCallback"/>


public ref class ConnectionManagerEventCallback abstract
{
public:
	/// <summary>Default Constructor</summary>
	ConnectionManagerEventCallback();


	/// <summary>Destructor</summary>
	virtual ~ConnectionManagerEventCallback();


	/// <summary>
	/// This function is called in response to a error after a call to RegisterEventCallback().
	/// Please note that if an EventCallback is registered to multiple connections, OnEvent() can
	/// be invoked concurrently from the different connection threads.
	/// </summary
	///
	/// <remarks>
	/// <b>DO NOT DESTROY</b> the Connection object that is passed into this function by the API.
	/// It is owned by the API and does not need to be managed by the client program. Also, it
	/// should not be stored by the client program beyond the scope of this callback function.
	/// </remarks>
	///
    /// <param name="connMgr">ConnectionManager on which the message was recieved</param>
    /// <param name="status">The status of the event being reported</param>
    /// <param name="event">The event that has occurred</param>
	virtual void OnEvent(ConnectionManager^ connMgr, Status^ status, Connection::ConnectionEvent event);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ConnectionManagerEventCallback();


	/// <summary>Pass through to callback managed class</summary>
	///
	/// <param name="connMgr">ConnectionManager on which the message was recieved</param>
    /// <param name="status">The status of the event being reported</param>
    /// <param name="event">The event that has occurred</param>
	void OnEventNative(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


internal:
	/// <summary>Get unmanaged implementation version</summary>
	///
    /// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::ConnectionManagerEventCallback* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::ConnectionManagerEventCallback* callback);


private:
	gmsec::api::mist::ConnectionManagerEventCallback* m_impl;
};

}  // end namespace MIST
}  // end namespace API
}  // end namespace GMSEC

#endif
