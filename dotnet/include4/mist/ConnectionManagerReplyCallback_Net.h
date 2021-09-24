/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CONNECTION_MANAGER_REPLY_CALLBACK_NET_H
#define GMSEC_API_CONNECTION_MANAGER_REPLY_CALLBACK_NET_H

// Managed
#include <mist/ConnectionManagerEventCallback_Net.h>

// C++ API native
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Message;

namespace MIST
{

// Forward declarations
ref class ConnectionManager;


/// <summary>
/// This class is the abstract base class for received replies from asynchronous request messages.
/// A user created class, derrived from this class, can be passed into
/// %Request() to have user code executed asynchronously when a reply is received or when an error occurs.
/// Please note that because users are able to create their own ConnectionManagerReplyCallback class,
/// reentrancy is not guaranteed unless they implement their own reentrancy rules.
/// Also note that because a ConnectionManagerReplyCallback can be registered to multiple connections,
/// it can be run concurrently amongst those connections.
/// </summary>
///
/// <seealso cref="ConnectionManager::request"/>


public ref class ConnectionManagerReplyCallback abstract : public ConnectionManagerEventCallback
{
public:
	/// <summary>Default Constructor</summary>
	ConnectionManagerReplyCallback();


	/// <summary>Destructor</summary>
	virtual ~ConnectionManagerReplyCallback();


	/// <summary>
	/// This function is called in response to a error after a call to RegisterEventCallback().
	/// Please note that if an EventCallback is registered to multiple connections, OnEvent() can
	/// be invoked concurrently from the different connection threads.
	/// </summary
	///
	/// <remarks>
	/// <b>DO NOT DESTROY</b> the Connection or Message objects that are passed into this function by the API.
	/// They are owned by the API and do not need to be managed by the client program. Also, they should not
	/// be stored by the client program beyond the scope of this callback function.
	/// </remarks>
	///
    /// <param name="connMgr">ConnectionManager on which the message was recieved</param>
    /// <param name="request">The pending request message</param>
    /// <param name="reply">The received reply message</param>
	virtual void OnReply(ConnectionManager^ connMgr, Message^ request, Message^ reply);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ConnectionManagerReplyCallback();


	/// <summary>Pass through to callback managed class</summary>
	///
	/// <param name="connMgr">ConnectionManager on which the message was recieved</param>
    /// <param name="request">The pending request message</param>
    /// <param name="reply">The received reply message</param>
	void OnReplyNative(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& request, const gmsec::api::Message& reply);


internal:
	/// <summary>Get unmanaged implementation version</summary>
	///
    /// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::ConnectionManagerReplyCallback* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::ConnectionManagerReplyCallback* callback);


private:
	gmsec::api::mist::ConnectionManagerReplyCallback* m_impl;
};

}  // end namespace MIST
}  // end namespace API
}  // end namespace GMSEC

#endif
