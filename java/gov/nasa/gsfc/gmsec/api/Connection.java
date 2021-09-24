/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Connection.java
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNIConnection;


/**
 * This is the public interface for middleware connections. Each middleware connection
 * implements a class derived from Connection to abstract middleware specific connection details.
 * The application will never access the middleware connection directly, but only through the
 * Connection "interface".
 * <p>
 * The connection class provides services available on an implemented GMSEC connection. An
 * application can create multiple connection objects with different parameters and different
 * middleware.
 * Creating multiple connections to the same middleware is not prevented but may not be supported
 * by all middleware implementations.
 * <p>
 * Connection class methods make use of a mutex by default and are therefore considered
 * thread-safe.
 * <p>
 * Example creation and use:
 * <pre>{@code
 * import gov.nasa.gsfc.gmsec.api.*;
 * import gov.nasa.gsfc.gmsec.api.util.Log;
 * 
 * public class TestConn
 * {
 *     public static void main(String[] args)
 *     {
 *         Config config = new Config(args);
 * 
 *         config.addValue("loglevel", "info");
 * 
 *         try
 *         {
 *             // Create connection
 *             Connection conn = Connection.create(config);
 * 
 *             // Connect to the middleware server
 *             conn.connect();
 * 
 *             // Create a message
 *             Message msg = new Message("GMSEC.FOO.BAR", Message.MessageKind.PUBLISH);
 * 
 *             // Publish the message
 *             conn.publish(msg);
 * 
 *             Log.info("Published Message:\n" + msg.toXML());
 * 
 *             // Disconnect from the middleware server
 *             conn.disconnect();
 *         }
 *         catch (GMSEC_Exception e)
 *         {
 *             Log.error("Exception: " + e.toString());
 *         }
 * 
 *         // Call shutdown routine for registered middleware(s) to
 *         // clean up any middleware-related resources.  Currently
 *         // only ActiveMQ users need to call this.
 *         Connection.shutdownAllMiddlewares();
 *     }
 * }
 * }</pre>
 *
 * @see Config
 * @see Message
*/
public class Connection
{
	private JNIConnection m_jniConnection = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param conn Object to reference for acquiring internal JNIConnection
	 * @return Internal JNIConnection object
	 */
	public static JNIConnection getInternal(Connection conn)
	{
		return (conn == null ? null : conn.m_jniConnection);
	}


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param conn Connection object
	 */
	public static void resetInternal(Connection conn)
	{
		if (conn != null)
		{
			conn.m_jniConnection = null;
		}
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jConn Internal JNIConnection object
	 * @throws GMSEC_Exception Thrown if given JNIConnection object is null.
	 */
	public Connection(JNIConnection jConn) throws GMSEC_Exception
	{
		if (jConn == null)
		{
			throw new GMSEC_Exception(StatusClassification.FACTORY_ERROR, StatusCode.UNINITIALIZED_OBJECT, "JNIConnection is null");
		}

		m_jniConnection = jConn;
	}


	/**
	 * Do not allow instantiation of this class.
	 * User's must use Connection.create().
	 */
	private Connection()
	{
	}


	/**
	 * Possible states for the Connection object.
	 */
	public enum ConnectionState
	{
		/** Not connected to the middleware server. */
		NOT_CONNECTED,

		/** Connected to middleware server. */
		CONNECTED,

		/** Connection to middleware server is broken; reattempting to connect. */
		RECONNECTING
	}


	/**
	 * Possible soft errors that can occur with Connection object.
	 */
	public enum ConnectionEvent
	{
		/** Error occurred while attempting to dispatch message. */
		DISPATCHER_ERROR_EVENT,

		/** A timeout occurred while attempting to receive a reply for a pending request. */
		REQUEST_TIMEOUT_EVENT,

		/** Successfully connected to the middleware server. */
		CONNECTION_SUCCESSFUL_EVENT,

		/** Connection to middleware server has been broken. */
		CONNECTION_BROKEN_EVENT,

		/** An attempt is being made to reconnect to the middleware server. */
		CONNECTION_RECONNECT_EVENT,

		/** An error, possibly fatal, has occurred with the connection to the middleware. */
		CONNECTION_EXCEPTION_EVENT,

		/** SmartSockets Guaranteed Message Delivery (GMD) error. */
		GMD_ERROR_EVENT,

		/** WebSphere MQ Asynchronous Put Status reporting event. */
		WSMQ_ASYNC_STATUS_CHECK_EVENT,

		/** Monitor all events. */
		ALL_EVENTS,

		/** Failure occurred while attempting to asynchronously publish a message. */
		MSG_PUBLISH_FAILURE_EVENT,

		/** Message failed validation. */
		INVALID_MESSAGE_EVENT
	}


	/**
	 * This static method can be used to create a Connection object.
	 *
	 * @param cfg The Config object that specifies the type of Connection object to create.
	 *
	 * @return A new Connection object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 * @throws GMSEC_Exception Thrown if configuration information cannot be used to deduce Connection type.
	 *
	 * @see Connection#destroy(Connection)
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
	 * This static method is used to destroy the Connection object.
	 *
	 * @param conn The Connection object to destroy
	 *
	 * @throws IllegalArgumentException Thrown if the given Connection object is null.
	 *
	 * @see Connection#create(Config)
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
	 * Returns a string that identifies the version of the API.
	 *
	 * @return A String.
	 */
	public static String getAPIVersion()
	{
		return JNIConnection.getAPIVersion();
	}


	/**
	 * This function establishes this connection to the middleware
	 *
	 * @throws GMSEC_Exception Thrown if an anomaly occurs while connecting.
	 */
	public void connect() throws GMSEC_Exception
	{
		m_jniConnection.connect();
	}


	/**
	 * Terminates the connection to the middleware.
	 *
	 * @throws GMSEC_Exception Thrown if an anomaly occurs while disconnecting.
	 */
	public void disconnect() throws GMSEC_Exception
	{
		m_jniConnection.disconnect();
	}


	/**
	 * This function returns the current state of the connection to the middleware.
	 *
	 * @return Enumerated ConnectionState value.
	 */
	public ConnectionState getState()
	{
		return m_jniConnection.getState();
	}


	/**
	 * Returns the root library name and therefore the
	 * connection type that this connection is associated with.
	 *
	 * @return A String.
	 */
	public String getLibraryRootName()
	{
		return m_jniConnection.getLibraryRootName();
	}


	/**
	 * Returns a string containing the version information for
	 * this connection's associated middleware.
	 *
	 * @return A String.
	 */
	public String getLibraryVersion()
	{
		return m_jniConnection.getLibraryVersion();
	}


	/**
	 * This function allows the registration of a callback for a particular
	 * error event.
	 *
	 * @param event Type of event to register.
	 * @param cb Object derived from EventCallback to register for this error event.
	 *
	 * @throws IllegalArgumentException Thrown if the given EventCallback is null.
	 *
	 * @see EventCallback
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
	 * Sets up a subscription to a particular subject or pattern.
	 * <p>
	 * Example subscription patterns:
	 * <pre>{@code
	 * // this will match only messages with this exact subject
	 * SubscriptionInfo info = conn.subscribe("GMSEC.MISSION.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages with any mission
	 * SubscriptionInfo info = conn.subscribe("GMSEC.*.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages that have AT LEAST ONE MORE element (will not match "GMSEC.MISSION.CONST.SAT")
	 * SubscriptionInfo info = conn.subscribe("GMSEC.MISSION.CONST.SAT.>");
	 *
	 * // this will match messages that have ZERO OR MORE elements (will match "GMSEC.MISSION.CONST.SAT")
	 * SubscriptionInfo info = conn.subscribe("GMSEC.MISSION.CONST.SAT.+");
	 *
	 * // this will match any event message
	 * SubscriptionInfo info = conn.subscribe("GMSEC.*.*.*.EVT.>");
	 *}</pre>
	 * <p>
	 * Note:
	 * Although subscription behavior is outlined as above, the actual behavior for a
	 * particular middleware implementation MAY slightly deviate from this behavior.
	 *
	 * @param subject Subject pattern to match received messages.
	 *
	 * @return SubscriptionInfo Handle used to cancel or modify subscription.
	 *
	 * @throws IllegalArgumentException Thrown if the given subject string is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection#unsubscribe(SubscriptionInfo)
	 * @see Connection#receive()
	 * @see Connection#dispatch(Message)
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
	 * Sets up a subscription to a particular subject or pattern and
	 * associates a callback to be called when messages matching the subject
	 * or pattern are received. If all subscriptions are performed using this
	 * function then the auto-dispatcher can be used to asynchronously receive
	 * messages. If receive() is used to pull messages then dispatch()
	 * will need to be called to ensure registered Callbacks are called.
	 * <p>
	 * Example subscription patterns:
	 * <pre>{@code
	 * // this will match any GMSEC-compliant message, and forward these messages to a callback
	 * MyCallback cb;
	 * SubscriptionInfo info = conn.subscribe("GMSEC.>", cb);
	 * }</pre>
	 * <p>
	 * Note:
	 * Although subscription behavior is outlined as above, the actual behavior for a
	 * particular middleware implementation MAY slightly deviate from this behavior.
	 *
	 * @param subject Subject pattern to match received messages.
	 * @param cb Callback to be called when message is received.
	 *
	 * @return SubscriptionInfo handle used to cancel or modify subscription.
	 *
	 * @throws IllegalArgumentException Thrown if the given subject string is null or contains an empty string, or
	 * the given Callback is null.
	 * @throws GMSEC_Exception Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection#unsubscribe(SubscriptionInfo)
	 * @see Connection#receive()
	 * @see Connection#dispatch(Message)
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
	 * Sets up a subscription to a particular subject or pattern and
	 * uses the provided config object to enable or disable special middleware-level
	 * subscription functionalities (eg. ActiveMQ Durable Consumer)
	 * <p>
	 * <b>See {@link Connection#subscribe(String)} for an explanation of subscription patterns</b>
	 *
	 * @param subject Subject pattern to match received messages.
	 * @param config Config object to be used for subscription operation.
	 *
	 * @return SubscriptionInfo handle used to cancel or modify subscription.
	 * Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
	 * to free resource.
	 *
	 * @throws IllegalArgumentException Thrown if the given subject string is null or contains an empty string, or
	 * the given Config object is null.
	 * @throws GMSEC_Exception Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection#unsubscribe(SubscriptionInfo)
	 * @see Connection#receive()
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
	 * Sets up a subscription to a particular subject or pattern and
	 * associates a callback to be called when messages matching the subject
	 * or pattern are received.  The method also uses the provided config object
	 * to enable or disable special middleware-level subscription functionalities
	 * (eg. ActiveMQ Durable Consumer).
	 * <p>
	 * If all subscriptions are performed using this method then the auto-dispatcher
	 * can be used to asynchronously receive messages. If receive() is used to pull
	 * messages then dispatch() will need to be called to ensure registered Callbacks
	 * are called.
	 * <p>
	 * Example subscription patterns:
	 * <pre>{@code
	 * // this will match any GMSEC-compliant message, and forward these messages to a callback
	 * MyCallback cb;
	 * SubscriptionInfo info = conn.subscribe("GMSEC.>", cb);
	 * }</pre>
	 * <p>
	 * Note:
	 * Although subscription behavior is outlined as above, the actual behavior for a
	 * particular middleware implementation MAY slightly deviate from this behavior.
	 *
	 * @param subject Subject pattern to match received messages.
	 * @param config Config object to be used for subscription operation.
	 * @param cb Callback to be called when message is received.
	 *
	 * @return SubscriptionInfo handle used to cancel or modify subscription.
	 *
	 * @throws IllegalArgumentException Thrown if the given subject string is null or contains an empty string, or
	 * the given Callback is null.
	 * @throws GMSEC_Exception Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection#unsubscribe(SubscriptionInfo)
	 * @see Connection#receive()
	 * @see Connection#dispatch(Message)
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
	 * Unsubscribes to a particular subject pattern, and will stop
	 * the reception of messages that match this pattern. It will also remove the
	 * registration of any callbacks with this subject pattern.
	 *
	 * @param info SubscriptionInfo handle from subscription.
	 *
	 * @throws IllegalArgumentException Thrown if the given SubscriptionInfo object is null.
	 * @throws GMSEC_Exception Thrown if the given SubscriptionInfo object originated from a different Connection object.
	 * @throws GMSEC_Exception Thrown if error occurs at the middleware level.
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#receive()
	 * @see Connection#dispatch(Message)
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
	 * Starts a thread that will dispatch messages asynchronously when they are received.
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
	 * Stops the auto dispatch thread.
	 *
	 * @return A value of true is returned if the auto-dispatcher was running and has been stopped; false otherwise.
	 */
	public boolean stopAutoDispatch()
	{
		return m_jniConnection.stopAutoDispatch(true);
	}


	/**
	 * Stop the auto dispatch thread.
	 *
	 * @param waitForCompletion If set to true, this method will block until the auto-dispatch
	 * thread has completed running.
	 *
	 * @return A value of true is returned if the auto-dispatcher was running and has been stopped; false otherwise.
	 */
	public boolean stopAutoDispatch(boolean waitForCompletion)
	{
		return m_jniConnection.stopAutoDispatch(waitForCompletion);
	}


	/**
	 * Publishes a {@link Message} to the middleware bus.
	 * <p>
	 * Note: The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param msg Message to be published
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to publish the message.
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#receive()
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
	 * Publishes the given message to the middleware using the given configuration to enable or disable 
	 * certain middleware-level publish functionalities (e.g. ActiveMQ - Durable Producer).
	 * <p>
	 * Note: The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param msg Message to be published
	 * @param mwConfig Config object for providing middleware configuration options
	 *
	 * @throws IllegalArgumentException Thrown if either the given Message or Config objects are null.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to publish the message.
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#receive()
	 */
	public void publish(Message msg, Config mwConfig) throws IllegalArgumentException, GMSEC_Exception
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}
		if (mwConfig == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniConnection.publish(msg, mwConfig);
	}


	/**
	 * Sends a request Message asynchronously. The callback will be
	 * called for the reply if it is received within the specified timeout. This
	 * function will not block. The timeout value is expressed in milliseconds.
	 * <p>
	 * Note 1: Client applications should maintain a reference to their ReplyCallback object when calling this method.
	 * This will prevent the JVM garbage collector from destroying the callback before it is ever called.
	 * <p>
	 * Note 2: The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request      Request message to be sent.
	 * @param timeout      Maximum time to wait for reply (in milliseconds).
	 * @param cb           Callback to call when reply is received.
	 * @param republish_ms Request message resubmission interval (in milliseconds). If set to a negative
	 *                     value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                     the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                     Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throws IllegalArgumentException Thrown if either the given Message or ReplyCallback objects are null.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to generate an asynchronous request.
	 *
	 * @see Connection#cancelRequest(ReplyCallback)
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
	 * Sends a request Message, waits for the specified timeout, and returns the received reply.
	 * This function will block until the reply is received or the timeout is reached.
	 * The timeout value is expressed in milliseconds.
	 * <p>
	 * Note: The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request      request message to be sent
	 * @param timeout      maximum time to wait for reply (in milliseconds)
	 * @param republish_ms request message resubmission interval (in milliseconds). If set to a negative
	 *                     value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                     the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                     Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @return Reply Message, or null if no reply received in time
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to generate an synchronous request.
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
	 * Cancels a pending request that may be associated with the given ReplyCallback.
	 *
	 * @param cb The ReplyCallback to disassociate from any pending requests.
	 *
	 * @throws IllegalArgumentException Thrown if the given ReplyCallback object is null.
	 *
	 * @see Connection#request(Message, int, ReplyCallback, int)
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
	 * Sends a reply Message in response to a given request.
	 * <p>
	 * Note: The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request The request message that was received, and to which we are responding to.
	 * @param reply The reply message to be sent.
	 *
	 * @throws IllegalArgumentException Thrown if either the given Request or Reply Messages objects are null.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to issue the reply.
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
	 * Dispatches the given Message to any callbacks that are subscribed with matching
	 * message subject patterns.
	 *
	 * @param msg Message to be dispatched
	 *
	 * @throws IllegalArgumentException Thrown if the given Messages object is null.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to dispatch the message.
	 *
	 * @see Connection#subscribe(String, Callback)
	 * @see Connection#receive()
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
	 * Returns the next message received.
	 * The received messages are determined by the message subscriptions set up with the
	 * {@link Connection#subscribe(String)}  function(s).
	 * <p>
	 * Note: This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @return A handle to the next available Message.
	 *
	 * @throws GMSEC_Exception Thrown on error
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#dispatch(Message)
	 */
	public Message receive() throws GMSEC_Exception
	{
		return receive(gmsecAPI.WAIT_FOREVER);
	}


	/**
	 * Returns the next message received within the specified timeout.
	 * The received messages are determined by the message subscriptions set up with the
	 * subscribe() function(s).
	 * <p>
	 * Note: This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @param timeout The maximum time to block waiting for a message, in milliseconds
	 *
	 * @return A handle to the next available Message, or null if a timeout occurs.
	 *
	 * @throws GMSEC_Exception Thrown on error
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#dispatch(Message)
	 */
	public Message receive(int timeout) throws GMSEC_Exception
	{
		return m_jniConnection.receive(timeout);
	}


	/**
	 * Exclude any incoming messages with the specified subject.
	 *
	 * @param subject The subject pattern to look for in incoming messages.
	 *
	 * @throws IllegalArgumentException Thrown if the given Subject string is null or contains an empty string.
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
	 * Remove an excluded subject, allowing incoming messages with the matching
	 * subject to once again be received.
	 *
	 * @param subject The subject pattern to remove.
	 *
	 * @throws IllegalArgumentException Thrown if the given Subject string is null or contains an empty string.
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
	 * Returns the name of the connection, automatically generated or user specified.
	 *
	 * @return A string.
	 */
	public String getName()
	{
		return m_jniConnection.getName();
	}


	/**
	 * Set the logical name of this connection. This can be used for
	 * Identifying connections withing a client program. If a name is not given,
	 * one will be automatically generated.
	 *
	 * @param name Name of this connection
	 *
	 * @throws IllegalArgumentException Thrown if the given name string is null or contains an empty string.
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
	 * Get the string ID for this connection.
	 *
	 * @return The ID string for this connection.
	 */
	public String getID()
	{
		return m_jniConnection.getID();
	}


	/**
	 * Returns a string containing middleware information.
	 *
	 * @return The middleware information string for this connection.
	 */
	public String getMWInfo()
	{
		return m_jniConnection.getMWInfo();
	}


	/**
	 * Returns middleware broker connection information.
	 *
	 * @return The middleware broker connection information.
	 */
	public String getConnectionEndpoint()
	{
		return m_jniConnection.getConnectionEndpoint();
	}


	/**
	 * Retrieves the number of messages queued for asynchronous publish operation.
	 *
	 * @return The number of messages remaining in the queue.
	 */
	public long getPublishQueueMessageCount()
	{
		return m_jniConnection.getPublishQueueMessageCount();
	}


	/**
	 * Calls shutdown routines for each middleware that has a shutdown routine registered.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 */
	public static void shutdownAllMiddlewares()
	{
		JNIConnection.shutdownAllMiddlewares();
	}


	/**
	 * Calls the shutdown routine for the middleware with the given name.
	 *
	 * @param name A string representing the library name of the gmsec wrapper for
	 * the middleware to shutdown; e.g., "gmsec_mb".
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 *
	 * @throws IllegalArgumentException Thrown if the given name string is null or contains an empty string.
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
