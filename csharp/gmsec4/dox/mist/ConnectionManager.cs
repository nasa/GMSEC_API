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
/// This class wraps the %GMSEC Connection object, supplying both normal middleware
/// interaction functions and some additional quality of service. This is accomplished
/// through standard services for producing %Log and Heartbeat messages, as well as
/// functionality for validating messages.
/// </summary>

public class ConnectionManager : global::System.IDisposable {

  /// <summary>Identifies the version of the API.</summary>
  public static string GetAPIVersion() {
  }

  /// <summary>
  /// Creates a new %ConnectionManager object with the given configuration.
  /// <p>
  /// Note: Message validation will be disabled, unless the configuration option GMSEC-MSG-CONTENT-VALIDATE
  /// is set to true. If GMSEC-MSG-CONTENT-VALIDATE is set to true, then messages will be validated
  /// against the default version of the %GMSEC Interface Specification Document (ISD). The version
  /// of the ISD can be changed by specifying the configuration option GMSEC-SPECIFICATION-VERSION to
  /// a valid value (e.g. 201400).
  /// </summary>
  ///
  /// <param name="cfg">The configuration object that specifies the type of connection object to create</param>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the configuration information cannot be used to deduce
  /// the connection type, or if an anomaly occurs while loading schemas for the specified ISD.
  /// </exception>
  public ConnectionManager(Config cfg) {
  }

  /// <summary>
  /// Creates a new %ConnectionManager object with the given configuration.
  /// <p>
  /// Note: If message validation is enabled, then messages will be validated against the default version of
  /// the %GMSEC Interface Specification Document (ISD). The version of the ISD can be changed by
  /// specifying the configuration option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
  /// </summary>
  ///
  /// <param name="cfg">The configuration object that specifies the type of connection object to create</param>
  /// <param name="validate">Used to indicate whether %ConnectionManager should validate messages that are produced</param>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the configuration information cannot be used to deduce
  /// the connection type, or if an anomaly occurs while loading schemas for the specified ISD.
  /// </exception>
  public ConnectionManager(Config cfg, bool validate) {
  }

  /// <summary>
  /// Creates a new %ConnectionManager object with the given configuration.
  /// <p>
  /// Note: If message validation is enabled, then messages will be validated against the default version of
  /// the %GMSEC Interface Specification Document (ISD). The version of the ISD can be changed by
  /// specifying the configuration option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
  /// </summary>
  ///
  /// <param name="cfg">The configuration object that specifies the type of connection object to create</param>
  /// <param name="validate">Used to indicate whether %ConnectionManager should validate messages that are produced</param>
  /// <param name="version">The version of the %GMSEC Interface Specification Document (ISD) to use when producing messages</param>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the configuration information cannot be used to deduce
  /// the connection type, or if an anomaly occurs while loading schemas for the specified ISD.
  /// </exception>
  ///
  /// <seealso cref="Gmsec"/>
  public ConnectionManager(Config cfg, bool validate, uint version) {
  }

  /// <summary>
  /// Establishes a connection with the %GMSEC middleware server.
  /// Once this call successfully returns, the %ConnectionManager is ready for message operations.
  /// </summary>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if an anomaly occurs while attempting to connect to the middleware server.
  /// </exception>
  public void Initialize() {
  }

  /// <summary>This function disconnects and destroys the underlying connection object.</summary>
  /// <exception cref="GmsecException">If an anomaly occurs while disconnecting</exception>
  public void Cleanup() {
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

  /// <summary>Returns the Specification object associated with the Connection Manager.</summary>
  public Specification GetSpecification() {
  }

  /// <summary>
  /// This method will allow for a user to register their custom subclass of the
  /// Specification class with the Connection Manager.  This custom Specification can
  /// implement its own validateMessage() method which can be used to perform validation
  /// of messages currently not performed by the GMSEC API.
  /// <p>
  /// Note the API does not assume ownership of the provided Specification object, nor
  /// does it make a copy of such. The user is responsible to ensure that
  /// the provided Specification object is not destroyed while the ConnectionManager is
  /// in possession of such.
  /// </summary>
  ///
  /// <param name="spec">A specialized subclass of the Specification class</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given Specification object is null</exception>
  public void SetSpecification(Specification spec) {
  }

  /// <summary>
  /// Sets the internal list of fields that are added to all messages that are created
  /// using the ConnectionManager.
  /// <p>
  /// Internal copies of the provided Fields are made, thus ownership is not retained by
  /// the ConnectionManager. The supplied set of fields will not be validated here;
  /// validation occurs at the time a message is to be published.
  /// </summary>
  ///
  /// <param name="standardFields">The list of fields to set as standard fields</param>
  public void SetStandardFields(FieldList standardFields) {
  }

  /// <summary>
  /// Returns the list of standard fields that are associated with the Connection Manager.
  /// </summary>
  public FieldList GetStandardFields() {
  }

  /// <summary>Adds the standard fields (if any) to the given Message object.</summary>
  ///
  /// <param name="msg">The message in which to add the standard fields</param>
  public void AddStandardFields(Message msg) {
  }

  /// <summary>Registers the given callback for the specified event.</summary>
  ///
  /// <param name="event">The event to monitor</param>
  /// <param name="ecb">The Connection Manager event callback</param>
  ///
  /// <exception cref="GmsecException">Thrown if the callback is null</exception>
  ///
  /// <seealso cref="Connection.ConnectionEvent"/>
  public void RegisterEventCallback(Connection.ConnectionEvent event, ConnectionManagerEventCallback ecb) {
  }

  /// <summary>This method subscribes to a particular subject or pattern.</summary>
  ///
  /// <param name="subject">subject pattern to match received messages</param>
  ///
  /// <returns>A MIST_SubscriptionInfo object is returned; this should be used when unsubscribing</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the given subject is null or consists of an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if a duplicate subscription is made</exception>
  ///
  /// <seealso cref="ConnectionManager.Receive"/>
  /// <seealso cref="ConnectionManager.Unsubscribe"/>
  public MIST_SubscriptionInfo Subscribe(string subject) {
  }

  /// <summary>
  /// This method subscribes to a particular subject or pattern, and dispatches received messages to
  /// the given callback.
  /// </summary>
  ///
  /// <param name="subject">subject pattern to match received messages</param>
  /// <param name="cb">Connection Manager callback to be used to receive subscribed messages</param>
  ///
  /// <returns>A MIST_SubscriptionInfo object is returned; this should be used when unsubscribing</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the given subject is null or consists of an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if the given callback is null</exception>
  /// <exception cref="GmsecException">Thrown if a duplicate subscription is made</exception>
  ///
  /// <seealso cref="ConnectionManager.StartAutoDispatch"/>
  /// <seealso cref="ConnectionManager.Unsubscribe"/>
  public MIST_SubscriptionInfo Subscribe(string subject, ConnectionManagerCallback cb) {
  }

  /// <summary>
  /// This method subscribes to a particular subject or pattern, and uses the config object
  /// provide to toggle between middleware-level subscription functionalities (e.g. ActiveMQ
  /// Durable Consumer).
  /// </summary>
  ///
  /// <param name="subject">subject pattern to match received messages</param>
  /// <param name="config">The configuration to be used for subscription operation</param>
  ///
  /// <returns>A MIST_SubscriptionInfo object is returned; this should be used when unsubscribing</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the given subject is null or consists of an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if a duplicate subscription is made</exception>
  ///
  /// <seealso cref="ConnectionManager.Receive"/>
  /// <seealso cref="ConnectionManager.Unsubscribe"/>
  public MIST_SubscriptionInfo Subscribe(string subject, Config config) {
  }

  /// <summary>
  /// This method subscribes to a particular subject or pattern, and dispatches received messages to
  /// the given callback. In addition, the method uses the provided config object to enable or disable
  /// middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).
  /// </summary>
  ///
  /// <param name="subject">subject pattern to match received messages</param>
  /// <param name="config">The configuration to be used for subscription operation</param>
  /// <param name="cb">Connection Manager callback to be used to receive subscribed messages</param>
  ///
  /// <returns>A MIST_SubscriptionInfo object is returned; this should be used when unsubscribing</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the given subject is null or consists of an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if the given callback is null</exception>
  /// <exception cref="GmsecException">Thrown if a duplicate subscription is made</exception>
  ///
  /// <seealso cref="ConnectionManager.StartAutoDispatch"/>
  /// <seealso cref="ConnectionManager.Unsubscribe"/>
  public MIST_SubscriptionInfo Subscribe(string subject, Config config, ConnectionManagerCallback cb) {
  }

  /// <summary>
  /// This method unsubscribes to a particular subject or pattern, and will stop the reception
  /// or messages that match this pattern. It will also remove the registration of any callbacks
  /// with this subject pattern.
  /// </summary>
  ///
  /// <param name="info">The reference to the MIST_SubscriptionInfo object</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given SubscriptionInfo object is null.</exception>
  /// <exception cref="GmsecException">Thrown if the given SubscriptionInfo object did not originate from the ConnectionManager object.</exception>
  /// <exception cref="GmsecException">Thrown if error occurs at the middleware level.</exception>

  public void Unsubscribe(MIST_SubscriptionInfo info) {
  }

  /// <summary>
  /// If this connection manager has been created with "validate"
  /// option disabled, this is a pass-through function to the underlying connection.
  /// Otherwise the message will be validated before it is published.
  /// </summary>
  ///
  /// <param name="msg">The message to publish</param>
  ///
  /// <exception cref="GmsecException">Thrown if message is non-PUBLISH kind of message.</exception>
  /// <exception cref="GmsecException">Thrown if message fails validation.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void Publish(Message msg) {
  }

  /// <summary>
  /// Publishes the given message to the middleware using the given configuration to enable or disable
  /// certain middleware-level publish methodalities (e.g. ActiveMQ - Durable Producer). Message will
  //  still be validated if message validation is enabled.
  /// <note type="note">
  /// The actual Message published to the middleware will contain tracking fields;
  /// to disable this feature, create a ConnectionManager object with the tracking=off
  /// configuration option.
  /// </note>
  /// </summary>
  ///
  /// <param name="msg">The message to publish</param>
  /// <param name="mwConfig">The configuration object for providing middleware configuration options</param>
  ///
  /// <exception cref="GmsecException">Thrown if message is non-PUBLISH kind of message.</exception>
  /// <exception cref="GmsecException">Thrown if message fails validation.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void Publish(Message msg, Config mwConfig) {
  }

  /// <summary>
  /// If this connection manager has been created with "validate"
  /// option disabled, this is a pass-through function to the underlying connection.
  /// Otherwise the request message will be validated before it is issued.
  /// </summary>
  ///
  /// <param name="request">The request message to issue</param>
  /// <param name="timeout">maximum time (in milliseconds) to wait for reply</param>
  /// <param name="cb">callback to call when reply is received</param>
  /// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
  ///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
  ///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
  ///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
  ///
  /// <exception cref="GmsecException">Thrown if message is non-REQUEST kind of message.</exception>
  /// <exception cref="GmsecException">An exception is thrown if a middleware error occurs.</exception>
  ///
  /// <seealso cref="ConnectionManager.Release"/>
  public void Request(Message request, int timeout, ConnectionManagerReplyCallback cb, int republish_ms) {
  }

  /// <summary>
  /// If this connection manager has been created with "validate"
  /// option disabled, this is a pass-through function to the underlying connection.
  /// Otherwise the request message will be validated before it is issued.
  /// </summary>
  ///
  /// <param name="request">The request message to issue</param>
  /// <param name="timeout">maximum time (in milliseconds) to wait for reply</param>
  /// <param name="cb">callback to call when reply is received</param>
  ///
  /// <exception cref="GmsecException">Thrown if message is non-REQUEST kind of message.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  ///
  /// <seealso cref="ConnectionManager.Release"/>
  public void Request(Message request, int timeout, ConnectionManagerReplyCallback cb) {
  }

  /// <summary>
  /// If this connection manager has been created with "validate"
  /// option disabled, this is a pass-through function to the underlying connection.
  /// Otherwise the request message will be validated before it is issued.
  /// </summary>
  ///
  /// <param name="request">The request message to issue</param>
  /// <param name="timeout">maximum time (in milliseconds) to wait for reply</param>
  /// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
  ///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
  ///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
  ///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
  ///
  /// <returns>A reply message, or null if a timeout occurs.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if message is non-REQUEST kind of message.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  ///
  /// <seealso cref="ConnectionManager.Release"/>
  public Message Request(Message request, int timeout, int republish_ms) {
  }

  /// <summary>
  /// If this connection manager has been created with "validate"
  /// option disabled, this is a pass-through function to the underlying connection.
  /// Otherwise the request message will be validated before it is issued.
  /// </summary>
  ///
  /// <param name="request">The request message to issue</param>
  /// <param name="timeout">maximum time (in milliseconds) to wait for reply</param>
  ///
  /// <returns>A reply message, or null if a timeout occurs.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if message is non-REQUEST kind of message.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  ///
  /// <seealso cref="ConnectionManager.Release"/>
  public Message Request(Message request, int timeout) {
  }

  /// <summary>
  /// Used to cancel a pending request that is associated with
  /// the given ConnectionManagerReplyCallback object.
  /// </summary>
  ///
  /// <param name="cb">The Connection Manager reply callback previously used with a call to Request()</param>
  ///
  /// <exception cref="GmsecException">Thrown if the callback is null.</exception>
  public void CancelRequest(ConnectionManagerReplyCallback cb) {
  }

  /// <summary>
  /// If this connection manager has been created with "validate"
  /// option disabled, this is a pass-through function to the underlying connection.
  /// Otherwise the request message will be validated before it is issued.
  /// </summary>
  ///
  /// <param name="request">The pending request message</param>
  /// <param name="reply">The reply message to send in response to the request</param>
  ///
  /// <exception cref="GmsecException">Thrown if request message is non-REQUEST kind of message.</exception>
  /// <exception cref="GmsecException">Thrown if reply message is non-REPLY kind of message.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void Reply(Message request, Message reply) {
  }

  /// <summary>Attempts to dispatch the given message to callbacks that have been previously registered.
  ///
  /// <param name="msg">The message to dispatch</param>
  public void Dispatch(Message msg) {
  }

  /// <summary>
  /// This method blocks until a message is returned, or the timeout period has elapsed.
  /// The received messages are determined by the %Subscribe() Function(s).
  /// </summary>
  ///
  /// <param name="timeout">The maximum time (in milliseconds) to wait to receive a message</param>
  ///
  /// <returns>
  /// The next message whose subject matches the pattern of those previously subscribed to, or null
  /// if a timeout occurs.
  /// </returns>
  ///
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  ///
  /// <seealso cref="ConnectionManager.Release"/>
  public Message Receive(int timeout) {
  }

  /// <summary>Destroys a Message object that has been previously returned by the API library.
  /// This method should NOT be used to destroy a Message object instantiated outside the API library,
  /// nor those received within a callback.</summary>
  ///
  /// <param name="message">A reference to a message object</param>
  ///
  /// <seealso cref="ConnectionManager.Receive"/>
  /// <seealso cref="ConnectionManager.Request"/>
  /// <seealso cref="ConnectionManager.RequestDirective"/>
  /// <seealso cref="ConnectionManager.RequestSimpleService"/>
  public void Release(Message message) {
  }

  /// <summary>
  /// This method will start a thread that will dispatch messages asynchronously when they are received.
  /// If this is used, all subscriptions must be made with callbacks or the messages will be dropped.
  /// If Receive() is called while the auto-dispatcher is used, the behavior will be undesireable
  /// and undefined.
  /// </summary>
  ///
  /// <returns>True if the operation is successful; false otherwise.</returns>
  ///
  /// <seealso cref="ConnectionManager.StopAutoDispatch"/>
  public bool StartAutoDispatch() {
  }

  /// <summary>This function will stop the auto dispatch thread.</summary>
  ///
  /// <param name="waitForCompletion">If set to true, method blocks until auto-dispatch thread has stopped</param>
  ///
  /// <returns>True if the operation is successful; false otherwise.</returns>
  ///
  /// <seealso cref="ConnectionManager.StartAutoDispatch"/>
  public bool StopAutoDispatch(bool waitForCompletion) {
  }

  /// <summary>This function will stop the auto dispatch thread.</summary>
  ///
  /// <returns>True if the operation is successful; false otherwise.</returns>
  ///
  /// <seealso cref="ConnectionManager.StartAutoDispatch"/>
  public bool StopAutoDispatch() {
  }

  /// <summary>Exclude any incoming messages with the specified subject.</summary>
  ///
  /// <param name="subject">The subject pattern to look for in incoming messages</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public void ExcludeSubject(string subject) {
  }

  /// <summary>
  /// Remove an excluded subject, allowing incoming messages with the matching
  /// subject to once again be received.
  /// </summary>
  ///
  /// <param name="subject">The subject pattern to remove from filtering list</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public void RemoveExcludedSubject(string subject) {
  }

  /// <summary>Returns the name of the connection manager, automatically generated or user specified.</summary>
  public string GetName() {
  }

  /// <summary>Sets the logical name of this connection manager. This can be used for identifying connections withing
  /// a client program. If a name is not given, one will be automatically generated.</summary>
  ///
  /// <param name="name">The connection name.</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given name is null or is an empty-string.</exception>
  public void SetName(string name) {
  }

  /// <summary>Get the string ID for this connection manager.</summary>
  public string GetID() {
  }

  /// <summary>Get the middleware information for this connection manager.</summary>
  public string GetMWInfo() {
  }

  /// <summary>Returns middleware broker connection information.</summary>
  public string GetConnectionEndpoint() {
  }

  /// <summary>Retrieves the number of messages queued for asynchronous publish operations.</summary>
  public ulong GetPublishQueueMessageCount() {
  }

  /// <summary>
  /// This method creates a message and passes ownership to the user. This message is populated with
  /// the standard set of required and optional heartbeat fields, as well as the required common fields defined
  /// in SetStandardFields().
  /// <p>
  /// MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be generated and
  /// added to the message automatically, according to the GMSEC Message Standard
  /// </summary>
  ///
  /// <param name="subject">The topic (subject) that will be applied to the returned messages</param>
  /// <param name="heartbeatFields">the list of fields to be added to the heartbeat message</param>
  ///
  /// <returns>A C2CX Heartbeat message.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public Message CreateHeartbeatMessage(string subject, FieldList heartbeatFields) {
  }

  /// <summary>
  /// This method creates an archetypal heartbeat message from the concatenation of the standard fields
  /// applied globally to this ConnectionManager and to this function. This message will then be validated,
  /// if the "validate" flag is set. If there is an error, an Exception will be thrown. If it has not been set,
  /// the message will be published at an interval supplied by the "PUB-RATE" field regardless of validation
  /// results. If no "PUB-RATE" has been defined, the service will default to the GMSEC standard 30 second
  /// heartbeat interval.
  /// <p>
  /// If users would like to have a COUNTER field added to the published heartbeat message, then the Heartbeat
  /// Service should be provided with this field within the list of field provided to this method.
  /// </summary>
  ///
  /// <param name="subject">The topic (subject) that will be applied to the returned messages</param>
  /// <param name="heartbeatFields">the list of fields to be added to the heartbeat message</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if for whatever reason the service cannot be started.</exception>
  public void StartHeartbeatService(string subject, FieldList heartbeatFields) {
  }

  /// <summary>
  /// This method terminates opertaion of the heartbeat service and cleans up related resources.
  /// It will be called by the global connection manager "cleanup" if not done manually by the user.
  /// </summary>
  public void StopHeartbeatService() {
  }

  /// <summary>
  /// This method can be used to update or set a field within the Heartbeat Message
  /// being published by the Heartbeat Service. If validation is enabled, the field will
  /// be validated before it is added to the Heartbeat Message. Should validation pass,
  /// the field will be persisted to the heartbeat service; otherwise an error is returned.
  /// <p>
  /// Note: If a (valid) PUB-RATE field is passed to this method, and the Heartbeat Service
  /// is running, then the Heartbeat Service publish rate will be changed to the provided
  /// rate. A publish rate of 0 seconds or less will be treated as an error.
  /// </summary>
  ///
  /// <param name="field">The field containing new or update information for the heartbeat message</param>
  ///
  /// <returns>The status of the operation.</returns>
  public Status SetHeartbeatServiceField(Field field) {
  }

  /// <summary>
  /// This method creates a Log Message and passes ownership to the user. This message is populated
  /// with the standard set of required and optional log fields, as well as the required common fields
  /// defined in SetStandardFields. The message is not validated at this time, as MSG-TEXT and SEVERITY
  /// fields must be set by the user at the time the message is to be sent.
  /// <p>
  /// This message automatically is generated with MESSAGE-TYPE and MESSAGE-SUBTYPE
  /// correctly set to the GMSEC Message Standard requirements, regardless of user
  /// input. EVENT-TIME defaults to the time which this function was invoked; the user
  /// may override this time once the message has been returned.
  /// </summary>
  ///
  /// <param name="subject">The topic (subject) to apply to the returned message</param>
  /// <param name="logFields">List of fields to be added to the log message</param>
  ///
  /// <returns>A log Message object.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public Message CreateLogMessage(string subject, FieldList logFields) {
  }

  /// <summary>
  /// This method sets the standard default fields which will be added to a new log message when PublishLog()
  /// is called.
  /// </summary>
  ///
  /// <param name="subject">The topic (subject) to apply to log messages</param>
  /// <param name="logFields">List of fields to be added to log messages</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public void SetLoggingDefaults(string subject, FieldList logFields) {
  }

  /// <summary>
  /// This method creates a log message comprised of the global fields from the entire ConnectionManager,
  /// the fields set in setLoggingDefaults, and the log text and severity fields as supplied as arguments to
  /// this function. If validation is enabled, the message will be published. If validation is enabled, the
  /// message will be validated.  If validation of the message fails, then an Exception is thrown.
  /// <p>
  /// This function automatically populates the message with the correct
  /// MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set automatically
  /// to the time of function invocation.
  /// </summary>
  ///
  /// <param name="logMessage">The message to apply to the log message</param>
  /// <param name="severity">The Field object defining the severity of the log message</param>
  ///
  /// <exception cref="GmsecException">Thrown if the logMessage is null.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void PublishLog(string logMessage, Field severity) {
  }

  /// <summary>
  /// This function creates a Directive message, per the description in the GMSEC
  /// interface specification document. The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the GMSEC interface specification document.
  /// <p>
  /// The request message will have its response flag set to false, so that receivers of the message will
  /// know a response is not necessary.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the request directive</param>
  /// <param name="directiveString">a field containing the string directive that this message is intended to convey</param>
  /// <param name="fields">list of supplemental fields that the user wishes to include with the directive message</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void RequestDirective(string subject, Field directiveString, FieldList fields) {
  }

  /// <summary>
  /// This function creates a Directive message, per the description in the %GMSEC
  /// Interface Specification Document (ISD). The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the %GMSEC ISD.
  /// <p>
  /// The request message will have its response flag set to true, so that receivers of the message will
  /// know a response is necessary.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the request directive</param>
  /// <param name="directiveString">a field containing the string directive that this message is intended to convey</param>
  /// <param name="fields">list of supplemental fields that the user wishes to include with the directive message</param>
  /// <param name="timeout">time (in milliseconds) to wait for a response message</param>
  /// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
  ///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
  ///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
  ///                            Config object used to create the Connection object. The minimum republish period allowed is 100ms.</param>
  ///
  /// <returns>A response message, or null if a timeout occurs.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public Message RequestDirective(string subject, Field directiveString, FieldList fields, int timeout, int republish_ms) {
  }

  public Message RequestDirective(string subject, Field directiveString, FieldList fields, int timeout) {
  }

  /// <summary>
  /// This function creates a Directive message, per the description in the %GMSEC
  /// Interface Specification Document (ISD). The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the %GMSEC ISD.
  /// <p>
  /// The request message will have its response flag set to true, so that receivers of the message will
  /// know a response is necessary.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the request directive</param>
  /// <param name="directiveString">a field containing the string directive that this message is intended to convey</param>
  /// <param name="sFields">list of supplemental fields that the user wishes to include with the directive message</param>
  /// <param name="timeout">time (in milliseconds) to wait for a response message</param>
  /// <param name="cb">the callback to be invoked upon reception of a response message</param>
  /// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
  ///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
  ///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
  ///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the callback is null</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void RequestDirective(string subject, Field directiveString, FieldList sFields, int timeout, ConnectionManagerReplyCallback cb, int republish_ms) {
  }

  /// <summary>
  /// This function creates a Directive message, per the description in the %GMSEC
  /// Interface Specification Document (ISD). The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the %GMSEC ISD.
  /// <p>
  /// The request message will have its response flag set to true, so that receivers of the message will
  /// know a response is necessary.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the request directive</param>
  /// <param name="directiveString">a field containing the string directive that this message is intended to convey</param>
  /// <param name="sFields">list of supplemental fields that the user wishes to include with the directive message</param>
  /// <param name="timeout">time (in milliseconds) to wait for a response message</param>
  /// <param name="cb">the callback to be invoked upon reception of a response message</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void RequestDirective(string subject, Field directiveString, FieldList sFields, int timeout, ConnectionManagerReplyCallback cb) {
  }

  /// <summary>
  /// This function acknowledges a Directive message, per the description in the %GMSEC
  /// Interface Specification Document (ISD). The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the %GMSEC ISD.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the acknowledgement message</param>
  /// <param name="request">the original request directive message</param>
  /// <param name="response">an enumerated value specifying the response code to the directive request</param>
  /// <param name="fields">list of supplemental fields which should be included with the message</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void AcknowledgeDirectiveRequest(string subject, Message request, ResponseStatus.Response response, FieldList fields) {
  }

  /// <summary>
  /// This method creates a valid resource message, as described in the %GMSEC
  /// Interface Specification Document (ISD). This function then publishes the
  /// message to the %GMSEC bus on the supplied subject.
  /// </summary>
  ///
  /// <param name="subject">subject on which to publish the message</param>
  /// <param name="sampleInterval">interval (in seconds) between samples</param>
  /// <param name="averageInterval">interval (in seconds) over which to make a moving average of samples</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void PublishResourceMessage(string subject, int sampleInterval, int averageInterval) {
  }

  /// <summary>
  /// This method creates a valid resource message, as described in the %GMSEC
  /// Interface Specification Document (ISD). This function then publishes the
  /// message to the %GMSEC bus on the supplied subject.
  /// </summary>
  ///
  /// <param name="subject">subject on which to publish the message</param>
  /// <param name="sampleInterval">interval (in seconds) between samples</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void PublishResourceMessage(string subject, int sampleInterval) {
  }

  /// <summary>
  /// This method creates a valid resource message, as described in the %GMSEC
  /// Interface Specification Document (ISD). This function then publishes the
  /// message to the %GMSEC bus on the supplied subject.
  /// </summary>
  ///
  /// <param name="subject">subject on which to publish the message</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void PublishResourceMessage(string subject) {
  }

  /// <summary>
  /// This function creates a valid resource message, as described in the %GMSEC
  /// Interface Specification Document (ISD), and returns that message to the user.
  /// </summary>
  ///
  /// <param name="subject">subject with which to create the message</param>
  /// <param name="sampleInterval">interval (in seconds) between samples</param>
  /// <param name="averageInterval">interval (in seconds) over which to make a moving average of samples</param>
  ///
  /// <returns>A %GMSEC resource Message.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public Message CreateResourceMessage(string subject, int sampleInterval, int averageInterval) {
  }

  /// <summary>
  /// This function creates a valid resource message, as described in the %GMSEC
  /// Interface Specification Document (ISD), and returns that message to the user.
  /// </summary>
  ///
  /// <param name="subject">subject with which to create the message</param>
  /// <param name="sampleInterval">interval (in seconds) between samples</param>
  ///
  /// <returns>A %GMSEC resource Message.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public Message CreateResourceMessage(string subject, int sampleInterval) {
  }

  /// <summary>
  /// This function creates a valid resource message, as described in the %GMSEC
  /// Interface Specification Document (ISD), and returns that message to the user.
  /// </summary>
  ///
  /// <param name="subject">subject with which to create the message</param>
  ///
  /// <returns>A %GMSEC resource Message.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public Message CreateResourceMessage(string subject) {
  }

  /// <summary>
  /// This method starts the resource message service, which publishes
  /// resource messages at a fixed interval as defined by this function.
  /// </summary>
  ///
  /// <param name="subject">subject with which to create the message</param>
  /// <param name="interval">interval (in seconds) as to when messages are published to the bus</param>
  /// <param name="sampleInterval">interval (in seconds) between samples</param>
  /// <param name="averageInterval">interval (in seconds) over which to make a moving average of samples</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public void StartResourceMessageService(string subject, int interval, int sampleInterval, int averageInterval) {
  }

  /// <summary>
  /// This method starts the resource message service, which publishes
  /// resource messages at a fixed interval as defined by this function.
  /// </summary>
  ///
  /// <param name="subject">subject with which to create the message</param>
  /// <param name="interval">interval (in seconds) as to when messages are published to the bus</param>
  /// <param name="sampleInterval">interval (in seconds) between samples</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public void StartResourceMessageService(string subject, int interval, int sampleInterval) {
  }

  /// <summary>
  /// This method starts the resource message service, which publishes
  /// resource messages at a fixed interval as defined by this function.
  /// </summary>
  ///
  /// <param name="subject">subject with which to create the message</param>
  /// <param name="interval">interval (in seconds) as to when messages are published to the bus</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public void StartResourceMessageService(string subject, int interval) {
  }

  /// <summary>
  /// This method starts the resource message service, which publishes
  /// resource messages at a fixed interval as defined by this function.
  /// </summary>
  ///
  /// <param name="subject">subject with which to create the message</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  public void StartResourceMessageService(string subject) {
  }

  /// <summary>This method stops the resource message service.</summary>
  public bool StopResourceMessageService() {
  }

  /// <summary>
  /// This method creates a Simple Service message, per the description in the %GMSEC
  /// Interface Specification Document (ISD). The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the %GMSEC ISD.
  /// <p>
  /// The request message will have its response flag set to false, so that receivers of the message will
  /// know a response is not necessary.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the request directive</param>
  /// <param name="opName">the name of the operation of the service</param>
  /// <param name="opNumber">the operation number for the service message is intended to convey</param>
  /// <param name="fields">list of supplemental fields that the user wishes to include with the directive message</param>
  /// <param name="params">a list of the ServiceParam objects providing meta data for this service invocation</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void RequestSimpleService(string subject, string opName, Field opNumber, FieldList fields, ServiceParamList params) {
  }

  /// <summary>
  /// This method creates a Simple Service message, per the description in the GMSEC
  /// interface specification document. The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the GMSEC interface specification document.
  /// <p>
  /// The request message will have its response flag set to false, so that receivers of the message will
  /// know a response is not necessary.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the request directive</param>
  /// <param name="opName">the name of the operation of the service</param>
  /// <param name="opNumber">the operation number for the service message is intended to convey</param>
  /// <param name="fields">list of supplemental fields that the user wishes to include with the directive message</param>
  /// <param name="params">a list of the ServiceParam objects providing meta data for this service invocation</param>
  /// <param name="timeout">the maximum time period (in milliseconds) to wait for a response</param>
  /// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
  ///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
  ///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
  ///                            Config object used to create the Connection object. The minimum republish period allowed is 100ms.</param>
  ///
  /// <returns>A response Message, or null if a timeout occurs.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public Message RequestSimpleService(string subject, string opName, Field opNumber, FieldList fields, ServiceParamList params, int timeout, int republish_ms) {
  }

  /// <summary>
  /// This method creates a Simple Service message, per the description in the %GMSEC
  /// Interface Specification Document (ISD). The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the %GMSEC ISD.
  /// <p>
  /// The request message will have its response flag set to true, so that receivers of the message will
  /// know a response is necessary.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the request directive</param>
  /// <param name="opName">the name of the operation of the service</param>
  /// <param name="opNumber">the operation number for the service message is intended to convey</param>
  /// <param name="sFields">list of supplemental fields that the user wishes to include with the directive message</param>
  /// <param name="sParams">a list of the ServiceParam objects providing meta data for this service invocation</param>
  /// <param name="timeout">the maximum time period (in milliseconds) to wait for a response</param>
  /// <param name="cb">the callback to be invoked upon reception of a response message</param>
  /// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
  ///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
  ///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
  ///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if a the given callback is null.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void RequestSimpleService(string subject, string opName, Field opNumber, FieldList sFields, ServiceParamList sParams, int timeout, ConnectionManagerReplyCallback cb, int republish_ms) {
  }

  /// <summary>
  /// This method creates a Simple Service message, per the description in the %GMSEC
  /// Interface Specification Document (ISD). The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the %GMSEC ISD.
  /// <p>
  /// The request message will have its response flag set to true, so that receivers of the message will
  /// know a response is necessary.
  /// </summary>
  ///
  /// <param name="subject">subject which to include with the request directive</param>
  /// <param name="opName">the name of the operation of the service</param>
  /// <param name="opNumber">the operation number for the service message is intended to convey</param>
  /// <param name="sFields">list of supplemental fields that the user wishes to include with the directive message</param>
  /// <param name="sParams">a list of the ServiceParam objects providing meta data for this service invocation</param>
  /// <param name="timeout">the maximum time period (in milliseconds) to wait for a response</param>
  /// <param name="cb">the callback to be invoked upon reception of a response message</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if a the given callback is null.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void RequestSimpleService(string subject, string opName, Field opNumber, FieldList sFields, ServiceParamList sParams, int timeout, ConnectionManagerReplyCallback cb) {
  }

  /// <summary>
  /// This method acknowledges a Simple Service message, per the description in the %GMSEC
  /// Interface Specification Document (ISD). The message is populated with the standard
  /// fields for this connection, as well as required versioning and message description
  /// information as described in the %GMSEC ISD.
  /// </summary>
  ///
  /// <param name="subject">subject on which to publish the message</param>
  /// <param name="request">the original request directive message</param>
  /// <param name="status">enumerated value specifying the response status to the simple service request</param>
  /// <param name="fields">list of supplemental fields which may be included with the message</param>
  ///
  /// <exception cref="GmsecException">Thrown if a the given subject is null or is an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the request Message cannot be validated</exception>
  /// <exception cref="GmsecException">Thrown if a middleware error occurs.</exception>
  public void AcknowledgeSimpleService(string subject, Message request, ResponseStatus.Response status, FieldList fields) {
  }

}

}
