/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 *  @file ConnectionManager.h
 *
 *  @brief The ConnectionManager is designed to provide several high-level management
 *  functions for operation of a GMSEC connection object. These management functions 
 *  include the application of message content validation, the operation of a
 *  standard service for producing heartbeats, and a set of functions which streamline 
 *  log message production.
 */

#ifndef GMSEC_CONNECTIONMANAGER_H
#define GMSEC_CONNECTIONMANAGER_H

#include <gmsec4/Connection.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/ServiceParam.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/Deprecated.h>
#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <stddef.h>   // for size_t


namespace gmsec
{
namespace api
{
	// Forward declarations(s)
	class Config;
	class Field;

namespace mist
{
	// Forward declarations(s)
	class ConnectionManagerCallback;
	class ConnectionManagerEventCallback;
	class ConnectionManagerReplyCallback;
	class MessageValidator;
	class SubscriptionInfo;
	class Specification;

namespace internal
{
	// Forward declarations(s)
	class ConnectionManagerBuddy;
	class InternalConnectionManager;
	class CMCallback;
	class CMEventCallback;
	class CMReplyCallback;
}


/**
 * @class ConnectionManager
 *
 * @brief This class wraps the GMSEC Connection object, supplying both normal
 * middleware interaction functions and some additional quality of service. 
 * This is accomplished through standard services for producing Log and 
 * Heartbeat messages, as well as functionality for validating messages. 
 * 
 * If the object is configured to validate messages, all messages sent 
 * from this ConnectionManager via either Publish or Request will be compared
 * to an available schema definition. If a template definition for the message
 * in question is available, it will be validated. The message will not be sent
 * over the middleware if validation fails. Messages without a template
 * definition can be sent as well.
 *
 * @sa Connection @n
 *		Config
 */
class GMSEC_API ConnectionManager
{
public:
	/**
	 * @fn const char* getAPIVersion()
	 *
	 * @brief This function identifies the version of the API.
	 *
	 * @return String containing API version information.
	 */
	static const char* CALL_TYPE getAPIVersion();


	/**
	 * @fn ConnectionManager(const Config &cfg, bool validate = false, unsigned int version = GMSEC_ISD_CURRENT)
	 *
	 * @brief Constructor - Initializes the ConnectionManager instance without
	 * attempting a connection to the middleware.
	 *
	 * @note Message validation will be disabled, unless otherwise indicated.  If enabled, or if the configuration
	 * option GMSEC-MSG-CONTENT-VALIDATE is set to true, then messages will be validated against the default version
	 * of the GMSEC Interface Specification Document (ISD).  The version of the ISD can be changed by specifying the
	 * configuration option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
	 *
	 * @param cfg - The config object used to construct the underlying 
	 * GMSEC connection object
	 * @param validate - boolean flag to indicate whether the ConnectionManager
	 * should validate messages (default is false)
	 * @param version - the version of the GMSEC message specification to be used
	 * in validating messages (default is GMSEC_ISD_CURRENT)
	 *
	 * @throw An Exception is thrown if the configuration information cannot be used to deduce
	 * a Connection type, or if an anomaly occurs while loading schemas for the specified ISD.
	 */
	ConnectionManager(const Config& cfg, bool validate = false, unsigned int version = GMSEC_ISD_CURRENT);


	/**
	 * @fn ~ConnectionManager()
	 *
	 * @desc Destructor - tears down the ConnectionManager.
	 * If cleanup had not been called already, the function invokes that function.
	 *
	 * @sa ConnectionManager::cleanup() @n
	 */
	~ConnectionManager();


	/**
	 * @fn void initialize()
	 *
	 * @desc Establishes a connection with the GMSEC middleware server.
	 * Once this call successfully returns, the ConnectionManager is ready for message operations.
	 *
	 * @throw An Exception is thrown if an anomaly occurs while attempting to connect to the middleware server.
	 */
	void CALL_TYPE initialize();


	/**
	 * @fn void cleanup()
	 *
	 * @desc Cleanup - This function disconnects and destroys the 
	 * underlying Connection object, returning an error status if this operation is
	 * not successful.
	 *
	 * @throws Exception if an anomaly occurs while disconnecting
	 *
	 * @sa Connection:disconnect()
	 */
	void CALL_TYPE cleanup();


	/**
	 * @fn ConnectionState getState() const
	 *
	 * @brief This function returns the current state of the connection to the middleware.
	 *
	 * @return Enumerated ConnectionState value.
	 */
	Connection::ConnectionState CALL_TYPE getState() const;


	/**
	 * @fn const char* getLibraryRootName() const
	 *
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this connection is associated with.
	 *
	 * @return root library name
	 */
	const char* CALL_TYPE getLibraryRootName() const;


	/**
	 * @fn const char* getLibraryVersion() const
	 * @brief This function returns a string containing the version information for
	 * this connection's associated middleware.
	 * @return library version
	 */
	const char* CALL_TYPE getLibraryVersion() const;


	/**
	 * @fn Specification& getSpecification() const
	 *
	 * @desc Returns the Specification object associated with the Connection Manager.
	 *
	 * @return A Specification object.
	 */
	Specification& CALL_TYPE getSpecification() const;


	/**
	 * @fn void setStandardFields(const gmsec::api::util::DataList<Field*>& standardFields)
	 *
	 * @brief Sets the internal list of fields that are added to all messages that are created
	 * using the ConnectionManager.  Internal copies of the provided Fields are made, thus
	 * ownership is not retained by the ConnectionManager. The supplied set of fields will not
	 * be validated here; validation occurs at the time a message is to be published.
	 *
	 * @param standardFields - A list of fields to be copied to the internal set of fields.
	 */
	void CALL_TYPE setStandardFields(const gmsec::api::util::DataList<Field*>& standardFields);


	/**
	 * @fn const gmsec::api::util::DataList<Field*>& getStandardFields() const
	 *
	 * @brief Returns a reference to the standard fields that have been set within the Connection Manager.
	 *
	 * @note Users should not attempt to destroy the fields within the list, for they are owned by the Connection Manager.
	 *
	 * @sa setStandardFields()
	 */
	const gmsec::api::util::DataList<Field*>& CALL_TYPE getStandardFields() const;


	/**
	 * @fn void addStandardFields(Message& msg) const
	 *
	 * @brief Adds the standard fields (if any) to the given Message object.
	 *
	 * @param msg - A Message object
	 *
	 * @sa setStandardFields()
	 */
	void CALL_TYPE addStandardFields(Message& msg) const;


	/**
	 * @fn void registerMessageValidator(MessageValidator* validator)
	 *
	 * @brief Registers the give message validator to be used when message validation takes place.
	 *
	 * @param validator - A custom message validation object.
	 *
	 * @throw An Exception is thrown if the validator object is NULL.
	 */
	void CALL_TYPE registerMessageValidator(MessageValidator* validator);


	/**
	 * @fn void registerEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback *cb)
	 *
	 * @brief Uses the given event callback to notify applications of notable events occuring with the Connection Manager
	 * or the connection to the GMSEC Bus.
	 *
	 * @param event - type of event to register
	 * @param cb - object derived from EventCallback to register for this error event
	 *
	 * @sa Connection::registerEventCallback(Connection::ConnectionEvent event, EventCallback* cb) @n
	 *
	 * @throw An Exception is thrown if the callback is NULL.
	 */
	void CALL_TYPE registerEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback* cb);


	/**
	 * @fn SubscriptionInfo* subscribe(const char* subject, ConnectionManagerCallback* cb = NULL)
	 *
	 * @brief This method subscribes to a particular subject or pattern and if provided.  If a
	 * callback is given, then it will be associated with the subscription.
	 * If all subscriptions are setup to use a callback, then the auto-dispatcher
	 * should be used to asynchronously receive messages.  An alternative is to use receive() to
	 * receive messages, and then to forward them to callbacks using dispatch().
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received (optional)
	 *
	 * @return SubscriptionInfo handle used to cancel or modify subscription.
	 * ConnectionManager maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
	 * to free resource.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the subject string is NULL.
	 */
	SubscriptionInfo* CALL_TYPE subscribe(const char *subject, ConnectionManagerCallback* cb = NULL);


	/**
	 * @fn SubscriptionInfo* subscribe(const char* subject, const Config& config, ConnectionManagerCallback* cb = NULL)
	 *
	 * @brief This method subscribes to a particular subject or pattern and uses the provided config object
	 * to enable or disable middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).
	 * If a callback is given, then it will be associated with the subscription.
	 * If all subscriptions are setup to use a callback, then the auto-dispatcher
	 * should be used to asynchronously receive messages.  An alternative is to use receive() to
	 * receive messages, and then to forward them to callbacks using dispatch().
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config  - config object to be used for subscription operation
	 * @param cb      - callback to be called when message is received (optional)
	 *
	 * @return SubscriptionInfo handle used to cancel or modify subscription.
	 * ConnectionManager maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
	 * to free resource.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the subject string is NULL.
	 */
	SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& config, ConnectionManagerCallback* cb = NULL);


	/**
	 * @fn void unsubscribe(SubscriptionInfo*& info)
	 *
	 * @brief This method unsubscribes to a particular subject or pattern, and will stop the reception
	 * or messages that match this pattern.  It will also remove the registration of any callbacks
	 * with this subject pattern.
	 *
	 * @param info - SubscriptionInfo handle from subscription.
	 *
	 * @throw Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw Exception is thrown if SubscriptionInfo object is null or originated from a different ConnectionManager object.
	 * @throw Exception is thrown if error occurs at the middleware level.
	 */
	void CALL_TYPE unsubscribe(SubscriptionInfo*& info);


	/**
	 * @fn void publish(const Message& msg)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the message will be validated before it is published.
	 *
	 * @param msg - The GMSEC message to be published.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message fails validation, or if any other severe
	 * error occurs with sending the message.
	 */
	void CALL_TYPE publish(const Message& msg);


	/**
	 * @fn void publish(const Message& msg, const Config& mwConfig)
	 *
	 * @brief Publishes the given message to the middleware
	 * using the given configuration to enable or disable certain middleware-level
	 * publish functionalities (e.g. ActiveMQ - Durable Producer). Message will still
	 * be validated if message validation is enabled.
	 *
	 * @note The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a ConnectionManager object with the tracking=off
	 * configuration option.
	 *
	 * @param msg - The GMSEC message to be published
	 * @param mwConfig - config object for providing middleware configuration options
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message fails validation, or if any other severe
	 * error occurs with sending the message.
	 */
	void CALL_TYPE publish(const Message& msg, const Config &mwConfig);


	/**
	 * @fn void request(const Message& request, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param cb - callback to call when reply is received
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the request message fails validation, or if any other severe
	 * error occurs with issuing the request.
	 *
	 * @sa cancelRequest()
	 */
	void CALL_TYPE request(const Message& request, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn Message* request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @return reply - reply message (if received)
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::release
	 */
	Message* CALL_TYPE request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn void cancelRequest(ConnectionManagerReplyCallback* cb)
	 *
	 * @brief This function can be used to cancel a pending request that is associated with
	 * the given ConnectionManagerReplyCallback object.
	 *
	 * @param cb - The ConnectionManagerReplyCallback to disassociate from a pending request.
	 *
	 * @sa void request(const Message& request, GMSEC_I32 timeout, ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 */
	void CALL_TYPE cancelRequest(ConnectionManagerReplyCallback* cb);


	/**
	 * @fn void reply(const Message& request, const Message& reply)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 *
	 * @param request - the pending request message
	 * @param reply - the reply message to be sent
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::request
	 */
	void CALL_TYPE reply(const Message& request, const Message& reply);


	/**
	 * @fn void dispatch(const Message& msg)
	 *
	 * @brief Attempts to dispatch the given message to callbacks that have been previously
	 * registered.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @param msg - message to be dispatched
	 */
	void CALL_TYPE dispatch(const Message& msg);


	/**
	 * @fn Message* receive(GMSEC_I32 timeout)
	 *
	 * @brief This function is a pass-through function to the underlying connection.
	 * This function returns the next message received within the specified timeout.
	 * The received messages are determined by the %subscribe() function(s), but
	 * %dispatchMsg() needs to be called messages received from this function to
	 * ensure all registered callbacks are executed.
	 *
	 * This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @param timeout - the maximum time to block waiting for a message, in milliseconds
	 *
	 * @return The next received message, if any.  If a timeout occurs, NULL is returned.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::subscribe @n
	 *     ConnectionManager::release
	 */
	Message* CALL_TYPE receive(GMSEC_I32 timeout);


	/**
	 * @fn void release(Message*& msg)
	 *
	 * @brief This function destroys a message and cleans up any associated memory.
	 *
	 * @param msg - message to be destroyed
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::receive()
	 */
	void CALL_TYPE release(Message*& msg);


	/**
	 * @fn bool startAutoDispatch()
	 *
	 * @desc This method will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages will be dropped.
	 * If receive() is called while the auto-dispatcher is used, the behavior will be undesireable
	 * and undefined.
	 *
	 * @return True of the dispatcher started successfully; false otherwise.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::subscribe() @n
	 *     ConnectionManager::stopAutoDispatch()
	 */
	bool CALL_TYPE startAutoDispatch();


	/**
	 * @fn bool stopAutoDispatch(bool waitForCompletion = true)
	 *
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @param [optional] waitForCompletion - if set to false, this method will not block; otherwise the method will
	 * block until the auto-dispatch thread has completed running.
	 *
	 * @return A value of true is returned if the auto-dispatcher was running and has been stopped; false otherwise.
	 * If the waitForCompletion flag is set to false, then users can expect a return value of false.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::startAutoDispatch()
	 */
	bool CALL_TYPE stopAutoDispatch(bool waitForCompletion = true);


	/**
	 * @fn excludeSubject(const char* subject)
	 *
	 * @desc Exclude any incoming messages with the specified subject.
	 *
	 * @param subject - The subject pattern to look for in incoming messages.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::ExcludeSubject()
	 */
	void excludeSubject(const char *subject);


	/**
	 * @fn removeExcludedSubject(const char* subject)
	 *
	 * @desc Remove an excluded subject, allowing incoming messages with the matching
	 *  subject to once again be received.
	 *
	 * @param subject - The subject pattern to remove.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::RemoveExcludedSubject()
	 */
	void removeExcludedSubject(const char *subject);


	/**
	 * @fn const char* getName() const
	 *
	 * @brief Returns the name of the connection, automatically generated or user specified.
	 *
	 * @return A string
	 */
	const char* CALL_TYPE getName() const;


	/**
	 * @fn void setName(const char* name)
	 *
	 * @brief Set the logical name of this connection. This can be used for
	 * Identifying connections withing a client program. If a name is not given,
	 * one will be automatically generated.
	 *
	 * @param name - name of this connection
	 */
	void CALL_TYPE setName(const char* name);


	/**
	 * @fn const char* getID() const
	 *
	 * @desc Get the string ID for this connection.
	 */
	const char* CALL_TYPE getID() const;


	/**
	 * @fn const char* getMWInfo() const
	 *
	 * @desc Returns a string containing middleware information.
	 */
	const char* CALL_TYPE getMWInfo() const;


	/**
	 * @fn const char* getConnectionEndpoint() const
	 *
	 * @desc Returns middleware broker connection information.
	 */
	const char* CALL_TYPE getConnectionEndpoint() const;


	/**
	 * @fn getPublishQueueMessageCount() const
	 *
	 * @brief Retrieves the number of messages queued for asynchronous publish operations
	 *
	 * @return The number of messages in the publish queue
	 */
	GMSEC_U64 CALL_TYPE getPublishQueueMessageCount() const;


	/**
	 * @fn Message createHeartbeatMessage(const char* subject, const gmsec::api::util::DataList<Field*>& heartbeatFields)
	 *
	 * @brief This method creates a message and passes ownership to the user. This message is populated with
	 * the standard set of required and optional heartbeat fields, as well as the required common fields defined
	 * in setStandardFields().
	 *
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param subject         - the topic (subject) that will be applied to the returned messages.
	 * @param heartbeatFields - list of fields to be added to the heartbeat
	 *
	 * @return A C2CX Heartbeat Message
	 *
	 * @throw An Exception is thrown if the subject is NULL.
	 */
	Message CALL_TYPE createHeartbeatMessage(const char* subject, const gmsec::api::util::DataList<Field*>& heartbeatFields);


	/**
	 * @fn void startHeartbeatService(const char* subject, const gmsec::api::util::DataList<Field*>& heartbeatFields);
	 *
	 * @brief This method creates an archetypal heartbeat message from the concatenation of the standard fields
	 * applied globally to this ConnectionManager and to this function. This message will then be validated,
	 * if the "validate" flag is set. If there is an error, an Exception will be thrown. If it has not been set,
	 * the message will be published at an interval supplied by the "PUB-RATE" field regardless of validation
	 * results. If no "PUB-RATE" has been defined, the service will default to the GMSEC standard 30 second
	 * heartbeat interval.
	 *
	 * If users would like to have a COUNTER field added to the published heartbeat message, then the Heartbeat
	 * Service should be provided with this field within the list of fields provided to this method.
	 *
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param subject         - subject to set as the default for heartbeat messages
	 * @param heartbeatFields - list of fields to be added to the published heartbeat messages
	 *
	 * @throw An Exception is thrown if the subject is NULL.
	 */
	void CALL_TYPE startHeartbeatService(const char* subject, const gmsec::api::util::DataList<Field*>& heartbeatFields);


	/**
	 * @fn void stopHeartbeatService()
	 *
	 * @desc This method terminates opertaion of the heartbeat service and cleans up related resources.
	 * It will be called by the global connection manager "cleanup" if not done manually by the user.
	 */
	void CALL_TYPE stopHeartbeatService();


	/**
	 * @fn void changeComponentStatus(const Field& componentStatus)
	 *
	 * @brief This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an Exception is thrown.
	 *
	 * @param componentStatus - Field containing updated information for the Heartbeat Message
	 *
	 * @return The status of the operation.
	 *
	 * @note This method has been deprecated.  Use setHeartbeatServiceField() instead.
	 */
	GMSEC_DEPRECATED Status CALL_TYPE changeComponentStatus(const Field& componentStatus);


	/**
	 * @fn void changeComponentInfo(const Field& componentInfo)
	 *
	 * @brief This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an Exception is thrown.
	 *
	 * @param componentInfo - Field containing updated information for the Heartbeat Message
	 *
	 * @return The status of the operation.
	 *
	 * @note This method has been deprecated.  Use setHeartbeatServiceField() instead.
	 */
	GMSEC_DEPRECATED Status CALL_TYPE changeComponentInfo(const Field& componentInfo);


	/**
	 * @fn void changeCPUMemory(const Field& cpuMemory)
	 *
	 * @brief This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an Exception is thrown.
	 *
	 * @param cpuMemory - Field containing updated information for the Heartbeat Message
	 *
	 * @return The status of the operation.
	 *
	 * @note This method has been deprecated.  Use setHeartbeatServiceField() instead.
	 */
	GMSEC_DEPRECATED Status CALL_TYPE changeCPUMemory(const Field& cpuMemory);


	/**
	 * @fn void changeCPUUtil(const Field& cpuUtil)
	 *
	 * @brief This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an Exception is thrown.
	 *
	 * @param cpuUtil- Field containing updated information for the Heartbeat Message
	 *
	 * @return The status of the operation.
	 *
	 * @note This method has been deprecated.  Use setHeartbeatServiceField() instead.
	 */
	GMSEC_DEPRECATED Status CALL_TYPE changeCPUUtil(const Field& cpuUtil);


	/**
	 * @fn Status setHeartbeatServiceField(const Field& field)
	 *
	 * @brief This method can be used to update or set a field within the Heartbeat Message
	 * being published by the Heartbeat Service.  If validation is enabled, the field will
	 * be validated before it is added to the Heartbeat Message.  Should validation pass,
	 * the field will be persisted to the heartbeat service; otherwise an error is returned.
	 *
	 * @param field - the field containing new or updated information for the Heartbeat Message
	 *
	 * @return The status of the operation.
	 *
	 * @note If a (valid) PUB-RATE field is passed to this method, and the Heartbeat Service
	 * is running, then the Heartbeat Service publish rate will be changed to the provided
	 * rate.  Note that a publish rate of 0 seconds or less will be treated as an error.
	 */
	Status CALL_TYPE setHeartbeatServiceField(const Field& field);


	/**
	 * @fn Message createLogMessage(const char* subject, const gmsec::api::util::DataList<Field*>& logFields)
	 *
	 * @brief This method creates a Log Message and passes ownership to the user. This message is populated
	 * with the standard set of required and optional log fields, as well as the required common fields
	 * defined in setStandardFields. The message is not validated at this time, as MSG-TEXT and SEVERITY
	 * fields must be set by the user at the time the message is to be sent.
	 *
	 * This message automatically is generated with MESSAGE-TYPE and MESSAGE-SUBTYPE
	 * correctly set to the GMSEC Message Standard requirements, regardless of user
	 * input. EVENT-TIME defaults to the time which this function was invoked; the user
	 * may override this time once the message has been returned.
	 *
	 * @param subject   - the topic (subject) to apply to the returned message.
	 * @param logFields - list of fields to be added to the log message. 
	 *
	 * @return A Log Message.
	 */
	Message CALL_TYPE createLogMessage(const char* subject, const gmsec::api::util::DataList<Field*>& logFields);


	/**
	 * @fn void setLoggingDefaults(const char* subject, const gmsec::api::util::DataList<Field*>& logFields)
	 *
	 * @brief This method sets the standard default fields which will be added to a new log message when publishLog
	 * is called.
	 *
	 * @param subject   - subject to use for log messages
	 * @param logFields - list of fields to be added to log messages.
	 */
	void CALL_TYPE setLoggingDefaults(const char* subject, const gmsec::api::util::DataList<Field*>& logFields);


	/**
	 *  @fn void publishLog(const char* logMessage, const Field& severity)
	 *
	 * @brief This method creates a log message comprised of the global fields from the entire ConnectionManager,
	 * the fields set in setLoggingDefaults, and the log text and severity fields as supplied as arguments to
	 * this function. If validation is enabled, the message will be published. If validation is enabled, the
	 * message will be validated.  If validation of the message fails, then an Exception is thrown.
	 *
	 * This function automatically populates the message with the correct
	 * MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set automatically
	 * to the time of function invocation. 
	 *
	 * @param logMessage - the message to log
	 * @param severity - the Field object defining the severity of the log message.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the Log Message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	void CALL_TYPE publishLog(const char* logMessage, const Field& severity);


	/**
	 * @fn void requestDirective(const char* subject,
	 *                           const Field& directiveString, 
	 *                           const gmsec::api::util::DataList<Field*>& fields)
	 *
	 * @brief This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes publish(Message msg) to place that message on the GMSEC
	 * bus. This implicitly sets the message's response flag to false, so that receivers 
	 * of the message will know a response is not necessary
	 *
	 * @param subject - subject on which to publish the message
	 * @param directiveString - a field containing the string directive that this message is intended to convey
	 * @param fields - a list of and supplemental fields that the user wishes to include with the directive message.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	void CALL_TYPE requestDirective(const char * subject,
	                                const Field& directiveString, 
	                                const gmsec::api::util::DataList<Field*>& fields);


	/**
	 * @fn void requestDirective(const char* subject,
	 *                           const Field& directiveString,
	 *                           const gmsec::api::util::DataList<Field*>& fields,
	 *                           GMSEC_I32 timeout, 
	 *                           ConnectionManagerReplyCallback* cb,
	 *                           GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 *
	 * @brief This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes request(const Message& request, GMSEC_I32 timeout, 
	 * ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 * to place that message on the GMSEC bus. This implicitly sets the message's response flag to true,
	 * so that receivers of the message will know a response is necessary.
	 *
	 * @param subject - subject on which to publish the message
	 * @param directiveString - a field containing the string directive that this
	 * message is intended to convey
	 * @param fields - a list of and supplemental fields that the user wishes to include
	 * with the directive message.
	 * @param timeout - The time to wait before a response to the message will no longer
	 * be routed to the supplied callback
	 * @param cb - the callback to be invoked upon reception of a response message
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	void CALL_TYPE requestDirective(const char* subject,
	                                const Field& directiveString,
	                                const gmsec::api::util::DataList<Field*>& fields,
	                                GMSEC_I32 timeout, 
	                                ConnectionManagerReplyCallback* cb,
	                                GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn Message* requestDirective(const char* subject,
     *                               const Field& directiveString, 
	 *                               const gmsec::api::util::DataList<Field*>& fields,
	 *                               GMSEC_I32 timeout, 
	 *                               GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 *
	 * @brief This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes request(const Message& request, GMSEC_I32 timeout, 
	 * GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER) to place that message on the GMSEC bus.
	 * This implicitly sets the message's response flag to true, so that receivers of the message will 
	 * know a response is necessary.
	 *
	 * @param subject - subject on which to publish the message
	 * @param directiveString - a field containing the string directive that this
	 * message is intended to convey
	 * @param fields - a list of and supplemental fields that the user wishes to include
	 * with the directive message.
	 * @param timeout - The time to wait before a response to the message will no longer
	 * be routed to the supplied callback
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 *
	 * @return A pointer to a message received in response to the directive request
	 * message. This message must be released with release(Message *msg). This function
	 * will return NULL of a message is not received in the proscribed period.
	 *
	 */
	Message* CALL_TYPE requestDirective(const char * subject,
	                                    const Field& directiveString, 
	                                    const gmsec::api::util::DataList<Field*>& fields,
	                                    GMSEC_I32 timeout, 
	                                    GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn void acknowledgeDirectiveRequest(const char* subject,
	 *                                      const Message& request,
	 *                                      ResponseStatus::Response ssResponse,
	 *                                      const gmsec::api::util::DataList<Field*>& fields)
	 *
	 * @brief This function acknowledges a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes reply(const Message& request, const Message& reply) to
	 * place that message on the GMSEC bus and route it to the original requestor
	 *
	 * @param subject - subject on which to publish the message
	 * @param request - the original directive message
	 * @param ssResponse - an enumeration specifying the response code to the directive
	 * request.
	 * @param fields - supplemental information fields which may be included with the message
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 *
	 */
	void CALL_TYPE acknowledgeDirectiveRequest(const char* subject,
	                                           const Message &request, 
	                                           ResponseStatus::Response ssResponse,
	                                           const gmsec::api::util::DataList<Field*>& fields);


	/**
	 * @fn void publishResourceMessage(const char* subject, size_t sampleInterval = 1, size_t averageInterval = 10)
	 *
	 * @brief This function creates a valid resource message, as described in the GMSEC
	 * specification. This function then publishes the message to the GMSEC bus on the 
	 * supplied subject.
	 *
	 * @param subject - subject on which to publish the message
	 * @param sampleInterval - distance between samples (in seconds)
	 * @param averageInterval - time interval over which to make a moving average of samples (in seconds)
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if a severe error occurs while attempting to
	 * publish the message.
	 */
	void CALL_TYPE publishResourceMessage(const char* subject, size_t sampleInterval = 1, size_t averageInterval = 10);


	/**
	 * @fn Message createResourceMessage(const char* subject, size_t sampleInterval = 1, size_t averageInternal = 10)
	 *
	 * @brief This function creates a valid resource message, as described in the GMSEC
	 * specification, and returns that message to the user.
	 *
	 * @param subject - subject with which to create the message
	 * @param sampleInterval - distance between samples (in seconds)
	 * @param averageInterval - time interval over which to make a moving average of samples (in seconds)
	 */
	Message CALL_TYPE createResourceMessage(const char* subject, size_t sampleInterval = 1, size_t averageInterval = 10);


	/**
	 * @fn void startResourceMessageService(const char* subject, size_t intervalSeconds = 60, size_t sampleInterval = 1, size_t averageInterval = 10)
	 *
	 * @brief This function starts the resource message service, which publishes 
	 * resource messages at a fixed interval as defined by this function.
	 *
	 * @param subject - subject with which to create the message
	 * @param intervalSeconds - This the interval, in seconds, where messages are published to the bus.
	 * @param sampleInterval - This is the the time distance between samples (in seconds).
	 * @param averageInterval - time window over which to make a moving average of samples (in seconds)
	 */
	void CALL_TYPE startResourceMessageService(const char* subject, size_t intervalSeconds = 60, size_t sampleInterval = 1, size_t averageInterval = 10);


	/**
	 * @fn bool stopResourceMessageService()
	 *
	 * @brief This function stops the resource message service
	 *
	 * @return Returns true if the resource message service was running before invocation of this
	 * function, false otherwise.
	 */
	bool CALL_TYPE stopResourceMessageService();


	/**
	 * @fn void acknowledgeSimpleService(const char* subject,
	 *                                   const Message& request,
	 *                                   ResponseStatus::Response ssResponse,
	 *                                   const gmsec::api::util::DataList<Field*>& fields)
	 *
	 * @brief This function acknowledges a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes reply(const Message& request, const Message& reply) to
	 * place that message on the GMSEC bus and route it to the original requestor
	 *
	 * @param subject - subject on which to publish the message
	 * @param request - the original directive message
	 * @param ssResponse - an enumeration specifying the response code to the simple service
	 * request.
	 * @param fields - supplemental information fields which may be included with the message
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 *
	 */
	void CALL_TYPE acknowledgeSimpleService(const char* subject,
	                                        const Message& request,
	                                        ResponseStatus::Response ssResponse,
	                                        const gmsec::api::util::DataList<Field*>& fields);


	/**
	 * @fn void requestSimpleService(const char* subject,
	 *                               const char* opName, 
	 *                               const Field& opNumber,
	 *                               const gmsec::api::util::DataList<Field*>& fields, 
	 *                               const gmsec::api::util::DataList<ServiceParam*>& sParams)
	 *
	 * @brief This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes publish(Message msg) to place that message on the GMSEC
	 * bus. This implicitly sets the message's response flag to false, so that receivers 
	 * of the message will know a response is not necessary
	 *
	 * @param subject - subject on which to publish the message
	 * @param opName - the name of the operation of the service
	 * @param opNumber - the operation number for the service
	 * message is intended to convey
	 * @param fields - a list of and supplemental fields that the user wishes to include
	 * with the directive message.
	 * @param - a list of the ServiceParam objects providing meta data for this service
	 * invocation
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	void CALL_TYPE requestSimpleService(const char* subject,
	                                    const char * opName, 
	                                    const Field& opNumber,
	                                    const gmsec::api::util::DataList<Field*>& fields, 
	                                    const gmsec::api::util::DataList<ServiceParam*>& sParams);


	/**
	 * @fn void requestSimpleService(const char* subject,
	 *                               const char* opName, 
	 *                               const Field& opNumber,
	 *                               const gmsec::api::util::DataList<Field*>& fields, 
	 *                               const gmsec::api::util::DataList<ServiceParam*>& sParams,
	 *                               GMSEC_I32 timeout, 
	 *                               ConnectionManagerReplyCallback* cb,
	 *                               GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 *
	 * @brief This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes request(const Message& request, GMSEC_I32 timeout, 
	 * ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 * to place that message on the GMSEC bus. This implicitly sets the message's response flag to true,
	 * so that receivers of the message will know a response is necessary
	 *
	 * @param subject - subject on which to publish the message
	 * @param opName - the name of the operation of the service
	 * @param opNumber - the operation number for the service
	 * message is intended to convey
	 * @param fields - a list of and supplemental fields that the user wishes to include
	 * with the directive message.
	 * @param - a list of the ServiceParam objects providing meta data for this service
	 * invocation
	 * @param timeout - The time to wait before a response to the message will no longer
	 * be routed to the supplied callback
	 * @param cb - the callback to be invoked upon reception of a response message
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	void CALL_TYPE requestSimpleService(const char* subject,
	                                    const char* opName, 
	                                    const Field& opNumber,
	                                    const gmsec::api::util::DataList<Field*>& fields, 
	                                    const gmsec::api::util::DataList<ServiceParam*>& sParams,
	                                    GMSEC_I32 timeout, 
	                                    ConnectionManagerReplyCallback* cb,
	                                    GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn Message* requestSimpleService(const char* subject,
	 *                                   const char* opName, 
	 *                                   const Field& opNumber,
	 *                                   const gmsec::api::util::DataList<Field*>& fields, 
	 *                                   const gmsec::api::util::DataList<ServiceParam*>& sParams,
	 *                                   GMSEC_I32 timeout, 
	 *                                   GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 *
	 * @brief This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes request(const Message& request, GMSEC_I32 timeout, 
	 * GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER) to place that message on the GMSEC bus. This implicitly
	 * sets the message's response flag to true, so that receivers of the message will 
	 * know a response is necessary
	 *
	 * @param subject - subject on which to publish the message
	 * @param opName - the name of the operation of the service
	 * @param opNumber - the operation number for the service
	 * message is intended to convey
	 * @param fields - a list of and supplemental fields that the user wishes to include
	 * with the directive message.
	 * @param - a list of the ServiceParam objects providing meta data for this service
	 * invocation
	 * @param timeout - The time to wait before a response to the message will no longer
	 * be routed to the supplied callback
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throw An Exception is thrown if the %ConnectionManager has not been initialized.
	 * @throw An Exception is thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 *
	 * @return A pointer to a message received in response to the directive request
	 * message. This message must be released with release(Message *msg). This function
	 * will return NULL of a message is not received in the proscribed period.
	 */
	Message* CALL_TYPE requestSimpleService(const char* subject,
	                                        const char* opName, 
	                                        const Field& opNumber,
	                                        const gmsec::api::util::DataList<Field*>& fields, 
	                                        const gmsec::api::util::DataList<ServiceParam*>& sParams,
	                                        GMSEC_I32 timeout, 
	                                        GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn void shutdownAllMiddlewares()
	 *
	 * @desc Calls shutdown routines for each middleware that has a shutdown routine registered.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 */
	static void shutdownAllMiddlewares();


	/* @cond For C API support ONLY! */
	void CALL_TYPE registerMessageValidator(GMSEC_MessageValidator* validator);
	/* @endcond */


	/* @cond For C API support ONLY! */
	void CALL_TYPE registerEventCallback(Connection::ConnectionEvent event, GMSEC_ConnectionMgrEventCallback* ecb);
	/* @endcond */


	/* @cond For C API support ONLY! */
	SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& cfg, GMSEC_ConnectionMgrCallback* cb);
	/* @endcond */


	/* @cond For C API support ONLY! */
	void CALL_TYPE request(const Message& req,
	                       GMSEC_I32 timeout,
	                       GMSEC_ConnectionMgrReplyCallback* rcb,
	                       GMSEC_ConnectionMgrEventCallback* ecb,
	                       GMSEC_I32 republish_ms);
	/* @endcond */


	/* @cond For C API support ONLY! */
	void CALL_TYPE cancelRequest(GMSEC_ConnectionMgrReplyCallback* rcb);
	/* @endcond */


	/* @cond For C API support ONLY! */
	void CALL_TYPE requestDirective(const char* subject,
	                                const Field& directiveString,
	                                const gmsec::api::util::DataList<Field*>& fields,
	                                GMSEC_I32 timeout, 
	                                GMSEC_ConnectionMgrReplyCallback* rcb,
	                                GMSEC_ConnectionMgrEventCallback* ecb,
	                                GMSEC_I32 republish_ms);
	/* @endcond */


	/* @cond For C API support ONLY! */
	void CALL_TYPE requestSimpleService(const char* subject,
	                                    const char* opName, 
	                                    const Field& opNumber,
	                                    const gmsec::api::util::DataList<Field*>& fields, 
	                                    const gmsec::api::util::DataList<ServiceParam*>& params,
	                                    GMSEC_I32 timeout, 
	                                    GMSEC_ConnectionMgrReplyCallback* rcb,
	                                    GMSEC_ConnectionMgrEventCallback* ecb,
	                                    GMSEC_I32 republish_ms);
	/* @endcond */


private:
	// not implemented
	ConnectionManager();
	ConnectionManager(const ConnectionManager &);
	ConnectionManager &operator=(const ConnectionManager &);

	gmsec::api::mist::internal::InternalConnectionManager* m_internal;

	friend class gmsec::api::mist::internal::ConnectionManagerBuddy;
};

}  //namespace mist
}  //namespace api
}  //namespace gmsec

#endif
