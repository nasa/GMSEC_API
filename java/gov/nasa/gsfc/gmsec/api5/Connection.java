/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Connection.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api5.jni.JNIConnection;

import java.util.Map;
import java.util.HashMap;


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
	private static void resetInternal(Connection conn)
	{
		if (conn != null)
		{
			conn.m_jniConnection = null;
		}
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jConn Internal JNIConnection object
	 * @throws GmsecException Thrown if given JNIConnection object is null.
	 */
	public Connection(JNIConnection jConn)
		throws GmsecException
	{
		if (jConn == null)
		{
			throw new GmsecException(StatusClassification.FACTORY_ERROR, StatusCode.UNINITIALIZED_OBJECT, "JNIConnection is null");
		}
		m_jniConnection = jConn;
	}


	/**
	 * Do not allow instantiation of this class.
	 * Users must use Connection.create().
	 */
	private Connection()
	{
	}


	/**
	 * Possible soft errors that can occur with Connection object.
	 */
	public enum Event
	{
		/** Error occurred while attempting to dispatch message. */
		DISPATCHER_ERROR_EVENT(0),

		/** A timeout occurred while attempting to receive a reply for a pending request. */
		REQUEST_TIMEOUT_EVENT(1),

		/** Successfully connected to the middleware server. */
		CONNECTION_SUCCESSFUL_EVENT(2),

		/** Connection to middleware server has been broken. */
		CONNECTION_BROKEN_EVENT(3),

		/** An attempt is being made to reconnect to the middleware server. */
		CONNECTION_RECONNECT_EVENT(4),

		/** An error, possibly fatal, has occurred with the connection to the middleware. */
		CONNECTION_EXCEPTION_EVENT(5),

		/** WebSphere MQ Asynchronous Put Status reporting event. */
		WSMQ_ASYNC_STATUS_CHECK_EVENT(6),

		/** Failure occurred while attempting to asynchronously publish a message. */
		MSG_PUBLISH_FAILURE_EVENT(7),

		/** Message failed validation. */
		INVALID_MESSAGE_EVENT(8),

		/** Monitor all events. */
		ALL_EVENTS(99);

		private int event;

		private static Map<Integer, Event> map = new HashMap<>();

		private Event(int event) {
			this.event = event;
		}

		static {
			for (Event ce : Event.values()) {
				map.put(ce.event, ce);
			}
		}

		/**
		 * Converts the enumerated Event enumerated type to integer.
		 * @return An integer value representing an Event.
		 */
		public int getValue() {
			return event;
		}

		/**
		 * Converts the integer value into an enumerated Event type.
		 * @param event An integer value representing an Event enumerated type.
		 * @return An enumerated Event, or null.
		 */
		public static Event valueOf(int event) {
			return map.get(event);
		}
	}


	/**
	 * This static method can be used to create a Connection object.
	 *
	 * @param cfg The Config object that specifies the type of Connection object to create.
	 *
	 * @return A new Connection object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 * @throws GmsecException Thrown if configuration information cannot be used to deduce middleware type.
	 *
	 * @see Connection#destroy(Connection)
	 */
	public static Connection create(Config cfg)
		throws IllegalArgumentException, GmsecException
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		return JNIConnection.create(cfg);
	}


	/**
	 * This static method can be used to create a Connection object with a MessageFactory object.
	 * This constructor is meant to be used by an application that intends on creating multiple
	 * connection objects, where each will use the same MessageFactory objects.
	 * This will assist in reducing the memory footprint of the application. Do NOT destroy
	 * the MessageFactory object while the Connection object(s) are in use!
	 *
	 * @param cfg     The Config object that specifies the type of Connection object to create.
	 * @param factory The MessageFactory object to apply to the Connection object.
	 *
	 * @return A new Connection object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 * @throws IllegalArgumentException Thrown if the given MessageFactory object is null.
	 * @throws GmsecException Thrown if configuration information cannot be used to deduce middleware type.
	 *
	 * @see Connection#destroy(Connection)
	 */
	public static Connection create(Config cfg, MessageFactory factory)
		throws IllegalArgumentException, GmsecException
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}
		if (factory == null)
		{
			throw new IllegalArgumentException("MessageFactory is null");
		}

		return JNIConnection.create(cfg, factory);
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
	public static void destroy(Connection conn)
		throws IllegalArgumentException
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
	 * @throws GmsecException Thrown if an anomaly occurs while connecting.
	 */
	public void connect()
		throws GmsecException
	{
		m_jniConnection.connect();
	}


	/**
	 * Terminates the connection to the middleware.
	 */
	public void disconnect()
	{
		m_jniConnection.disconnect();
	}


	/**
	 * Returns the root library name and therefore the
	 * connection type that this connection is associated with.
	 *
	 * @return A String.
	 */
	public String getLibraryName()
	{
		return m_jniConnection.getLibraryName();
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
	 * Returns the Config object that is associated with the Connection.
	 *
	 * Note:
	 * The returned Config object is for reference purposes only. 
	 * Changing the config object will not affect the Connection
	 *
	 * @return A Config object.
	 */
	public Config getConfig()
	{
		return m_jniConnection.getConfig();
	}


	/**
	 * Returns the MessageFactory object that is associated with the Connection.
	 *
	 * @return A MessageFactory object.
	 */
	public MessageFactory getMessageFactory()
	{
		return m_jniConnection.getMessageFactory();
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
	public void registerEventCallback(Event event, EventCallback cb)
		throws IllegalArgumentException
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
	 * @throws GmsecException Thrown if not connected to the GMSEC bus.
	 * @throws GmsecException Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection#unsubscribe(SubscriptionInfo)
	 * @see Connection#receive()
	 * @see Connection#dispatch(Message)
	 */
	public SubscriptionInfo subscribe(String subject)
		throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if not connected to the GMSEC bus.
	 * @throws GmsecException Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection#unsubscribe(SubscriptionInfo)
	 * @see Connection#receive()
	 * @see Connection#dispatch(Message)
	 */
	public SubscriptionInfo subscribe(String subject, Callback cb)
		throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if not connected to the GMSEC bus.
	 * @throws GmsecException Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection#unsubscribe(SubscriptionInfo)
	 * @see Connection#receive()
	 */
	public SubscriptionInfo subscribe(String subject, Config config)
		throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if not connected to the GMSEC bus.
	 * @throws GmsecException Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection#unsubscribe(SubscriptionInfo)
	 * @see Connection#receive()
	 * @see Connection#dispatch(Message)
	 */
	public SubscriptionInfo subscribe(String subject, Config config, Callback cb)
		throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the given SubscriptionInfo object originated from a different Connection object.
	 * @throws GmsecException Thrown if not connected to the GMSEC bus.
	 * @throws GmsecException Thrown if error occurs at the middleware level.
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#receive()
	 * @see Connection#dispatch(Message)
	 */
	public void unsubscribe(SubscriptionInfo info)
		throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if an error occurs while attempting to publish the message.
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#receive()
	 */
	public void publish(Message msg) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if an error occurs while attempting to publish the message.
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#receive()
	 */
	public void publish(Message msg, Config mwConfig) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if an error occurs while attempting to generate an asynchronous request.
	 */
	public void request(Message request, int timeout, ReplyCallback cb, int republish_ms) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if an error occurs while attempting to generate an synchronous request.
	 *
	 * @see Message#destroy(Message)
	 */
	public Message request(Message request, int timeout, int republish_ms) throws IllegalArgumentException, GmsecException
	{
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}

		return m_jniConnection.request(request, timeout, republish_ms);
	}


	/**
	 * Sends a reply Message in response to a given request.
	 * <p>
	 * Note: The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param request The request message that was received, and to which a response is being sent.
	 * @param reply The reply message to be sent.
	 *
	 * @throws IllegalArgumentException Thrown if the given Reply Message objects is null.
	 * @throws GmsecException Thrown if an error occurs while attempting to issue the reply.
	 */
	public void reply(Message request, Message reply) throws IllegalArgumentException, GmsecException
	{
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
	 * @throws GmsecException Thrown if an error occurs while attempting to dispatch the message.
	 *
	 * @see Connection#subscribe(String, Callback)
	 * @see Connection#receive()
	 */
	public void dispatch(Message msg) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown on error
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#dispatch(Message)
	 * @see Message#destroy(Message)
	 */
	public Message receive() throws GmsecException
	{
		return receive(Gmsec.WAIT_FOREVER);
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
	 * @throws GmsecException Thrown on error
	 *
	 * @see Connection#subscribe(String)
	 * @see Connection#dispatch(Message)
	 * @see Message#destroy(Message)
	 */
	public Message receive(int timeout) throws GmsecException
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
	 * Returns a string containing info regarding the connection endpoint.
	 *
	 * @return The connection endpoint information string for this connection.
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
