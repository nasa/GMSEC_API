/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Connection.java
 *
 * @brief Class that provides a public interface for middleware connections.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNIConnection;


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
 * Connection class methods make use of a mutex by default and are therefore considered
 * thread-safe.
 *
 * Example creation and use:
 * @code
 * 
 * //Create config from command line arguments
 * Config cfg = new Config(argc,argv);
 *
 * try {
 *     // Create the Connection
 *     Connection conn = Connection.create(cfg);
 *
 *     // Establish the connection
 *     conn.connect();
 *
 *     ...
 *
 *     // Disconnect from middleware server
 *     conn.disconnect();
 *
 *     // Destroy the Connection object
 *     Connection.destroy(conn);
 * }
 * catch (IllegalArgumentException | GMSEC_Exception e) {
 *     //handle error
 * }
 * @endcode
 *
 * @sa Config
*/
public class Connection
{
	private JNIConnection m_jniConnection = null;


	public static JNIConnection getInternal(Connection conn)
	{
		return (conn == null ? null : conn.m_jniConnection);
	}


	public static void resetInternal(Connection conn)
	{
		if (conn != null)
		{
			conn.m_jniConnection = null;
		}
	}


	public Connection(JNIConnection jConn) throws GMSEC_Exception
	{
		if (jConn == null)
		{
			throw new GMSEC_Exception(StatusClassification.FACTORY_ERROR, StatusCode.UNINITIALIZED_OBJECT, "JNIConnection is null");
		}

		m_jniConnection = jConn;
	}


	// Do not allow instantiation of this class.
	// User's must use create().
	//
	private Connection()
	{
	}


	/**
	 * @enum ConnectionState
	 *
	 * @brief Possible states for the Connection object.
	 */
	public enum ConnectionState
	{
		NOT_CONNECTED,      ///< Not connected to middleware server.
		CONNECTED,          ///< Connected to middleware server.
		RECONNECTING        ///< Connection to middleware server is broken; reattempting to connect.
	}


	/**
	 * @enum ConnectionEvent
	 *
	 * @brief Possible soft errors that can occur with Connection object.
	 */
	public enum ConnectionEvent
	{
		DISPATCHER_ERROR_EVENT,           ///< Error occurred while attempting to dispatch message.
		REQUEST_TIMEOUT_EVENT,            ///< A timeout occurred while attempting to receive a reply for a pending request.
		CONNECTION_SUCCESSFUL_EVENT,      ///< Successfully connected to the middleware server.
		CONNECTION_BROKEN_EVENT,          ///< Connection to middleware server has been broken
		CONNECTION_RECONNECT_EVENT,       ///< An attempt is being made to reconnect to the middleware server.
		CONNECTION_EXCEPTION_EVENT,       ///< An error, possibly fatal, has occurred with the connection to the middleware.
		GMD_ERROR_EVENT,                  ///< SmartSockets Guaranteed Message Delivery (GMD) error.
		WSMQ_ASYNC_STATUS_CHECK_EVENT,    ///< WebSphere MQ Asynchronous Put Status reporting event.
		ALL_EVENTS,                       ///< Monitor all events
		MSG_PUBLISH_FAILURE_EVENT         ///< Failure occurred while attempting to asynchronously publish a message.
	}


	/**
	 * @fn Connection create(Config cfg)
	 *
	 * @brief This static method can be used to create a Connection object.
	 *
	 * @param cfg - the Config object that specifies the type of Connection object to create.
	 *
	 * @return A new Connection object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Config object is null.
	 * @throws A GMSEC_Exception is thrown if configuration information cannot be used to deduce Connection type.
	 *
	 * @sa destroy()
	 */
	public static Connection create(Config cfg) throws IllegalArgumentException, GMSEC_Exception
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		return JNIConnection.create(cfg);
	}


	/**
	 * @fn void destroy(Connection conn)
	 *
	 * @brief This static method is used to destroy the Connection object.
	 *
	 * @param conn - the Connection object to destroy
	 *
	 * @throws An IllegalArgumentException is thrown if the given Connection object is null.
	 *
	 * @sa create()
	 */
	public static void destroy(Connection conn) throws IllegalArgumentException
	{
		if (conn == null)
		{
			throw new IllegalArgumentException("Connection is null");
		}

		JNIConnection.destroy(conn);

		Connection.resetInternal(conn);
	}


	/**
	 * @fn String getAPIVersion()
	 *
	 * @brief This function identifies the version of the API.
	 */
	public static String getAPIVersion()
	{
		return JNIConnection.getAPIVersion();
	}


	/**
	 * @fn void connect()
	 *
	 * @brief This function establishes this connection to the middleware
	 *
	 * @throws GMSEC_Exception on severe connection error
	 */
	public void connect() throws GMSEC_Exception
	{
		m_jniConnection.connect();
	}


	/**
	 * @fn void disconnect()
	 *
	 * @brief This function terminates this connection to the middleware.
	 */
	public void disconnect()
	{
		m_jniConnection.disconnect();
	}


	/**
	 * @fn ConnectionState getState()
	 *
	 * @brief This function returns the current state of the connection to the middleware.
	 *
	 * @return Enumerated ConnectionState value.
	 */
	public ConnectionState getState()
	{
		return m_jniConnection.getState();
	}


	/**
	 * @fn String getLibraryRootName()
	 *
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this connection is associated with.
	 *
	 * @return root library name
	 */
	public String getLibraryRootName()
	{
		return m_jniConnection.getLibraryRootName();
	}


	/**
	 * @fn String getLibraryVersion()
	 *
	 * @brief This function returns a string containing the version information for
	 * this connection's associated middleware.
	 *
	 * @return library version
	 */
	public String getLibraryVersion()
	{
		return m_jniConnection.getLibraryVersion();
	}


	/**
	 * @fn void registerEventCallback(ConnectionEvent event, EventCallback cb)
	 *
	 * @brief This function allows the registration of a callback for a particular
	 * error event.
	 *
	 * @param event - type of event to register
	 * @param cb - object derived from EventCallback to register for this error event
	 *
	 * @throws An IllegalArgumentException is thrown if the given EventCallback is null.
	 *
	 * @sa EventCallback
	 */
	public void registerEventCallback(ConnectionEvent event, EventCallback cb) throws IllegalArgumentException
	{
		if (cb == null)
		{
			throw new IllegalArgumentException("EventCallback is null");
		}

		m_jniConnection.registerEventCallback(event, cb);
	}


	/**
	 * @fn SubscriptionInfo subscribe(String subject)
	 *
	 * @brief This function subscribes to a particular subject or pattern.
	 *
	 * Example subscription patterns:
	 *
	 * // this will match only messages with this exact subject @n
	 * SubscriptionInfo info = conn.subscribe("GMSEC.MISSION.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages with any mission @n
	 * SubscriptionInfo info = conn.subscribe("GMSEC.*.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages that have AT LEAST ONE MORE TAG @n
	 * //     (will not match "GMSEC.MISSION.CONST.SAT") @n
	 * SubscriptionInfo info = conn.subscribe("GMSEC.MISSION.CONST.SAT.>");
	 *
	 * // this will match messages that have ZERO OR MORE TAG @n
	 * //     (will match "GMSEC.MISSION.CONST.SAT") @n
	 * SubscriptionInfo* info = conn->subscribe("GMSEC.MISSION.CONST.SAT.+");
	 *
	 * // this will match any event message @n
	 * SubscriptionInfo info = conn.subscribe("GMSEC.*.*.*.EVT.>");
	 *
	 * @note
	 * Although subscription behavior is outlined as above, the actual behavior for a
	 * particular middleware implementation MAY slightly deviate from this behavior.
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 *
	 * @return SubscriptionInfo - handle used to cancel or modify subscription.
	 *
	 * @throws An IllegalArgumentException is thrown if the given subject string is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if an error occurs with establishing the subscription
	 *
	 * @sa unsubscribe() @n
	 *     receive() @n
	 *     dispatch() @n
	 */
	public SubscriptionInfo subscribe(String subject) throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}

		return m_jniConnection.subscribe(subject);
	}


	/**
	 * @fn SubscriptionInfo subscribe(String subject, Callback cb)
	 *
	 * @brief This function subscribes to a particular subject or pattern and
	 * associates a callback to be called when messages matching the subject
	 * or pattern are received. If all subscriptions are performed using this
	 * function then the auto-dispatcher can be used to asynchronously receive
	 * messages. If getNextMsg() is used to pull messages then dispatchMsg()
	 * will need to be called to ensure registered Callbacks are called.
	 *
	 * Example subscription patterns:
	 *
	 * // this will match any GMSEC-compliant message, and forward these messages to a callback @n
	 * MyCallback cb; @n
	 * SubscriptionInfo info = conn.subscribe("GMSEC.>", cb);
	 *
	 * @note
	 * Although subscription behavior is outlined as above, the actual behavior for a
	 * particular middleware implementation MAY slightly deviate from this behavior.
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 *
	 * @return SubscriptionInfo - handle used to cancel or modify subscription.
	 *
	 * @throws An IllegalArgumentException is thrown if the given subject string is null or contains an empty string, or
	 * the given Callback is null.
	 * @throws A GMSEC_Exception is thrown if an error occurs with establishing the subscription
	 *
	 * @sa unsubscribe() @n
	 *     receive() @n
	 *     dispatch() @n
	 */
	public SubscriptionInfo subscribe(String subject, Callback cb) throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}
		if (cb == null)
		{
			throw new IllegalArgumentException("Callback is null");
		}

		return m_jniConnection.subscribe(subject, cb);
	}


	/**
	 * @fn SubscriptionInfo subscribe(String subject, Config config)
	 *
	 * @brief This function subscribes to a particular subject or pattern and
	 * uses the provided config object to enable or disable special middleware-level
	 * subscription functionalities (eg. ActiveMQ Durable Consumer)
	 *
	 * <b>See subscribe() for an explanation of subscription patterns</b>
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config - config object to be used for subscription operation
	 *
	 * @return SubscriptionInfo - handle used to cancel or modify subscription.
	 * Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
	 * to free resource.
	 *
	 * @throws An IllegalArgumentException is thrown if the given subject string is null or contains an empty string, or
	 * the given Config object is null.
	 * @throws A GMSEC_Exception is thrown if an error occurs with establishing the subscription
	 *
	 * @sa unsubscribe() @n
	 *     receive()
	 */
	public SubscriptionInfo subscribe(String subject, Config config) throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		return m_jniConnection.subscribe(subject, config);
	}


	/**
	 * @fn SubscriptionInfo subscribe(String subject, Config config, Callback cb)
	 *
	 * @brief This function subscribes to a particular subject or pattern and
	 * associates a callback to be called when messages matching the subject
	 * or pattern are received.  The method also uses the provided config object
	 * to enable or disable special middleware-level subscription functionalities
	 * (eg. ActiveMQ Durable Consumer).
	 * If all subscriptions are performed using this method then the auto-dispatcher
	 * can be used to asynchronously receive messages. If receive() is used to pull
	 * messages then dispatchMsg() will need to be called to ensure registered Callbacks
	 * are called.
	 *
	 * Example subscription patterns:
	 *
	 * // this will match any GMSEC-compliant message, and forward these messages to a callback @n
	 * MyCallback cb; @n
	 * SubscriptionInfo info = conn.subscribe("GMSEC.>", cb);
	 *
	 * @note
	 * Although subscription behavior is outlined as above, the actual behavior for a
	 * particular middleware implementation MAY slightly deviate from this behavior.
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config - config object to be used for subscription operation
	 * @param cb - callback to be called when message is received
	 *
	 * @return SubscriptionInfo - handle used to cancel or modify subscription.
	 *
	 * @throws An IllegalArgumentException is thrown if the given subject string is null or contains an empty string, or
	 * the given Callback is null.
	 * @throws A GMSEC_Exception is thrown if an error occurs with establishing the subscription
	 *
	 * @sa unsubscribe() @n
	 *     receive() @n
	 *     dispatch() @n
	 */
	public SubscriptionInfo subscribe(String subject, Config config, Callback cb) throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config is null");
		}
		if (cb == null)
		{
			throw new IllegalArgumentException("Callback is null");
		}

		return m_jniConnection.subscribe(subject, config, cb);
	}


	/**
	 * @fn void unsubscribe(SubscriptionInfo info)
	 *
	 * @brief This function unsubscribes to a particular subject pattern, and will stop
	 * the reception of messages that match this pattern. It will also remove the
	 * registration of any callbacks with this subject pattern.
	 *
	 * @param info - SubscriptionInfo handle from subscription.
	 *
	 * @throws An IllegalArgumentException is thrown if the given SubscriptionInfo object is null.
	 * @throws A GMSEC_Exception is thrown if the given SubscriptionInfo object originated from a different Connection object.
	 *
	 * @sa subscribe() @n
	 *     receive() @n
	 *     dispatch() @n
	 */
	public void unsubscribe(SubscriptionInfo info) throws IllegalArgumentException, GMSEC_Exception
	{
		if (info == null)
		{
			throw new IllegalArgumentException("SubscriptionInfo is null");
		}

		m_jniConnection.unsubscribe(info);
	}


	/**
	 * @fn boolean startAutoDispatch()
	 *
	 * @brief This function will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped. If
	 * receive() is called while the auto-dispatcher is used, the behavior will be undesireable and undefined.
	 *
	 * @return A value of true is returned if the auto-dispatcher has been started; false otherwise.
	 */
	public boolean startAutoDispatch()
	{
		return m_jniConnection.startAutoDispatch();
	}


	/**
	 * @fn boolean stopAutoDispatch()
	 *
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @return A value of true is returned if the auto-dispatcher was running and has been stopped; false otherwise.
	 */
	public boolean stopAutoDispatch()
	{
		return m_jniConnection.stopAutoDispatch(true);
	}


	/**
	 * @fn boolean stopAutoDispatch(boolean waitForCompletion)
	 *
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @param waitForCompletion - if set to true, this method will block until the auto-dispatch
	 * thread has completed running.
	 *
	 * @return A value of true is returned if the auto-dispatcher was running and has been stopped; false otherwise.
	 */
	public boolean stopAutoDispatch(boolean waitForCompletion)
	{
		return m_jniConnection.stopAutoDispatch(waitForCompletion);
	}


	/**
	 * @fn void publish(Message msg)
	 *
	 * @brief This function will publish a message to the middleware.
	 *
	 * @note The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param msg - message to be published
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 * @throws A GMSEC_Exception is thrown if an error occurs while attempting to publish the message.
	 *
	 * @sa subscribe() @n
	 *     receive() @n
	 */
	public void publish(Message msg) throws IllegalArgumentException, GMSEC_Exception
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		m_jniConnection.publish(msg);
	}


	/**
	 * @fn void publish(Message msg, Config config)
	 *
	 * @brief This function will publish a message to the middleware
	 * using the config object provided to toggle between special middleware-level
	 * publish functionalities. (eg. ActiveMQ - Durable Producer)
	 *
	 * @note The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param msg - message to be published
	 * @param config - config object to be used by the publish operation
	 *
	 * @throws An IllegalArgumentException is thrown if either the given Message or Config objects are null.
	 * @throws A GMSEC_Exception is thrown if an error occurs while attempting to publish the message.
	 *
	 * @sa subscribe() @n
	 *     receive() @n
	 */
	public void publish(Message msg, Config config) throws IllegalArgumentException, GMSEC_Exception
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniConnection.publish(msg, config);
	}


	/**
	 * @fn void request(Message request, int timeout, ReplyCallback cb, int republish_ms)
	 *
	 * @brief This function will send a request asyncronously. The callback will be
	 * called for the reply if it is received within the specified timeout. This
	 * function will not block. The timeout value is expressed in milliseconds.
	 *
	 * @note 1) Client applications should maintain a reference to their ReplyCallback object when calling this method.
	 * This will prevent the JVM garbage collector from destroying the callback before it is ever called.
	 *
	 * @note 2) The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request      - request message to be sent
	 * @param timeout      - maximum time to wait for reply (in milliseconds)
	 * @param cb           - Callback to call when reply is received
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set  to a negative value (eg. -1)
	 *                       it will never republish a request message.  If set to 0, the period will default to 60000ms,
	 *                       unless the user has provided an alternate time period via the Config object used to create
	 *                       the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throws An IllegalArgumentException is thrown if either the given Message or ReplyCallback objects are null.
	 * @throws A GMSEC_Exception is thrown if an error occurs while attempting to generate an asynchronous request.
	 *
	 * @sa cancelRequest()
	 */
	public void request(Message request, int timeout, ReplyCallback cb, int republish_ms) throws IllegalArgumentException, GMSEC_Exception
	{
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}
		if (cb == null)
		{
			throw new IllegalArgumentException("ReplyCallback is null");
		}

		m_jniConnection.request(request, timeout, cb, republish_ms);
	}


	/**
	 * @fn Message request(Message request, int timeout, int republish_ms)
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
	 * @param request      - request message to be sent
	 * @param timeout      - maximum time to wait for reply (in milliseconds)
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set  to a negative value (eg. -1)
	 *                       it will never republish a request message.  If set to 0, the period will default to 60000ms,
	 *                       unless the user has provided an alternate time period via the Config object used to create
	 *                       the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @return Reply Message, or null if no reply received in time
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 * @throws A GMSEC_Exception is thrown if an error occurs while attempting to generate an synchronous request.
	 */
	public Message request(Message request, int timeout, int republish_ms) throws IllegalArgumentException, GMSEC_Exception
	{
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}

		return m_jniConnection.request(request, timeout, republish_ms);
	}


	/**
	 * @fn void cancelRequest(ReplyCallback cb)
	 *
	 * @brief This function can be used to cancel a pending request that may be associated with
	 * the given ReplyCallback.
	 *
	 * @param cb - The ReplyCallback to disassociate from any pending requests.
	 *
	 * @throws An IllegalArgumentException is thrown if the given ReplyCallback object is null.
	 *
	 * @sa void request(Message request, int timeout, ReplyCallback cb, int republish_ms)
	 */
	public void cancelRequest(ReplyCallback cb) throws IllegalArgumentException
	{
		if (cb == null)
		{
			throw new IllegalArgumentException("ReplyCallback is null");
		}

		m_jniConnection.cancelRequest(cb);
	}


	/**
	 * @fn void reply(Message request, Message reply)
	 *
	 * @brief This function will send a reply in response to a given request.
	 *
	 * @note The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request - the request message that was received, and to which we are responding to
	 * @param reply - the reply message to be sent
	 *
	 * @throws An IllegalArgumentException is thrown if either the given Request or Reply Messages objects are null.
	 * @throws A GMSEC_Exception is thrown if an error occurs while attempting to issue the reply.
	 */
	public void reply(Message request, Message reply) throws IllegalArgumentException, GMSEC_Exception
	{
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}
		if (reply == null)
		{
			throw new IllegalArgumentException("Reply Message is null");
		}

		m_jniConnection.reply(request, reply);
	}


	/**
	 * @fn void dispatch(Message msg)
	 *
	 * @brief This function will cause the any callbacks that are registered with matching
	 * message subject patterns to be called.
	 *
	 * @param msg - message to be dispatched
	 *
	 * @throws An IllegalArgumentException is thrown if the given Messages object is null.
	 * @throws A GMSEC_Exception is thrown if an error occurs while attempting to dispatch the message.
	 *
	 * @sa subscribe() @n
	 *     receive()
	 */
	public void dispatch(Message msg) throws IllegalArgumentException, GMSEC_Exception
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		m_jniConnection.dispatch(msg);
	}


	/**
	 * @fn Message receive()
	 *
	 * @brief This function returns the next message received.
	 * The received messages are determined by the message subscriptions set up with the
	 * %subscribe() function(s).
	 *
	 * @note This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @return A handle to the next available Message.
	 *
	 * @throws A GMSEC_Exception is thrown on error
	 *
	 * @sa subscribe() @n
	 *     dispatch() @n
	 */
	public Message receive() throws GMSEC_Exception
	{
		return receive(gmsecAPI.WAIT_FOREVER);
	}


	/**
	 * @fn Message receive(int timeout)
	 *
	 * @brief This function returns the next message received within the specified timeout.
	 * The received messages are determined by the message subscriptions set up with the
	 * %subscribe() function(s).
	 *
	 * @note This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @param timeout - the maximum time to block waiting for a message, in milliseconds
	 *
	 * @return A handle to the next available Message, or null if a timeout occurs.
	 *
	 * @throws A GMSEC_Exception is thrown on error
	 *
	 * @sa subscribe() @n
	 *     dispatch() @n
	 */
	public Message receive(int timeout) throws GMSEC_Exception
	{
		return m_jniConnection.receive(timeout);
	}


	/**
	 * @fn void excludeSubject(String subject)
	 *
	 *  @desc Exclude any incoming messages with the specified subject.
	 *
	 *  @param subject - The subject pattern to look for in incoming messages.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Subject string is null or contains an empty string.
	 */
	public void excludeSubject(String subject) throws IllegalArgumentException
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}

		m_jniConnection.excludeSubject(subject);
	}


	/**
	 * @fn void removeExcludedSubject(String subject)
	 *
	 *  @desc Remove an excluded subject, allowing incoming messages with the matching
	 *  subject to once again be received.
	 *
	 *  @param subject - The subject pattern to remove.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Subject string is null or contains an empty string.
	 */
	public void removeExcludedSubject(String subject) throws IllegalArgumentException
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}

		m_jniConnection.removeExcludedSubject(subject);
	}


	/**
	 * @fn String getName()
	 *
	 * @brief Returns the name of the connection, automatically generated or user specified.
	 *
	 * @return A string
	 */
	public String getName()
	{
		return m_jniConnection.getName();
	}


	/**
	 * @fn void setName(String name)
	 *
	 * @brief Set the logical name of this connection. This can be used for
	 * Identifying connections withing a client program. If a name is not given,
	 * one will be automatically generated.
	 *
	 * @param name - name of this connection
	 *
	 * @throws An IllegalArgumentException is thrown if the given name string is null or contains an empty string.
	 */
	public void setName(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Name is null or contains an empty string");
		}

		m_jniConnection.setName(name);
	}


	/**
	 * @fn const char* getID() const
	 * @desc Get the string ID for this connection.
	 */
	public String getID()
	{
		return m_jniConnection.getID();
	}


	/**
	 * @fn const char* getMWInfo() const
	 *  @desc Returns a string containing middleware information.
	 */
	public String getMWInfo()
	{
		return m_jniConnection.getMWInfo();
	}


	/**
	 * @fn long getPublishQueueMessageCount()
	 *
	 * @brief Retrieves the number of messages queued for asynchronous publish operation.
	 *
	 * @returns The number of messages in the publish queue.
	 */
	public long getPublishQueueMessageCount()
	{
		return m_jniConnection.getPublishQueueMessageCount();
	}


	/**
	 * @fn void shutdownAllMiddlewares()
	 *
	 * @brief Calls shutdown routines for each middleware that has a shutdown routine
	 * registered.  If the status returned here is an error, it is not guaranteed that
	 * all shutdown routines were called.
	 */
	public static void shutdownAllMiddlewares()
	{
		JNIConnection.shutdownAllMiddlewares();
	}


	/**
	 * @fn void shutdownMiddleware(String name)
	 *
	 * @brief Calls the shutdown routine for the middleware with the given name.
	 *
	 * @param name A string representing the library name of the gmsec wrapper for
	 * the middleware to shutdown; e.g., "gmsec_mb".
	 *
	 * @throws An IllegalArgumentException is thrown if the given name string is null or contains an empty string.
	 */
	public static void shutdownMiddleware(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Name is null or contains an empty string");
		}

		JNIConnection.shutdownMiddleware(name);
	}
}
