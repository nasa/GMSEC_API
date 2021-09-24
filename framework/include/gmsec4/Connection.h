/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Connection.h
 *
 *  @brief This file contains the opaque wrapper for middleware connections.
 *
 *
**/

#ifndef GMSEC_API_CONNECTION_H
#define GMSEC_API_CONNECTION_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Callback;
class Config;
class EventCallback;
class Message;
class ReplyCallback;
class SubscriptionInfo;

namespace internal
{
	class ConnectionBuddy;
	class ConnectionFactory;
	class InternalConnection;
}


/**
 * @class Connection
 * @brief This is the public interface for middleware connections. Each middleware connection
 * implements a class derived from Connection to abstract middleware specific connection details.
 * The application will never access the middleware connection directly, but only through the
 * Connection "interface".
 *
 * The connection class provides services available on an implemented GMSEC connection. An
 * application can create multiple connection objects with different parameters and different
 * middleware.
 * Creating multiple connections to the same middleware is not prevented but may not be supported
 * by all middleware implementations.
 *
 * Connection objects make use of a gmsec::util::AutoMutex by default and are therefore considered
 * thread-safe.
 *
 * Example creation and use:
 * @code
 * #include <gmsec4_cpp.h>
 * 
 * using namespace gmsec::api;
 * 
 * int main(int argc, char** argv)
 * {
 *     Config      config(argc, argv);
 *     Connection* conn = NULL;
 * 
 *     config.addValue("loglevel", "info");
 * 
 *     try
 *     {
 *         // Create connection
 *         conn = Connection::create(config);
 * 
 *         // Connect to the middleware server
 *         conn->connect();
 * 
 *         // Create a message
 *         Message msg("GMSEC.FOO.BAR", Message::PUBLISH);
 * 
 *         // Publish the message
 *         conn->publish(msg);
 *
 *         GMSEC_INFO << "Published Message:\n" << msg.toXML();
 * 
 *         // Disconnect from the middleware server
 *         conn->disconnect();
 *     }
 *     catch (const Exception& e)
 *     {
 *         GMSEC_ERROR << "Exception: " << e.what();
 *     }
 * 
 *     if (conn != NULL)
 *     {
 *         // Destroy the connection 
 *         Connection::destroy(conn);
 *     }
 * 
 *     // Call shutdown routine for registered middleware(s) to
 *     // clean up any middleware-related resources.  Currently
 *     // only ActiveMQ users need to call this.
 *     Connection::shutdownAllMiddlewares();
 * }
 * @endcode
 *
 * @sa Config
 */

class GMSEC_API Connection
{
public:
	/**
	 * @enum ConnectionState
	 * @brief Possible states for the Connection object.
	 */
	enum ConnectionState
	{
		NOT_CONNECTED = 0,      ///< Not connected to middleware server.
		CONNECTED = 1,          ///< Connected to middleware server.
		RECONNECTING = 2        ///< Connection to middleware server is broken; reattempting to connect.
	};

	/**
	 * @enum ConnectionEvent
	 * @brief Possible soft errors that can occur with Connection object.
	 */
	enum ConnectionEvent
	{
		DISPATCHER_ERROR_EVENT = 0,           ///< Error occurred while attempting to dispatch message.
		REQUEST_TIMEOUT_EVENT = 1,            ///< A timeout occurred while attempting to receive a reply for a pending request.
		CONNECTION_SUCCESSFUL_EVENT = 2,      ///< Successfully connected to the middleware server.
		CONNECTION_BROKEN_EVENT = 3,          ///< Connection to middleware server has been broken
		CONNECTION_RECONNECT_EVENT = 4,       ///< An attempt is being made to reconnect to the middleware server.
		CONNECTION_EXCEPTION_EVENT = 5,       ///< An error, possibly fatal, has occurred with the connection to the middleware.
		GMD_ERROR_EVENT = 6,                  ///< SmartSockets Guaranteed Message Delivery (GMD) error.
		WSMQ_ASYNC_STATUS_CHECK_EVENT = 7,    ///< WebSphere MQ Asynchronous Put status check.
		ALL_EVENTS = 8,                       ///< Monitor all events.
		MSG_PUBLISH_FAILURE_EVENT = 9,        ///< Failure occured while attempting to publish aggregated message.
		INVALID_MESSAGE_EVENT = 10            ///< Message failed validation
	};


	/**
	 * @fn ~Connection()
	 * @brief Class destructor.
	 */
	~Connection();


	/**
	 * @fn const char* getAPIVersion()
	 * @brief This function identifies the version of the API.
	 * @return String containing API version information.
	 */
	static const char* CALL_TYPE getAPIVersion();


	/**
	 * @fn Connection* create(const Config& cfg)
	 * @brief This static method can be used to create a Connection object.
	 *
	 * @param cfg - the Config object that specifies the type of Connection object to create.
	 *
	 * @return Pointer to Connection object.
	 *
	 * @throw Exception if configuration information cannot be used to deduce Connection type.
	 *
	 * @sa destroy()
	 */
	static Connection* CALL_TYPE create(const Config& cfg);


	/**
	 * @fn void destroy(Connection*& conn)
	 * @brief This static method is used to destroy the Connection object.
	 *
	 * @param conn - the Connection object to destroy
	 *
	 * @throw Exception if connection object is NULL.
	 *
	 * @sa create()
	 */
	static void CALL_TYPE destroy(Connection*& conn);


	/**
	 * @fn void connect()
	 * @brief This function establishes this connection to the middleware
	 *
	 * @throws Exception if an anomaly occurs while connecting
	 */
	void CALL_TYPE connect();


	/**
	 * @fn void disconnect()
	 * @desc This function terminates this connection to the middleware.
	 * It is automatically called by the destructor if necessary
	 *
	 * @throws Exception if an anomaly occurs while disconnecting
	 */
	void CALL_TYPE disconnect(); 


	/**
	 * @fn ConnectionState getState() const
	 * @brief This function returns the current state of the connection to the middleware.
	 * @return Enumerated ConnectionState value.
	 */
	ConnectionState CALL_TYPE getState() const;


	/**
	 * @fn const char* getLibraryRootName() const
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this connection is associated with.
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
	 * @fn void registerEventCallback(ConnectionEvent event, EventCallback* cb)
	 * @brief This function allows the registration of a callback for a particular
	 * error event.
	 *
	 * @param event - type of event to register
	 * @param cb - object derived from EventCallback to register for this error event
	 * @throw Exception if the EventCallback is NULL.
	 *
	 * @sa EventCallback
	 */
	void CALL_TYPE registerEventCallback(ConnectionEvent event, EventCallback* cb);


	/**
	 * @fn SubscriptionInfo* subscribe(const char* subject, Callback* cb = NULL)
	 *
	 * @brief This function subscribes to a particular subject or pattern and
	 * associates a callback to be called when messages matching the subject
	 * or pattern are received. If all subscriptions are performed using this
	 * function then the auto-dispatcher can be used to asynchronously receive
	 * messages. If receive() is used to pull messages then dispatch()
	 * will need to be called to ensure registered Callbacks are called.
	 *
	 * Example subscription patterns:
	 * @code
	 * // this will match only messages with this exact subject @n
	 * SubscriptionInfo* info = conn->subscribe("GMSEC.MISSION.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages with any mission @n
	 * SubscriptionInfo* info = conn->subscribe("GMSEC.*.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages that have AT LEAST ONE MORE TAG @n
	 * //     (will not match "GMSEC.MISSION.CONST.SAT") @n
	 * SubscriptionInfo* info = conn->subscribe("GMSEC.MISSION.CONST.SAT.>");
	 *
	 * // this will match messages that have ZERO OR MORE TAG @n
	 * //     (will match "GMSEC.MISSION.CONST.SAT") @n
	 * SubscriptionInfo* info = conn->subscribe("GMSEC.MISSION.CONST.SAT.+");
	 *
	 * // this will match any event message @n
	 * SubscriptionInfo* info = conn->subscribe("GMSEC.*.*.*.EVT.>");
	 *
	 * // this will match any GMSEC-compliant message, and forward these messages to a callback @n
	 * MyCallback cb; @n
	 * SubscriptionInfo* info = conn->subscribe("GMSEC.>", &cb);
	 * @endcode
	 *
	 * @note
	 * Although subscription behavior is outlined as above, the actual behavior for a
	 * particular middleware implementation MAY slightly deviate from this behavior.
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 *
	 * @return SubscriptionInfo handle used to cancel or modify subscription.
	 * Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
	 * to free resource.
	 *
	 * @throw Exception on error establishing subscription
	 *
	 * @sa unsubscribe() @n
	 *     receive() @n
	 *     dispatch() @n
	 *     startAutoDispatch() @n
	 *     stopAutoDispatch()
	 */
	SubscriptionInfo* CALL_TYPE subscribe(const char* subject, Callback* cb = NULL);


	/**
	 * @fn SubscriptionInfo* subscribe(const char* subject, const Config& config, Callback* cb = NULL)
	 *
	 * @brief This function subscribes to a particular subject or pattern and uses the provided config
	 * object to enable or disable middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).
	 * If a callback is specified, then the auto-dispatcher can be used to deliver
	 * messages to the callback. If receive() is used to pull messages, then dispatch()
	 * will need to be called to ensure registered Callbacks are called.
	 *
	 * <b>See Connection::subscribe() for an explanation of subscription patterns</b>
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config  - config object to be used for subscription operation
	 * @param cb      - callback to be called when message is received
	 *
	 * @return SubscriptionInfo handle used to cancel or modify subscription.
	 * Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
	 * to free resource.
	 *
	 * @throw Exception on error establishing subscription
	 *
	 * @sa unsubscribe() @n
	 *     receive()
	 */
	SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& config, Callback* cb = NULL);


	/**
	 * @fn void unsubscribe(SubscriptionInfo*& info)
	 *
	 * @brief This function unsubscribes to a particular subject pattern, and will stop
	 * the reception of messages that match this pattern. It will also remove the
	 * registration of any callbacks with this subject pattern.
	 *
	 * @param info - SubscriptionInfo handle from subscription.
	 *
	 * @throw Exception if SubscriptionInfo object is null or originated from a different Connection object.
	 * @throw Exception if error occurs at the middleware level.
	 *
	 * @sa subscribe() @n
	 *     receive() @n
	 *     dispatch() @n
	 *     startAutoDispatch() @n
	 *     stopAutoDispatch()
	 */
	void CALL_TYPE unsubscribe(SubscriptionInfo*& info);


	/**
	 * @fn bool startAutoDispatch()
	 *
	 * @desc This function will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped. If
	 * receive() is called while the auto-dispatcher is used, the behavior will be undesireable and undefined.
	 *
	 * @return A value of true is returned if the auto-dispatcher has been started; false otherwise.
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
	 * @sa Connection::startAutoDispatch()
	 */
	bool CALL_TYPE stopAutoDispatch(bool waitForCompletion = true);


	/**
	 * @fn void publish(const Message& msg)
	 * @brief This function will publish a message to the middleware.
	 *
	 * @note The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param msg - message to be published
	 *
	 * @throw Exception if error occurs while attempting to publish the message
	 *
	 * @sa subscribe() @n
	 *     receive() @n
	 *     startAutoDispatch() @n
	 *     stopAutoDispatch()
	 */
	void CALL_TYPE publish(const Message& msg); 


	/**
	 * @fn void publish(const Message& msg, const Config& mwConfig)
	 *
	 * @brief Publishes the given message to the middleware
	 * using the given configuration to enable or disable certain middleware-level
	 * publish functionalities (e.g. ActiveMQ - Durable Producer).
	 *
	 * @note The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param msg - message to be published
	 * @param mwConfig - config object for providing middleware configuration options
	 *
	 * @throw Exception if error occurs while attempting to publish the message
	 *
	 * @sa subscribe() @n
	 *     receive() @n
	 *     startAutoDispatch() @n
	 *     stopAutoDispatch()
	 */
	void CALL_TYPE publish(const Message& msg, const Config& mwConfig);


	/**
	 * @fn void request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 * @brief This function will send a request asyncronously. The callback will be
	 * called for the reply if it is received within the specified timeout. This
	 * function will not block. The timeout value is expressed in milliseconds.
	 *
	 * @note The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request - request message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param cb - Callback to call when reply is received
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throw Exception on error with generating async request, or if ReplyCallback is NULL.
	 *
	 * @sa cancelRequest()
	 */
	void CALL_TYPE request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn Message* request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 * @brief This function will send a request, wait for the specified timeout, and return
	 * the received reply.
	 * This function will block until the reply is received or the timeout is reached.
	 * The timeout value is expressed in milliseconds.
	 *
	 * @note The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request - request message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @return Reply Message, or null if no reply received in time
	 *
	 * @throw Exception on error transmitting request message
	 *
	 * @sa release()
	 */
	Message* CALL_TYPE request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn void cancelRequest(ReplyCallback* cb)
	 *
	 * @brief This function can be used to cancel a pending request that may be associated with
	 * the given ReplyCallback.
	 *
	 * @param cb - The ReplyCallback to disassociate from any pending requests.
	 *
	 * @sa void request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 */
	void CALL_TYPE cancelRequest(ReplyCallback* cb);


	/**
	 * @fn void reply(const Message& request, const Message& reply)
	 * @brief This function will send a reply in response to a given request.
	 *
	 * @note The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request - the request message that was received, and to which we are responding to
	 * @param reply - the reply message to be sent
	 * @throw Exception on error transmitting message
	 */
	void CALL_TYPE reply(const Message& request, const Message& reply);


	/**
	 * @fn void dispatch(const Message& msg)
	 * @brief This function will cause the any callbacks that are registered with matching
	 * message subject patterns to be called.
	 *
	 * @param msg - message to be dispatched
	 * @throw Exception on error
	 *
	 * @sa subscribe() @n
	 *     receive()
	 */
	void CALL_TYPE dispatch(const Message& msg);


	/**
	 * @fn Message* receive(GMSEC_I32 timeout = -1)
	 *
	 * @brief This function returns the next message received within the specified timeout.
	 * The received messages are determined by the message subscriptions set up with the
	 * %subscribe() function(s).
	 * @note This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @param timeout - the maximum time (in milliseconds) to block waiting for a message; if -1
	 * is specified for the timeout, then the call will block indefinitely.
	 *
	 * @return A pointer to the next available Message, or NULL if a timeout occurs.  All received
	 * messages should be destroyed using release().
	 *
	 * @throw Exception on error
	 *
	 * @sa subscribe() @n
	 *     dispatch() @n
	 *     release()
	 */
	Message* CALL_TYPE receive(GMSEC_I32 timeout = -1);


	/**
	 * @fn void release(Message*& msg)
	 *
	 * @brief This function will destroy a Message object that has been returned by receive().  This
	 * method should NOT be used to destroy any other Message object.
	 *
	 * @param msg - message to be released/destroyed
	 * @throw Exception on error
	 *
	 * @sa receive()
	 * @sa request()
	 */
	void CALL_TYPE release(Message*& msg);


	/**
	 * @fn excludeSubject( const char *subject )
	 * @desc Exclude any incoming messages with the specified subject.
	 *
	 * @param subject - The subject pattern to look for in incoming messages.
	 */
	void excludeSubject(const char *subject);


	/**
	 * @fn removeExcludedSubject( const char *subject )
	 * @desc Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.
	 *
	 * @param subject - The subject pattern to remove.
	 */
	void removeExcludedSubject(const char *subject);


	/**
	 * @fn const char* getName() const
	 * @brief Returns the name of the connection, automatically generated or user specified.
	 *
	 * @return A string
	 */
	const char* CALL_TYPE getName() const;


	/**
	 * @fn void setName(const char* name)
	 * @brief Set the logical name of this connection. This can be used for
	 * Identifying connections withing a client program. If a name is not given,
	 * one will be automatically generated.
	 *
	 * @param name - name of this connection
	 */
	void CALL_TYPE setName(const char* name);


	/**
	 * @fn const char* getID() const
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
	 * @fn void shutdownAllMiddlewares()
	 *
	 * @desc Calls shutdown routines for each middleware that has a shutdown routine registered.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 */
	static void shutdownAllMiddlewares();


	/**
	 * @fn void shutdownMiddleware(const char* name)
	 * @brief Calls the shutdown routine for the middleware with the given name.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 * @param name A string representing the library name of the gmsec wrapper for
	 * the middleware to shutdown; e.g., "gmsec_mb".
	 */
	static void shutdownMiddleware(const char* name);


	/* @cond For C API support ONLY! */
	void CALL_TYPE registerEventCallback(ConnectionEvent event, GMSEC_EventCallback* ecb);
	/* @endcond */

	/* @cond For C API support ONLY! */
	SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& cfg, GMSEC_Callback* cb);
	/* @endcond */

	/* @cond For C API support ONLY! */
	void CALL_TYPE request(const Message& req, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms);
	/* @endcond */

	/* @cond For C API support ONLY! */
	void CALL_TYPE cancelRequest(GMSEC_ReplyCallback* rcb);
	/* @endcond */


private:
	/**
	 * @fn Connection(InternalConnection* iConn)
	 * @brief Default constructor.
	 */
	Connection(gmsec::api::internal::InternalConnection* iConn);


	// Defined, but not implemented
	Connection();
	Connection(const Connection&);
	Connection& operator=(const Connection&);


	// Member data
	gmsec::api::internal::InternalConnection* m_internal;

	friend class gmsec::api::internal::ConnectionBuddy;
	friend class gmsec::api::internal::ConnectionFactory;
};

} //namespace api
} //namespace gmsec

#endif
