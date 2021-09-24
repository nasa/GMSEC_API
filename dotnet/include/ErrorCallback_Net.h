
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/    
#ifndef ErrorCallback_Net
#define ErrorCallback_Net

// managed
#include "Message_Net.h"
#include "GMSEC_Net.h"

// native
#include "gmsec/ErrorCallback.h"

using namespace System;
using namespace gmsec;

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

// Forward references
ref class Config;
ref class Connection;
ref class Status;


/// <summary>
/// This class is the abstract base class for error callbacks.
/// A user created class, derrived from this class, can be passed into
/// RegisterErrorCallback() to have user code executed asynchronously
/// when an error occurs in the connection object.
/// </summary>
/// <example>
/// <code lang="C++">
/// //Example callback class: C++ CLI
/// public ref class DumpErrorCallback : public ErrorCallback
/// {
/// public:
/// 	virtual void OnError(Connection^ conn, Message^ msg, Status^ status, String^ event) override
/// 	{
/// 		Console::WriteLine(status->Get());
/// 	}
/// };
/// //Example callback registration:
/// DumpErrorCallback^ cb = gcnew DumpErrorCallback();
/// Status^ result = conn->RegisterErrorCallback("CONNECTION_REQUEST_TIMEOUT", cb);
/// if( result->IsError() ) { /* handle error */ }
/// </code>
/// <code lang="C#">
/// //Example callback class: C#
/// public class DumpErrorCallback : ErrorCallback
/// {
/// 	public override void OnError(Connection conn, Message msg, Status status, String eventStr)
/// 	{
/// 		Console.WriteLine(status.Get());
/// 	}
/// };
/// //Example callback registration:
/// DumpErrorCallback cb = new DumpErrorCallback();
/// Status result = conn.RegisterErrorCallback("CONNECTION_REQUEST_TIMEOUT", cb);
/// if (result.IsError()) { /* handle error */ }
/// </code>
/// <code lang="VB.NET">
/// 'Example callback class: VB
/// Public Class DumpErrorCallback
///     Inherits ErrorCallback
///     Public Overrides Sub OnError(conn As Connection, msg As Message, status As Status, eventStr As String)
///         Console.WriteLine(status.Get())
///     End Sub
/// End Class
/// 'Example callback registration:
/// Dim cb As New DumpErrorCallback()
/// Dim result As Status
/// result = conn.RegisterErrorCallback("CONNECTION_REQUEST_TIMEOUT", cb)
/// If (result.IsError()) Then
///     'handle error
/// End If
/// </code>
/// <code lang="f#">
/// type DumpErrorCallback() =
///     inherit ErrorCallback()
///         override this.OnError(conn, msg, status, eventStr) = 
///             Console.WriteLine(status.Get());
/// let cb = new DumpErrorCallback();
/// let result = conn.RegisterErrorCallback("CONNECTION_REQUEST_TIMEOUT", cb);
/// if( result.IsError() ) then () // Handle Error
/// </code>
/// </example>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class ErrorCallback abstract
{
private:
	gmsec::ErrorCallback * mImpl;

protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ErrorCallback();

	/// <summary>
	/// Pass through to callback managed class
	/// </summary>
	/// <param name="conn">connection</param>
	/// <param name="msg">msg that gets error</param>
	/// <param name="status">status showing error</param>
	/// <param name="event">string of event</param>
	void OnErrorNative(gmsec::Connection *conn, gmsec::Message *msg, gmsec::Status *status, const char* event);

internal:	
	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::ErrorCallback * GetUnmanImpl();
	
	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanImpl(gmsec::ErrorCallback* errorCallback);
public:
	/// <summary>Constructor</summary>
	ErrorCallback();
	
	/// <summary>Destructor</summary>
	virtual ~ErrorCallback();

	/// <summary>This function is called in response to a error after a call to RegisterErrorCallback().</summary>
	/// <remarks>
	/// <b>DO NOT DESTROY</b> the Connection, Message, or Status that is passed into this function by the API.
	/// They are owned by the API and do not need to be managed by the client program. Also, they can
	/// not be stored by the client program beyond the scope of this callback function. In order to store
	/// the gmsec::Message, it must be cloned with Connection::CloneMessage().
	/// Message is not returned in all uses of the callback, if it is not used, msg will be set to NULL.
	/// </remarks>
	/// <example>
	/// <code>
	/// virtual void CALL_TYPE OnError(Connection *conn, Message *msg, Status *status, const char* event)
	/// </code>
	/// </example>
	/// <param name="conn">connection</param>
	/// <param name="msg">msg that gets error</param>
	/// <param name="status">status showing error</param>
	/// <param name="event_str">string of event</param>
	virtual void OnError(Nasa::Gmsec::Net::Connection ^ conn, 
		Nasa::Gmsec::Net::Message ^ msg, Nasa::Gmsec::Net::Status ^ status, 
		String ^ event_str);
};

}
}
}

#endif  // ErrorCallback_Net

