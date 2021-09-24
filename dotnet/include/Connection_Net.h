
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
#ifndef Connection_Net
#define Connection_Net

// managed
#include "Callback_Net.h"
#include "ErrorCallback_Net.h"
#include "ReplyCallback_Net.h"
#include "GMSEC_Net.h"
#include "GMSECDefs_Net.h"
#include "GMSECErrors_Net.h"

// native
#include "gmsec/Connection.h"

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

// Forward references
ref class ConnectionFactory;

/// <summary>
/// This is the public interface for middleware connections. Each middleware connection
/// implements a class derived from Connection to abstract middleware specific connection details.
/// The application will never access the middleware connection directly, but only through the Connection "interface".
///
/// The connection class provides services available on an implemented GMSEC connection. An application
/// can create multiple connection objects with different parameters and different middleware.
/// Creating multiple connections to the same middleware is not prevented but may not be supported
/// by all middleware implementations.
/// </summary>
/// <example>
/// <code lang="C++">
/// //Example creation and use: C++ CLI
/// Connection ^conn;
/// //Create config from command line arguments
/// Config^ cfg = gcnew Config(argv);
/// //Create the Connection
/// Status^ result = ConnectionFactory::Create(cfg, conn);
/// if( result->IsError() ) { /* handle error */ }
/// //Establish the connection
/// result = conn->Connect();
/// if( result->IsError() ) { /* handle error */ }
/// </code>
/// <code lang="C#">
/// //Example creation and use: C#
/// Connection conn;
/// //Create config from command line arguments
/// Config cfg = new Config(argv);
/// //Create the Connection
/// Status result = ConnectionFactory.Create(cfg, out conn);
/// if (result.IsError()) { /* handle error */ }
/// //Establish the connection
/// result = conn.Connect();
/// if (result.IsError()) { /* handle error */ }
/// </code>
/// <code lang="VB.NET">
/// 'Example creation and use: VB
/// Dim conn As Connection
/// conn = Nothing
/// 'Create config from command line arguments
/// Dim cfg As New Config(argv)
/// 'Create the Connection
/// Dim result As Status
/// result = ConnectionFactory.Create(cfg, conn)
/// If (result.IsError()) Then
///     'handle error
/// End If
/// 'Establish the connection
/// result = conn.Connect()
/// If (result.IsError()) Then
///     'handle error
/// End If
/// </code>
/// <code lang="f#">
/// //Create config from command line arguments
/// let cfg = new Config(argv);
/// //Create the Connection
/// let (result, conn) = ConnectionFactory.Create(cfg);
/// if( result.IsError() ) then () // Handle Error
/// //Establish the connection
/// let result2 = conn.Connect();
/// if( result2.IsError() ) then () // Handle Error
/// </code>
/// </example>
/// <seealso cref="ConnectionFactory" />
/// <seealso cref="Config" />
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class Connection
{
private:
	gmsec::Connection * mImpl;

	/// <summary>This function establishes this connection to the middleware</summary>
	Connection();

	/// <summary>Destructor</summary>
	~Connection();

internal:
	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::Connection * GetUnmanImpl();

	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanImpl(gmsec::Connection* connection);

	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	/// <param name="conn">unmanaged implementation to initialize with</param>
	Connection(gmsec::Connection * conn);

protected:
	/// <summary>Finalizer, set objects to null</summary>
	!Connection();

public:
	/// <summary>This function establishes this connection to the middleware</summary>
	/// <returns>status - result of the connection operation</returns>
    Nasa::Gmsec::Net::Status ^ Connect();


	/// <summary>This function terminates this connection to the middleware.
	/// It is automatically called by the destructor if necessary</summary>
	/// <returns>status - result of the connection operation</returns>
    Nasa::Gmsec::Net::Status ^ Disconnect();

	/// <summary>This function returns t/f whether the connection has
	/// been established. Note that this function will return true even if the
	/// connection has been broken and the client is attempting to reconnect,
	/// it simply reflects whether or not a Connect() call has been made on this
	/// connection.</summary>
	/// <returns>true if connected</returns>
	bool IsConnected();

	/// <summary>This function identifies the root library name and therefore the
	/// connection type that this connection is associated with. For example,
	/// the root library name for the icsswb middleware library is "gmsec_icsswb"
	/// and matches the name of the windows library (gmsec_icsswb.dll) and the
	/// UNIX library (libgmsec_icsswb.so).</summary>
	/// <seealso cref="Message::GetLibraryRootName()" />
	/// <returns>root library name</returns>
	String ^ GetLibraryRootName();

	/// <summary>This function returns a string containing the version information for
	/// this connection's associated middleware.</summary>
	/// <returns>library version</returns>
	String ^ GetLibraryVersion();

	/// <summary>This function allows the registration of a callback for a particular
	/// error event. Event names are middleware dependent.
	/// All connection types implement at least following error events:
	/// </summary>
	/// <remarks><para>"CONNECTION_DISPATCHER_ERROR" - Auto-dispatcher error callback that gets called
	///  whenever an error occurs inside the auto-dispatcher.</para>
	/// <para>"CONNECTION_REQUEST_TIMEOUT" - Request w/callback error callback that gets called
	/// whenever an error occurs while trying to process an request (ex. timeout)</para>
    /// </remarks>
	/// <seealso cref="ErrorCallback" />
	/// <param name="event_str">name of event to register</param>
	/// <param name="ecb">object derrived from ErrorCallback to register for this error event</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ RegisterErrorCallback(
		const String ^ event_str, Nasa::Gmsec::Net::ErrorCallback ^ ecb);

	/// <summary>
	/// This function subscribes to a particular subject or pattern. This
	/// causes middleware routing of messages that match that subject or pattern
	/// be queued for this process. Messages that are subscribed to without callback
	/// need to be pulled from the queue using GetNextMsg() and are thrown away if
	/// the auto-dispatcher is used.
	/// Example subscription patterns in the code section:
	/// </summary>
	/// <remarks>Although subscription behavior is outlines as above, the actual behavior for a particular
	/// middleware implementation MAY deviate from this behavior slightly.
	/// </remarks>
	/// <example>
	/// <code>
	/// // this will match only messages with this exact subject
	/// conn-&gt;Subscribe("gmsec.mission.const.sat.evt.msg");
	/// // this will match messages with any mission
	/// conn-&gt;Subscribe("gmsec.*.const.sat.evt.msg");
	/// // this will match messages that have AT LEAST ONE MORE TAG
	/// //	(will not match "gmsec.mission.const.sat")
	/// conn-&gt;Subscribe("gmsec.mission.const.sat.&gt;");
	/// // this will match any event message
	/// conn-&gt;Subscribe("gmsec.*.*.*.evt.&gt;");
    /// </code>
	/// </example>
	/// <seealso cref="Connection::GetNextMsg([Out] Message^% msg, UInt32 timeout)" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <param name="subject">subject pattern to match received messages</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ Subscribe(const String ^ subject);

	/// <summary>
	/// This function subscribes to a particular subject or pattern and
	/// associates a callback to be called when messages matching the subject
	/// or pattern are received. If all subscriptions are performed using this
	/// function then the auto-dispatcher can be used to asynchronously receive
	/// messages. If GetNextMsg() is used to pull messages then DispatchMsg()
	/// will need to be called to ensure registered Callbacks are called.
	/// <b>see Connection::Subscribe(const char *subject) for an explaination of subscription patterns</b>
	/// </summary>
	/// <seealso cref="Connection::Subscribe(const String^ subject)" />
	/// <seealso cref="Connection::GetNextMsg([Out] Message^% msg, UInt32 timeout)" />
	/// <seealso cref="Connection::DispatchMsg(Message^ msg)" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <seealso cref="Connection::StopAutoDispatch()" />
	/// <param name="subject">subject pattern to match received messages</param>
	/// <param name="cb">callback to be called when message is received</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ Subscribe(const String ^ subject, Nasa::Gmsec::Net::Callback ^ cb);

	/// <summary>This function unsubscribes to a particular subject pattern, and will stop the reception
	/// of messages that match this pattern. It will also remove the registration of any callbacks with
	/// this subject pattern.</summary>
	/// <seealso cref="Connection::GetNextMsg([Out] Message^% msg, UInt32 timeout)" />
	/// <seealso cref="Connection::DispatchMsg(Message^ msg)" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <param name="subject">subject pattern that was used to match received messages</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ UnSubscribe( 
		const String ^ subject);

	/// <summary>This function unsubscribes a single callback to a particular subject pattern, and
	/// will not unsubscribe the reception of the message. It will prevent a particular callback
	/// from being called by the auto-dispatch or DispatchMsg(), but the message will still be
	/// received for GetNextMsg().</summary>
	/// <seealso cref="Connection::Subscribe(const String^ subject)" />
	/// <seealso cref="Connection::GetNextMsg([Out] Message^% msg, UInt32 timeout)" />
	/// <seealso cref="Connection::DispatchMsg(Message^ msg)" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <param name="subject">subject pattern that was used to match received messages</param>
	/// <param name="cb">callback to be called when message is received</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ UnSubscribe(const String ^ subject, Nasa::Gmsec::Net::Callback ^ cb);

	/// <summary>
	/// This function creates a message for this particular middleware connection.
	/// The kind and subject are set to defaults dependent upon the particular middleware implementation.
	/// </summary>
	/// <param name="msg">Message pointer to be filled by created message</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ CreateMessage(
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %msg);

	/// <summary>
	/// This function creates a message for this particular middleware connection.
	/// The subject name for this call MUST be a valid subject name and NOT a pattern.
	/// </summary>
	/// <param name="subject">subject under which this message will eventually be published/requested</param>
	/// <param name="msgKind">indentifier for the intended message kind</param>
	/// <param name="msg">Message pointer to be filled by created message</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ CreateMessage( 
		const String ^ subject, GMSECMsgKindDefs msgKind, 
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %msg);

	/// <summary>The Config object can be used to set Subject, Kind or middleware specific settings.
	/// The options are specific to middleware implementation and are listed in the <i>GMSEC User's Guide</i></summary>
	/// <param name="subject">subject under which this message will eventually be published/requested</param>
	/// <param name="msgKind">indentifier for the intended message kind</param>
	/// <param name="msg">Message pointer to be filled by created message</param>
	/// <param name="config">Config object used to pass parameters to the middleware specific message implementation</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ CreateMessage(const String ^ subject,
		GMSECMsgKindDefs msgKind, [Runtime::InteropServices::Out] 
		Nasa::Gmsec::Net::Message ^ %msg, Nasa::Gmsec::Net::Config ^ config);

	/// <summary>
	/// This function will call "CloneMessage" to copy a message to this connection
	/// from another connection, only if necessary. This is used by the API, or a client program
	/// to publish a message that was created or recieved on on middleware to another.
	/// </summary>
	/// <remarks>
	/// ConvertCleanup() needs to be called with the same parameters, after the message is no
	/// longer needed, to ensure any memory required is cleaned up.
	/// </remarks>
	/// <seealso cref="Connection::ConvertCleanup( Message^ in, Message^ out )" />
	/// <param name="in_msg">message to convert FROM</param>
	/// <param name="out_msg">out parameter filled with the coverted message (could be the same if conversion wasn't necessary)</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ ConvertMessage(Nasa::Gmsec::Net::Message ^ in_msg, 
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %out_msg);

	/// <summary>This function cleans up any memory allocated by ConvertMessage() when
	/// finished with the associated message. The parameters need to be
	/// exactly the same as those originally passed to ConvertMessage().</summary>
	/// <param name="in_msg">message that was originally coverted FROM</param>
	/// <param name="out_msg">message that was output by the ConvertMessage() call</param>
	/// <returns>status - result of the connection operation</returns>
	/// <seealso cref="Connection::ConvertMessage( Message^ in, Message^% out )" />
	Nasa::Gmsec::Net::Status ^ ConvertCleanup( 
		Nasa::Gmsec::Net::Message ^ in_msg, Nasa::Gmsec::Net::Message ^ out_msg);

	/// <summary>
	/// This function copies a message without knowing what type it is. This
	/// function can be used to copy a message's contents.
	/// </summary>
	/// <remarks>
	/// The 'out' message MUST BE CLEANED UP with DestroyMessage() by the client application.
	/// </remarks>
	/// <seealso cref="Connection::DestroyMessage(Message^ msg)" />
	/// <param name="in_msg">message to be cloned</param>
	/// <param name="out_msg">cloned message</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ CloneMessage(Nasa::Gmsec::Net::Message ^ in_msg, 
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %out_msg);

	/// <summary>This function destroys a message and cleans up any associated memory.</summary>
	/// <param name="msg">message to be destroyed</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ DestroyMessage(Nasa::Gmsec::Net::Message ^ msg);

	/// <summary>This function will publish a message to the middleware.</summary>
	/// <param name="msg">message to be published</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ Publish(Nasa::Gmsec::Net::Message ^ msg);

	/// <summary>This function will send a request asyncronously. The callback will be called for the reply
	/// if it is received within the specified timeout. This function will not block.
	/// The timeout value is expressed in milliseconds.
	/// </summary>
	/// <remarks>
	/// <B>IMPORTANT:</B> The request message passed into this function will be cleaned up when the processing
	/// is complete, therefore it must be created new and not stored or cleaned up by the client program.
    /// </remarks>
	/// <param name="request_msg">message to be sent</param>
	/// <param name="timeout">maximum time to wait for reply (in milliseconds)</param>
	/// <param name="rcb">Callback to call when reply is receieved</param>
	/// <param name="republish_ms">Republish time, in ms</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ Request(Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, 
		Nasa::Gmsec::Net::ReplyCallback ^ rcb, Int32 republish_ms);

	/// <summary>This function will send a request, wait for the specified timeout, and return the received reply
	/// This function will block until the reply is received or the timeout is reached.
	/// The timeout value is expressed in milliseconds.</summary>
	/// <param name="request_msg">message to be sent</param>
	/// <param name="timeout">maximum time to wait for reply (in milliseconds)</param>
	/// <param name="reply_msg">out parameter reply message if received</param>
	/// <param name="republish_ms">Republish time, in ms</param>
	/// <returns>status - result of the connection operation</returns>
	Nasa::Gmsec::Net::Status ^ Request(Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, 
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %reply_msg, Int32 republish_ms);

	/// <summary>This function will send a request asyncronously. The callback will be called for the reply
	/// if it is received within the specified timeout. This function will not block.
	/// The timeout value is expressed in milliseconds.
	/// </summary>
	/// <remarks>
	/// <B>IMPORTANT:</B> The request message passed into this function will be cleaned up when the processing
	/// is complete, therefore it must be created new and not stored or cleaned up by the client program. 
    /// </remarks>
	/// <param name="request_msg">message to be sent</param>
	/// <param name="timeout">maximum time to wait for reply (in milliseconds)</param>
	/// <param name="rcb">Callback to call when reply is receieved</param>
	/// <returns>result of the request</returns>
	Nasa::Gmsec::Net::Status ^ Request(Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, 
		Nasa::Gmsec::Net::ReplyCallback ^ rcb);

	/// <summary>
	/// This function will send a request, wait for the specified timeout, and return the received reply
	/// This function will block until the reply is received or the timeout is reached.
	/// The timeout value is expressed in milliseconds.</summary>
	/// <param name="request_msg">message to be sent</param>
	/// <param name="timeout">maximum time to wait for reply (in milliseconds)</param>
	/// <param name="reply_msg">out parameter reply message if received</param>
	/// <returns>result of the request</returns>
	Nasa::Gmsec::Net::Status ^ Request(Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, 
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %reply_msg);

	/// <summary>This function will send a reply to a given request.</summary>
	/// <param name="request_msg">the recieved request that we are responding to</param>
	/// <param name="reply_msg">the reply to be sent</param>
	/// <returns>result of the reply operation</returns>
	Nasa::Gmsec::Net::Status ^ Reply(Nasa::Gmsec::Net::Message ^ request_msg, 
		Nasa::Gmsec::Net::Message ^ reply_msg);

	/// <summary>This function will start a thread that will dispatch messages asynchronously when they are received.
	/// If this is used, all subscriptions must be made with callbacks or the messages with be dropped.
	/// If GetNextMessage() is called while the auto-dispatcher is used, the behavior will be undesireable
	/// and undefined.</summary>
	/// <seealso cref="Connection::Subscribe(const String^ subject, Callback^ cb)" />
	/// <returns>result of the start operation</returns>
	Nasa::Gmsec::Net::Status ^ StartAutoDispatch();

	/// <summary>This function will stop the auto dispatch thread..</summary>
	/// <returns>result of the stop operation</returns>
	Nasa::Gmsec::Net::Status ^ StopAutoDispatch();

	/// <summary>This function will stop the auto dispatch thread..</summary>
	/// <param name="waitForComplete">if should wait for auto dispatch thread to complete</param>
	/// <returns>result of the stop operation</returns>
	Nasa::Gmsec::Net::Status ^ StopAutoDispatch(bool waitForComplete);

	/// <summary>This function returns the next message received within the specified timeout.
	/// The received messages are determined by the Subscribe() function(s), but
	/// DispatchMsg() needs to be called messages received from this function to
	/// ensure all registered callbacks are executed.
	/// </summary>
	/// <remarks>This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.</remarks>
	/// <seealso cref="Connection::Subscribe(const String^ subject)" />
	/// <seealso cref="Connection::Subscribe(const String^ subject, Callback^ cb)" />
	/// <seealso cref="Connection::DispatchMsg(Message^ msg)" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <param name="msg">out parameter, the next received message, if any</param>
	/// <param name="timeout">the maximum time to block waiting for a message, in milliseconds</param>
	/// <returns>result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetNextMsg(
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %msg, 
		Int32 timeout);

	/// <summary>This function will cause the any callbacks that are registered with matching
	/// patterns to be called.</summary>
	/// <seealso cref="Connection::GetNextMsg([Out] Message^% msg, UInt32 timeout)" />
    /// <param name="msg">message to be dispatched</param>
	/// <returns>result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ DispatchMsg( 
		Nasa::Gmsec::Net::Message ^ msg);

	/// <summary>When running with the auto-dispatcher, it may be necessary to monitor the
	/// status as it runs within a seperate thread.  This method allows access to the
	/// last status error reported by the dispatcher.  Once the status is read, the
	/// status is cleared.</summary>
	/// <remarks>
	/// Another way to be notified in the case of a dispatcher error is to register an
	/// error callback with the RegisterErrorCallback() function.
	/// </remarks>
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <seealso cref="Connection::StartAutoDispatch()" />
	/// <seealso cref="Connection::RegisterErrorCallback(const String ^ event_str, ErrorCallback ^ ecb)" />
	/// <returns>result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetLastDispatcherStatus();

	/// <summary>Get the logical name of this connection, if one has been assigned.
	/// This is useful for identifying connections within a client program.</summary>
	/// <returns>name of this connection</returns>
	const String ^ GetName();

	/// <summary>Set the logical name of this connection. This can be useful for
	/// Identifying connections withing a client program.</summary>
	/// <param name="name">name of this connection</param>
	void SetName(const String ^ name);


	/// <summary>Thread Safe alternative method for GetMWINFO(). </summary>
	/// <remarks>NOTE: This may ONLY be called after connected, using some middlewares</remarks>
	/// <returns>String of result</returns>
	String ^ GetMWINFO();
};

}
}
}

#endif  // Connection_Net


