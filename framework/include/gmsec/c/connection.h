
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file connection.h
 *
 *  @brief This file contains functions for managing connections.
 *
 *  Example creation and use:
 *  @code
 * GMSEC_CONNECTION_OBJECT conn = NULL;
 *
 * //Create the Connection with a config object
 * gmsec_CreateConnectionForConfig(cfg,&conn,result);
 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
 *	//handle error
 *
 * //Establish the connection
 * gmsec_Connect(conn,result);
 * if( GMSEC_STATUS_NO_ERROR != gmsec_GetStatusClass(result) )
 *	//handle error
 *  @endcode
 *
**/

#ifndef gmsec_c_connection_h
#define gmsec_c_connection_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn gmsec_Connect(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function establishes this connection to the middleware
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_Disconnect()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_Connect(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_Disconnect(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function terminates this connection to the middleware.
	 * It will be automatically called by the destructor
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_Connect()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_Disconnect(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_GetLibraryVersion(GMSEC_CONNECTION_OBJECT conn, const char **libver, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function returns a string containing the version information for
	 * this connection's associated middleware.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] libver - buffer for the library version
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_GetLibraryVersion(GMSEC_CONNECTION_OBJECT conn, const char **libver, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_GetLibraryRootName(GMSEC_CONNECTION_OBJECT conn, const char **libname, GMSEC_STATUS_OBJECT status)
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this connection is associated with. For example,
	 * the root library name for the mb middleware library is "gmsec_mb"
	 * and matches the name of the windows library (gmsec_mb.dll) and the
	 * UNIX library (libgmsec_mb.so).
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] libname - buffer for the library name
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_GetLibraryRootName(GMSEC_CONNECTION_OBJECT conn, const char **libname, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_RegisterErrorCallback(GMSEC_CONNECTION_OBJECT conn, const char *event, GMSEC_ERROR_CALLBACK *cb, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function allows the registration of a callback for a particular
	 * error event. Event names are middleware dependent.
	 *
	 * All connection types implement at least following error events:
	 *
	 * GMSEC_CONNECTION_DISPATCHER_ERROR - Auto-dispatcher error callback that gets called
	 *		whenever an error occurs inside the auto-dispatcher.
	 *
	 * GMSEC_CONNECTION_REQUEST_TIMEOUT - Request w/callback error callback that gets called
	 *		whenever an error occurs while trying to process an request (ex. timeout)
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] conn - Connection handle
	 * @param[in] event - event to cause callback
	 * @param[in] cb - error callback function
	 * @param[out] status - out parameter operation status
	 *
	 * @sa GMSEC_ERROR_CALLBACK
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_RegisterErrorCallback(GMSEC_CONNECTION_OBJECT conn, const char *event, GMSEC_C_ERROR_CALLBACK *cb, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_Subscribe(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function subscribes to a particular subject pattern. This
	 * causes middleware routing of messages that match that subject or pattern
	 * be queued for this process. Messages that are subscribed to without callback
	 * need to be pulled from the queue using GetNextMsg() and are thrown away if
	 * the auto-dispatcher is used.
	 *
	 * Example subscription patterns:
	 * @code
	 * // this will match only messages with this exact subject
	 * gmsec_Subscribe(conn,"gmsec.mission.const.sat.evt.msg",status);
	 *
	 * // this will match messages with any mission
	 * gmsec_Subscribe(conn,"gmsec.*.const.sat.evt.msg",status);
	 *
	 * // this will match messages that have AT LEAST ONE MORE TAG
	 * //	(will not match "gmsec.mission.const.sat")
	 * gmsec_Subscribe(conn,"gmsec.mission.const.sat.>",status);
	 *
	 * // this will match messages that have ZERO OR MORE TAG
	 * //	(will match "gmsec.mission.const.sat")
	 * gmsec_Subscribe(conn,"gmsec.mission.const.sat.+",status);
	 *
	 * // this will match any event message
	 * gmsec_Subscribe(conn,"gmsec.*.*.*.evt.>",status);
	 * @endcode
	 *
	 * @note Although subscription behavior is outlines as above, the actual behavior for a particular
	 * middleware implementation MAY deviate from this behavior slightly.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  subject - subject to subscribe to
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_UnSubscribe()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_Subscribe(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_SubscribeWCallback(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_CALLBACK *cb, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function subscribes to a particular subject pattern and
	 * associates a callback to be called when messages matching the subject
	 * or pattern are received. If all subscriptions are performed using this
	 * function then the auto-dispatcher can be used to asynchronously receive
	 * messages, although if GetNextMsg() is used to pull messages then
	 * DispatchMsg() will need to be called to ensure registered Callbacks
	 * are called.
	 *
	 * <b>see gmsec_Subscribe(GMSEC_CONNECTION_OBJECT conn, char *subject, GMSEC_STATUS_OBJECT status) for an explaination of subscription patterns</b>
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  subject - subject to subscribe to
	 * @param[in]  cb - callback invoked when subject received
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_UnSubscribeWCallback()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_SubscribeWCallback(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_C_CALLBACK *cb, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_UnSubscribe(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_STATUS_OBJECT status)
	 * @brief This function unsubscribes to a particular subject pattern.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  subject - subject unsubscribing from
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_Subscribe()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_UnSubscribe(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_UnSubscribeWCallback(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_CALLBACK *cb, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function unsubscribes a single callback to a particular subject pattern, and
	 * will not unsubscribe the reception of the message. It will prevent a particular callback
	 * from being called by the auto-dispatch or DispatchMsg(), but the message will still be
	 * received for GetNextMsg().
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  subject - subject unsubscribing from
	 * @param[in]  cb - callback invoked when subject received
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_SubscribeWCallback()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_UnSubscribeWCallback(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_CALLBACK *cb, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_CreateMessageDflt(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT *msg, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function creates a message for this particular middleware connection.
	 * The kind & subject are set to defaults dependent upon the particular middleware implementation.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] msg - Message handle created message will be returned in
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_DestroyMessage()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_CreateMessageDflt(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT *msg, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_CreateMessage(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_MSG_KIND msgKind, GMSEC_MESSAGE_OBJECT *msg, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function creates a message for this particular middleware connection.
	 * The subject name for this call MUST be a valid subject name and NOT a pattern.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in] conn - Connection handle
	 * @param[in] subject - subject message will be sent to
	 * @param[in] msgKind - GMSEC message kind
	 * @param[out] msg - Message handle created message will be returned in
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_DestroyMessage()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_CreateMessage(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_MSG_KIND msgKind, GMSEC_MESSAGE_OBJECT *msg, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_CreateMessageWCFG(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_MSG_KIND msgKind, GMSEC_MESSAGE_OBJECT *msg, GMSEC_CONFIG_OBJECT config, GMSEC_STATUS_OBJECT status)
	 *
	 * This function creates a message for this particular middleware connection with passed in configuration items.
	 * The subject name for this call MUST be a valid subject name and NOT a pattern.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  subject - subject message will be sent to
	 * @param[in]  msgKind - GMSEC message kind
	 * @param[out] msg - Message handle created message will be returned in
	 * @param[in]  config - Config object Handle to pass in middleware specific parameters.
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_DestroyMessage()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_CreateMessageWCFG(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_MSG_KIND msgKind, GMSEC_MESSAGE_OBJECT *msg, GMSEC_CONFIG_OBJECT config, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_CloneMessage(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT in, GMSEC_MESSAGE_OBJECT *out, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function copies a message without knowing what type it is. This
	 * function can be used to copy a message's contents.
	 *
	 * @note The 'out' message MUST BE CLEANED UP with DestroyMessage() by the client application.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  in - Message handle of object to be copied
	 * @param[out] out - Message handle of object to be created
	 * @param[out] status - out parameter operation status
	 *
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_CloneMessage(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT in, GMSEC_MESSAGE_OBJECT *out, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_DestroyMessage(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function destroys a message and cleans up associated memory
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  msg - Message handle of object to be cleaned
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_CreateMessageDflt() @n
	 *     gmsec_CreateMessage()	@n
	 *     gmsec_CreateMessageWCFG()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_DestroyMessage(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_Publish(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function will publish a message to the middleware
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  msg - Message handle to send
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_Publish(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_RequestWCallback(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_CALLBACK *cb, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function will send a request asyncronously. The callback will be called for the reply
	 * if it is received within the specified timeout. This function will not block.
	 * The timeout value is expressed in milliseconds.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  request - Message handle of request
	 * @param[in]  timeout - maximum time to wait for reply (in milliseconds)
	 * @param[in]  cb - Callback function pointer
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_RequestWCallback(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_C_CALLBACK *cb, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_RequestWReplyCallback(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function will send a request asyncronously. The callback will be called for the reply
	 * if it is received within the specified timeout. If an error occurs the error callback will be
	 * called. This function will not block. The timeout value is expressed in milliseconds.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  request - Message handle of request
	 * @param[in]  timeout - maximum time to wait for reply (in milliseconds)
	 * @param[in]  cb - Reply Callback function pointer
	 * @param[in]  er - Error Callback function pointer
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_RequestWReplyCallback(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_C_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_Request(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_MESSAGE_OBJECT *reply, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function will send a request, wait for the specified timeout, and return the received reply
	 * This function will block until the reply is received or the timeout is reached.
	 * The timeout value is expressed in milliseconds.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  request - Request Message handle
	 * @param[in]  timeout - maximum time to wait for reply (in milliseconds)
	 * @param[out] reply - pointer to Message handle for reply message
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_Request(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_MESSAGE_OBJECT *reply, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_Reply(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request,GMSEC_MESSAGE_OBJECT reply, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function will send a reply to a given request.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[in]  request - Message handle to request message
	 * @param[in]  reply - Message handle to reply message
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_Reply(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request,GMSEC_MESSAGE_OBJECT reply, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_StartAutoDispatch(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped.
	 * If GetNextMessage() is called while the auto-dispatcher is used, the behavior will be undesireable
	 * and undefined.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_StopAutoDispatch()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_StartAutoDispatch(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_StopAutoDispatch(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] status - out parameter operation status
	 *
	 * @sa gmsec_StartAutoDispatch()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_StopAutoDispatch(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_GetNextMsg(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT *msg, GMSEC_I32 timeout, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function returns the next message received within the specified timeout.
	 * The received messages are determined by the %Subscribe() function(s), but
	 * %DispatchMsg() needs to be called messages received from this function to
	 * ensure all registered callbacks are executed. @n
	 * @n
	 * This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] msg - Message handle for storage
	 * @param[in]  timeout - time to wait
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_GetNextMsg(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT *msg, GMSEC_I32 timeout, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_IsConnected(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function returns 1 if the Connection handle is connected.
	 * The function may return an error status if there were problems extracting the isConnected
	 * value from the Connection handle.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API int gmsec_IsConnected(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status);

	/** @fn gmsec_DispatchMsg(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief This function will cause the any callbacks that are registered with matching
	 * patterns to be called.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] msg - Message handle to be dispatched
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_DispatchMsg(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status);


	/** @fn gmsec_GetLastDispatcherStatus(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
	 *
	 * @brief When running with the auto-dispatcher, it may be necessary to monitor the
	 * status as it runs within a seperate thread.  This method allows access to the
	 * last status error reported by the dispatcher.  Once the status is read, the
	 * status is cleared.
	 *
	 * @note Another way to be notified in the case of a dispatcher error is to register an
	 * error callback with the RegisterErrorCallback() function.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  conn - Connection handle
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_GetLastDispatcherStatus(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_c_connection_h */
