/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file connection.h
 *
 *  @brief This file contains functions for the management of a connection object.
 *
 * Example creation and use:
 * @code
 * #include <gmsec4_c.h>
 * 
 * int main(int argc, const char** argv)
 * {
 *     GMSEC_Config     config = configCreateWithArgs(argc, argv);
 *     GMSEC_Status     status = statusCreate();
 *     GMSEC_Connection conn   = NULL;
 *     GMSEC_Message    msg    = NULL;
 *
 *     configAddValue(config, "loglevel", "info", status);
 *     if (statusIsError(status)) goto Error;
 *
 *     // Create connection
 *     conn = connectionCreate(config, status);
 *     if (statusIsError(status)) goto Error;
 *
 *     // Connect to the middleware server
 *     connectionConnect(conn, status);
 *     if (statusIsError(status)) goto Error;
 *
 *     // Create a message
 *     msg = messageCreate("GMSEC.FOO.BAR", GMSEC_PUBLISH, status);
 *     if (statusIsError(status)) goto Error;
 *
 *     // Publish the message
 *     connectionPublish(conn, msg, status);
 *     if (statusIsError(status)) goto Error;
 *
 *     GMSEC_INFO("Published Message:\n%s", messageToXML(msg, NULL));
 *
 *     // Disconnect from the middleware server
 *     connectionDisconnect(conn, status);
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
 *         // Destroy the message
 *         messageDestroy(&msg);
 *     }
 *
 *     if (conn != NULL)
 *     {
 *         // Destroy the connection 
 *         connectionDestroy(&conn);
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


#ifndef GMSEC_API_C_CONNECTION_H
#define GMSEC_API_C_CONNECTION_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


/**
 * @enum GMSEC_ConnectionState
 *
 * @sa connectionGetState
 */
typedef enum
{
	GMSEC_NOT_CONNECTED = 0,    /**< Connection not established */
	GMSEC_CONNECTED = 1,        /**< Connection established */
	GMSEC_RECONNECTING = 2      /**< Connection broken and attempt is being made to re-establishe it */
} GMSEC_ConnectionState;


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Connection connectionCreate(GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief Creates a new Connection object with the given configuration.
	 *
	 * @param[in]  config - the configuration object that specifies the type of connection object to create.
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to a Connection object, or NULL if a failure occurs.  If the latter occurs,
	 * examine the status for the reason.
	 */
	GMSEC_API GMSEC_Connection connectionCreate(GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn void connectionDestroy(GMSEC_Connection* conn)
	 *
	 * @brief Destroy the connection object associated with the given handle.
	 *
	 * @param [in,out] conn - a handle to a connection object.
	 */
	GMSEC_API void connectionDestroy(GMSEC_Connection* conn);


	/**
	 * @fn const char* connectionGetAPIVersion(void)
	 *
	 * @brief Returns the GMSEC API version number.
	 *
	 * @return A string containing the API version number.
	 */
	GMSEC_API const char* connectionGetAPIVersion(void);


	/**
	 * @fn void connectionConnect(GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief Establishes a connection to the middleware server.
	 *
	 * @param[in]  conn   - the handle to the Connection object.
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void connectionConnect(GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn void connectionDisconnect(GMSEC_Connection conn, GMSEC_Status status);
	 *
	 * @brief Terminates an existing connection to the middleware server.
	 *
	 * @param[in]  conn   - the handle to the Connection object.
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void connectionDisconnect(GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn GMSEC_ConnectionState connectionGetState(const GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief Returns the current state of the Connection object.
	 *
	 * @param[in]  conn   - the handle to the Connection object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A ConnectionState enumerated value.
	 */
	GMSEC_API GMSEC_ConnectionState connectionGetState(const GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn const char* connectionGetLibraryRootName(const GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief Returns the name of the middleware library being used by the Connection object.
	 *
	 * @param[in]  conn   - the handle to the Connection object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A string containing the library name.
	 */
	GMSEC_API const char* connectionGetLibraryRootName(const GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn const char* connectionGetLibraryVersion(const GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief Returns the version number of the middleware being used by the Connection object.
	 *
	 * @param[in]  conn   - the handle to the Connection object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A string containing the version number of the middleware.
	 */
	GMSEC_API const char* connectionGetLibraryVersion(const GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn void connectionRegisterEventCallback(GMSEC_Connection conn, GMSEC_ConnectionEvent event, GMSEC_EventCallback* cb, GMSEC_Status status)
	 *
	 * @brief Registers an event callback for the desired event.
	 *
	 * @param[in]  conn   - the handle to the Connection object.
	 * @param[in]  event  - the event of interest to monitor.
	 * @param[in]  cb     - the address of the event callback function.
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void connectionRegisterEventCallback(GMSEC_Connection conn, GMSEC_ConnectionEvent event, GMSEC_EventCallback* cb, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionInfo connectionSubscribe(GMSEC_Connection conn, const char* subject, GMSEC_Status status)
	 *
	 * @brief This function subscribes to a particular subject or pattern.
	 *
	 * @param[in]  conn    - the handle to the Connection object
	 * @param[in]  subject - the subject/topic to subscribe to
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a GMSEC_SubscriptionInfo object.
	 *
	 * @sa connectionReceive
	 * @sa connectionUnsubscribe
	 */
	GMSEC_API GMSEC_SubscriptionInfo connectionSubscribe(GMSEC_Connection conn, const char* subject, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionInfo connectionSubscribeWithCallback(GMSEC_Connection conn, const char* subject, GMSEC_Callback* cb, GMSEC_Status status)
	 *
	 * @brief This function subscribes to a particular subject or pattern and associates a callback to be called when
	 * messages matching the subject or pattern are received. If all subscriptions are performed using this function
	 * then the auto-dispatcher can be used to asynchronously receive messages. If connectionReceive() is used to pull
	 * messages then connectionDispatch() will need to be called to ensure registered callbacks are called.
	 *
	 * @param[in]  conn    - the handle to the Connection object
	 * @param[in]  subject - the subject/topic to subscribe to
	 * @param[in]  cb      - the address of the callback function.
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a GMSEC_SubscriptionInfo object.
	 *
	 * @sa connectionUnsubscribe
	 */
	GMSEC_API GMSEC_SubscriptionInfo connectionSubscribeWithCallback(GMSEC_Connection conn, const char* subject, GMSEC_Callback* cb, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionInfo connectionSubscribeWithConfig(GMSEC_Connection conn, const char* subject, GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief This function subscribes to a particular subject or pattern and uses the config object provided to toggle
	 * between special middleware-level subscription functionalities. (eg. ActiveMQ - Durable Consumer). 
	 *
	 * @param[in]  conn    - the handle to the Connection object
	 * @param[in]  subject - the subject/topic to subscribe to
	 * @param[in]  config  - the handle to a configuration object
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a GMSEC_SubscriptionInfo object.
	 *
	 * @sa connectionUnsubscribe
	 */
	GMSEC_API GMSEC_SubscriptionInfo connectionSubscribeWithConfig(GMSEC_Connection conn, const char* subject, GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionInfo connectionSubscribeWithConfigAndCallback(GMSEC_Connection conn, const char* subject, GMSEC_Config config, GMSEC_Callback* cb, GMSEC_Status status)
	 *
	 * @brief This function subscribes to a particular subject or pattern and uses the config object provided to toggle
	 * between special middleware-level subscription functionalities. (eg. ActiveMQ - Durable Consumer). 
	 *
	 * @param[in]  conn    - the handle to the Connection object
	 * @param[in]  subject - the subject/topic to subscribe to
	 * @param[in]  config  - the handle to a configuration object
	 * @param[in]  cb      - the address of the callback function.
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a GMSEC_SubscriptionInfo object.
	 *
	 * @sa connectionUnsubscribe
	 */
	GMSEC_API GMSEC_SubscriptionInfo connectionSubscribeWithConfigAndCallback(GMSEC_Connection conn, const char* subject, GMSEC_Config config, GMSEC_Callback* cb, GMSEC_Status status);


	/**
	 * @fn void connectionUnsubscribe(GMSEC_Connection conn, GMSEC_SubscriptionInfo* info, GMSEC_Status status)
	 *
	 * @brief This function unsubscribes to a particular subject pattern, and will stop the reception of messages that
	 * match this pattern. It will also remove the registration of any callbacks with this subject pattern.
	 *
	 * @param[in]     conn   - the handle to the Connection object
	 * @param[in,out] info   - the handle to a GMSEC_SubscriptionInfo object
	 * @param[out]    status - out parameter operation result status
	 */
	GMSEC_API void connectionUnsubscribe(GMSEC_Connection conn, GMSEC_SubscriptionInfo* info, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL connectionStartAutoDispatch(GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief This function will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped. If
	 * receive() is called while the auto-dispatcher is used, the behavior will be undesireable and undefined.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A value of true is returned if the auto-dispatcher has been started; false otherwise.
	 */
	 GMSEC_API GMSEC_BOOL connectionStartAutoDispatch(GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL connectionStopAutoDispatch(GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A value of true is returned if the auto-dispatcher was running and has been stopped; false otherwise.
	 */
	 GMSEC_API GMSEC_BOOL connectionStopAutoDispatch(GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn void connectionPublish(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief This function will publish a message to the middleware.
	 *
	 * @note The actual message that is published to the middleware will contain tracking fields; to disable this
	 * feature, create a Connection object with the tracking=off configuration option.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[in]  msg    - the handle to a Message object
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void connectionPublish(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn void connectionPublishWithConfig(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Config mwConfig, GMSEC_Status status)
	 *
	 * @brief Publishes the given message to the middleware
	 * using the given configuration to enable or disable certain middleware-level
	 * publish functionalities (e.g. ActiveMQ - Durable Producer).
	 *
	 * @note The actual message that is published to the middleware will contain tracking fields; to disable this
	 * feature, create a Connection object with the tracking=off configuration option.
	 *
	 * @param[in]  conn     - the handle to the Connection object
	 * @param[in]  msg      - the handle to a Message object
	 * @param[in]  mwConfig - the handle to a Config object for providing middleware configuration options
	 * @param[out] status   - out parameter operation result status
	 */
	GMSEC_API void connectionPublishWithConfig(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn void connectionRequestWithCallback(GMSEC_Connection conn, GMSEC_Message request, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status)
	 *
	 * @brief This function will send a request asyncronously. The callback will be called for the reply if it is
	 * received within the specified timeout. This function will not block. The timeout value is expressed in milliseconds.
	 *
	 * @note The actual message that is published to the middleware will contain tracking fields; to disable this
	 * feature, create a Connection object with the tracking=off configuration option.
	 *
	 * @param[in]  conn         - the handle to the Connection object
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
	GMSEC_API void connectionRequestWithCallback(GMSEC_Connection conn, GMSEC_Message request, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionRequest(GMSEC_Connection conn, GMSEC_Message request, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status)
	 *
	 * @brief This function will send a request, wait for the specified timeout, and return the received reply. This function
	 * will block until the reply is received or the timeout is reached. The timeout value is expressed in milliseconds.
	 *
	 * @note The actual message that is published to the middleware will contain tracking fields; to disable this
	 * feature, create a Connection object with the tracking=off configuration option.
	 *
	 * @param[in]  conn         - the handle to the Connection object
	 * @param[in]  request      - the handle to a request Message object
	 * @param[in]  timeout      - the maximum time to wait for reply (in milliseconds)
	 * @param[in]  republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 *                            value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                            the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                            Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 * @param[out] status       - out parameter operation result status
	 *
	 * @return A handle to the received reply Message, or NULL if a timeout or an error occurs.  If the status indicates no error, then it is
	 * safe to assume that a timeout occurred.
	 *
	 * Example code:
	 * @code
	 * GMSEC_Message reply = connectionRequest(conn, request, timeout, -1, status);
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
	 *     connectionRelease(&reply);
	 * }
	 * @endcode
	 * @sa connectionRelease
	 */
	GMSEC_API GMSEC_Message connectionRequest(GMSEC_Connection conn, GMSEC_Message request, GMSEC_I32 timeout, GMSEC_I32 republish_ms, GMSEC_Status status);


	/**
	 * @fn void connectionCancelRequest(GMSEC_Connection conn, GMSEC_ReplyCallback* cb, GMSEC_Status status)
	 *
	 * @brief This function can be used to cancel a pending request that may be associated with the given reply callback
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[in]  cb     - the handle to a reply callback that is currently registered with a pending request
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void connectionCancelRequest(GMSEC_Connection conn, GMSEC_ReplyCallback* cb, GMSEC_Status status);


	/**
	 * @fn void connectionReply(GMSEC_Connection conn, GMSEC_Message request, GMSEC_Message reply, GMSEC_Status status)
	 *
	 * @brief This function will send a reply in response to a given request.
	 *
	 * @param[in]  conn    - the handle to the Connection object
	 * @param[in]  request - the handle to the request message that was received, and to which we are responding to
	 * @param[in]  reply   - the handle to the reply message to be sent
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionReply(GMSEC_Connection conn, GMSEC_Message request, GMSEC_Message reply, GMSEC_Status status);


	/**
	 * @fn void connectionDispatch(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief This function will cause the any callbacks that are registered with matching message subject patterns to be called.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[in]  msg    - the handle to the message to dispatch (send)
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void connectionDispatch(GMSEC_Connection conn, GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message connectionReceive(GMSEC_Connection conn, GMSEC_I32 timeout, GMSEC_Status status)
	 *
	 * @brief This function returns the next message received within the specified timeout. The received messages are
	 * determined by the message subscriptions set up with the connectionSubscribe() function.
	 *
	 * @note This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * Example usage:
	 * @code
	 * GMSEC_Message msg = connectionReceive(conn, 1000, status);
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
	 * @param[in]  conn    - the handle to the Connection object
	 * @param[in]  timeout - the the maximum time (in milliseconds) to block waiting for a message; ; if -1
	 * is specified for the timeout, then the call will block indefinitely.
	 * @param[out] status  - out parameter operation result status
	 *
	 * @return A handle to a Message object, or NULL if a timeout or an error occurs.  Check the status to
	 * determine if an error has occurred (note, a timeout is not considered an error).
	 *
	 * @sa connectionSubscribe
	 * @sa connectionSubscribeWithConfig
	 * @sa connectionRelease
	 */
	GMSEC_API GMSEC_Message connectionReceive(GMSEC_Connection conn, GMSEC_I32 timeout, GMSEC_Status status);


	/**
	 * @fn void connectionRelease(GMSEC_Connection conn, GMSEC_Message* msg, GMSEC_Status status)
	 *
	 * @brief This function will destroy a Message object that has been returned by connectionReceive() or connectionRequest().
	 * This method should NOT be used to destroy any other Message object; for such messages, use messageDestroy().
	 *
	 * @param[in]     conn   - the handle to the Connection object
	 * @param[in,out] msg    - the handle to the message that is to be released/destroyed
	 * @param[out]    status - out parameter operation result status
	 *
	 * @sa connectionReceive
	 * @sa connectionRequest
	 */
	GMSEC_API void connectionRelease(GMSEC_Connection conn, GMSEC_Message* msg, GMSEC_Status status);


	/**
	 * @fn void connectionExcludeSubject(GMSEC_Connection conn, const char* subject, GMSEC_Status status)
	 *
	 * @brief Exclude any incoming messages with the specified subject.
	 *
	 * @param[in]  conn    - the handle to the Connection object
	 * @param[in]  subject - the subject pattern to look for in incoming messages
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionExcludeSubject(GMSEC_Connection conn, const char* subject, GMSEC_Status status);


	/**
	 * @fn void connectionRemoveExcludedSubject(GMSEC_Connection conn, const char* subject, GMSEC_Status status)
	 *
	 * @brief Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.
	 *
	 * @param[in]  conn    - the handle to the Connection object
	 * @param[in]  subject - the subject pattern to remove
	 * @param[out] status  - out parameter operation result status
	 */
	GMSEC_API void connectionRemoveExcludedSubject(GMSEC_Connection conn, const char* subject, GMSEC_Status status);


	/**
	 * @fn const char* connectionGetName(const GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief Returns the name of the connection, automatically generated or user specified.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[out] status - out parameter operation result status
	 *
	 * @return The connection name.
	 */
	GMSEC_API const char* connectionGetName(const GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn void connectionSetName(GMSEC_Connection conn, const char* name, GMSEC_Status status)
	 *
	 * @brief Set the logical name of this connection. This can be used for Identifying connections withing a client
	 * program. If a name is not given, one will be automatically generated.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[in]  name   - the name to assign to this connection object
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void connectionSetName(GMSEC_Connection conn, const char* name, GMSEC_Status status);


	/**
	 * @fn const char* connectionGetID(const GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief Get the string ID for this connection.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[out] status - out parameter operation result status
	 *
	 * @return The connection ID.
	 */
	GMSEC_API const char* connectionGetID(const GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn const char* connectionGetMWInfo(const GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief Returns a string containing middleware information.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[out] status - out parameter operation result status
	 *
	 * @return Middleware information.
	 */
	GMSEC_API const char* connectionGetMWInfo(const GMSEC_Connection conn, GMSEC_Status status);


    /**
     * @fn const char* connectionGetConnectionEndpoint(const GMSEC_Connection conn, GMSEC_Status status)
     *
     * @desc Returns middleware broker connection information.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[out] status - out parameter operation result status
     */
    GMSEC_API const char* connectionGetConnectionEndpoint(const GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn GMSEC_U64 connectionGetPublishQueueMessageCount(const GMSEC_Connection conn, GMSEC_Status status)
	 *
	 * @brief Retrieves the number of messages queued for asynchronous publish operations.
	 *
	 * @param[in]  conn   - the handle to the Connection object
	 * @param[out] status - out parameter operation result status
	 *
	 * @return The number of messages in the publish queue.
	 */
	GMSEC_API GMSEC_U64 connectionGetPublishQueueMessageCount(const GMSEC_Connection conn, GMSEC_Status status);


	/**
	 * @fn void connectionShutdownAllMiddlewares(void)
	 *
	 * @desc Calls shutdown routines for each middleware that has a shutdown routine registered. 
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 */
	GMSEC_API void connectionShutdownAllMiddlewares(void);


	/**
	 * @fn void connectionShutdownMiddleware(const char* name)
	 *
	 * @brief Calls the shutdown routine for the middleware with the given name.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 *
	 * @param[in] name - the name of the middleware
	 */
	GMSEC_API void connectionShutdownMiddleware(const char* name);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
