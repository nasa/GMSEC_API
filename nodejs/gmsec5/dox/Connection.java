/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Connection.java
 */

package gmsec;


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
	}


	/**
	 * Constructor for creating a Connection object.
	 *
	 * @param config The Config object that specifies the type of Connection object to create.
	 *
	 * @return A new Connection object.
	 *
	 * @throws SWIG_Exception Thrown if the given Config object is null.
	 * @throws GmsecException Thrown if configuration information cannot be used to deduce middleware type.
	 */
	public Connection(config);


	/**
	 * Constructor for creating a Connection object with a MessageFactory object.
	 * This constructor is meant to be used by an application that intends on creating multiple
	 * connection objects, where each will use the same MessageFactory objects.
	 * This will assist in reducing the memory footprint of the application. Do NOT destroy
	 * the MessageFactory object while the Connection object(s) are in use!
	 *
	 * @param config  The Config object that specifies the type of Connection object to create.
	 * @param factory The MessageFactory object to apply to the Connection object.
	 *
	 * @return A new Connection object.
	 *
	 * @throws SWIG_Exception Thrown if the given Config object is null.
	 * @throws SWIG_Exception Thrown if the given MessageFactory object is null.
	 * @throws GmsecException Thrown if configuration information cannot be used to deduce middleware type.
	 */
	public Connection(config, factory);


	/**
	 * Returns a string that identifies the version of the API.
	 *
	 * @return A String.
	 */
	public static getAPIVersion();


	/**
	 * This function establishes this connection to the middleware
	 *
	 * @throws GmsecException Thrown if an anomaly occurs while connecting.
	 */
	public connect();


	/**
	 * Terminates the connection to the middleware.
	 */
	public disconnect();


	/**
	 * Returns the root library name and therefore the
	 * connection type that this connection is associated with.
	 *
	 * @return A String.
	 */
	public getLibraryName();


	/**
	 * Returns a string containing the version information for
	 * this connection's associated middleware.
	 *
	 * @return A String.
	 */
	public getLibraryVersion();


	/**
	 * Returns the Config object that is associated with the Connection.
	 *
	 * Note:
	 * The returned Config object is for reference purposes only. 
	 * Changing the config object will not affect the Connection
	 *
	 * @return A Config object.
	 */
	public getConfig();


	/**
	 * Returns the MessageFactory object that is associated with the Connection.
	 *
	 * @return A MessageFactory object.
	 */
	public getMessageFactory();


	/**
	 * Sets up a subscription to a particular subject or pattern.
	 * <p>
	 * Example subscription patterns:
	 * <pre>{@code
	 * // this will match only messages with this exact subject
	 * let info = conn.setupSubscription("GMSEC.MISSION.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages with any mission
	 * let info = conn.setupSubscription("GMSEC.*.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages that have AT LEAST ONE MORE element (will not match "GMSEC.MISSION.CONST.SAT")
	 * let info = conn.setupSubscription("GMSEC.MISSION.CONST.SAT.>");
	 *
	 * // this will match messages that have ZERO OR MORE elements (will match "GMSEC.MISSION.CONST.SAT")
	 * let info = conn.setupSubscription("GMSEC.MISSION.CONST.SAT.+");
	 *
	 * // this will match any event message
	 * let info = conn.setupSubscription("GMSEC.*.*.*.EVT.>");
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
	 * @throws SWIG_Exception Thrown if the given subject string is null.
	 * @throws GmsecException Thrown if not connected to the GMSEC bus.
	 * @throws GmsecException Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection.unsubscribe()
	 * @see Connection.receive()
	 */
	public setupSubscription(subject);


	/**
	 * Sets up a subscription to a particular subject or pattern and
	 * uses the provided config object to enable or disable special middleware-level
	 * subscription functionalities (eg. ActiveMQ Durable Consumer)
	 * <p>
	 * <b>See {@link Connection.setupSubscription(String)} for an explanation of subscription patterns</b>
	 *
	 * @param subject  Subject pattern to match received messages.
	 * @param mwConfig Config object for providing middleware configuration options
	 *
	 * @return SubscriptionInfo handle used to cancel or modify subscription.
	 * Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe()
	 * to free resource.
	 *
	 * @throws SWIG_Exception Thrown if the given subject string is null.
	 * @throws SWIG_Exception Thrown if the given Config object is null.
	 * @throws GmsecException Thrown if not connected to the GMSEC bus.
	 * @throws GmsecException Thrown if an error occurs with establishing the subscription.
	 *
	 * @see Connection.unsubscribe()
	 * @see Connection.receive()
	 */
	public setupSubscriptionWithConfig(subject, mwConfig);


	/**
	 * Unsubscribes to a particular subject pattern, and will stop
	 * the reception of messages that match this pattern.
	 *
	 * @param info SubscriptionInfo handle from subscription.
	 *
	 * @throws SWIG_Exception Thrown if the given SubscriptionInfo object is null.
	 * @throws GmsecException Thrown if the given SubscriptionInfo object originated from a different Connection object.
	 * @throws GmsecException Thrown if not connected to the GMSEC bus.
	 * @throws GmsecException Thrown if error occurs at the middleware level.
	 *
	 * @see Connection.setupSubscription()
	 * @see Connection.receive()
	 */
	public unsubscribe(info);


	/**
	 * Publishes a {@link Message} to the middleware bus.
	 * <p>
	 * Note: The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param message Message to be published
	 *
	 * @throws SWIG_Exception Thrown if the given Message object is null.
	 * @throws GmsecException Thrown if an error occurs while attempting to publish the message.
	 *
	 * @see Connection.setupSubscription()
	 * @see Connection.receive()
	 */
	public publish(message);


	/**
	 * Publishes the given message to the middleware using the given configuration to enable or disable 
	 * certain middleware-level publish functionalities (e.g. ActiveMQ - Durable Producer).
	 * <p>
	 * Note: The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param message  Message to be published
	 * @param mwConfig Config object for providing middleware configuration options
	 *
	 * @throws SWIG_Exception Thrown if either the given Message or Config objects are null.
	 * @throws GmsecException Thrown if an error occurs while attempting to publish the message.
	 *
	 * @see Connection.setupSubscription()
	 * @see Connection.receive()
	 */
	public publishWithConfig(message, mwConfig);


	/**
	 * Sends a request Message, waits for the specified timeout, and returns the received reply.
	 * This function will block until the reply is received or the timeout is reached.
	 * The timeout value is expressed in milliseconds.
	 * <p>
	 * Note: The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param reqMessage   request message to be sent
	 * @param timeout      maximum time to wait for reply (in milliseconds)
	 * @param republish_ms request message resubmission interval (in milliseconds). If set to a negative
	 *                     value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                     the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                     Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @return Reply Message, or null if no reply received in time
	 *
	 * @throws SWIG_Exception Thrown if the given Message object is null.
	 * @throws GmsecException Thrown if an error occurs while attempting to generate an synchronous request.
	 */
	public request(reqMessage, timeout, republish_ms);


	/**
	 * Sends a reply Message in response to a given request.
	 * <p>
	 * Note: The actual Message that is sent to the middleware will contain tracking fields;
	 * to disable this feature, create a Connection object with the tracking=off
	 * configuration option.
	 *
	 * @param reqMessage The request message that was received, and to which a response is being sent.
	 * @param repMessage The reply message to be sent.
	 *
	 * @throws SWIG_Exception Thrown if either the Request or Reply Message objects are null.
	 * @throws GmsecException Thrown if an error occurs while attempting to issue the reply.
	 */
	public reply(reqMessage, repMessage);


	/**
	 * Returns the next message received within the specified timeout.
	 * The received messages are determined by the message subscriptions set up with
	 * setupSubscription().
	 * <p>
	 *
	 * @param timeout The maximum time to block waiting for a message, in milliseconds
	 *
	 * @return A handle to the next available Message, or null if a timeout occurs.
	 *
	 * @throws GmsecException Thrown on error
	 *
	 * @see Connection.setupSubscription()
	 */
	public receive(timeout);


	/**
	 * Exclude any incoming messages with the specified subject.
	 *
	 * @param subject The subject pattern to look for in incoming messages.
	 *
	 * @throws SWIG_Exception Thrown if the given subject string is null.
	 * @throws GmsecException Thrown if a malformed subject is provided.
	 */
	public excludeSubject(subject);


	/**
	 * Remove an excluded subject, allowing incoming messages with the matching
	 * subject to once again be received.
	 *
	 * @param subject The subject pattern to remove.
	 *
	 * @throws SWIG_Exception Thrown if the given subject string is null.
	 * @throws GmsecException Thrown if a malformed subject is provided.
	 */
	public removeExcludedSubject(subject);


	/**
	 * Returns the name of the connection, automatically generated or user specified.
	 *
	 * @return A string.
	 */
	public getName();


	/**
	 * Set the logical name of this connection. This can be used for
	 * Identifying connections withing a client program. If a name is not given,
	 * one will be automatically generated.
	 *
	 * @param name Name of this connection
	 *
	 * @throws SWIG_Exception Thrown if the given name string is null.
	 * @throws GmsecException Thrown if the name string is an empty string.
	 */
	public setName(name);


	/**
	 * Get the string ID for this connection.
	 *
	 * @return The ID string for this connection.
	 */
	public getID();


	/**
	 * Returns a string containing middleware information.
	 *
	 * @return The middleware information string for this connection.
	 */
	public getMWInfo();


	/**
	 * Returns a string containing info regarding the connection endpoint.
	 *
	 * @return The connection endpoint information string for this connection.
	 */
	public getConnectionEndpoint();


	/**
	 * Retrieves the number of messages queued for asynchronous publish operation.
	 *
	 * @return The number of messages remaining in the queue.
	 */
	public getPublishQueueMessageCount();
}
