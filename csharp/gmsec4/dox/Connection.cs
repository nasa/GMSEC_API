/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>
/// This is the public interface for middleware connections. Each middleware connection
/// implements a class derived from %Connection to abstract middleware specific connection
/// details. The application will never access the middleware connection directly, but
/// only through the %Connection "interface".
/// </summary>

public class Connection : global::System.IDisposable {

  /// <summary>Identifies the version of the API.</summary>
  public static string GetAPIVersion() {
  }

  /// <summary>Creates an instance of a Connection object based on the given configuration.</summary>
  /// <param name="cfg">The configuration object that specifies the type of Connection object to create.</param>
  /// <exception cref="GmsecException">The configuration information cannot be used to deduce the connection object type.</exception>
  public static Connection Create(Config cfg) {
  }

  /// <summary>Establishes a connection to the middleware.</summary>
  /// <exception cref="GmsecException">If an anomaly occurs while connecting</exception>
  public void Connect() {
  }

  /// <summary>Terminates the connection to the middleware.</summary>
  /// <exception cref="GmsecException">If an anomaly occurs while disconnecting</exception>
  public void Disconnect() {
  }

  /// <summary>Returns the current state of the connection to the middleware.</summary>
  /// <seealso cref="Connection.ConnectionState"/>
  public Connection.ConnectionState GetState() {
  }

  /// <summary>Identifies the root library name and therefore the connection type that this connection is associated with.</summary>
  public string GetLibraryRootName() {
  }

  /// <summary>Identifies the version information for this connection's associated middleware.</summary>
  public string GetLibraryVersion() {
  }

  /// <summary>Allows for the registration of a callback to listen for a particular events, or for all events.</summary>
  /// <param name="event">The event to monitor.</param>
  /// <param name="ecb">The event callback to register.</param>
  public void RegisterEventCallback(Connection.ConnectionEvent event, EventCallback ecb) {
  }

  /// <summary>This method subscribes to a particular subject or pattern and associates a callback to be called
  /// when messages matching the subject or pattern are received. If all subscriptions are performed using this
  /// method then the auto-dispatcher can be used to asynchronously receive messages. If Receive() is used to pull
  /// messages then Dispatch() will need to be called to ensure registered Callbacks are called.</summary>
  /// <param name="subject">The subject string to use for registering the subscription.</param>
  /// <param name="cb">The callback to associate with the subscription.</param>
  /// <returns>A handle to a SubscriptionInfo object, which is required when cancelling a subscription.</returns>
  /// <seealso cref="Connection.Unsubscribe"/>
  public SubscriptionInfo Subscribe(string subject, Callback cb) {
  }

  /// <summary>This method subscribes to a particular subject or pattern and associates a callback to be called
  /// when messages matching the subject or pattern are received.</summary>
  /// <param name="subject">The subject string to use for registering the subscription.</param>
  /// <returns>A handle to a SubscriptionInfo object, which is required when cancelling a subscription.</returns>
  /// <seealso cref="Connection.Unsubscribe"/>
  public SubscriptionInfo Subscribe(string subject) {
  }

  /// <summary>This method subscribes to a particular subject or pattern and associates a callback to be called
  /// when messages matching the subject or pattern are received.  In addition, the provided config object is used
  /// to enable or disable special middleware-level subscription methodalities (e.g. ActiveMQ Durable Consumer).
  /// If all subscriptions are performed using this method then the auto-dispatcher can be used to asynchronously
  /// receive messages. If Receive() is used to pull messages then Dispatch() will need to be called to ensure
  /// registered Callbacks are called.</summary>
  /// <param name="subject">The subject string to use for registering the subscription.</param>
  /// <param name="config">The configuration object.</param>
  /// <param name="cb">The callback to associate with the subscription.</param>
  /// <returns>A handle to a SubscriptionInfo object, which is required when cancelling a subscription.</returns>
  /// <seealso cref="Connection.Unsubscribe"/>
  public SubscriptionInfo Subscribe(string subject, Config config, Callback cb) {
  }

  /// <summary>Subscribes to a particular subject and uses the provided config object to enable or disable special
  /// middleware-level subscription methodalities (e.g. ActiveMQ Durable Consumer).</summary> 
  /// <param name="subject">The subject string to use for registering the subscription.</param>
  /// <param name="config">The configuration object.</param>
  /// <returns>A handle to a SubscriptionInfo object, which is required when cancelling a subscription.</returns>
  /// <seealso cref="Connection.Unsubscribe"/>
  public SubscriptionInfo Subscribe(string subject, Config config) {
  }

  /// <summary>Unsubscribes to a particular subject pattern, and will stop the reception of messages that match
  /// this pattern. It will also remove the registration of any callbacks with this subject pattern.</summary>
  /// <param name="info">A reference to the SubscriptionInfo object that is associated with a subscription.</param>
  /// <exception cref="GmsecException">Thrown if the given SubscriptionInfo object is null.</exception>
  /// <exception cref="GmsecException">Thrown if the given SubscriptionInfo object did not originate from the Connection object.</exception>
  /// <exception cref="GmsecException">Thrown if error occurs at the middleware level.</exception>
  public void Unsubscribe(SubscriptionInfo info) {
  }

  /// <summary>Starts a thread that will dispatch messages asynchronously when they are received. If this is used,
  /// all subscriptions must be made with callbacks or the messages with be dropped. If Receive() is called while
  /// the auto-dispatcher is used, the behavior will be undesireable and undefined.</summary>
  public bool StartAutoDispatch() {
  }

  /// <summary>Stop the auto dispatch thread.</summary>
  /// <param name="waitForCompletion">If set to true, the method will block until the auto-dispatch thread has completed.</param>
  public bool StopAutoDispatch(bool waitForCompletion) {
  }

  /// <summary>Stop the auto dispatch thread.</summary>
  public bool StopAutoDispatch() {
  }

  /// <summary>Publishes a message to the middleware</summary>
  /// <param name="msg">The message to publish.</param>
  /// <exception cref="GmsecException">If an error occurs while attempting to publish the message.</exception>
  public void Publish(Message msg) {
  }

  /// <summary>
  /// Publishes the given message to the middleware using the given configuration to enable or disable
  /// certain middleware-level publish methodalities (e.g. ActiveMQ - Durable Producer).
  /// <note type="note">
  /// The actual Message published to the middleware will contain tracking fields;
  /// to disable this feature, create a Connection object with the tracking=off
  /// configuration option.
  /// </note>
  /// </summary>
  /// <param name="msg">The message to publish.</param>
  /// <param name="mwConfig">Configuration object for providing middleware configuration options.</param>
  /// <exception cref="GmsecException">If an error occurs while attempting to publish the message.</exception>
  public void Publish(Message msg, Config mwConfig) {
  }

  /// <summary>Sends a request asyncronously. The callback will be called for the reply if it is received within
  /// the specified timeout. This method will not block. The timeout value is expressed in milliseconds.</summary>
  /// <param name="request">The request message</param>
  /// <param name="timeout">The maximum time (in milliseconds) to wait for a reply</param>
  /// <param name="rcb">The reply callback to use to receive the reply</param>
  /// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
  ///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
  ///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
  ///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
  /// <exception cref="GmsecException">If an error occurs while attempting to issue the request or receive the reply message.</exception>
  public void Request(Message request, int timeout, ReplyCallback rcb, int republish_ms) {
  }

  /// <summary>Sends a request asyncronously. The callback will be called for the reply if it is received within
  /// the specified timeout. This method will not block. The timeout value is expressed in milliseconds.</summary>
  /// <param name="request">The request message</param>
  /// <param name="timeout">The maximum time (in milliseconds) to wait for a reply</param>
  /// <param name="rcb">The reply callback to use to receive the reply</param>
  /// <exception cref="GmsecException">If an error occurs while attempting to issue the request or receive the reply message.</exception>
  public void Request(Message request, int timeout, ReplyCallback rcb) {
  }

  /// <summary>
  /// Sends a request, waits for the specified timeout, and returns the received reply (if any).
  /// This method will block until the reply is received or the timeout is reached. The timeout value is expressed in milliseconds.
  /// </summary>
  /// <param name="request">The request message</param>
  /// <param name="timeout">The maximum time (in milliseconds) to wait for a reply</param>
  /// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
  ///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
  ///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
  ///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
  /// <exception cref="GmsecException">If an error occurs while attempting to issue the request or receive the reply message.</exception>
  /// <seealso cref="Connection.Release"/>
  public Message Request(Message request, int timeout, int republish_ms) {
  }

  /// <summary>
  /// Sends a request, waits for the specified timeout, and returns the received reply (if any).
  /// This method will block until the reply is received or the timeout is reached. The timeout value is expressed in milliseconds.
  /// </summary>
  /// <param name="request">The request message</param>
  /// <param name="timeout">The maximum time (in milliseconds) to wait for a reply</param>
  /// <exception cref="GmsecException">If an error occurs while attempting to issue the request or receive the reply message.</exception>
  /// <seealso cref="Connection.Release"/>
  public Message Request(Message request, int timeout) {
  }

/*
  /// <summary>Cancels a pending request that may be associated with the given ReplyCallback.</summary>
  /// <param name="rcb">The reply callback that is currently being used to receive a reply message</param>
  public void CancelRequest(ReplyCallback rcb) {
  }
*/

  /// <summary>Sends a reply in response to a given request.</summary>
  /// <param name="request">The request message that was received, and to which we are responding to.</param>
  /// <param name="reply">The reply message to be sent.</param>
  /// <exception cref="GmsecException">If an error occurs while attempting to issue the reply message.</exception>
  public void Reply(Message request, Message reply) {
  }

  /// <summary>Used to invoke any callbacks that are registered with matching message subject patterns.</summary>
  /// <param name="msg">The message to dispatch to the callbacks.</param>
  /// <exception cref="GmsecException">If an error occurs while attempting to issue the reply message.</exception>
  public void Dispatch(Message msg) {
  }

  /// <summary>Returns the next message received, within the specified timeout.</summary>
  /// <param name="timeout">The timeout period, in milliseconds.</param>
  /// <returns>A handle to a message.</returns>
  /// <seealso cref="Connection.Release"/>
  public Message Receive(int timeout) {
  }

  /// <summary>Returns the next message received.  Note that this method will block indefinitely until a message is received.</summary>
  /// <returns>A handle to a message.</returns>
  /// <seealso cref="Connection.Release"/>
  public Message Receive() {
  }

  /// <summary>Excludes any incoming messages with the specified subject from being received.</summary>
  /// <param name="subject">The subject pattern to use when filtering messages.</param>
  /// <exception cref="GmsecException">Thrown if the given subject is null or contains an empty string.</exception>
  public void ExcludeSubject(string subject) {
  }

  /// <summary>Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.</summary>
  /// <param name="subject">The subject pattern to remove.</param>
  /// <exception cref="GmsecException">Thrown if the given subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if the given subject not currently being excluded.</exception>
  public void RemoveExcludedSubject(string subject) {
  }

  /// <summary>Returns the name of the connection, automatically generated or user specified.</summary>
  public string GetName() {
  }

  /// <summary>Sets the logical name of this connection. This can be used for identifying connections withing
  /// a client program. If a name is not given, one will be automatically generated.</summary>
  /// <param name="name">The connection name.</param>
  /// <exception cref="GmsecException">Thrown if the given name is null or contains an empty string.</exception>
  public void SetName(string name) {
  }

  /// <summary>Get the string ID for this connection.</summary>
  public string GetID() {
  }

  /// <summary>Get the middleware information for this connection.</summary>
  public string GetMWInfo() {
  }

  /// <summary>Returns middleware broker connection information.</summary>
  public string GetConnectionEndpoint() {
  }

  /// <summary>Retrieves the number of messages queued for asynchronous publish operations.</summary>
  public ulong GetPublishQueueMessageCount() {
  }

  /// <summary>Destroys a Message object that has been previously returned by the API library.
  /// This method should NOT be used to destroy a Message object instantiated outside the API library,
  /// nor those received within a callback.</summary>
  ///
  /// <param name="msg">A reference to a message object</param>
  ///
  /// <seealso cref="Connection.Receive"/>
  /// <seealso cref="Connection.Request"/>
  public void Release(Message msg) {
  }

  /// <summary>Destroys an instance of a Connection object.</summary>
  /// <param name="conn">The reference to the connection object to destroy.</param>
  /// <exception cref="GmsecException">The connection object is null.</exception>
  public static void Destroy(Connection conn) {
  }

  /// <summary>Possible states for the Connection object.</summary>
  public enum ConnectionState {
    /// <summary>Not connected to the middleware server.</summary>
    NOT_CONNECTED = 0,

    /// <summary>Connected to the middleware server.</summary>
    CONNECTED = 1,

    /// <summary>%Connection to the middleware server is broken; reattempting to connect.</summary>
    RECONNECTING = 2
  }

  /// <summary>Possible soft errors/events that can occur with Connection object.</summary>
  public enum ConnectionEvent {
    /// <summary>Error occurred while attempting to dispatch message.</summary>
    DISPATCHER_ERROR_EVENT = 0,

    /// <summary>A timeout occurred while attempting to receive a reply for a pending request.</summary>
    REQUEST_TIMEOUT_EVENT = 1,

    /// <summary>Successfully connected to the middleware server.</summary>
    CONNECTION_SUCCESSFUL_EVENT = 2,

    /// <summary>%Connection to middleware server has been broken.</summary>
    CONNECTION_BROKEN_EVENT = 3,

    /// <summary>An attempt is being made to reconnect to the middleware server.</summary>
    CONNECTION_RECONNECT_EVENT = 4,

    /// <summary>An error, possibly fatal, has occurred with the connection to the middleware.</summary>
    CONNECTION_EXCEPTION_EVENT = 5,

    /// <summary>SmartSockets Guaranteed Message Delivery (GMD) error.</summary>
    GMD_ERROR_EVENT = 6,

    /// <summary>WebSphere MQ Asynchronous Put Status reporting event.</summary>
    WSMQ_ASYNC_STATUS_CHECK_EVENT = 7,

    /// <summary>Monitor all events.</summary>
    ALL_EVENTS = 8,

    /// <summary>Failure occurred while attempting to asynchronously publish a message.</summary>
    MSG_PUBLISH_FAILURE_EVENT = 9,

    /// <summary>Message failed validation.</summary>
    INVALID_MESSAGE_EVENT = 10
  }

}

}
