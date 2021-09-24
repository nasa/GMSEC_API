/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CONNECTION_NET_H
#define GMSEC_API_CONNECTION_NET_H


// C++ API native
#include <gmsec4/Connection.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Callback;
ref class Config;
ref class EventCallback;
ref class Message;
ref class ReplyCallback;
ref class SubscriptionInfo;


/// <summary>
/// This is the public interface for middleware connections. Each middleware connection
/// implements a class derived from Connection to abstract middleware specific connection
/// details. The application will never access the middleware connection directly, but
/// only through the Connection "interface".
/// </summary>


public ref class Connection
{
public:
	enum class ConnectionState
	{
		NOT_CONNECTED = 0,
		CONNECTED = 1,
		RECONNECTING = 2
	};


	enum class ConnectionEvent
	{
		DISPATCHER_ERROR_EVENT = 0,
		REQUEST_TIMEOUT_EVENT = 1,
		CONNECTION_SUCCESSFUL_EVENT = 2,
		CONNECTION_BROKEN_EVENT = 3,
		CONNECTION_RECONNECT_EVENT = 4,
		CONNECTION_EXCEPTION_EVENT = 5,
		GMD_ERROR_EVENT = 6,
		WSMQ_ASYNC_STATUS_CHECK_EVENT = 7,
		ALL_EVENTS = 8,
		MSG_PUBLISH_FAILURE_EVENT = 9,
		INVALID_MESSAGE_EVENT = 10
	};


	~Connection();


	/// <summary>Creates an instance of a Connection object based on the given configuration.</summary>
	/// <param name="config">The configuration object that specifies the type of Connection object to create.</param>
	/// <exception cref="GMSEC_Exception">The configuration information cannot be used to deduce the connection object type.</exception>
	static Connection^ Create(GMSEC::API::Config^ config);


	/// <summary>Destroys an instance of a Connection object.</summary>
	/// <param name="conn">The reference to the connection object to destroy.</param>
	/// <exception cref="GMSEC_Exception">The connection object is null.</exception>
	static void Destroy(Connection^ %conn);


	/// <summary>Identifies the version of the API.</summary>
	static System::String^ GetAPIVersion();


	/// <summary>Calls shutdown routines for each middleware that has a shutdown routine registered.</summary>
	static void ShutdownAllMiddlewares();


	/// <summary>Calls shutdown routines for the specific middleware identified by name.</summary>
	/// <param name="name">The name of the middleware to shutdown.</param>
	static void ShutdownMiddleware(System::String^ name);


	/// <summary>Establishes a connection to the middleware.</summary>
	/// <exception cref="GMSEC_Exception">If an anomaly occurs while connecting</exception>
	void Connect();


	/// <summary>Terminates the connection to the middleware.</summary>
	/// <exception cref="GMSEC_Exception">If an anomaly occurs while disconnecting</exception>
 	void Disconnect();


	/// <summary>Returns the current state of the connection to the middleware.</summary>
	/// <seealso cref="Connection::ConnectionState"/>
	ConnectionState GetState();


	/// <summary>Identifies the root library name and therefore the connection type that this connection is associated with.</summary>
	System::String^ GetLibraryRootName();


	/// <summary>Identifies the version information for this connection's associated middleware.</summary>
	System::String^ GetLibraryVersion();


	/// <summary>Allows for the registration of a callback to listen for a particular events, or for all events.</summary>
	/// <param name="event">The event to monitor.</param>
	/// <param name="ecb">The event callback to register.</param>
	void RegisterEventCallback(ConnectionEvent event, EventCallback^ ecb);


	/// <summary>This function subscribes to a particular subject or pattern and associates a callback to be called
	/// when messages matching the subject or pattern are received.</summary>
	/// <param name="subject">The subject string to use for registering the subscription.</param>
	/// <returns>A handle to a SubscriptionInfo object, which is required when cancelling a subscription.</returns>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to subscribe.</exception>
	/// <seealso cref="Connection::Unsubscribe"/>
	SubscriptionInfo^ Subscribe(System::String^ subject);


	/// <summary>This function subscribes to a particular subject or pattern and associates a callback to be called
	/// when messages matching the subject or pattern are received. If all subscriptions are performed using this
	/// function then the auto-dispatcher can be used to asynchronously receive messages. If Receive() is used to pull
	/// messages then Dispatch() will need to be called to ensure registered Callbacks are called.</summary>
	/// <param name="subject">The subject string to use for registering the subscription.</param>
	/// <param name="cb">The callback to associate with the subscription.</param>
	/// <returns>A handle to a SubscriptionInfo object, which is required when cancelling a subscription.</returns>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to subscribe.</exception>
	/// <seealso cref="Connection::Unsubscribe"/>
	SubscriptionInfo^ Subscribe(System::String^ subject, Callback^ cb);


	/// <summary>Subscribes to a particular subject and uses the provided config object to enable or disable special
	/// middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).</summary> 
	/// <param name="subject">The subject string to use for registering the subscription.</param>
	/// <param name="config">The configuration object.</param>
	/// <returns>A handle to a SubscriptionInfo object, which is required when cancelling a subscription.</returns>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to subscribe.</exception>
	/// <seealso cref="Connection::Unsubscribe"/>
	SubscriptionInfo^ Subscribe(System::String^ subject, Config^ config);


	/// <summary>This function subscribes to a particular subject or pattern and associates a callback to be called
	/// when messages matching the subject or pattern are received.  In addition, the provided config object is used
	/// to enable or disable special middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).
	/// If all subscriptions are performed using this function then the auto-dispatcher can be used to asynchronously
	/// receive messages. If Receive() is used to pull messages then Dispatch() will need to be called to ensure
	/// registered Callbacks are called.</summary>
	/// <param name="subject">The subject string to use for registering the subscription.</param>
	/// <param name="config">The configuration object.</param>
	/// <param name="cb">The callback to associate with the subscription.</param>
	/// <returns>A handle to a SubscriptionInfo object, which is required when cancelling a subscription.</returns>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to subscribe.</exception>
	/// <seealso cref="Connection::Unsubscribe"/>
	SubscriptionInfo^ Subscribe(System::String^ subject, Config^ config, Callback^ cb);


	/// <summary>Unsubscribes to a particular subject pattern, and will stop the reception of messages that match
	/// this pattern. It will also remove the registration of any callbacks with this subject pattern.</summary>
	/// <param name="info">A reference to the SubscriptionInfo object that is associated with a subscription.</param>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to unsubscribe.</exception>
	void Unsubscribe(SubscriptionInfo^ %info);


	/// <summary>Starts a thread that will dispatch messages asynchronously when they are received. If this is used,
	/// all subscriptions must be made with callbacks or the messages with be dropped. If Receive() is called while
	/// the auto-dispatcher is used, the behavior will be undesireable and undefined.</summary>
	bool StartAutoDispatch();


	/// <summary>Stop the auto dispatch thread.</summary>
	bool StopAutoDispatch();


	/// <summary>Stop the auto dispatch thread.</summary>
	/// <param name="waitForComplete">If set to true, the method will block until the auto-dispatch thread has completed.</param>
	bool StopAutoDispatch(bool waitForCompletion);


	/// <summary>Publishes a message to the middleware</summary>
	/// <param name="message">The message to publish.</summary>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to publish the message.</exception>
	void Publish(Message^ message);


	/// <summary>
	/// Publishes the given message to the middleware using the given configuration to enable or disable
	/// certain middleware-level publish functionalities (e.g. ActiveMQ - Durable Producer).
	/// <note type="note">
	/// The actual Message published to the middleware will contain tracking fields;
	/// to disable this feature, create a Connection object with the tracking=off
	/// configuration option.
	/// </note>
	/// </summary>
	/// <param name="message">The message to publish.
	/// <param name="mwConfig">Configuration object for providing middleware configuration options
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to publish the message.</exception>
	void Publish(Message^ message, Config^ mwConfig);


	/// <summary>Sends a request asyncronously. The callback will be called for the reply if it is received within
	/// the specified timeout. This function will not block. The timeout value is expressed in milliseconds.</summary>
	/// <param name="request">The request message</param>
	/// <param name="timeout">The maximum time (in milliseconds) to wait for a reply</param>
	/// <param name="rcb">The reply callback to use to receive the reply</param>
	/// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
	///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to issue the request or receive the reply message.</exception>
	void Request(Message^ request, System::Int32 timeout, ReplyCallback^ rcb, System::Int32 republish_ms);


	/// <summary>Sends a request, wait for the specified timeout, and return the received reply. This function will block until
	/// the reply is received or the timeout is reached. The timeout value is expressed in milliseconds.</summary>
	/// <param name="request">The request message</param>
	/// <param name="timeout">The maximum time (in milliseconds) to wait for a reply</param>
	/// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
	///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to issue the request or receive the reply message.</exception>
	/// <seealso cref="Connection::Release"/>
	Message^ Request(Message^ request, System::Int32 timeout, System::Int32 republish_ms);


	/// <summary>Cancels a pending request that may be associated with the given ReplyCallback.</summary>
	/// <param name="rcb">The reply callback that is currently being used to receive a reply message</param>
	void CancelRequest(ReplyCallback^ rcb);


	/// <summary>Sends a reply in response to a given request.</summary>
	/// <param name="request">The request message that was received, and to which we are responding to.</param>
	/// <param name="reply">The reply message to be sent.</param>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to issue the reply message.</exception>
	void Reply(Message^ request, Message^ reply);


	/// <summary>Used to invoke any callbacks that are registered with matching message subject patterns.</summary>
	/// <param name="message">The message to dispatch to the callbacks.</param>
	/// <exception cref="GMSEC_Exception">If an error occurs while attempting to issue the reply message.</exception>
	void Dispatch(Message^ message);


	/// <summary>Returns the next message received.  Note that this method will block indefinitely until a message is received.</summary>
	/// <returns>A handle to a message.</returns>
	/// <seealso cref="Connection::Release"/>
	Message^ Receive();


	/// <summary>Returns the next message received, within the specified timeout.</summary>
	/// <param name="timeout">The timeout period, in milliseconds.</param>
	/// <returns>A handle to a message.</returns>
	/// <seealso cref="Connection::Release"/>
	Message^ Receive(System::Int32 timeout);


	/// <summary>Destroys a Message object that has been previously returned by the API library.
	/// This method should NOT be used to destroy a Message object instantiated outside the API library,
	/// nor those received within a callback.</summary>
	///
	/// <param name="message">A reference to a message object</param>
	///
	/// <seealso cref="Connection::Receive"/>
	/// <seealso cref="Connection::Request"/>
	void Release(Message^ %message);


	/// <summary>Excludes any incoming messages with the specified subject from being received.</summary>
	/// <param name="subject">The subject pattern to use when filtering messages.</param>
	void ExcludeSubject(System::String^ subject);


	/// <summary>Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.</summary>
	/// <param name="subject">The subject pattern to remove.</param>
	void RemoveExcludedSubject(System::String^ subject);


	/// <summary>Returns the name of the connection, automatically generated or user specified.</summary>
	System::String^ GetName();


	/// <summary>Sets the logical name of this connection. This can be used for identifying connections withing
	/// a client program. If a name is not given, one will be automatically generated.</summary>
	/// <param name="name">The connection name.</param>
	void SetName(System::String^ name);


	/// <summary>Get the string ID for this connection.</summary>
	System::String^ GetID();


	/// <summary>Get the middleware information for this connection.</summary>
	System::String^ GetMWInfo();


	/// <summary>Retrieves the number of messages queued for asynchronous publish operations.</summary>
	System::UInt64 GetPublishQueueMessageCount();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Connection();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="conn">unmanaged implementation to initialize with</param>
	Connection(gmsec::api::Connection* conn);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::Connection* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::Connection* conn);


private:
	Connection();

	gmsec::api::Connection* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
