
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
#ifndef Callback_Net
#define Callback_Net

// managed
#include "Connection_Net.h"
#include "Message_Net.h"
#include "GMSEC_Net.h"

// native
#include "gmsec/Callback.h"

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

// Forward references
ref class Config;
ref class Status;

/// <summary>
/// This class is the abstract base class for received message callbacks.
/// A user created class, derrived from this class, can be passed into
/// Subscribe() and Request() to have user code executed asynchronously
/// when a message is received.
/// </summary>
/// <example>
/// <code lang="C++">
/// Example Callback Class: C++ CLI:
/// public ref class PublishCallback : public Callback
/// {
/// 	public:
/// 	virtual void OnMessage(Connection^ conn, Message^ msg) override
/// 	{
/// 		String^ tmp;
/// 		msg->ToXML(tmp);
/// 		Console::WriteLine(tmp);
/// 	}
/// };
/// //Example Callback registration: C++ CLI:
/// PublishCallback^ cb = gcnew PublishCallback();
/// Status^ result = conn->Subscribe("gmsec.*.publish", cb);
/// if( result->IsError() ) { /* handle error */ }
/// </code>
/// <code lang="C#">
/// Example Callback Class: C#:
/// public class PublishCallback : Callback
/// {
/// 	public override void OnMessage(Connection conn, Message msg)
/// 	{
/// 		String tmp;
/// 		msg.ToXML(out tmp);
/// 		Console.WriteLine(tmp);
/// 	}
/// };
/// //Example Callback registration: C#:
/// PublishCallback cb = new PublishCallback();
/// Status result = conn.Subscribe("gmsec.*.publish", cb);
/// if (result.IsError()) { /* handle error */ }
/// </code>
/// <code lang="VB.NET">
/// 'Example Callback Class: VB:
/// Public Class PublishCallback
///     Inherits Callback
///     Public Overrides Sub OnMessage(conn As Connection, msg As Message)
///         Dim tmp As String
///         tmp = Nothing
///         msg.ToXML(tmp)
///         Console.WriteLine(tmp)
///     End Sub
/// End Class
/// 'Example Callback registration: VB:
/// Dim cb As New PublishCallback()
/// Dim result As Status
/// result = conn.Subscribe("gmsec.*.publish", cb)
/// If (result.IsError()) Then
///		' handle error
/// </code>
/// <code lang="f#">
/// //Example Callback registration: F#:
/// type PublishCallback() =
///     inherit Callback()
///         override this.OnMessage(conn, msg) = 
///             let (res, tmp) = msg.ToXML();
///             Console.WriteLine(tmp);
/// let cb = new PublishCallback();
/// let result = conn.Subscribe("gmsec.*.publish", cb);
/// if( result.IsError() ) then () // Handle Error
/// </code>
/// </example>
/// <seealso cref="Connection::Subscribe(const String ^ subject, Nasa::Gmsec::Net::Callback ^ cb)" />
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class Callback abstract
{
private:
	gmsec::Callback * mImpl;

internal:
	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::Callback * GetUnmanImpl();
	
	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanImpl(gmsec::Callback* callback);

protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Callback();

	/// <summary>
	/// Pass through to callback managed class
	/// </summary>
	/// <param name="conn">connection on which the message was recieved</param>
    /// <param name="msg">the recieved message</param>
	void OnMessageNative(gmsec::Connection *conn, gmsec::Message *msg);

public:
	/// <summary>Default Constructor</summary>
	Callback();
	
	/// <summary>Destructor</summary>
	virtual ~Callback();

	/// <summary>This function is called by the API in response to a message, from either the DispatchMsg()
	/// call or inside the Auto-dispatcher after a StartAutoDispatch() call. A class derrived from
	/// Callback needs to be registered with a connection, using Subscribe() in order
	/// to be called for a particular subject registration pattern.
	///  The prototype for this funtion is in the code section:</summary>
	/// <remarks>
	/// <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
	/// They are owned by the API and do not need to be managed by the client program. Also, they can
	/// not be stored by the client program beyond the scope of this callback function. In order to store
	/// the <see cref="Message"/>, it must be cloned with Connection::CloneMessage()"/>.
	/// </remarks>
    /// <param name="conn">connection on which the message was recieved</param>
    /// <param name="msg">the recieved message</param>
	virtual void OnMessage(Nasa::Gmsec::Net::Connection ^ conn, Nasa::Gmsec::Net::Message ^ msg);
};

}
}
}

#endif  // Callback_Net

