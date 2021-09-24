/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file connection_manager.h
 *
 * @brief This file contains functions for the management of a MIST ConnectionManager object.
 * The ConnectionManager wraps the GMSEC Connection object, supplying both normal
 * middleware interaction functions and some additional quality of service. 
 * This is accomplished through standard services for producing Log and 
 * Heartbeat messages, as well as functionality for validating messages. 
 * 
 * If the ConnectionManager is configured to validate messages, all messages sent 
 * from this ConnectionManager via either publish or eequest will be compared
 * to an available schema definition. If a template definition for the message
 * in question is available, it will be validated. The message will not be sent
 * over the middleware if validation fails. Messages without a template
 * definition can be sent as well.
 *
 *
 * Example creation and use:
 * @code
 * #include <gmsec4_c.h>
 * 
 * int main(int argc, char** argv)
 * {
 *     GMSEC_Config        config  = configCreateWithArgs(argc, argv);
 *     GMSEC_Status        status  = statusCreate();
 *     GMSEC_ConnectionMgr connMgr = NULL;
 *     GMSEC_Message       msg     = NULL;
 * 
 *     configAddValue(config, "loglevel", "info", status);
 *     if (statusIsError(status)) goto Error;
 * 
 *     // Create connection manager
 *     connMgr = connectionManagerCreate(config, status);
 *     if (statusIsError(status)) goto Error;
 * 
 *     // Initialize connection manager
 *     connectionManagerInitialize(connMgr, status);
 *     if (statusIsError(status)) goto Error;
 * 
 *     // Create a message
 *     msg = messageCreate("GMSEC.FOO.BAR", GMSEC_PUBLISH, status);
 *     if (statusIsError(status)) goto Error;
 * 
 *     // Publish the message
 *     connectionManagerPublish(connMgr, msg, status);
 *     if (statusIsError(status)) goto Error;
 * 
 *     GMSEC_INFO("Published Message:\n%s", messageToXML(msg, NULL));
 * 
 *     // Cleanup connection manager
 *     connectionManagerCleanup(connMgr, status);
 *     if (statusIsError(status)) goto Error;
 * 
 *     goto Cleanup;
 * 
 * Error:
 *     GMSEC_ERROR("Error occurred; reason: %s", statusGet(status));
 * 
 * Cleanup:
 *     if (msg != NULL)
 *     {
 *         messageDestroy(&msg);
 *     }
 * 
 *     if (connMgr != NULL)
 *     {
 *         connectionManagerDestroy(&connMgr);
 *     }
 * 
 *     // Other clean up
 *     statusDestroy(&status);
 *     configDestroy(&config);
 * 
 *     // Call shutdown routine for registered middleware(s) to
 *     // clean up any middleware-related resources.  Currently
 *     // only ActiveMQ users need to call this.
 *     connectionShutdownAllMiddlewares();
 *
 *     return 0;
 * }
 * @endcode
 */


#ifndef GMSEC_API_C_CONNECTION_MANAGER_H
#define GMSEC_API_C_CONNECTION_MANAGER_H

#include <gmsec4/c/mist/mist_defs.h>

#include <gmsec4/c/connection.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>
#include <gmsec4/util/Deprecated.h>

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn const char* connectionManagerGetAPIVersion()
	 *
	 * @brief This function identifies the version of the API.
	 *
	 * @return String containing API version information.
	 */
	GMSEC_API const char* connectionManagerGetAPIVersion();


	/**
	 * @fn GMSEC_ConnectionMgr connectionManagerCreate(const GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief Creates a new ConnectionManager object with the given configuration.
	 *
	 * @note Message validation will be disabled, unless the configuration option GMSEC-MSG-CONTENT-VALIDATE
	 * is set to true.  If GMSEC-MSG-CONTENT-VALIDATE is set to true, then messages will be validated against
	 * the default version of the GMSEC Interface Specification Document (ISD).  The version of the ISD can
	 * be changed by specifying the configuration option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
	 *
	 * @param[in]  config - the configuration object that specifies the type of connection object to create.
	 * @param[out] status - the result of the operation.
	 *
	 * @return A handle to a ConnectionManager object.
	 *
	 * @sa connectionManagerCreateUsingValidation
	 * @sa connectionManagerDestroy
	 */
	GMSEC_API GMSEC_ConnectionMgr connectionManagerCreate(const GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn GMSEC_ConnectionMgr connectionManagerCreateUsingValidation(const GMSEC_Config config, GMSEC_BOOL validate, GMSEC_Status status)
	 *
	 * @brief Creates a new ConnectionMnaager object with the given configuration, and allows user to indicate whether the ConnectionManager
	 * will validate produced messages using the default version of the GMSEC Interface Specification Document (ISD).
	 *
	 * @note If message validation is enabled, then messages will be validated against the default version of the GMSEC Interface Specification
	 * Document (ISD).  The version of the ISD can be changed by specifying the configuration option GMSEC-SPECIFICATION-VERSION to a valid
	 * value (e.g. 201400).
	 *
	 * @param[in]  config   - the configuration object that specifies the type of connection object to create.
	 * @param[in]  validate - used to indicate whether the ConnectionManager should validate messages that are produced.
	 * @param[out] status   - the result of the operation.
	 *
	 * @return A handle to a ConnectionManager object.
	 *
	 * @sa connectionManagerCreate
	 * @sa connectionManagerDestroy
	 */
	GMSEC_API GMSEC_ConnectionMgr connectionManagerCreateUsingValidation(const GMSEC_Config config, GMSEC_BOOL validate, GMSEC_Status status);


	/**
	 * @fn GMSEC_ConnectionMgr connectionManagerCreateUsingISD(const GMSEC_Config config, GMSEC_BOOL validate, unsigned int version, GMSEC_Status status)
	 *
	 * @brief Creates a new ConnectionMnaager object with the given configuration, using the specified version of the GMSEC Interface Specification
	 * Document (ISD) for producing messages.
	 *
	 * @param[in]  config   - the configuration object that specifies the type of connection object to create.
	 * @param[in]  validate - used to indicate whether the ConnectionManager should validate messages that are produced.
	 * @param[in]  version  - the version of the GMSEC Interface Specification Document (ISD) to be used.
	 * @param[out] status   - the result of the operation.
	 *
	 * @return A handle to a ConnectionManager object.
	 *
	 * @sa connectionManagerCreate
	 * @sa connectionManagerDestroy
	 */
	GMSEC_API GMSEC_ConnectionMgr connectionManagerCreateUsingISD(const GMSEC_Config config, GMSEC_BOOL validate, unsigned int version, GMSEC_Status status);


	/**
	 * @fn void connectionManagerDestroy(GMSEC_ConnectionMgr* connMgr)
	 *
	 * @brief Destroys the handle to the given ConnectionManager.
	 *
	 * @param[in,out] connMgr - the handle to the ConnectionManager object
	 */
	GMSEC_API void connectionManagerDestroy(GMSEC_ConnectionMgr* connMgr);


	/**
	 * @fn void connectionManagerInitialize(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Establishes a connection with the GMSEC middleware server.
	 * Upon success, the ConnectionManager is ready for message operations.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - the result of the operation
	 *
	 * @sa connectionManagerCleanup
	 */
	GMSEC_API void connectionManagerInitialize(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn void connectionManagerCleanup(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Disconnects and destroys the underlying connection object associated with the ConnectionManager.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void connectionManagerCleanup(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn GMSEC_ConnectionState connectionManagerGetState(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief This function returns the current state of the connection to the middleware.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return Enumerated GMSEC_ConnectionState value.
	 */
	 GMSEC_API GMSEC_ConnectionState connectionManagerGetState(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn const char* connectionManagerGetLibraryRootName(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this connection is associated with.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return root library name
	 */
	GMSEC_API const char* connectionManagerGetLibraryRootName(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn const char* connectionManagerGetLibraryVersion(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 * @brief This function returns a string containing the version information for
	 * this connection's associated middleware.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - the result of the operation
	 *
	 * @return library version
	 */
	GMSEC_API const char* connectionManagerGetLibraryVersion(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn GMSEC_Specification connectionManagerGetSpecification(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Returns the Specification object associated with the Connection Manager.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to a Specification object.
	 */
	GMSEC_API GMSEC_Specification connectionManagerGetSpecification(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRegisterMessageValidator(GMSEC_ConnectionMgr connMgr, GMSEC_MessageValidator* validator, GMSEC_Status status);
	 *
	 * @brief This method will allow for a user to register their custom message validator
	 * function with the Connection Manager. This custom message validator can be used to
	 * to perform validation of messages in addition to what is performed by the GMSEC API.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager
	 * @param[in]  validator - the address of custom function used to validate a GMSEC message.
	 * @param[out] status    - the status of the operation.
	 */
	GMSEC_API void connectionManagerRegisterMessageValidator(GMSEC_ConnectionMgr connMgr, GMSEC_MessageValidator* validator, GMSEC_Status status);


	/**
	 * @fn void connectionManagerSetStandardFields(GMSEC_ConnectionMgr connMgr, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Sets the internal list of fields that are added to all messages that are created
	 * using the ConnectionManager.  Internal copies of the provided Fields are made, thus
	 * ownership is not retained by the ConnectionManager. The supplied set of fields will not
	 * be validated here; validation occurs at the time a message is to be published.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  fields    - the array of fields to associate with the ConnectionManager
	 * @param[in]  numFields - the number of fields in the array
	 * @param[out] status    - the result of the operation
	 */
	GMSEC_API void connectionManagerSetStandardFields(GMSEC_ConnectionMgr connMgr, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn GMSEC_Field* connectionManagerGetStandardFields(GMSEC_ConnectionMgr connMgr, size_t* numFields, GMSEC_Status status)
	 *
	 * @brief Returns the standard fields that have been associated with the Connection Manager.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[out] numFields - the number of fields in the array that is returned
	 * @param[out] status    - the result of the operation
	 *
	 * @return An array of GMSEC_Field objects.
	 *
	 * @note Use freeFieldArray() to deallocate the memory associated with the returned GMSEC_Field array.
	 *
	 * @sa freeFieldArray()
	 */
	GMSEC_API GMSEC_Field* connectionManagerGetStandardFields(GMSEC_ConnectionMgr connMgr, size_t* numFields, GMSEC_Status status);


	/**
	 * @fn void connectionManagerAddStandardFields(const GMSEC_ConnectionMgr connMgr, GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Adds the standard field (if any) to the given Message object.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  msg     - the handle to the message where to add the standard fields
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void connectionManagerAddStandardFields(const GMSEC_ConnectionMgr connMgr, GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRegisterEventCallback(GMSEC_ConnectionMgr connMgr, GMSEC_ConnectionEvent event, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_Status status)
	 *
	 * @brief Registers an event callback for the desired event.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  event   - the event of interest to monitor
	 * @param[in]  ecb     - the adress of the connection manager event callback function
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void connectionManagerRegisterEventCallback(GMSEC_ConnectionMgr connMgr, GMSEC_ConnectionEvent event, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionInfo connectionManagerSubscribe(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status)
	 *
	 * @brief This function subscribes to a particular subject or pattern.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  subject - the subject/topic to subscribe to
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a SubcriptionInfo object.
	 *
	 * @sa connectionManagerReceive
	 * @sa connectionManagerUnsubscribe
	 */
	GMSEC_API GMSEC_SubscriptionInfo connectionManagerSubscribe(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionInfo connectionManagerSubscribeWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_ConnectionMgrCallback* cb, GMSEC_Status status)
	 *
	 * @brief This function subscribes to a particular subject or pattern and associates a callback to be called when
	 * messages matching the subject or pattern are received. If all subscriptions are performed using this function
	 * then the auto-dispatcher can be used to asynchronously receive messages. If connectionReceive() is used to pull
	 * messages then connectionManagerDispatch() will need to be called to ensure registered callbacks are called.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  subject - the subject/topic to subscribe to
	 * @param[in]  cb      - the address of the callback function.
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a SubcriptionInfo object.
	 *
	 * @sa connectionManagerStartAutoDispatch
	 * @sa connectionManagerUnsubscribe
	 */
	GMSEC_API GMSEC_SubscriptionInfo connectionManagerSubscribeWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_ConnectionMgrCallback* cb, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionInfo connectionManagerSubscribeWithConfig(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief This function subscribes to a particular subject or pattern and uses the config object provided to toggle
	 * between special middleware-level subscription functionalities. (eg. ActiveMQ - Durable Consumer). 
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  subject - the subject/topic to subscribe to
	 * @param[in]  config  - the handle to a configuration object
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a SubcriptionInfo object.
	 *
	 * @sa connectionManagerUnsubscribe
	 */
	GMSEC_API GMSEC_SubscriptionInfo connectionManagerSubscribeWithConfig(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionInfo connectionManagerSubscribeWithConfigAndCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Config config, GMSEC_ConnectionMgrCallback* cb, GMSEC_Status status)
	 *
	 * @brief This function subscribes to a particular subject or pattern and uses the config object provided to toggle
	 * between special middleware-level subscription functionalities. (eg. ActiveMQ - Durable Consumer). 
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  subject - the subject/topic to subscribe to
	 * @param[in]  config  - the handle to a configuration object
	 * @param[in]  cb      - the address of the callback function.
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a SubcriptionInfo object.
	 *
	 * @sa connectionManagerUnsubscribe
	 */
	GMSEC_API GMSEC_SubscriptionInfo connectionManagerSubscribeWithConfigAndCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Config config, GMSEC_ConnectionMgrCallback* cb, GMSEC_Status status);


	/**
	 * @fn void connectionManagerUnsubscribe(GMSEC_ConnectionMgr connMgr, GMSEC_SubscriptionInfo* info, GMSEC_Status status)
	 *
	 * @brief This function unsubscribes to a particular subject pattern, and will stop the reception of messages that
	 * match this pattern. It will also remove the registration of any callbacks with this subject pattern.
	 *
	 * @param[in]     connMgr - the handle to the ConnectionManager object
	 * @param[in,out] info    - the handle to a SubscriptionInfo object
	 * @param[out]    status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerUnsubscribe(GMSEC_ConnectionMgr connMgr, GMSEC_SubscriptionInfo* info, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL connectionManagerStartAutoDispatch(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief This function will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped. If
	 * receive() is called while the auto-dispatcher is used, the behavior will be undesireable and undefined.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A value of GMSEC_TRUE is returned if the auto-dispatcher has been started; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL connectionManagerStartAutoDispatch(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL connectionManagerStopAutoDispatch(GMSEC_ConnectionMgr conn, GMSEC_Status status)
	 *
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A value of GMSEC_TRUE is returned if the auto-dispatcher was running and has been stopped; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL connectionManagerStopAutoDispatch(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn void connectionManagerPublish(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief This function will publish a message to the middleware.
	 *
	 * @note The actual message that is published to the middleware will contain tracking fields; to disable this
	 * feature, create a ConnectionManager object with the tracking=off configuration option.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  msg     - the handle to a Message object
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerPublish(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void connectionManagerPublishWithConfig(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, const GMSEC_Config mwConfig, GMSEC_Status status)
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
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  msg       - the handle to a Message object
	 * @param[in]  mwConfig  - the handle to a Config object for providing middleware configuration options
	 * @param[out] status    - out parameter operation result status
	 */
	GMSEC_API void connectionManagerPublishWithConfig(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, const GMSEC_Config mwConfig, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRequestWithCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status)
	 *
	 * @brief This function will send a request asyncronously. The callback will be called for the reply if it is
	 * received within the specified timeout. This function will not block. The timeout value is expressed in milliseconds.
	 *
	 * @note The actual message that is published to the middleware will contain tracking fields; to disable this
	 * feature, create a ConnectionManager object with the tracking=off configuration option.
	 *
	 * @param[in]  connMgr      - the handle to the ConnectionManager object
	 * @param[in]  request      - the handle to a request Message object
	 * @param[in]  timeout      - the maximum time to wait for reply (in milliseconds)
	 * @param[in]  rcb          - the address of the reply callback function to call when a reply is received.
	 * @param[in]  ecb          - the address of the event callback function to call if/when an event is issued.
	 * @param[in]  republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 *                            value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 * @param[out] status       - out parameter operation result status
	 */
GMSEC_API void connectionManagerRequestWithCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionManagerRequest(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status)
	 *
	 * @brief This function will send a request, wait for the specified timeout, and return the received reply. This function
	 * will block until the reply is received or the timeout is reached. The timeout value is expressed in milliseconds.
	 *
	 * @note The actual message that is published to the middleware will contain tracking fields; to disable this
	 * feature, create a ConnectionManager object with the tracking=off configuration option.
	 *
	 * @param[in]  connMgr      - the handle to the ConnectionManager object
	 * @param[in]  request      - the handle to a request Message object
	 * @param[in]  timeout      - the maximum time to wait for reply (in milliseconds)
	 * @param[in]  republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 *                            value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 * @param[out] status       - out parameter operation result status
	 *
	 * @return A handle to a reply Message, or NULL if a timeout or an error occurs.  If the return value is NULL and the status indicates no error, then it is
	 * safe to assume that a timeout occurred.
	 *
	 * Example code:
	 * @code
	 * GMSEC_Message reply = connectionManagerRequest(connMgr, request, timeout, GMSEC_REQUEST_REPUBLISH_NEVER, status);
	 *
	 * if (statusIsError(status) == GMSEC_TRUE)
	 * {
	 *     // handle error
	 * }
	 * else if (reply == NULL)
	 * {
	 *     // timeout occurred
	 * }
	 * else
	 * {
	 *     //do something with reply message...
	 *
	 *     connectionManagerRelease(&reply);
	 * }
	 * @endcode
	 * @sa connectionManagerRelease
	 */
	GMSEC_API GMSEC_Message connectionManagerRequest(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status);


	/**
	 * @fn void connectionManagerCancelRequest(GMSEC_ConnectionMgr connMgr, GMSEC_ConnectionMgrReplyCallback* cb, GMSEC_Status status)
	 *
	 * @brief This function can be used to cancel a pending request that may be associated with the given reply callback
	 *
	 * @param[in]  connMgr - the handle to the Connection object
	 * @param[in]  cb      - the handle to a reply callback that is currently registered with a pending request
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerCancelRequest(GMSEC_ConnectionMgr connMgr, GMSEC_ConnectionMgrReplyCallback* cb, GMSEC_Status status);


	/**
	 * @fn void connectionManagerReply(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, const GMSEC_Message reply, GMSEC_Status status)
	 *
	 * @brief This function will send a reply in response to a given request.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  request - the handle to the request message that was received, and to which we are responding to
	 * @param[in]  reply   - the handle to the reply message to be sent
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerReply(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, const GMSEC_Message reply, GMSEC_Status status);


	/**
	 * @fn void connectionManagerDispatch(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief This function will cause the any callbacks that are registered with matching message subject patterns to be called.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  msg     - the handle to the message to dispatch (send)
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerDispatch(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionManagerReceive(GMSEC_ConnectionMgr connMgr, GMSEC_I32 timeout, GMSEC_Status status)
	 *
	 * @brief This function returns the next message received within the specified timeout. The received messages are
	 * determined by the message subscriptions set up with the connectionManagerSubscribe() function.
	 *
	 * @note This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * Example usage:
	 * @code
	 * GMSEC_Message msg = connectionManagerReceive(connMgr, 1000, status);
	 *
	 * if (statusIsError(status) == GMSEC_TRUE)
	 * {
	 *     // error
	 * }
	 * else if (!msg)
	 * {
	 *     // timeout
	 * }
	 * else
	 * {
	 *     // received message
	 *
	 *     connectionRelease(msg);
	 * }
	 * @endcode
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  timeout - the the maximum time (in milliseconds) to block waiting for a message; ; if -1
	 * is specified for the timeout, then the call will block indefinitely.
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a Message object, or NULL if a timeout or an error occurs.  Check the status to
	 * determine if an error has occurred (note, a timeout is not considered an error).
	 *
	 * @sa connectionManagerSubscribe
	 * @sa connectionManagerSubscribeWithConfig
	 * @sa connectionManagerRelease
	 */
	GMSEC_API GMSEC_Message connectionManagerReceive(GMSEC_ConnectionMgr connMgr, GMSEC_I32 timeout, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRelease(GMSEC_ConnectionMgr connMgr, GMSEC_Message* msg, GMSEC_Status status)
	 *
	 * @brief This function will destroy a Message object that has been returned by connectionManagerReceive() or
	 * connectionManagerRequest().
	 * This method should NOT be used to destroy any other Message object; for such messages, use messageDestroy().
	 *
	 * @param[in]     connMgr - the handle to the ConnectionManager object
	 * @param[in,out] msg     - the handle to the message that is to be released/destroyed
	 * @param[out]    status  - out parameter operation result status
	 *
	 * @sa connectionManagerReceive
	 * @sa connectionManagerRequest
	 */
	GMSEC_API void connectionManagerRelease(GMSEC_ConnectionMgr connMgr, GMSEC_Message* msg, GMSEC_Status status);


	/**
	 * @fn void connectionManagerExcludeSubject(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status)
	 *
	 * @brief Exclude any incoming messages with the specified subject.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  subject - the subject pattern to look for in incoming messages
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerExcludeSubject(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRemoveExcludedSubject(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status)
	 *
	 * @brief Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  subject - the subject pattern to remove
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerRemoveExcludedSubject(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status);


	/**
	 * @fn const char* connectionManagerGetName(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Returns the name of the connection manager, automatically generated or user specified.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A string
	 */
	GMSEC_API const char* connectionManagerGetName(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn void connectionManagerSetName(GMSEC_ConnectionMgr connMgr, const char* name, GMSEC_Status status)
	 *
	 * @brief Set the logical name of this connection. This can be used for
	 * Identifying connections withing a client program. If a name is not given,
	 * one will be automatically generated.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  name    - name of this connection manager
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerSetName(GMSEC_ConnectionMgr connMgr, const char* name, GMSEC_Status status);


	/**
	 * @fn const char* connectionManagerGetID(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Get the string ID for this connection.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API const char* connectionManagerGetID(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn const char* connectionManagerGetMWInfo(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Returns a string containing middleware information.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API const char* connectionManagerGetMWInfo(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn const char* connectionManagerGetConnectionEndpoint(const GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @desc Returns middleware broker connection information.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API const char* connectionManagerGetConnectionEndpoint(const GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn GMSEC_U64 connectionManagerGetPublishQueueMessageCount(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Retrieves the number of messages queued for asynchronous publish operations
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return The number of messages in the publish queue
	 */
	GMSEC_API GMSEC_U64 connectionManagerGetPublishQueueMessageCount(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionManagerCreateHeartbeatMessage(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Creates a message and passes ownership to the user. This message is populated with
	 * the standard set of required and optional heartbeat fields, as well as the required common fields defined
	 * in connectionManagerSetStandardFields().
	 *
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE, fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  subject   - the subject pattern to remove
	 * @param[in]  fields    - array of fields to be added to the message
	 * @param[in]  numFields - number of field handles in the array
	 * @param[out] status    - out parameter operation result status
	 *
	 * @return On success, a handle to a Message object that contains a C2CX Heartbeat message; otherwise NULL.
	 * In case of the latter, check the returned status.
	 *
	 * @sa connectionManagerRelease
	 */
	GMSEC_API GMSEC_Message connectionManagerCreateHeartbeatMessage(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void connectionManagerStartHeartbeatService(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Creates an archetypal heartbeat message from the concatenation of the standard fields
	 * applied globally to this ConnectionManager and to this function. This message will then be validated,
	 * if the "validate" flag is set. If there is an error, the appropriate status will be returned. If it
	 * has not been set, the message will be published at an interval supplied by the "PUB-RATE" field
	 * regardless of validation results. If no "PUB-RATE" has been defined, the service will default to the
	 * GMSEC standard 30 second heartbeat interval.
	 *
	 * If users would like to have a COUNTER field added to the published heartbeat message, then the Heartbeat
	 * Service should be provided with this field within the array of fields provided to this function.
	 *
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE, fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  subject   - the subject pattern to remove
	 * @param[in]  fields    - array of fields to be added to the heartbeat messages that are published
	 * @param[in]  numFields - number of field handles in the array
	 * @param[out] status    - out parameter operation result status
	 */
	GMSEC_API void connectionManagerStartHeartbeatService(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void connectionManagerStopHeartbeatService(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Terminates the operation of the heartbeat service and cleans up related resources.
	 * This function is automatically called by the global connection manager "cleanup" if not manually
	 * done by the user.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerStopHeartbeatService(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn void connectionManagerChangeComponentStatus(GMSEC_ConnectionMgr connMgr, const GMSEC_Field componentStatus, GMSEC_Status status)
	 *
	 * @brief Updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error will be included with the status.
	 *
	 * @param[in]  connMgr         - the handle to the ConnectionManager object
	 * @param[in]  componentStatus - the handle to the field containing component status data
	 * @param[out] status          - out parameter operation result status
	 *
	 * @note This function has been deprecated; use connectionManagerSetHeartbeatField() instead.
	 */
	GMSEC_DEPRECATED GMSEC_API void connectionManagerChangeComponentStatus(GMSEC_ConnectionMgr connMgr, const GMSEC_Field componentStatus, GMSEC_Status status);


	/**
	 * @fn void connectionManagerChangeComponentInfo(GMSEC_ConnectionMgr connMgr, const GMSEC_Field componentInfo, GMSEC_Status status)
	 *
	 * @brief Updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error will be included with the status.
	 *
	 * @param[in]  connMgr       - the handle to the ConnectionManager object
	 * @param[in]  componentInfo - the handle to the field containing component information
	 * @param[out] status        - out parameter operation result status
	 *
	 * @note This function has been deprecated; use connectionManagerSetHeartbeatField() instead.
	 */
	GMSEC_DEPRECATED GMSEC_API void connectionManagerChangeComponentInfo(GMSEC_ConnectionMgr connMgr, const GMSEC_Field componentInfo, GMSEC_Status status);


	/**
	 * @fn void connectionManagerChangeCPUMemory(GMSEC_ConnectionMgr connMgr, const GMSEC_Field cpuMemory, GMSEC_Status status)
	 *
	 * @brief Updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error will be included with the status.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  cpuMemory - the handle to the field containing CPU memory data
	 * @param[out] status    - out parameter operation result status
	 *
	 * @note This function has been deprecated; use connectionManagerSetHeartbeatField() instead.
	 */
	GMSEC_DEPRECATED GMSEC_API void connectionManagerChangeCPUMemory(GMSEC_ConnectionMgr connMgr, const GMSEC_Field cpuMemory, GMSEC_Status status);


	/**
	 * @fn void connectionManagerChangeCPUUtil(GMSEC_ConnectionMgr connMgr, const GMSEC_Field cpuUtil, GMSEC_Status status)
	 *
	 * @brief Updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error will be included with the status.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  cpuUtil - the handle to the field containing CPU utilization data
	 * @param[out] status  - out parameter operation result status
	 *
	 * @note This function has been deprecated; use connectionManagerSetHeartbeatField() instead.
	 */
	GMSEC_DEPRECATED GMSEC_API void connectionManagerChangeCPUUtil(GMSEC_ConnectionMgr connMgr, const GMSEC_Field cpuUtil, GMSEC_Status status);


	/**
	 * @fn void connectionManagerSetHeartbeatServiceField(GMSEC_ConnectionMgr connMgr, const GMSEC_Field field, GMSEC_Status status)
	 *
	 * @brief Updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error will be included with the status.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[in]  field   - the handle to the field to include within the heartbeat message
	 * @param[out] status  - out parameter operation result status
	 *
	 * @note If a (valid) PUB-RATE field is passed to this method, and the Heartbeat Service
	 * is running, then the Heartbeat Service publish rate will be changed to the provided
	 * rate.  Note that a publish rate of 0 seconds or less will be treated as an error.
	 */
	GMSEC_API void connectionManagerSetHeartbeatServiceField(GMSEC_ConnectionMgr connMgr, const GMSEC_Field field, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionManagerCreateLogMessage(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Creates a Log Message and passes ownership to the user. This message is populated
	 * with the standard set of required and optional log fields, as well as the required common fields
	 * defined in connectionManagerSetStandardFields. The message is not validated at this time, as MSG-TEXT
	 * and SEVERITY fields must be set by the user at the time the message is to be sent.
	 *
	 * This message automatically is generated with MESSAGE-TYPE and MESSAGE-SUBTYPE
	 * correctly set to the GMSEC Message Standard requirements, regardless of user
	 * input. EVENT-TIME defaults to the time which this function was invoked; the user
	 * may override this time once the message has been returned.
	 *
	 * @note When the user done with the message, they should destroy it using messageDestroy or connectionManagerRelease.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  subject   - the subject to include with the log message
	 * @param[in]  fields    - the array of fields to add to the log messsage
	 * @param[in]  numFields - the number of fields in the array
	 * @param[out] status    - out parameter operation result status
	 *
	 * @return A handle to a Log Message, or NULL if an error occurs.  In case of the latter, examine the status.
	 *
	 * @sa connectionManagerRelease
	 */
	GMSEC_API GMSEC_Message connectionManagerCreateLogMessage(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void connectionManagerSetLoggingDefaults(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Sets the standard default fields which will be added to a new log message when connectionManagerPublishLog is called.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  subject   - the subject to include with the log message
	 * @param[in]  fields    - the array of fields to add to the log messsage
	 * @param[in]  numFields - the number of fields in the array
	 * @param[out] status    - out parameter operation result status
	 */
	GMSEC_API void connectionManagerSetLoggingDefaults(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void connectionManagerPublishLog(GMSEC_ConnectionMgr connMgr, const char* logMsg, const GMSEC_Field severity, GMSEC_Status status)
	 *
	 * @brief Creates a log message comprised of the global fields from the entire ConnectionManager, the fields set in
	 * connectionManagerSetLoggingDefaults, and the log text and severity fields as supplied as arguments to this function.
	 * If validation is enabled, the message will be published. If validation is enabled, the message will be validated.
	 * If validation of the message fails, then the appropriate error is returned with the status.
	 *
	 * This function automatically populates the message with the correct
	 * MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set automatically
	 * to the time of function invocation.
	 *
	 * @param[in]  connMgr  - the handle to the ConnectionManager object
	 * @param[in]  logMsg   - the text to be included with the log message
	 * @param[in]  severity - the handle to the field containing the log message's severity
	 * @param[out] status   - out parameter operation result status
	 */
	GMSEC_API void connectionManagerPublishLog(GMSEC_ConnectionMgr connMgr, const char* logMsg, const GMSEC_Field severity, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRequestDirectiveNoReply(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Creates a Request Directive message, per the description in the GMSEC Interface Specification Document (ISD). The message is
	 * populated with the standard fields for this connection, as well as required versioning and message description information as
	 * described in the GMSEC ISD.
	 *
	 * This function sends the request directive onto the GMSEC bus, and it implicitly sets the message's response flag to false, so that
	 * receivers of the message will know that a response (reply) is not necessary.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  subject   - the subject to include with the request directive message
	 * @param[in]  dirString - handle to the field containing the directive string
	 * @param[in]  fields    - array of fields to include with the request directive
	 * @param[in]  numFields - number of fields in the array
	 * @param[out] status    - out parameter operation result status
	 */
	GMSEC_API void connectionManagerRequestDirectiveNoReply(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRequestDirectiveWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status)
	 *
	 * @brief Creates a Request Directive message, per the description in the GMSEC Interface Specification Document (ISD). The message is
	 * populated with the standard fields for this connection, as well as required versioning and message description information as
	 * described in the GMSEC ISD.
	 *
	 * This function sends the request directive onto the GMSEC bus, and it implicitly sets the message's response flag to true, so that
	 * receivers of the message will know a that response (reply) should be issued.
	 *
	 * @param[in]  connMgr      - the handle to the ConnectionManager object
	 * @param[in]  subject      - the subject to include with the request directive message
	 * @param[in]  dirString    - handle to the field containing the directive string
	 * @param[in]  fields       - array of fields to include with the request directive
	 * @param[in]  numFields    - number of fields in the array
	 * @param[in]  timeout      - the time period (in milliseconds) to wait for a reply
	 * @param[in]  rcb          - address to a ConnectionManager Reply Callback function
	 * @param[in]  ecb          - address to a ConnectionManager Event Callback function
	 * @param[in]  republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 *                            value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 * @param[out] status       - out parameter operation result status
	 */
	GMSEC_API void connectionManagerRequestDirectiveWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionManagerRequestDirective(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status)
	 *
	 * @brief Creates a Request Directive message, per the description in the GMSEC Interface Specification Document (ISD). The message is
	 * populated with the standard fields for this connection, as well as required versioning and message description information as
	 * described in the GMSEC ISD.
	 *
	 * This function sends the request directive onto the GMSEC bus, and it implicitly sets the message's response flag to true, so that
	 * receivers of the message will know a that response (reply) should be issued.
	 *
	 * @param[in]  connMgr      - the handle to the ConnectionManager object
	 * @param[in]  subject      - the subject to include with the request directive message
	 * @param[in]  dirString    - handle to the field containing the directive string
	 * @param[in]  fields       - array of fields to include with the request directive
	 * @param[in]  numFields    - number of fields in the array
	 * @param[in]  timeout      - the time period (in milliseconds) to wait for a reply
	 * @param[in]  republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 *                            value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 * @param[out] status       - out parameter operation result status
	 *
	 * @return A handle to a reply Message, or NULL if a timeout or an error occurs.  If the return value is NULL and the status indicates no error, then it is
	 * safe to assume that a timeout occurred.
	 *
	 * @sa connectionManagerRelease
	 */
	GMSEC_API GMSEC_Message connectionManagerRequestDirective(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Field dirString, const GMSEC_Field fields[], size_t numFields, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status);


	/**
	 * @fn void connectionManagerAckDirectiveRequest(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Message request, GMSEC_ResponseStatus response, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief This function acknowledges a Request Directive message, per the description in the GMSEC Interface Specification Document (ISD).
	 * The message is populated with the standard fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC ISD.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  subject   - the subject to include with the request directive acknowledgement message
	 * @param[in]  request   - the handle to the request message that was received
	 * @param[in]  response  - the response status value
	 * @param[in]  fields    - an array of fields to be included with the ack message
	 * @param[in]  numFields - the number of fields in the array
	 * @param[out] status    - out parameter operation result status
	 */
	GMSEC_API void connectionManagerAckDirectiveRequest(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Message request, GMSEC_ResponseStatus response, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void connectionManagerPublishResourceMessage(GMSEC_ConnectionMgr connMgr, const char* subject, size_t sampleInterval, size_t averageInterval, GMSEC_Status status)
	 *
	 * @brief This function creates a resource message, as described in the GMSEC ISD. This function then publishes the message to the GMSEC bus using the 
	 * supplied subject.
	 *
	 * @param[in]  connMgr         - the handle to the ConnectionManager object
	 * @param[in]  subject         - the subject to include with the resource message
	 * @param[in]  sampleInterval  - time period (in seconds) between samples
	 * @param[in]  averageInterval - time interval (in seconds) over which to perform a moving average of samples
	 * @param[out] status          - out parameter operation result status
	 */
	GMSEC_API void connectionManagerPublishResourceMessage(GMSEC_ConnectionMgr connMgr, const char* subject, size_t sampleInterval, size_t averageInterval, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionManagerCreateResourceMessage(GMSEC_ConnectionMgr connMgr, const char* subject, size_t sampleInterval, size_t averageInterval, GMSEC_Status status)
	 *
	 * @brief Creates a valid resource message, as described in the GMSEC ISD, and returns that message to the user.
	 *
	 * @param[in]  connMgr         - the handle to the ConnectionManager object
	 * @param[in]  subject         - the subject to include with the resource message
	 * @param[in]  sampleInterval  - time period (in seconds) between samples
	 * @param[in]  averageInterval - time interval (in seconds) over which to perform a moving average of samples
	 * @param[out] status          - out parameter operation result status
	 *
	 * @return A handle to a resource message, or NULL if an error occurs.  In case of the latter, examine the status.
	 *
	 * @sa connectionManagerRelease
	 */
	GMSEC_API GMSEC_Message connectionManagerCreateResourceMessage(GMSEC_ConnectionMgr connMgr, const char* subject, size_t sampleInterval, size_t averageInterval, GMSEC_Status status);


	/**
	 * @fn void connectionManagerStartResourceMessageService(GMSEC_ConnectionMgr connMgr, const char* subject, size_t intervalSeconds, size_t sampleInterval, size_t averageInterval, GMSEC_Status status)
	 *
	 * @brief Starts the resource message service, which publishes resource messages at a fixed interval as defined by this function.
	 *
	 * @param[in]  connMgr         - the handle to the ConnectionManager object
	 * @param[in]  subject         - the subject to include with the resource message
	 * @param[in]  interval        - the interval (in seconds) at which the messages are published to the bus
	 * @param[in]  sampleInterval  - time period (in seconds) between samples
	 * @param[in]  averageInterval - time interval (in seconds) over which to perform a moving average of samples
	 * @param[out] status          - out parameter operation result status
	 */
	GMSEC_API void connectionManagerStartResourceMessageService(GMSEC_ConnectionMgr connMgr, const char* subject, size_t intervalSeconds, size_t sampleInterval, size_t averageInterval, GMSEC_Status status);


	/**
	 * @fn void connectionManagerStopResourceMessageService(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
	 *
	 * @brief Stops the resource message service.
	 *
	 * @param[in]  connMgr - the handle to the ConnectionManager object
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionManagerStopResourceMessageService(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);


	/**
	 * @fn void connectionManagerAckSimpleService(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Message request, GMSEC_ResponseStatus response, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status)
	 *
	 * @brief Acknowledges a Simple Service message, per the description in the GMSEC Interface Specification Document (ISD). The message is
	 * populated with the standard fields for this connection, as well as required versioning and message description information as
	 * described in the GMSEC ISD.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  subject   - the subject to include with the acknowledgement message
	 * @param[in]  request   - the handle to the receive request message
	 * @param[in]  response  - the response status value
	 * @param[in]  fields    - array of fields to include in the acknowledgement message
	 * @param[in]  numFields - number of fields in the array
	 * @param[out] status    - out parameter operation result status
	 */
	GMSEC_API void connectionManagerAckSimpleService(GMSEC_ConnectionMgr connMgr, const char* subject, const GMSEC_Message request, GMSEC_ResponseStatus response, const GMSEC_Field fields[], size_t numFields, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRequestSimpleServiceNoReply(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber, const GMSEC_Field fields[], size_t numFields, const GMSEC_ServiceParam params[], size_t numParams, GMSEC_Status status)
	 *
	 * @brief Creates a Simple Service message, per the description in the GMSEC Interface Specification Document (ISD). The message is
	 * populated with the standard fields for this connection, as well as required versioning and message description information as
	 * described in the GMSEC ISD.
	 *
	 * This function sends the request directive onto the GMSEC bus, and it implicitly sets the message's response flag to false, so that
	 * receivers of the message will know that a response (reply) is not necessary.
	 *
	 * @param[in]  connMgr   - the handle to the ConnectionManager object
	 * @param[in]  subject   - the subject to include with the simple service message
	 * @param[in]  opName    - the name of the service operation
	 * @param[in]  opNumber  - the operation number for the service
	 * @param[in]  fields    - an array of fields to be included with the simple service message
	 * @param[in]  numFields - the number of fields in the array
	 * @param[in]  params    - the array of ServiceParam object providing meta data for this service invocation
	 * @param[in]  numParams - the number of ServiceParam objects
	 * @param[out] status    - out parameter operation result status
	 */
	GMSEC_API void connectionManagerRequestSimpleServiceNoReply(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber, const GMSEC_Field fields[], size_t numFields, const GMSEC_ServiceParam params[], size_t numParams, GMSEC_Status status);


	/**
	 * @fn void connectionManagerRequestSimpleServiceWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber, const GMSEC_Field fields[], size_t numFields, const GMSEC_ServiceParam params[], size_t numParams, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status)
	 *
	 * @brief Creates a Simple Service message, per the description in the GMSEC Interface Specification Document (ISD). The message is
	 * populated with the standard fields for this connection, as well as required versioning and message description information as
	 * described in the GMSEC ISD.
	 *
	 * This function sends the request directive onto the GMSEC bus, and it implicitly sets the message's response flag to true, so that
	 * receivers of the message will know that a response (reply) is required.
	 *
	 * @param[in]  connMgr      - the handle to the ConnectionManager object
	 * @param[in]  subject      - the subject to include with the simple service message
	 * @param[in]  opName       - the name of the service operation
	 * @param[in]  opNumber     - the operation number for the service
	 * @param[in]  fields       - an array of fields to be included with the simple service message
	 * @param[in]  numFields    - the number of fields in the array
	 * @param[in]  params       - the array of ServiceParam object providing meta data for this service invocation
	 * @param[in]  numParams    - the number of ServiceParam objects
	 * @param[in]  timeout      - the period (in milliseconds) to wait for a reply
	 * @param[in]  rcb          - the address to the ConnectionManager Reply Callback function
	 * @param[in]  ecb          - the address to the ConnectionManager Event Callback function
	 * @param[in]  republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 *                            value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 * @param[out] status       - out parameter operation result status
	 */
	GMSEC_API void connectionManagerRequestSimpleServiceWithCallback(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber, const GMSEC_Field fields[], size_t numFields, const GMSEC_ServiceParam params[], size_t numParams, GMSEC_I32 timeout, GMSEC_ConnectionMgrReplyCallback* rcb, GMSEC_ConnectionMgrEventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionManagerRequestSimpleService(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber, const GMSEC_Field fields[], size_t numFields, const GMSEC_ServiceParam params[], size_t numParams, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status)
	 *
	 * @brief Creates a Simple Service message, per the description in the GMSEC Interface Specification Document (ISD). The message is
	 * populated with the standard fields for this connection, as well as required versioning and message description information as
	 * described in the GMSEC ISD.
	 *
	 * This function sends the request directive onto the GMSEC bus, and it implicitly sets the message's response flag to true, so that
	 * receivers of the message will know that a response (reply) is required.
	 *
	 * @param[in]  connMgr      - the handle to the ConnectionManager object
	 * @param[in]  subject      - the subject to include with the simple service message
	 * @param[in]  opName       - the name of the service operation
	 * @param[in]  opNumber     - the operation number for the service
	 * @param[in]  fields       - an array of fields to be included with the simple service message
	 * @param[in]  numFields    - the number of fields in the array
	 * @param[in]  params       - the array of ServiceParam object providing meta data for this service invocation
	 * @param[in]  numParams    - the number of ServiceParam objects
	 * @param[in]  timeout      - the period (in milliseconds) to wait for a reply
	 * @param[in]  republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 *                            value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 * @param[out] status       - out parameter operation result status
	 *
	 * @return A handle to a reply Message, or NULL if a timeout or an error occurs.  If the return value is NULL and the status indicates no error, then it is
	 * safe to assume that a timeout occurred.
	 */
	GMSEC_API GMSEC_Message connectionManagerRequestSimpleService(GMSEC_ConnectionMgr connMgr, const char* subject, const char* opName, const GMSEC_Field opNumber, const GMSEC_Field fields[], size_t numFields, const GMSEC_ServiceParam params[], size_t numParams, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status);


	/**
	 * @fn void connectionManagerShutdownAllMiddlewares(void)
	 *
	 * @desc Calls shutdown routines for each middleware that has a shutdown routine registered.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 */
	GMSEC_API void connectionManagerShutdownAllMiddlewares(void);


	/**
	 * @fn void freeFieldArray(GMSEC_Field* array)
	 *
	 * @brief To be used ONLY to free an array of GMSEC_Field object(s) that is returned/owned by the API.
	 *
	 * @param[in] array - the array to be freed/deallocated.
	 */
	GMSEC_API void freeFieldArray(GMSEC_Field* array);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
