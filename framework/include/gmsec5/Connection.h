/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Connection.h
 *
 * @brief Defines the opaque wrapper for middleware connections.
 */

#ifndef GMSEC_API5_CONNECTION_H
#define GMSEC_API5_CONNECTION_H

#include <gmsec5/Message.h>

#include <gmsec5/c/connection.h>

#include <gmsec5/util/List.h>
#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>

#include <stddef.h>   // for size_t


namespace gmsec
{
namespace api5
{
	// Forward declarations(s)
	class Config;
	class Callback;
	class EventCallback;
	class ReplyCallback;
	class Field;
	class MessageFactory;
	class MessageValidator;
	class Specification;
	class SubscriptionInfo;

	namespace internal
	{
	class ConnectionBuddy;
	class InternalConnection;
	}


/**
 * @class Connection
 *
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
 * Connection objects make use of a gmsec::api5::util::AutoMutex by default and are therefore
 * considered thread-safe.
 *
 * Example creation and use:
 * @code
 * #include <gmsec5_cpp.h>
 * 
 * using namespace gmsec::api5;
 * 
 * int main(int argc, char** argv)
 * {
 *     Config config(argc, argv);
 * 
 *     try
 *     {
 *         // Create connection
 *         Connection conn(config);
 * 
 *         // Connect to the middleware server
 *         conn.connect();
 * 
 *         // Create a message
 *         Message msg = conn.getMessageFactory().createMessage("HB");
 * 
 *         // Publish the message
 *         conn.publish(msg);
 *
 *         GMSEC_INFO << "Published Message:\n" << msg.toXML();
 * 
 *         // Disconnect from the middleware server
 *         conn.disconnect();
 *     }
 *     catch (const GmsecException& e)
 *     {
 *         GMSEC_ERROR << "GmsecException: " << e.what();
 *     }
 * 
 *     // Call shutdown routine for registered middleware(s) to
 *     // clean up any middleware-related resources. Currently
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
	 * @enum Event
	 * @brief Possible soft errors that can occur with Connection object.
	 */
	enum class Event
	{
		DISPATCHER_ERROR_EVENT = 0,           ///< Error occurred while attempting to dispatch message.
		REQUEST_TIMEOUT_EVENT = 1,            ///< A timeout occurred while attempting to receive a reply for a pending request.
		CONNECTION_SUCCESSFUL_EVENT = 2,      ///< Successfully connected to the middleware server.
		CONNECTION_BROKEN_EVENT = 3,          ///< Connection to middleware server has been broken
		CONNECTION_RECONNECT_EVENT = 4,       ///< An attempt is being made to reconnect to the middleware server.
		CONNECTION_EXCEPTION_EVENT = 5,       ///< An error, possibly fatal, has occurred with the connection to the middleware.
		WSMQ_ASYNC_STATUS_CHECK_EVENT = 6,    ///< WebSphere MQ Asynchronous Put status check.
		MSG_PUBLISH_FAILURE_EVENT = 7,        ///< Failure occured while attempting to publish aggregated message.
		INVALID_MESSAGE_EVENT = 8,            ///< Message failed validation

		ALL_EVENTS = 99                       ///< Monitor all events.
	};


	/**
	 * @fn Connection(const Config& config)
	 *
	 * @brief Constructor
	 *
	 * @param config - The config object used to construct the connection object
	 *
	 * @throw A GmsecException is thrown if the configuration information cannot be used to deduce a middleware type.
	 * @throw A GmsecException is thrown if any other anomaly occurs.
	 */
	Connection(const Config& config);


	/**
	 * @fn Connection(const Config& config, MessageFactory& factory)
	 *
	 * @brief Constructor that accepts a %MessageFactory object
	 * This constructor is meant to be used by an application that intends on creating multiple
	 * connection objects, where each will use the same MessageFactory object.
	 * This will assist in reducing the memory footprint of the application. Do NOT destroy
	 * the MessageFactory object while the Connection object(s) are in use!
	 *
	 * @param config  - The config object used to construct the connection object
	 * @param factory - The %MessageFactory object to apply to the %Connection object
	 *
	 * @throw A GmsecException is thrown if the configuration information cannot be used to deduce a Connection type.
	 */
	Connection(const Config& config, MessageFactory& factory);


	/**
	 * @fn ~Connection()
	 *
	 * @brief Destructor - Tears down the Connection. If disconnect() had not been called already, it will be invoked.
	 *
	 * @sa Connection::disconnect()
	 */
	~Connection();


	/**
	 * @fn const char* getAPIVersion()
	 *
	 * @brief Identifies the version of the API.
	 *
	 * @return String containing API version information.
	 */
	static const char* CALL_TYPE getAPIVersion();


	/**
	 * @fn void connect()
	 *
	 * @brief Establishes a connection with the GMSEC middleware server.
	 * Once this call successfully returns, the Connection object is ready for message operations.
	 *
	 * @throw A GmsecException is thrown if an anomaly occurs while attempting to connect to the middleware server.
	 */
	void CALL_TYPE connect();


	/**
	 * @fn void disconnect()
	 *
	 * @brief Disconnects and destroys the connection to the middleware server.
	 */
	void CALL_TYPE disconnect();


	/**
	 * @fn const char* getLibraryName() const
	 *
	 * @brief Identifies the root library name and therefore the connection type that this connection is associated with.
	 *
	 * @return Library name as a string.
	 */
	const char* CALL_TYPE getLibraryName() const;


	/**
	 * @fn const char* getLibraryVersion() const
	 *
	 * @brief This function returns a string containing the version information for this connection's associated middleware.
	 *
	 * @return Library version as a string.
	 */
	const char* CALL_TYPE getLibraryVersion() const;


	/**
	 * @fn MessageFactory& getMessageFactory() const
	 *
	 * @brief Returns the MessageFactory object associated with the Connection.
	 *
	 * @return A MessageFactory object.
	 */
	MessageFactory& CALL_TYPE getMessageFactory() const;


	/**
	 * @fn void registerEventCallback(Connection::Event event, EventCallback* cb)
	 *
	 * @brief Uses the given event callback to notify applications of notable events occuring with
	 * the Connection object or the connection to the GMSEC Bus.
	 *
	 * @param event - type of event to register
	 * @param cb - object derived from EventCallback to register for this error event
	 *
	 * @throw A GmsecException is thrown if the callback is NULL.
	 */
	void CALL_TYPE registerEventCallback(Connection::Event event, EventCallback* cb);


	/**
	 * @fn SubscriptionInfo* subscribe(const char* subject, Callback* cb = NULL)
	 *
	 * @brief This method subscribes to the given subject (or pattern).
	 * If a callback is given, then it will be associated with the subscription.
	 * If all subscriptions are setup to use a callback, then the auto-dispatcher
	 * should be used to asynchronously receive messages. An alternative is to use receive() to
	 * receive messages, and then to forward them to callbacks using dispatch().
	 *
	 * @param subject - Subject pattern to match received messages
	 * @param cb      - Callback to be called when message is received (optional)
	 *
	 * @return SubscriptionInfo handle used to cancel a subscription. %Connection object maintains
	 * ownership of SubscriptionInfo; user should not destroy the resource.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the subject is invalid.
	 *
	 * @sa connect()
	 * @sa dispatch() 
	 * @sa startAutoDispatch
	 * @sa unsubscribe() 
	 */
	SubscriptionInfo* CALL_TYPE subscribe(const char *subject, Callback* cb = NULL);


	/**
	 * @fn SubscriptionInfo* subscribe(const char* subject, const Config& config, Callback* cb = NULL)
	 *
	 * @brief This method subscribes to a particular subject or pattern and uses the provided config object
	 * to enable or disable middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).
	 * If a callback is given, then it will be associated with the subscription.
	 * If all subscriptions are setup to use a callback, then the auto-dispatcher
	 * should be used to asynchronously receive messages. An alternative is to use receive() to
	 * receive messages, and then to forward them to callbacks using dispatch().
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config  - Config object for setting up middleware specific subscription operation
	 * @param cb      - Callback to be called when message is received (optional)
	 *
	 * @return SubscriptionInfo handle used to cancel a subscription. %Connection object maintains
	 * ownership of SubscriptionInfo; user should not destroy the resource.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the subject is invalid.
	 * @throw A GmsecException is thrown if a subscription has already been established for the given subject.
	 *
	 * @sa connect()
	 * @sa dispatch() 
	 * @sa startAutoDispatch
	 * @sa unsubscribe() 
	 */
	SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& config, Callback* cb = NULL);


	/**
	 * @fn void unsubscribe(SubscriptionInfo*& info)
	 *
	 * @brief This method unsubscribes to a particular subject or pattern, and will stop the reception
	 * or messages that match this pattern. It will also remove the registration of any callbacks
	 * with this subject pattern.
	 *
	 * @param info - SubscriptionInfo handle from subscription.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if SubscriptionInfo object is null or originated from a different Connection object.
	 * @throw A GmsecException is thrown if error occurs at the middleware level.
	 *
	 * @sa connect()
	 */
	void CALL_TYPE unsubscribe(SubscriptionInfo*& info);


	/**
	 * @fn bool startAutoDispatch()
	 *
	 * @brief This function will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped. If
	 * receive() is called while the auto-dispatcher is used, the behavior will be undesireable and undefined.
	 *
	 * @return A value of true is returned if the auto-dispatcher has been started; false otherwise.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 *
	 * @sa connect()
	 * @sa stopAutoDispatch()
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
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 *
	 * @sa connect()
	 * @sa startAutoDispatch()
	 */
	bool CALL_TYPE stopAutoDispatch(bool waitForCompletion = true);


	/**
	 * @fn void publish(const Message& msg)
	 *
	 * @brief Publishes the given message to the middleware bus.
	 *
	 * @note The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param msg - The Message to be published.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the Message is not a PUBLISH-kind message.
	 * @throw A GmsecException is thrown if middleware error occurs while attempting to publish the message.
	 *
	 * @sa connect()
	 * @sa subscribe()
	 * @sa receive()
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
	 * @param msg      - The Message to be published.
	 * @param mwConfig - The Config object for providing middleware configuration options.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the Message is not a PUBLISH-kind message.
	 * @throw A GmsecException is thrown if middleware error occurs while attempting to publish the message.
	 *
	 * @sa connect()
	 * @sa subscribe()
	 * @sa receive()
	 */
	void CALL_TYPE publish(const Message& msg, const Config& mwConfig);


	/**
	 * @fn void request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 *
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
	 * @param cb - Reply Callback to call when reply is received
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative
	 * value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 * the period will default to 60000ms, unless the user has provided an alternate time period via the
	 * Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the Message is not a REQUEST-kind message.
	 * @throw A GmsecException is thrown if ReplyCallback is NULL.
	 * @throw A GmsecException is thrown if the request/response functionality is disabled.
	 * @throw A GmsecException is thrown on error with generating async request.
	 *
	 * @sa acknowledge()
	 */
	void CALL_TYPE request(const Message& request, GMSEC_I32 timeout, ReplyCallback* cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn Message* request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
	 *
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
	 * @return A pointer to the Reply Message, or NULL if a timeout occurs. Received
	 * message should be destroyed using Message::destroy().
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the %Message is not a REQUEST-kind message.
	 * @throw A GmsecException is thrown if the request/response functionality is disabled.
	 * @throw A GmsecException is thrown on error transmitting request message
	 *
	 * @sa acknowledge()
	 * @sa Message::destroy()
	 */
	Message* CALL_TYPE request(const Message& request, GMSEC_I32 timeout, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);


	/**
	 * @fn void reply(const Message& request, const Message& reply)
	 *
	 * @brief Sends a reply message in response to the given request.
	 *
	 * @note The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param response - The request message that was received, and to which a response is being sent
	 * @param reply    - The reply message to be sent
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the %Message objects have an invalid kind.
	 * @throw A GmsecException is thrown on error transmitting message.
	 */
	void CALL_TYPE reply(const Message& request, const Message& reply);


	/**
	 * @fn void dispatch(const Message& msg)
	 *
	 * @brief Message is conveyed to all callbacks that are registered with matching
	 * message subject patterns.
	 *
	 * @param msg - The %Message to be dispatched.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 *
	 * @sa subscribe()
	 * @sa receive()
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
	 * @return A pointer to the next available Message, or NULL if a timeout occurs. Received
	 * message should be destroyed using Message::destroy().
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the %Connection's auto-dispatcher has been enabled.
	 *
	 * @sa subscribe()
	 * @sa dispatch()
	 * @sa Message::destroy()
	 */
	Message* CALL_TYPE receive(GMSEC_I32 timeout = GMSEC_WAIT_FOREVER);


	/**
	 * @fn excludeSubject(const char* subject)
	 *
	 * @brief Exclude any incoming messages with the specified subject.
	 *
	 * @param subject - The subject pattern to look for in incoming messages.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the subject is invalid.
	 *
	 * @sa subscribe()
	 * @sa removeExcludedSubject()
	 */
	void CALL_TYPE excludeSubject(const char *subject);


	/**
	 * @fn removeExcludedSubject(const char* subject)
	 *
	 * @brief Remove an excluded subject, allowing incoming messages with the matching
	 * subject to once again be received.
	 *
	 * @param subject - The subject pattern to remove.
	 *
	 * @throw A GmsecException is thrown if a connection to the middleware server has not been established.
	 * @throw A GmsecException is thrown if the subject is invalid.
	 *
	 * @sa subscribe()
	 * @sa excludeSubject()
	 */
	void CALL_TYPE removeExcludedSubject(const char *subject);


	/**
	 * @fn const char* getName() const
	 *
	 * @brief Returns the name of the connection, automatically generated or user specified.
	 *
	 * @return A string with the %Connection object's name.
	 */
	const char* CALL_TYPE getName() const;


	/**
	 * @fn void setName(const char* name)
	 *
	 * @brief Set the logical name of this connection. This can be used for
	 * identifying connections within a client program. If a name is not given,
	 * one will be automatically generated.
	 *
	 * @param name - name of this connection
	 */
	void CALL_TYPE setName(const char* name);


	/**
	 * @fn const char* getID() const
	 *
	 * @brief Get the string ID for this connection.
	 */
	const char* CALL_TYPE getID() const;


	/**
	 * @fn const char* getMWInfo() const
	 *
	 * @brief Returns a string containing middleware information.
	 */
	const char* CALL_TYPE getMWInfo() const;


	/**
	 * @fn const char* getConnectionEndpoint() const
	 *
	 * @brief Returns middleware broker connection information.
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
	 * @brief Calls shutdown routines for each middleware that has a shutdown routine registered.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 */
	static void CALL_TYPE shutdownAllMiddlewares();


	/**
	 * @fn void shutdownMiddleware(const char* name)
	 * @brief Calls the shutdown routine for the middleware with the given name.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 * @param name A string representing the library name of the gmsec wrapper for
	 * the middleware to shutdown; e.g., "gmsec_mb".
	 */
	static void CALL_TYPE shutdownMiddleware(const char* name);


	/* @cond For C API support ONLY! */
	void CALL_TYPE registerEventCallback(Event event, GMSEC_EventCallback* ecb);
	/* @endcond */

	/* @cond For C API support ONLY! */
	SubscriptionInfo* CALL_TYPE subscribe(const char* subject, const Config& cfg, GMSEC_Callback* cb);
	/* @endcond */

	/* @cond For C API support ONLY! */
	void CALL_TYPE request(const Message& req, GMSEC_I32 timeout, GMSEC_ReplyCallback* rcb, GMSEC_EventCallback* ecb, GMSEC_I32 republish_ms);
	/* @endcond */


private:
	// Defined, but not implemented
	Connection();
	Connection(const Connection&);
	Connection& operator=(const Connection&);

	friend class gmsec::api5::internal::ConnectionBuddy;

	gmsec::api5::util::StdUniquePtr<gmsec::api5::internal::InternalConnection> m_internal;
};

}  //namespace api5
}  //namespace gmsec

#endif
