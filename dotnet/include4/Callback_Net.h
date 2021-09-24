/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CALLBACK_NET_H
#define GMSEC_API_CALLBACK_NET_H


// C++ API native
#include <gmsec4/Callback.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Connection;
ref class Message;


/// <summary>
/// This class is the abstract base class for received message callbacks.
/// A user created class, derrived from this class, can be passed into
/// Subscribe() and Request() to have user code executed asynchronously
/// when a message is received.
/// </summary>
///
/// <seealso cref="Connection::Subscribe(System::String^ subject, GMSEC::API::Callback^ cb)"/>


public ref class Callback abstract
{
public:
	/// <summary>Default Constructor</summary>
	Callback();


	/// <summary>Destructor</summary>
	virtual ~Callback();


	/// <summary>
	/// This function is called by the API in response to a message, from either the Dispatch()
	/// call or inside the Auto-dispatcher after a StartAutoDispatch() call. A class derrived from
	/// Callback needs to be registered with a connection, using Subscribe() in order
	/// to be called for a particular subject registration pattern.
	/// </summary>
	///
	/// <remarks>
	/// <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
	/// They are owned by the API and do not need to be managed by the client program. Also, they can
	/// not be stored by the client program beyond the scope of this callback function.
	/// </remarks>
	///
    /// <param name="conn">connection on which the message was recieved</param>
    /// <param name="msg">the recieved message</param>
	virtual void OnMessage(Connection^ conn, Message^ msg);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Callback();


	/// <summary>Pass through to callback managed class</summary>
	///
	/// <param name="conn">connection on which the message was received</param>
    /// <param name="msg">the received message</param>
	void OnMessageNative(gmsec::api::Connection& conn, const gmsec::api::Message& msg);


internal:
	/// <summary>Get unmanaged implementation version</summary>
	///
    /// <returns>Unmanaged implementation</returns>
	gmsec::api::Callback* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::Callback* callback);


private:
	gmsec::api::Callback* m_impl;
};

}  // end namespace API
}  // end namespace GMSEC

#endif
