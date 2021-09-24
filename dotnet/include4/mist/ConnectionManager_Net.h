/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CONNECTION_MANAGER_NET_H
#define GMSEC_API_CONNECTION_MANAGER_NET_H


#include <Connection_Net.h>
#include <mist/MIST_Defs_Net.h>


// C++ API native
#include <gmsec4/mist/ConnectionManager.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Config;
ref class Field;
ref class Message;
ref class Status;

namespace MIST
{

// Forward declarations
ref class ConnectionManagerCallback;
ref class ConnectionManagerEventCallback;
ref class ConnectionManagerReplyCallback;
ref class ServiceParam;
ref class Specification;
ref class SubscriptionInfo;


/// <summary>
/// This class wraps the GMSEC Connection object, supplying both normal middleware
/// interaction functions and some additional quality of service. This is accomplished
/// through standard services for producing Log and Heartbeat messages, as well as
/// functionality for validating messages.
/// </summary>


public ref class ConnectionManager
{
public:
	/// <summary>Identifies the version of the API.</summary>
	static System::String^ GetAPIVersion();


	/// <summary>
	/// Creates a new ConnectionManager object with the given configuration.
	/// </summary>
	///
	/// <note>
	/// Message validation will be disabled, unless the configuration option GMSEC-MSG-CONTENT-VALIDATE
	/// is set to true.  If GMSEC-MSG-CONTENT-VALIDATE is set to true, then messages will be validated
	/// against the default version of the GMSEC Interface Specification Document (ISD).  The version
	/// of the ISD can be changed by specifying the configuration option GMSEC-SPECIFICATION-VERSION to
	/// a valid value (e.g. 201400).
	/// </note>
	///
	/// <param name="config">The configuration object that specifies the type of connection object to create</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An Exception is thrown if the configuration information cannot be used to deduce
	/// a Connection type, or if an anomaly occurs while loading schemas for the specified ISD.
	/// </exception>
	ConnectionManager(Config^ config);


	/// <summary>
	/// Creates a new ConnectionManager object with the given configuration.
	/// </summary>
	///
	/// <note>
	/// If message validation is enabled, then messages will be validated against the default version of
	/// the GMSEC Interface Specification Document (ISD).  The version of the ISD can be changed by
	/// specifying the configuration option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
	/// </note>
	///
	/// <param name="config">The configuration object that specifies the type of connection object to create</param>
	/// <param name="validate">Used to indicate whether ConnectionManager should validate messages that are produced</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An Exception is thrown if the configuration information cannot be used to deduce
	/// a Connection type, or if an anomaly occurs while loading schemas for the specified ISD.
	/// </exception>
	ConnectionManager(Config^ config, bool validate);


	/// <summary>
	/// Creates a new ConnectionManager object with the given configuration.
	/// </summary>
	///
	/// <param name="config">The configuration object that specifies the type of connection object to create</param>
	/// <param name="validate">Used to indicate whether ConnectionManager should validate messages that are produced</param>
	/// <param name="version">The version of the GMSEC Interface Specification Document (ISD) to use when producing messages</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An Exception is thrown if the configuration information cannot be used to deduce
	/// a Connection type, or if an anomaly occurs while loading schemas for the specified ISD.
	/// </exception>
	///
	/// <seealso cref="MIST_Defs_Net.h"/>
	ConnectionManager(Config^ config, bool validate, unsigned int version);


	/// <summary>Destructor</summmary>
	~ConnectionManager();


	/// <summary>
	/// Establishes a connection with the GMSEC middleware server.
	/// Once this call successfully returns, the ConnectionManager is ready for message operations.
	/// </summary>
	///
	/// <exception cref="GMSEC_Exception">
	/// An Exception is thrown if an anomaly occurs while attempting to connect to the middleware server.
	/// </exception>
	void Initialize();


	/// <summary>This function disconnects and destroys the underlying Connection object.</summary>
	/// <exception cref="GMSEC_Exception">If an anomaly occurs while disconnecting</exception>
	void Cleanup();


	/// <summary>Returns the current state of the connection to the middleware.</summary>
	/// <seealso cref="Connection::ConnectionState"/>
	Connection::ConnectionState GetState();


	/// <summary>Identifies the root library name and therefore the connection type that this connection is associated with.</summary>
	System::String^ GetLibraryRootName();


	/// <summary>Identifies the version information for this connection's associated middleware.</summary>
	System::String^ GetLibraryVersion();


	/// <summary>Returns the Specification object associated with the Connection Manager.</summary>
	Specification^ GetSpecification();


	/// <summary>
	/// Sets the internal list of fields that are added to all messages that are created
	/// using the ConnectionManager.  Internal copies of the provided Fields are made, thus
	/// ownership is not retained by the ConnectionManager. The supplied set of fields will not
	/// be validated here; validation occurs at the time a message is to be published.
	/// </summary>
	///
	/// <param name="standardFields">The list of fields to set as standard fields</param>
	void SetStandardFields(System::Collections::Generic::List<Field^>^ standardFields);


	/// <summary>
	/// Returns the list of standard fields that are associated with the Connection Manager.
	/// </summary>
	System::Collections::Generic::List<Field^>^ GetStandardFields();


	/// <summary>Adds the standard fields (if any) to the given Message object.</summary>
	///
	/// <param name="toMsg">The message in which to add the standard fields</param>
	void AddStandardFields(Message^ toMsg);


	/// <summary>Registers the given callback for the specified event.</summary>
	///
	/// <param name="event">The event to monitor</param>
	/// <param name="ecb">The Connection Manager event callback</param>
	///
	/// <seealso cref="Connection::ConnectionEvent"/>
	void RegisterEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback^ ecb);


	/// <summary>This method subscribes to a particular subject or pattern.</summary>
	///
	/// <param name="subject">subject pattern to match received messages</param>
	///
	/// <returns>A SubscriptionInfo object is returned; this should be used when unsubscribing</returns>
	///
	/// <seealso cref="ConnectionManager::Receive"/>
	/// <seealso cref="ConnectionManager::Unsubscribe"/>
	SubscriptionInfo^ Subscribe(System::String^ subject);


	/// <summary>
	/// This method subscribes to a particular subject or pattern, and dispatches received messages to
	/// the given callback.
	/// </summary>
	///
	/// <param name="subject">subject pattern to match received messages</param>
	/// <param name="cb">Connection Manager callback to be used to receive subscribed messages</param>
	///
	/// <returns>A SubscriptionInfo object is returned; this should be used when unsubscribing</returns>
	///
	/// <seealso cref="ConnectionManager::StartAutoDispatch"/>
	/// <seealso cref="ConnectionManager::Unsubscribe"/>
	SubscriptionInfo^ Subscribe(System::String^ subject, ConnectionManagerCallback^ cb);


	/// <summary>
	/// This method subscribes to a particular subject or pattern, and uses the config object
	/// provide to toggle between middleware-level subscription functionalities (e.g. ActiveMQ
	/// Durable Consumer).
	/// </summary>
	///
	/// <param name="subject">subject pattern to match received messages</param>
	/// <param name="config">The configuration to be used for subscription operation</param>
	///
	/// <seealso cref="ConnectionManager::Receive"/>
	/// <seealso cref="ConnectionManager::Unsubscribe"/>
	SubscriptionInfo^ Subscribe(System::String^ subject, Config^ config);


	/// <summary>
	/// This method subscribes to a particular subject or pattern, and dispatches received messages to
	/// the given callback.  In addition, the method uses the provided config object to enable or disable
	/// middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).
	/// </summary>
	///
	/// <param name="subject">subject pattern to match received messages</param>
	/// <param name="config">The configuration to be used for subscription operation</param>
	/// <param name="cb">Connection Manager callback to be used to receive subscribed messages</param>
	///
	/// <returns>A SubscriptionInfo object is returned; this should be used when unsubscribing</returns>
	///
	/// <seealso cref="ConnectionManager::StartAutoDispatch"/>
	/// <seealso cref="ConnectionManager::Unsubscribe"/>
	SubscriptionInfo^ Subscribe(System::String^ subject, Config^ config, ConnectionManagerCallback^ cb);


	/// <summary>
	/// This method unsubscribes to a particular subject or pattern, and will stop the reception
	/// or messages that match this pattern.  It will also remove the registration of any callbacks
	/// with this subject pattern.
	/// </summary>
	///
	/// <param name="info">The reference to the SUbscriptionInfo object</param>
	void Unsubscribe(SubscriptionInfo^ % info);


	/// <summary>
	/// This method will start a thread that will dispatch messages asynchronously when they are received.
	/// If this is used, all subscriptions must be made with callbacks or the messages will be dropped.
	/// If Receive() is called while the auto-dispatcher is used, the behavior will be undesireable
	/// and undefined.
	/// </summary>
	///
	/// <returns>True if the operation is successful; false otherwise.</returns>
	///
	/// <seealso cref="ConnectionManager::stopAutoDispatch"/>
	bool StartAutoDispatch();


	/// <summary>This function will stop the auto dispatch thread.</summary>
	///
	/// <returns>True if the operation is successful; false otherwise.</returns>
	///
	/// <seealso cref="ConnectionManager::startAutoDispatch"/>
	bool StopAutoDispatch();


	/// <summary>This function will stop the auto dispatch thread.</summary>
	///
	/// <param name="waitForCompletion">If set to true, method blocks until auto-dispatch thread has stopped</param>
	///
	/// <returns>True if the operation is successful; false otherwise.</returns>
	///
	/// <seealso cref="ConnectionManager::startAutoDispatch"/>
	bool StopAutoDispatch(bool waitForCompletion);


	/// <summary>
	/// If this connection manager has been created with "validate"
	/// option disabled, this is a pass-through function to the underlying connection.
	/// Otherwise the message will be validated before it is published.
	/// </summary>
	///
	/// <param name="msg">The message to publish</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	void Publish(Message^ msg);


	/// <summary>
	/// Publishes the given message to the middleware
	/// using the given configuration to enable or disable certain middleware-level
	/// publish functionalities (e.g. ActiveMQ - Durable Producer). Message will still
	/// be validated if message validation is enabled.
	/// <note type="note">
	/// The actual Message published to the middleware will contain tracking fields;
	/// to disable this feature, create a ConnectionManager object with the tracking=off
	/// configuration option.
	/// </note>
	/// </summary>
	///
	/// <param name="msg">The message to publish</param>
	/// <param name="mwConfig">Configuration object for providing middleware configuration options
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	void Publish(Message^ msg, Config^ mwConfig);


	/// <summary>
	/// If this connection manager has been created with "validate"
	/// option disabled, this is a pass-through function to the underlying connection.
	/// Otherwise the request message will be validated before it is issued.
	/// </summary>
	///
	/// <param name="msg">The request message to issue</param>
	/// <param name="timeout">maximum time (in milliseconds) to wait for reply/param>
	/// <param name="rcb">callback to call when reply is received</param>
	/// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
	///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	void Request(Message^ requestMsg, System::Int32 timeout, ConnectionManagerReplyCallback^ rcb, System::Int32 republish_ms);


	/// <summary>
	/// If this connection manager has been created with "validate"
	/// option disabled, this is a pass-through function to the underlying connection.
	/// Otherwise the request message will be validated before it is issued.
	/// </summary>
	///
	/// <param name="msg">The request message to issue</param>
	/// <param name="timeout">maximum time (in milliseconds) to wait for reply/param>
	/// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
	///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
	///
	/// <returns>A reply message, or null if a timeout occurs.</returns>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	///
	/// <seealso cref="ConnectionManager::Release"/>
	Message^ Request(Message^ requestMsg, System::Int32 timeout, System::Int32 republish_ms);


	/// <summary>
	/// Used to cancel a pending request that is associated with
	/// the given ConnectionManagerReplyCallback object.
	/// </summary>
	///
	/// <param name="rcb">The Connection Manager reply callback previously used with a call to Request()</param>
	void CancelRequest(ConnectionManagerReplyCallback^ rcb);


	/// <summary>
	/// If this connection manager has been created with "validate"
	/// option disabled, this is a pass-through function to the underlying connection.
	/// Otherwise the request message will be validated before it is issued.
	/// </summary>
	///
	/// <param name="requestMsg">The pending request message</param>
	/// <param name="replyMsg">The reply message to send in response to the request</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	void Reply(Message^ requestMsg, Message^ replyMsg);


	/// <summary>Attempts to dispatch the given message to callbacks that have been previously registered.</summary>
	///
	/// <param name="msg">The message to dispatch</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	void Dispatch(Message^ msg);


	/// <summary>
	/// A pass-through method to the underlying connection.
	/// This method blocks until a message is returned.
	/// The received messages are determined by the %Subscribe() Function(s).
	/// </summary>
	///
	/// <returns>The next message whose subject matches the pattern of those previously subscribed to.</returns>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	///
	/// <seealso cref="ConnectionManager::Release"/>
	Message^ Receive();


	/// <summary>
	/// A pass-through method to the underlying connection.
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
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	///
	/// <seealso cref="ConnectionManager::Release"/>
	Message^ Receive(System::Int32 timeout);


	/// <summary>Destroys a Message object that has been previously returned by the API library.
	/// This method should NOT be used to destroy a Message object instantiated outside the API library,
	/// nor those received within a callback.</summary>
	///
	/// <param name="message">A reference to a message object</param>
	///
	/// <seealso cref="ConnectionManager::Receive"/>
	/// <seealso cref="ConnectionManager::Request"/>
	/// <seealso cref="ConnectionManager::RequestDirective"/>
	/// <seealso cref="ConnectionManager::RequestSimpleService"/>
	void Release(Message^ %message);


	/// <summary>Exclude any incoming messages with the specified subject.</summary>
	///
	/// <param name="subject">The subject pattern to look for in incoming messages</param>
	void ExcludeSubject(System::String^ subject);


	/// <summary>
	/// Remove an excluded subject, allowing incoming messages with the matching
	/// subject to once again be received.
	/// </summary>
	///
	/// <param name="subject">The subject pattern to remove from filtering list</param>
	void RemoveExcludedSubject(System::String^ subject);


	/// <summary>Returns the name of the connection manager, automatically generated or user specified.</summary>
	System::String^ GetName();


	/// <summary>Sets the logical name of this connection manager. This can be used for identifying connections withing
	/// a client program. If a name is not given, one will be automatically generated.</summary>
	/// <param name="name">The connection name.</param>
	void SetName(System::String^ name);


	/// <summary>Get the string ID for this connection manager.</summary>
	System::String^ GetID();


	/// <summary>Get the middleware information for this connection manager.</summary>
	System::String^ GetMWInfo();


	/// <summary>Retrieves the number of messages queued for asynchronous publish operations.</summary>
	System::UInt64 GetPublishQueueMessageCount();


	/// <summary>
	/// This method creates a message and passes ownership to the user. This message is populated with
	/// the standard set of required and optional heartbeat fields, as well as the required common fields defined
	/// in SetStandardFields().
	///
	/// MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be generated and
	/// added to the message automatically, according to the GMSEC Message Standard
	/// </summary>
	///
	/// <param name="subject">The topic (subject) that will be applied to the returned messages</param>
	/// <param name="heartbeatFields">the list of fields to be added to the heartbeat message</param>
	///
	/// <returns>A C2CX Heartbeat message.</returns>
	Message^ CreateHeartbeatMessage(System::String^ subject, System::Collections::Generic::List<Field^>^ heartbeatFields);


	/// <summary>
	/// This method creates an archetypal heartbeat message from the concatenation of the standard fields
	/// applied globally to this ConnectionManager and to this function. This message will then be validated,
	/// if the "validate" flag is set. If there is an error, an Exception will be thrown. If it has not been set,
	/// the message will be published at an interval supplied by the "PUB-RATE" field regardless of validation
	/// results. If no "PUB-RATE" has been defined, the service will default to the GMSEC standard 30 second
	/// heartbeat interval.
	///
	/// If users would like to have a COUNTER field added to the published heartbeat message, then the Heartbeat
	/// Service should be provided with this field within the list of field provided to this method.
	/// </summary>
	///
	/// <param name="subject">The topic (subject) that will be applied to the returned messages</param>
	/// <param name="heartbeatFields">the list of fields to be added to the heartbeat message</param>
	void StartHeartbeatService(System::String^ subject, System::Collections::Generic::List<Field^>^ heartbeatFields);


	/// <summary>
	/// This method terminates opertaion of the heartbeat service and cleans up related resources.
	/// It will be called by the global connection manager "cleanup" if not done manually by the user.
	/// </summary>
	void StopHeartbeatService();


	/// <summary>
	/// This method updates the field which is included within the Heartbeat Message being
	/// published by the Heartbeat Service.  If validation is enabled, the message will then be
	/// re-validated with the modified field set. Should the validation pass, the change will be
	/// persisted to the heartbeat service, otherwise an exception is thrown.
	/// </summary>
	///
	/// <param name="compenentStatus">The component status field</param>
	///
	/// <returns>The status of the operation.</returns>
	///
	/// <note>This method has been deprecated; use SetHeartbeatServiceField() instead.</note>
	Status^ ChangeComponentStatus(Field^ componentStatus);


	/// <summary>
	/// This method updates the field which is included within the Heartbeat Message being
	/// published by the Heartbeat Service.  If validation is enabled, the message will then be
	/// re-validated with the modified field set. Should the validation pass, the change will be
	/// persisted to the heartbeat service, otherwise an exception is thrown.
	/// </summary>
	///
	/// <param name="compenentInfo">The component information field</param>
	///
	/// <returns>The status of the operation.</returns>
	///
	/// <note>This method has been deprecated; use SetHeartbeatServiceField() instead.</note>
	Status^ ChangeComponentInfo(Field^ componentInfo);


	/// <summary>
	/// This method updates the field which is included within the Heartbeat Message being
	/// published by the Heartbeat Service.  If validation is enabled, the message will then be
	/// re-validated with the modified field set. Should the validation pass, the change will be
	/// persisted to the heartbeat service, otherwise an exception is thrown.
	/// </summary>
	///
	/// <param name="cpuMemory">The CPU memory field</param>
	///
	/// <returns>The status of the operation.</returns>
	///
	/// <note>This method has been deprecated; use SetHeartbeatServiceField() instead.</note>
	Status^ ChangeCPUMemory(Field^ cpuMemory);


	/// <summary>
	/// This method updates the field which is included within the Heartbeat Message being
	/// published by the Heartbeat Service.  If validation is enabled, the message will then be
	/// re-validated with the modified field set. Should the validation pass, the change will be
	/// persisted to the heartbeat service, otherwise an exception is thrown.
	/// </summary>
	///
	/// <param name="cpuUtil">The CPU utilization field</param>
	///
	/// <returns>The status of the operation.</returns>
	///
	/// <note>This method has been deprecated; use SetHeartbeatServiceField() instead.</note>
	Status^ ChangeCPUUtil(Field^ cpuUtil);


	/// <summary>
	/// This method can be used to update or set a field within the Heartbeat Message
	/// being published by the Heartbeat Service.  If validation is enabled, the field will
	/// be validated before it is added to the Heartbeat Message.  Should validation pass,
	/// the field will be persisted to the heartbeat service; otherwise an error is returned.
	/// </summary>
	///
	/// <param name="field">The field containing new or update information for the heartbeat message</param>
	///
	/// <returns>The status of the operation.</returns>
	///
	/// <note>If a (valid) PUB-RATE field is passed to this method, and the Heartbeat Service
	/// is running, then the Heartbeat Service publish rate will be changed to the provided
	/// rate.  Note that a publish rate of 0 seconds or less will be treated as an error.</note>
	Status^ SetHeartbeatServiceField(Field^ field);


	/// <summary>
	/// This method creates a Log Message and passes ownership to the user. This message is populated
	/// with the standard set of required and optional log fields, as well as the required common fields
	/// defined in SetStandardFields. The message is not validated at this time, as MSG-TEXT and SEVERITY
	/// fields must be set by the user at the time the message is to be sent.
	/// 
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
	Message^ CreateLogMessage(System::String^ subject, System::Collections::Generic::List<Field^>^ logFields);


	/// <summary>
	/// This method sets the standard default fields which will be added to a new log message when PublishLog()
	/// is called.
	/// </summary>
	///
	/// <param name="subject">The topic (subject) to apply to log messages</param>
	/// <param name="logFields">List of fields to be added to log messages</param>
	void SetLoggingDefaults(System::String^ subject, System::Collections::Generic::List<Field^>^ logFields);


	/// <summary>
	/// This method creates a log message comprised of the global fields from the entire ConnectionManager,
	/// the fields set in setLoggingDefaults, and the log text and severity fields as supplied as arguments to
	/// this function. If validation is enabled, the message will be published. If validation is enabled, the
	/// message will be validated.  If validation of the message fails, then an Exception is thrown.
	/// 
	/// This function automatically populates the message with the correct
	/// MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set automatically
	/// to the time of function invocation.
	/// </summary>
	///
	/// <param name="subject">The topic (subject) to apply to the log message</param>
	/// <param name="severity">The Field object defining the severity of the log message</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the Log Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	void PublishLog(System::String^ logMessage, Field^ severity);


	/// <summary>
	/// This function creates a Directive message, per the description in the GMSEC
	/// interface specification document. The message is populated with the standard
	/// fields for this connection, as well as required versioning and message description
	/// information as described in the GMSEC interface specification document.
	/// 
	/// The request message will have its response flag set to false, so that receivers of the message will
	/// know a response is not necessary.
	/// </summary>
	///
	/// <param name="subject">subject which to include with the request directive</param>
	/// <param name="directiveString">a field containing the string directive that this message is intended to convey</param>
	/// <param name="fields">list of supplemental fields that the user wishes to include with the directive message</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the request Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	void RequestDirective(System::String^ subject, Field^ directiveString, System::Collections::Generic::List<Field^>^ fields);


	/// <summary>
	/// This function creates a Directive message, per the description in the GMSEC
	/// interface specification document. The message is populated with the standard
	/// fields for this connection, as well as required versioning and message description
	/// information as described in the GMSEC interface specification document.
	/// 
	/// The request message will have its response flag set to true, so that receivers of the message will
	/// know a response is necessary.
	/// </summary>
	///
	/// <param name="subject">subject which to include with the request directive</param>
	/// <param name="directiveString">a field containing the string directive that this message is intended to convey</param>
	/// <param name="fields">list of supplemental fields that the user wishes to include with the directive message</param>
	/// <param name="timeout">time (in milliseconds) to wait for a response message</param>
	/// <param name="rcb">the callback to be invoked upon reception of a response message</param>
	/// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
	///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the request Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	void RequestDirective(System::String^ subject, Field^ directiveString, System::Collections::Generic::List<Field^>^ fields,
	                      System::Int32 timeout, ConnectionManagerReplyCallback^ rcb, System::Int32 republish_ms);


	/// <summary>
	/// This function creates a Directive message, per the description in the GMSEC
	/// interface specification document. The message is populated with the standard
	/// fields for this connection, as well as required versioning and message description
	/// information as described in the GMSEC interface specification document.
	/// 
	/// The request message will have its response flag set to true, so that receivers of the message will
	/// know a response is necessary.
	/// </summary>
	///
	/// <param name="subject">subject which to include with the request directive</param>
	/// <param name="directiveString">a field containing the string directive that this message is intended to convey</param>
	/// <param name="fields">list of supplemental fields that the user wishes to include with the directive message</param>
	/// <param name="timeout">time (in milliseconds) to wait for a response message</param>
	/// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
	///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
	///
	/// <returns>A response message, or null if a timeout occurs.</returns>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the request Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	Message^ RequestDirective(System::String^ subject, Field^ directiveString, System::Collections::Generic::List<Field^>^ fields,
	                          System::Int32 timeout, System::Int32 republish_ms);


	/// <summary>
	/// This function acknowledges a Directive message, per the description in the GMSEC
	/// interface specification document. The message is populated with the standard
	/// fields for this connection, as well as required versioning and message description
	/// information as described in the GMSEC interface specification document.
	/// </summary>
	///
	/// <param name="subject">subject which to include with the acknowledgement message</param>
	/// <param name="request">the original request directive message</param>
	/// <param name="status">an enumerated value specifying the response code to the directive request</param>
	/// <param name="fields">list of supplemental fields which should be included with the message</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the response Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	void AcknowledgeDirectiveRequest(System::String^ subject, Message^ request, ResponseStatus status,
	                                 System::Collections::Generic::List<Field^>^ fields);


	/// <summary>
	/// This method creates a valid resource message, as described in the GMSEC
	/// specification. This function then publishes the message to the GMSEC bus on the
	/// supplied subject.
	/// </summary>
	///
	/// <param name="subject">subject on which to publish the message</param>
	/// <param name="sampleInterval">interval (in seconds) between samples</param>
	/// <param name="averageInterval">interval (in seconds) over which to make a moving average of samples</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a middleware error occurs.</exception>
	void PublishResourceMessage(System::String^ subject, long sampleInterval, long averageInterval);


	/// <summary>
	/// This function creates a valid resource message, as described in the GMSEC
	/// specification, and returns that message to the user.
	/// </summary>
	///
	/// <param name="subject">subject with which to create the message</param>
	/// <param name="sampleInterval">interval (in seconds) between samples</param>
	/// <param name="averageInterval">interval (in seconds) over which to make a moving average of samples</param>
	///
	/// <returns>A GMSEC resource Message.</returns>
	Message^ CreateResourceMessage(System::String^ subject, long sampleInterval, long averageInterval);


	/// <summary>
	/// This method starts the resource message service, which publishes
	/// resource messages at a fixed interval as defined by this function.
	/// </summary>
	///
	/// <param name="subject">subject with which to create the message</param>
	/// <param name="interval">interval (in seconds) as to when messages are published to the bus</param>
	/// <param name="sampleInterval">interval (in seconds) between samples</param>
	/// <param name="averageInterval">interval (in seconds) over which to make a moving average of samples</param>
	void StartResourceMessageService(System::String^ subject, long interval, long sampleInterval, long averageInterval);


	/// <summary>This method stops the resource message service.</summary>
	void StopResourceMessageService();


	/// <summary>
	/// This method creates a Simple Service message, per the description in the GMSEC
	/// interface specification document. The message is populated with the standard
	/// fields for this connection, as well as required versioning and message description
	/// information as described in the GMSEC interface specification document.
	/// 
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
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the request Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	void RequestSimpleService(System::String^ subject, System::String^ opName, Field^ opNumber,
	                          System::Collections::Generic::List<Field^>^ fields, System::Collections::Generic::List<ServiceParam^>^ params);


	/// <summary>
	/// This method creates a Simple Service message, per the description in the GMSEC
	/// interface specification document. The message is populated with the standard
	/// fields for this connection, as well as required versioning and message description
	/// information as described in the GMSEC interface specification document.
	///
	/// The request message will have its response flag set to true, so that receivers of the message will
	/// know a response is necessary.
	/// </summary>
	///
	/// <param name="subject">subject which to include with the request directive</param>
	/// <param name="opName">the name of the operation of the service</param>
	/// <param name="opNumber">the operation number for the service message is intended to convey</param>
	/// <param name="fields">list of supplemental fields that the user wishes to include with the directive message</param>
	/// <param name="params">a list of the ServiceParam objects providing meta data for this service invocation</param>
	/// <param name="timeout">the maximum time period (in milliseconds) to wait for a response</param>
	/// <param name="rcb">the callback to be invoked upon reception of a response message</param>
	/// <param name="republish_ms">request message resubmission interval (in milliseconds). If set to a negative
	///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the request Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	void RequestSimpleService(System::String^ subject, System::String^ opName, Field^ opNumber,
		                      System::Collections::Generic::List<Field^>^ fields, System::Collections::Generic::List<ServiceParam^>^ params,
	                          System::Int32 timeout, ConnectionManagerReplyCallback^ rcb, System::Int32 republish_ms);


	/// <summary>
	/// This method creates a Simple Service message, per the description in the GMSEC
	/// interface specification document. The message is populated with the standard
	/// fields for this connection, as well as required versioning and message description
	/// information as described in the GMSEC interface specification document.
	/// 
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
	///                            value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	///                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	///                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.</param>
	///
	/// <returns>A response Message, or null if a timeout occurs.</returns>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the request Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	Message^ RequestSimpleService(System::String^ subject, System::String^ opName, Field^ opNumber,
	                              System::Collections::Generic::List<Field^>^ fields, System::Collections::Generic::List<ServiceParam^>^ params,
	                              System::Int32 timeout, System::Int32 republish_ms);


	/// <summary>
	/// This method acknowledges a Simple Service message, per the description in the GMSEC
	/// interface specification document. The message is populated with the standard
	/// fields for this connection, as well as required versioning and message description
	/// information as described in the GMSEC interface specification document.
	/// </summary>
	///
	/// <param name="subject">subject on which to publish the message</params>
	/// <param name="request">the original request directive message</params>
	/// <param name="status">enumerated value specifying the response status to the simple service request</params>
	/// <param name="fields">list of supplemental fields which may be included with the message</params>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the response Message cannot be validated, or if a middleware error occurs.
	/// </exception>
	void AcknowledgeSimpleService(System::String^ subject, Message^ request, ResponseStatus status,
	                              System::Collections::Generic::List<Field^>^ fields);


	/// <summary>Calls shutdown routines for each middleware that has a shutdown routine registered.</summary>
	static void ShutdownAllMiddlewares();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ConnectionManager();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="connMgr">unmanaged implementation to initialize with</param>
	ConnectionManager(gmsec::api::mist::ConnectionManager* connMgr, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::ConnectionManager* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::ConnectionManager* connMgr, bool owned);


private:
	gmsec::api::mist::ConnectionManager* m_impl;
	bool                                 m_owned;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
