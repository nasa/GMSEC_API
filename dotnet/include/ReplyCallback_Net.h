
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
#ifndef ReplyCallback_Net
#define ReplyCallback_Net

// managed
#include "ErrorCallback_Net.h"
#include "GMSEC_Net.h"

// native
#include "gmsec/ReplyCallback.h"

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
/// This class is the abstract base class for received replies from asynchronous request messages.
/// A user created class, derrived from this class, can be passed into
/// Request() to have user code executed asynchronously when a relpy is received or when an error occurs.
/// </summary>
/// <example>
/// <code lang="C++">
/// //Example Callback class:
/// public ref class ReqReplyCallback : public ReplyCallback
/// {
/// 	public:
/// 	virtual void OnReply(Connection^ conn, Message^ request, Message^ reply) override
/// 	{
/// 		String^ tmp;
/// 		request->ToXML(tmp);
/// 		Console::WriteLine(tmp);
/// 		reply->ToXML(tmp);
/// 		Console::WriteLine(tmp);
/// 	}
/// 	virtual void OnError(Connection^ conn, Message^ request, Status^ result, String^ eventStr) override
/// 	{
/// 		Console::WriteLine(result->Get());
/// 	}
/// };
/// //Example ReplyCallback registration:
///  ReqReplyCallback^ cb = gcnew ReqReplyCallback();
///  Message ^request = nullptr;
///  // create request here
///  Status^ result = conn->Request(request, timeout, cb);
///  if( result->IsError() ) { /* handle error */ }
/// </code>
/// <code lang="C#">
/// //Example Callback class:
/// public class ReqReplyCallback : ReplyCallback
/// {
/// 	public override void OnReply(Connection conn, Message request, Message reply)
/// 	{
/// 		String tmp;
/// 		request.ToXML(out tmp);
/// 		Console.WriteLine(tmp);
///         reply.ToXML(out tmp);
/// 		Console.WriteLine(tmp);
/// 	}
/// 	public override void OnError(Connection conn, Message request, Status result, String eventStr)
/// 	{
/// 		Console.WriteLine(result.Get());
/// 	}
/// };
/// //Example ReplyCallback registration:
/// ReqReplyCallback cb = new ReqReplyCallback();
/// Message request = null;
/// // create request here
/// Status result = conn.Request(request, timeout, cb);
/// if (result.IsError()) { /* handle error */ }
/// </code>
/// <code lang="VB.NET">
/// //Example Callback class:
/// Public Class ReqReplyCallback
/// Inherits ReplyCallback
/// Public Overrides Sub OnReply(conn As Connection, request As Message, reply As Message)
///     Dim tmp As String
///     tmp = Nothing 
///     request.ToXML(tmp)
///     Console.WriteLine(tmp)
///     reply.ToXML(tmp)
///     Console.WriteLine(tmp)
/// End Sub
/// Public Overrides Sub OnError(conn As Connection, request As Message, result As Status, eventStr As String)
///     Console.WriteLine(result.Get())
/// End Sub
/// End Class
/// //Example ReplyCallback registration:
/// Dim cb As New ReqReplyCallback()
/// Dim request As Message
/// request = Nothing
/// ' create request here
/// Dim result As Status
/// result = conn.Request(request, timeout, cb)
/// If (result.IsError()) Then
///     ' handle error
/// End If
/// </code>
/// <code lang="f#">
/// type ReqReplyCallback() =
///     inherit ReplyCallback()
///         override this.OnError(conn, msg, status, eventStr) = 
///             Console.WriteLine(status.Get());
///         override this.OnReply(conn, request, reply) = 
///             let (result, tmp) = request.ToXML();
///             Console.WriteLine(tmp);
///             let (result2, tmp2) = reply.ToXML();
///             Console.WriteLine(tmp2);
/// let cb = new ReqReplyCallback();
/// let request : Message = null;
/// // create request here
/// let result3 = conn.Request(request, timeout, cb);
/// if( result3.IsError() ) then () // Handle Error
/// </code>
/// </example>
/// <seealso cref="Connection::Request(Message^ request, Int32 timeout, ReplyCallback^ cb, Int32 republish_ms)" />
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class ReplyCallback abstract : public Nasa::Gmsec::Net::ErrorCallback 
{
private:
	gmsec::ReplyCallback * mImpl;

internal:
	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ReplyCallback();

	/// <summary>
	/// Pass through to callback managed class
	/// </summary>
	/// <param name="conn">connection on which the message was recieved</param>
	/// <param name="request">the pending request</param>
	/// <param name="reply">the recieved reply</param>
	void OnReplyNative(gmsec::Connection *conn, gmsec::Message *request, gmsec::Message *reply);

public:
	/// <summary>Default Constructor</summary>
	ReplyCallback();
	
	/// <summary>Destructor</summary>
	virtual ~ReplyCallback();
	
	/// <summary>This function is called by the API in response to a reply recieved from a request,
	/// from within the Request call. A class derrived from gmsec::ReplyCallback needs to be
	/// passed into the Request() call.
	/// The prototype for this funtion is in the code section:
	/// The CALL_TYPE macro is required and is there to handle a compatibility
	/// problem with the Windows linker.
	/// </summary>
	/// <remarks>
	/// <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
	/// They are owned by the API and do not need to be managed by the client program. Also, they can
	/// not be stored by the client program beyond the scope of this callback function. In order to store
	/// the gmsec::Message, it must be cloned with Connection::CloneMessage().
	/// </remarks>
	/// <example>
	/// <code>
	/// virtual void CALL_TYPE OnReply(Connection *conn, Message *request, Message *reply)
	/// </code>  
	/// </example>
	/// <param name="conn">connection on which the message was recieved</param>
	/// <param name="request_msg">the pending request</param>
	/// <param name="reply_msg">the recieved reply</param>
	virtual void OnReply(Nasa::Gmsec::Net::Connection ^ conn, 
		Nasa::Gmsec::Net::Message ^ request_msg, Nasa::Gmsec::Net::Message ^ reply_msg);
	
	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::ReplyCallback * GetUnmanImpl();
};

}
}
}

#endif  // ReplyCallback_Net

