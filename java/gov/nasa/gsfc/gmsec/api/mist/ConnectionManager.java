/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConnectionManager.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.gmsecAPI;
import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.Connection;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;
import gov.nasa.gsfc.gmsec.api.Status;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.message.MistMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnectionManager;


/**
 * This class wraps the GMSEC Connection object, supplying both normal
 * middleware interaction functions and some additional quality of service. 
 * This is accomplished through standard services for producing Log and 
 * Heartbeat messages, as well as functionality for validating messages. 
 * <p>
 * If the object is configured to validate messages, all messages sent 
 * from this ConnectionManager via either Publish or Request will be compared
 * to an available schema definition. If a template definition for the message
 * in question is available, it will be validated. The message will not be sent
 * over the middleware if validation fails. Messages without a template
 * definition can be sent as well.
 *
 * @see Config
 * @see Connection
 * @see Message
 * @see Specification
*/
public class ConnectionManager
{
	private JNIConnectionManager m_jniConnMgr = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param obj Object to reference for acquiring internal JNIConnectionManager.
	 * @return Internal JNIConnectionManager object.
	 */
	public static JNIConnectionManager getInternal(ConnectionManager obj)
	{
		return (obj == null ? null : obj.m_jniConnMgr);
	}


	protected ConnectionManager(JNIConnectionManager jConnMgr)
	{
		m_jniConnMgr = jConnMgr;
	}


	protected ConnectionManager()
	{
	}


	/**
	 * This function identifies the version of the API.
	 *
	 * @return String containing API version information.
	 */
	public static String getAPIVersion()
	{
		return JNIConnectionManager.getAPIVersion();
	}


	/**
	 * Constructor - Initializes the ConnectionManager instance without attempting a
	 * connection to the middleware.
	 * <p>
	 * Note: Message validation will be disabled, unless the configuration option
	 * GMSEC-MSG-CONTENT-VALIDATE is set to true. If GMSEC-MSG-CONTENT-VALIDATE is set to true,
	 * then messages will be validated against the default version of the GMSEC Interface Specification
	 * Document (ISD). The version of the ISD can be changed by specifying the configuration option
	 * GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
	 *
	 * @param cfg The config object used to construct the underlying GMSEC connection object
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 * @throws GMSEC_Exception Thrown if the configuration information cannot be used to deduce
	 * a Connection type, or if an anomaly occurs while loading schemas for the specified ISD.
	 *
	 * @see gmsecMIST
	 */
	public ConnectionManager(Config cfg)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniConnMgr = new JNIConnectionManager(this, cfg);
	}


	/**
	 * Constructor - Initializes the ConnectionManager instance without attempting a
	 * connection to the middleware.
	 * <p>
	 * Note: If message validation is enabled, then messages will be validated against the default version
	 * of the GMSEC Interface Specification Document (ISD). The version of the ISD can be changed by
	 * specifying the configuration option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
	 *
	 * @param cfg The config object used to construct the underlying GMSEC connection object
	 * @param validate Flag to indicate whether the ConnectionManager should validate messages
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 * @throws GMSEC_Exception Thrown if the configuration information cannot be used to deduce
	 * a Connection type, or if an anomaly occurs while loading schemas for the specified ISD.
	 *
	 * @see gmsecMIST
	 */
	public ConnectionManager(Config cfg, boolean validate)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniConnMgr = new JNIConnectionManager(this, cfg, validate);
	}


	/**
	 * Constructor - Initializes the ConnectionManager instance without attempting a
	 * connection to the middleware.
	 *
	 * @param cfg The config object used to construct the underlying GMSEC connection object
	 * @param validate Flag to indicate whether the ConnectionManager should validate messages
	 * @param version The version of the GMSEC message specification to be used in validating
	 * messages (e.g. gmsecMIST.GMSEC_ISD_CURRENT)
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 * @throws GMSEC_Exception Thrown if the configuration information cannot be used to deduce
	 * a Connection type, or if an anomaly occurs while loading schemas for the specified ISD.
	 *
	 * @see gmsecMIST
	 */
	public ConnectionManager(Config cfg, boolean validate, int version)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniConnMgr = new JNIConnectionManager(this, cfg, validate, version);
	}


	/**
	 * Establishes a connection with the GMSEC middleware server.
	 * Once this call successfully returns, the ConnectionManager is ready for message operations.
	 *
	 * @throws GMSEC_Exception Thrown if an anomaly occurs while attempting to connect to the middleware server.
	 */
	public void initialize()
		throws GMSEC_Exception
	{
		m_jniConnMgr.initialize();
	}


	/**
	 * Disconnects the connection with the GMSEC middleware server.
	 *
	 * @throws GMSEC_Exception Thrown if an anomaly occurs while disconnecting.
	 */
	public void cleanup() throws GMSEC_Exception
	{
		m_jniConnMgr.cleanup();
	}


	/**
	 * This function returns the current state of the connection to the middleware.
	 *
	 * @return Enumerated ConnectionState value.
	 */
	public Connection.ConnectionState getState()
	{
		return m_jniConnMgr.getState();
	}


	/**
	 * This function returns the library version.
	 * @return The library root name.
	 */
	public String getLibraryRootName()
	{
		return m_jniConnMgr.getLibraryRootName();
	}


	/**
	 * This function returns the library version.
	 * @return The library version.
	 */
	public String getLibraryVersion()
	{
		return m_jniConnMgr.getLibraryVersion();
	}


	/**
	 * Returns the Specification object associated with the Connection Manager.
	 *
	 * @return A Specification object.
	 */
	public Specification getSpecification()
	{
		return m_jniConnMgr.getSpecification();
	}


	/**
	 * Sets the internal list of fields that are added to all messages that are created
	 * using the ConnectionManager. Internal copies of the provided Fields are made, thus
	 * ownership is not retained by the ConnectionManager. The supplied set of fields will not
	 * be validated here; validation occurs at the time a message is to be published.
	 *
	 * @param standardFields The list of fields to be copied to the internal set of fields to
	 * be appended to all messages that are issued by the ConnectionManager.
	 *
	 * @see MistMessage#setStandardFields(java.util.List) MistMessage.setStandardFields(java.util.List&lt;Field&gt;)
	 */
	public void setStandardFields(java.util.List<Field> standardFields)
	{
		if (standardFields != null && standardFields.size() > 0)
		{
			m_jniConnMgr.setStandardFields(standardFields);
		}
	}


	/**
	 * Returns the list of standard fields that have been set within the Connection Manager.
	 *
	 * @return The list of standards Fields that have been set/associated with the Connection Manager.
	 *
	 * @see ConnectionManager#setStandardFields(java.util.List) ConnectionManager.setStandardFields(java.util.List&lt;Field&gt;)
	 */
	public java.util.List<Field> getStandardFields()
	{
		return m_jniConnMgr.getStandardFields();
	}


	/**
	 * Adds the standard fields (if any) to the given Message object.
	 *
	 * @param msg A Message object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 *
	 * @see ConnectionManager#setStandardFields(java.util.List) ConnectionManager.setStandardFields(java.util.List&lt;Field&gt;)
	 */
	public void addStandardFields(Message msg) throws IllegalArgumentException
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		m_jniConnMgr.addStandardFields(msg);
	}


	/**
	 * Registers the given message validator to be used when message validation takes place.
	 *
	 * @param validator The custom message validator
	 *
	 * @throws IllegalArgumentException Thrown if the given validator object is null.
	 */
	public void registerMessageValidator(MessageValidator validator)
	{
		if (validator == null)
		{
			throw new IllegalArgumentException("MessageValidator is null");
		}

		m_jniConnMgr.registerMessageValidator(validator);
	}


	/**
	 * Registers the given callback for the specified event.
	 *
	 * @param event Type of event to register
	 * @param cb Object derived from EventCallback to register for this error event
	 *
	 * @throws IllegalArgumentException Thrown if the given callback object is null.
	 * @throws GMSEC_Exception Thrown if an unknown anomaly occurs.
	 */
	public void registerEventCallback(Connection.ConnectionEvent event, ConnectionManagerEventCallback cb)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (cb == null)
		{
			throw new IllegalArgumentException("ConnectionManagerEventCallback is null");
		}

		m_jniConnMgr.registerEventCallback(event, cb);
	}


	/**
	 * This method subscribes to a particular subject or pattern and if provided.
	 *
	 * @param subject Subject pattern to match received messages
	 *
	 * @return A SubscriptionInfo object is returned; this should be used when unsubscribing.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if the ConnectionManager has not been initialized.
	 * @throws GMSEC_Exception Thrown if the given SubscriptionInfo object originated from a different ConnectionManager object.
	 * @throws GMSEC_Exception Thrown if error occurs at the middleware level.
	 *
	 * @see ConnectionManager#unsubscribe(SubscriptionInfo)
	 */
	public SubscriptionInfo subscribe(String subject)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		return m_jniConnMgr.subscribe(subject);
	}


	/**
	 * This method subscribes to a particular subject or pattern and if provided. If a
	 * callback is given, then it will be associated with the subscription.
	 * If all subscriptions are setup to use a callback, then the auto-dispatcher
	 * should be used to asynchronously receive messages. An alternative is to use receive() to
	 * receive messages, and then to forward them to callbacks using dispatch().
	 *
	 * @param subject Subject pattern to match received messages
	 * @param cb Callback to be called when message is received
	 *
	 * @return A SubscriptionInfo object is returned; this should be used when unsubscribing.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the callback is null.
	 * @throws GMSEC_Exception Thrown if the ConnectionManager has not been initialized.
	 *
	 * @see ConnectionManager#unsubscribe(SubscriptionInfo)
	 */
	public SubscriptionInfo subscribe(String subject, ConnectionManagerCallback cb)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (cb == null)
		{
			throw new IllegalArgumentException("ConnectionManagerCallback is null");
		}

		return m_jniConnMgr.subscribe(subject, cb);
	}


	/**
	 * This method subscribes to a particular subject or pattern and uses
	 * the provided config object to enable or disable middleware-level subscription
	 * functionalities (e.g. ActiveMQ Durable Consumer).
	 *
	 * @param subject Subject pattern to match received messages
	 * @param config Config object to be used for subscription operation
	 *
	 * @return A SubscriptionInfo object is returned; this should be used when unsubscribing.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the config object is null.
	 * @throws GMSEC_Exception Thrown if the ConnectionManager has not been initialized.
	 *
	 * @see ConnectionManager#unsubscribe(SubscriptionInfo)
	 */
	public SubscriptionInfo subscribe(String subject, Config config)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config object is null");
		}

		return m_jniConnMgr.subscribe(subject, config);
	}


	/**
	 * This method subscribes to a particular subject or pattern and if provided.  If a
	 * callback is given, then it will be associated with the subscription.
	 * This method also uses the provided config object to enable or disable middleware-level
	 * subscription functionalities (e.g. ActiveMQ Durable Consumer).
	 * If all subscriptions are setup to use a callback, then the auto-dispatcher
	 * should be used to asynchronously receive messages.  An alternative is to use receive() to
	 * receive messages, and then to forward them to callbacks using dispatch().
	 *
	 * @param subject Subject pattern to match received messages
	 * @param config Config object to be used for subscription operation
	 * @param cb Callback to be called when message is received
	 *
	 * @return A SubscriptionInfo object is returned; this should be used when unsubscribing.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the callback is null.
	 * @throws GMSEC_Exception Thrown if the ConnectionManager has not been initialized.
	 *
	 * @see ConnectionManager#unsubscribe(SubscriptionInfo)
	 */
	public SubscriptionInfo subscribe(String subject, Config config, ConnectionManagerCallback cb)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config object is null");
		}
		if (cb == null)
		{
			throw new IllegalArgumentException("ConnectionManagerCallback is null");
		}

		return m_jniConnMgr.subscribe(subject, config, cb);
	}


	/**
	 * This method unsubscribes to a particular subject or pattern, and will stop the reception
	 * or messages that match this pattern. It will also remove the registration of any callbacks
	 * with this subject pattern.
	 *
	 * @param info The SubscriptionInfo object that was returned from a call to subscribe()
	 *
	 * @throws IllegalArgumentException Thrown if the SubscriptionInfo object is null.
	 * @throws GMSEC_Exception Thrown if the SubscriptionInfo object originated from a different ConnectionManager object.
	 * @throws GMSEC_Exception Thrown if error occurs at the middleware level.
	 */
	public void unsubscribe(SubscriptionInfo info) throws IllegalArgumentException, GMSEC_Exception
	{
		if (info == null)
		{
			throw new IllegalArgumentException("SubscriptionInfo is null");
		}

		m_jniConnMgr.unsubscribe(info);
	}


	/**
	 * If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the message will be validated before it is published.
	 *
	 * @param msg The GMSEC message to be published.
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 * @throws GMSEC_Exception Thrown if the message fails validation, or if any other severe
	 * error occurs with sending the message.
	 */
	public void publish(Message msg) throws IllegalArgumentException, GMSEC_Exception
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		m_jniConnMgr.publish(msg);
	}


	/**
	 * Publishes the given message to the middleware using the given configuration to
	 * enable or disable certain middleware-level publish functionalities
	 * (e.g. ActiveMQ - Durable Producer). Message will still
	 * be validated if message validation is enabled.
	 * <p>
	 * Note: The actual Message published to the middleware will contain tracking fields;
	 * to disable this feature, create a ConnectionManager object with the tracking=off
	 * configuration option.
	 *
	 * @param msg The GMSEC message to be published
	 * @param mwConfig Config object for providing middleware configuration options
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 * @throws GMSEC_Exception Thrown if the message fails validation, or if any other severe
	 * error occurs with sending the message.
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

		m_jniConnMgr.publish(msg, mwConfig);
	}


	/**
	 * If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 * <p>
	 * Note: Client applications should maintain a reference to their ConnectionManagerReplyCallback object
	 * when calling this method. This will prevent the JVM garbage collector from destroying the callback
	 * before it is ever called.
	 *
	 * @param request      Message to be sent
	 * @param timeout      Maximum time to wait for reply (in milliseconds)
	 * @param cb           Callback to call when reply is received
	 * @param republish_ms Request message resubmission interval (in milliseconds). If set to a negative
	 *                     value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
	 *                     the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                     Config object used to create the Connection object. The minimum republish period allowed is 100ms.
	 *
	 * @throws IllegalArgumentException Thrown if the given request Message object is null.
	 * @throws IllegalArgumentException Thrown if the given callback object is null.
	 * @throws GMSEC_Exception Thrown if the request message fails validation, or if any other severe
	 * error occurs with issuing the request.
	 *
	 * @see ConnectionManager#cancelRequest(ConnectionManagerReplyCallback)
	 */
	public void request(Message request, int timeout, ConnectionManagerReplyCallback cb, int republish_ms)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}
		if (cb == null)
		{
			throw new IllegalArgumentException("ConnectionManagerReplyCallback is null");
		}

		m_jniConnMgr.request(request, timeout, cb, republish_ms);
	}


	/**
	 * If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 *
	 * @param request      Message to be sent
	 * @param timeout      Maximum time to wait for reply (in milliseconds)
	 * @param republish_ms Request message resubmission interval (in milliseconds). If set to a negative
	 *                     value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                     the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                     Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @return Reply message (if received)
	 *
	 * @throws IllegalArgumentException Thrown if the given request Message object is null.
	 * @throws GMSEC_Exception Thrown if the request message fails validation, or if any other severe
	 * error occurs with issuing the request.
	 */
	public Message request(Message request, int timeout, int republish_ms)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}

		return m_jniConnMgr.request(request, timeout, republish_ms);
	}


	/**
	 * This function can be used to cancel a pending request that is associated with
	 * the given ConnectionManagerReplyCallback object.
	 *
	 * @param cb The ConnectionManagerReplyCallback to disassociate from a pending request.
	 *
	 * @throws IllegalArgumentException Thrown if the given callback object is null.
	 * @throws GMSEC_Exception TBD.
	 *
	 * @see ConnectionManager#request(Message, int, ConnectionManagerReplyCallback, int)
	 */
	public void cancelRequest(ConnectionManagerReplyCallback cb)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (cb == null)
		{
			throw new IllegalArgumentException("ConnectionManagerReplyCallback is null");
		}

		m_jniConnMgr.cancelRequest(cb);
	}


	/**
	 * If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 *
	 * @param request The pending request message
	 * @param reply The reply message to be sent
	 *
	 * @throws IllegalArgumentException Thrown if the given request Message object is null.
	 * @throws IllegalArgumentException Thrown if the given reply Message object is null.
	 * @throws GMSEC_Exception Thrown if the either message fails validation, or if any other severe
	 * error occurs with issuing the reply.
	 *
	 * @see ConnectionManager#request(Message, int, int)
	 * @see ConnectionManager#request(Message, int, ConnectionManagerReplyCallback, int)
	 */
	public void reply(Message request, Message reply)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}
		if (reply == null)
		{
			throw new IllegalArgumentException("Reply Message is null");
		}

		m_jniConnMgr.reply(request, reply);
	}


	/**
	 * Attempts to dispatch the given message to callbacks that have been previously
	 * registered.
	 *
	 * @param msg Message to be dispatched
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 * @throws GMSEC_Exception Thrown if a severe error occurs while dispatching the message.
	 */
	public void dispatch(Message msg)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		m_jniConnMgr.dispatch(msg);
	}


	/**
	 * This method is a pass-through to the underlying connection.
	 * This function returns the next message available which contains a subject that
	 * has been previously subscribed to. Until a message is received, this method
	 * will block.
	 *
	 * The received messages are determined by the subscribe() function(s), but
	 * dispatch() needs to be called messages received from this method to
	 * ensure all registered callbacks are executed.
	 *
	 * This method <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @return The next received message.
	 *
	 * @throws GMSEC_Exception Thrown if a severe error occurs while attempting to receive a message.
	 */
	public Message receive() throws GMSEC_Exception
	{
		return receive(gmsecAPI.WAIT_FOREVER);
	}


	/**
	 * This method is a pass-through to the underlying connection.
	 * This method returns the next message received within the specified timeout.
	 *
	 * The received messages are determined by the subscribe() function(s), but
	 * dispatch() needs to be called messages received from this function to
	 * ensure all registered callbacks are executed.
	 *
	 * This method <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @param timeout The maximum time to block waiting for a message, in milliseconds
	 *
	 * @return The next received message, if any. If a timeout occurs, null is returned.
	 *
	 * @throws GMSEC_Exception Thrown if a severe error occurs while attempting to receive a message.
	 */
	public Message receive(int timeout) throws GMSEC_Exception
	{
		return m_jniConnMgr.receive(timeout);
	}


	/**
	 * This method will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages will be dropped.
	 * If receive() is called while the auto-dispatcher is used, the behavior will be undesireable
	 * and undefined.
	 *
	 * @return True of the dispatcher started successfully; false otherwise.
	 *
	 * @throws GMSEC_Exception Thrown if the ConnectionManager has not been initialized.
	 *
	 * @see ConnectionManager#stopAutoDispatch()
	 */
	public boolean startAutoDispatch() throws GMSEC_Exception
	{
		return m_jniConnMgr.startAutoDispatch();
	}


	/**
	 * This function will stop the auto dispatch thread.
	 *
	 * @return True of the dispatcher stopped successfully; false otherwise.
	 *
	 * @throws GMSEC_Exception Thrown if the ConnectionManager has not been initialized.
	 *
	 * @see ConnectionManager#startAutoDispatch()
	 */
	public boolean stopAutoDispatch() throws GMSEC_Exception
	{
		return m_jniConnMgr.stopAutoDispatch(true);
	}


	/**
	 * This function will stop the auto dispatch thread.
	 *
	 * @return True of the dispatcher stopped successfully; false otherwise.
	 *
	 * @param waitForCompletion Boolean value to indicate whether or not to wait
	 * for the dispatcher to finish operations.
	 *
	 * @throws GMSEC_Exception Thrown if the ConnectionManager has not been initialized.
	 *
	 * @see ConnectionManager#startAutoDispatch()
	 */
	public boolean stopAutoDispatch(boolean waitForCompletion) throws GMSEC_Exception
	{
		return m_jniConnMgr.stopAutoDispatch(waitForCompletion);
	}


	/**
	 * Exclude any incoming messages with the specified subject.
	 *
	 * @param subject The subject pattern to look for in incoming messages.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if the given subject is invalid.
	 *
	 * @see ConnectionManager#removeExcludedSubject(String)
	 */
	public void excludeSubject(String subject) throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		m_jniConnMgr.excludeSubject(subject);
	}


	/**
	 * Remove an excluded subject, allowing incoming messages with the matching
	 * subject to once again be received.
	 *
	 * @param subject The subject pattern to remove.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if an invalid subject is provided.
	 *
	 * @see ConnectionManager#excludeSubject(String)
	 */
	public void removeExcludedSubject(String subject) throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		m_jniConnMgr.removeExcludedSubject(subject);
	}


	/**
	 * Returns the name of the connection, automatically generated or user specified.
	 *
	 * @return The name associated with the connection.
	 */
	public String getName()
	{
		return m_jniConnMgr.getName();
	}


	/**
	 * Set the logical name of this connection. This can be used for
	 * Identifying connections withing a client program. If a name is not given,
	 * one will be automatically generated.
	 *
	 * @param name Name of this connection.
	 *
	 * @throws IllegalArgumentException Thrown if the given name is null or consists of an empty string.
	 */
	public void setName(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Name is null or contain an empty string");
		}

		m_jniConnMgr.setName(name);
	}


	/**
	 * Returns the string ID for this connection.
	 *
	 * @return The ID string for this connection.
	 */
	public String getID()
	{
		return m_jniConnMgr.getID();
	}


	/**
	 * Returns the string containing middleware information.
	 *
	 * @return The middleware information string for this connection.
	 */
	public String getMWInfo()
	{
		return m_jniConnMgr.getMWInfo();
	}


	/**
	 * Returns middleware broker connection information.
	 *
	 * @return The middleware broker connection information.
	 */
	public String getConnectionEndpoint()
	{
		return m_jniConnMgr.getConnectionEndpoint();
	}


	/**
	 * Retrieves the number of messages queued for asynchronous publish operations
	 *
	 * @return The number of messages remaining in the queue.
	 */
	public long getPublishQueueMessageCount()
	{
		return m_jniConnMgr.getPublishQueueMessageCount();
	}


	/**
	 * This method creates a message and passes ownership to the user. This message is populated with
	 * the standard set of required and optional heartbeat fields, as well as the required common fields defined
	 * in setStandardFields().
	 * <p>
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param subject The topic (subject) that will be applied to the returned messages.
	 * @param heartbeatFields Set of fields to be added to the heartbeat
	 *
	 * @return A C2CX Heartbeat Message
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to build the Heartbeat Message.
	 */
	public Message createHeartbeatMessage(String subject, java.util.List<Field> heartbeatFields)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		return m_jniConnMgr.createHeartbeatMessage(subject, heartbeatFields);
	}


	/**
	 * This method creates an archetypal heartbeat message from the concatenation of the standard fields
	 * applied globally to this ConnectionManager and to this function. This message will then be validated,
	 * if the "validate" flag is set. If there is an error, an Exception will be thrown. If it has not been set,
	 * the message will be published at an interval supplied by the "PUB-RATE" field regardless of validation
	 * results. If no "PUB-RATE" has been defined, the service will default to the GMSEC standard 30 second
	 * heartbeat interval.
	 * <p>
	 * If users would like to have a COUNTER field added to the published heartbeat message, then the Heartbeat
	 * Service should be provided with this field within the list of field provided to this method.
	 * <p>
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and if applicable, C2CX-SUBTYPE fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param subject Subject to set as the default for heartbeat messages
	 * @param heartbeatFields Set of fields to be added to the published heartbeat messages
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if the Heartbeat Service is already running.
	 */
	public void startHeartbeatService(String subject, java.util.List<Field> heartbeatFields)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		m_jniConnMgr.startHeartbeatService(subject, heartbeatFields);
	}


	/**
	 * This method terminates opertaion of the heartbeat service and cleans up related resources.
	 * It will be called by the global connection manager "cleanup" if not done manually by the user.
	 *
	 * @throws GMSEC_Exception Thrown if the Heartbeat Service is not running.
	 */
	public void stopHeartbeatService() throws GMSEC_Exception
	{
		m_jniConnMgr.stopHeartbeatService();
	}


	/**
	 * This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service. If validation is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error is returned.
	 * <p>
	 * Note: This method has been deprecated; use {@link ConnectionManager#setHeartbeatServiceField(Field)} instead.
	 *
	 * @param componentStatus Field containing updated information for the Heartbeat Message.
	 *
	 * @return The status of the operation.
	 *
	 * @throws IllegalArgumentException Thrown if the given Field is null.
	 * @throws GMSEC_Exception Thrown if the given Field is not already present in the Heartbeat
	 * Message, or if by adding it to the Heartbeat Message, the message itself becomes invalid.
	 */
	@Deprecated
	public Status changeComponentStatus(Field componentStatus)
		throws IllegalArgumentException, GMSEC_Exception
	{
		return setHeartbeatServiceField(componentStatus);
	}


	/**
	 * This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service. If validation is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error is returned.
	 * <p>
	 * Note: This method has been deprecated; use {@link ConnectionManager#setHeartbeatServiceField(Field)} instead.
	 *
	 * @param componentInfo Field containing updated information for the Heartbeat Message.
	 *
	 * @return The status of the operation.
	 *
	 * @throws IllegalArgumentException Thrown if the given Field is null.
	 * @throws GMSEC_Exception Thrown if the given Field is not already present in the Heartbeat
	 * Message, or if by adding it to the Heartbeat Message, the message itself becomes invalid.
	 */
	@Deprecated
	public Status changeComponentInfo(Field componentInfo)
		throws IllegalArgumentException, GMSEC_Exception
	{
		return setHeartbeatServiceField(componentInfo);
	}


	/**
	 * This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service. If validation is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error is returned.
	 * <p>
	 * Note: This method has been deprecated; use {@link ConnectionManager#setHeartbeatServiceField(Field)} instead.
	 *
	 * @param cpuMemory Field containing updated information for the Heartbeat Message.
	 *
	 * @return The status of the operation.
	 *
	 * @throws IllegalArgumentException Thrown if the given Field is null.
	 * @throws GMSEC_Exception Thrown if the given Field is not already present in the Heartbeat
	 * Message, or if by adding it to the Heartbeat Message, the message itself becomes invalid.
	 */
	@Deprecated
	public Status changeCPUMemory(Field cpuMemory)
		throws IllegalArgumentException, GMSEC_Exception
	{
		return setHeartbeatServiceField(cpuMemory);
	}


	/**
	 * This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service. If validation is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error is returned.
	 * <p>
	 * Note: This method has been deprecated; use {@link ConnectionManager#setHeartbeatServiceField(Field)} instead.
	 *
	 * @param cpuUtil Field containing updated information for the Heartbeat Message.
	 *
	 * @return The status of the operation.
	 *
	 * @throws IllegalArgumentException Thrown if the given Field is null.
	 * @throws GMSEC_Exception Thrown if the given Field is not already present in the Heartbeat
	 * Message, or if by adding it to the Heartbeat Message, the message itself becomes invalid.
	 */
	@Deprecated
	public Status changeCPUUtil(Field cpuUtil)
		throws IllegalArgumentException, GMSEC_Exception
	{
		return setHeartbeatServiceField(cpuUtil);
	}


	/**
	 * This method can be used to set or update the given field within the Heartbeat Message
	 * being published by the Heartbeat Service. If validation is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an error is returned.
	 * <p>
	 * Note: If a (valid) PUB-RATE field is passed to this method, and the Heartbeat Service
	 * is running, then the Heartbeat Service publish rate will be changed to the provided
	 * rate. Note that a publish rate of 0 seconds or less will be treated as an error.
	 *
	 * @param field The field to add or set within the heartbeat message.
	 *
	 * @return The status of the operation.
	 *
	 * @throws IllegalArgumentException Thrown if the given Field is null.
	 * @throws GMSEC_Exception Thrown is thrown if an unexpected error occurs.
	 */
	public Status setHeartbeatServiceField(Field field)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (field == null)
		{
			throw new IllegalArgumentException("Field is null");
		}

		return m_jniConnMgr.setHeartbeatServiceField(field);
	}


	/**
	 * This method creates a Log Message and passes ownership to the user. This message is populated
	 * with the standard set of required and optional log fields, as well as the required common fields
	 * defined in setStandardFields(). The message is not validated at this time, as MSG-TEXT and SEVERITY
	 * fields must be set by the user at the time the message is to be sent.
	 * <p>
	 * This message automatically is generated with MESSAGE-TYPE and MESSAGE-SUBTYPE
	 * correctly set to the GMSEC Message Standard requirements, regardless of user
	 * input. EVENT-TIME defaults to the time which this function was invoked; the user
	 * may override this time once the message has been returned.
	 *
	 * @param subject The topic (subject) to apply to the returned message.
	 * @param logFields Set of fields to be added to the log message.
	 *
	 * @return A Log Message.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to build the Log Message.
	 */
	public Message createLogMessage(String subject, java.util.List<Field> logFields)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		return m_jniConnMgr.createLogMessage(subject, logFields);
	}


	/**
	 * This method sets the standard default fields which will be added to a new log message when publishLog
	 * is called.
	 *
	 * @param subject Subject to use for log messages
	 * @param logFields Set of fields to be added to log messages.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if an error occurs while reading over the supplied Field objects.
	 */
	public void setLoggingDefaults(String subject, java.util.List<Field> logFields)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		m_jniConnMgr.setLoggingDefaults(subject, logFields);
	}


	/**
	 * This method creates a log message comprised of the global fields from the entire ConnectionManager,
	 * the fields set in setLoggingDefaults, and the log text and severity fields as supplied as arguments to
	 * this function. If validation is enabled, the message will be published. If validation is enabled, the
	 * message will be validated. If validation of the message fails, then an exception is thrown.
	 * <p>
	 * This function automatically populates the message with the correct
	 * MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set automatically
	 * to the time of function invocation. 
	 *
	 * @param logMessage The message to log.
	 * @param severity The Field object defining the severity of the log message.
	 *
	 * @throws IllegalArgumentException Thrown if the message string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given severity Field object is null.
	 * @throws GMSEC_Exception Thrown if the Log Message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	public void publishLog(String logMessage, Field severity)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (logMessage == null || logMessage.isEmpty())
		{
			throw new IllegalArgumentException("Log message is null or contain an empty string");
		}
		if (severity == null)
		{
			throw new IllegalArgumentException("Severity Field is null");
		}

		m_jniConnMgr.publishLog(logMessage, severity);
	}


	/**
	 * This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 * <p>
	 * This function then invokes publish(Message msg) to place that message on the GMSEC
	 * bus. This implicitly sets the message's response flag to false, so that receivers 
	 * of the message will know a response is not necessary.
	 *
	 * @param subject Subject on which to publish the message
	 * @param directiveString A field containing the string directive that this message is intended to convey
	 * @param fields A list of and supplemental fields that the user wishes to include with the directive message.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given directive string Field object is null.
	 * @throws GMSEC_Exception Thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	public void requestDirective(String subject, Field directiveString, java.util.List<Field> fields)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (directiveString == null)
		{
			throw new IllegalArgumentException("Directive String Field is null");
		}

		m_jniConnMgr.requestDirective(subject, directiveString, fields);
	}


	/**
	 * This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 * <p>
	 * This function then invokes request(Message request, GMSEC_I32 timeout, 
	 * ConnectionManagerReplyCallback cb, int republish_ms) to place that message
	 * on the GMSEC bus. This implicitly sets the message's response flag to true, so that
	 * receivers of the message will know a response is necessary.
	 * <p>
	 * Note: Client applications should maintain a reference to their ConnectionManagerReplyCallback object
	 * when calling this method. This will prevent the JVM garbage collector from destroying the callback
	 * before it is ever called.
	 *
	 * @param subject         Subject on which to publish the message
	 * @param directiveString A field containing the string directive that this message is intended to convey
	 * @param fields          A list of and supplemental fields that the user wishes to include with the directive message.
	 * @param timeout         The time to wait before a response to the message will no longer be routed to the supplied callback
	 * @param cb              The callback to be invoked upon reception of a response message
	 * @param republish_ms    Request message resubmission interval (in milliseconds). If set to a negative
	 *                        value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                        the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                        Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given directive string Field object is null.
	 * @throws IllegalArgumentException Thrown if the given reply callback object is null.
	 * @throws GMSEC_Exception Thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	public void requestDirective(String subject, Field directiveString, java.util.List<Field> fields, int timeout, 
		ConnectionManagerReplyCallback cb, int republish_ms)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (directiveString == null)
		{
			throw new IllegalArgumentException("Directive String Field is null");
		}
		if (cb == null)
		{
			throw new IllegalArgumentException("ConnectionManagerReplyCallback is null");
		}

		m_jniConnMgr.requestDirective(subject, directiveString, fields, timeout, cb, republish_ms);
	}


	/**
	 * This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 * <p>
	 * This function then invokes request(Message request, int timeout, 
	 * int republish_ms) to place that message on the GMSEC bus. This implicitly
	 * sets the message's response flag to true, so that receivers of the message will 
	 * know a response is necessary.
	 *
	 * @param subject         Subject on which to publish the message
	 * @param directiveString A field containing the string directive that this message is intended to convey
	 * @param fields          A list of and supplemental fields that the user wishes to include with the directive message.
	 * @param timeout         The time to wait before a response to the message will no longer be routed to the supplied callback
	 * @param republish_ms    Request message resubmission interval (in milliseconds). If set to a negative
	 *                        value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                        the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                        Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @return A handle to a message received in response to the directive request message. This method 
	 * will return null if a message is not received in the proscribed period.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given directive string Field object is null.
	 * @throws GMSEC_Exception Thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	public Message requestDirective(String subject, Field directiveString, java.util.List<Field> fields, int timeout, int republish_ms)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (directiveString == null)
		{
			throw new IllegalArgumentException("Directive String Field is null");
		}

		return m_jniConnMgr.requestDirective(subject, directiveString, fields, timeout, republish_ms);
	}


	/**
	 * This function acknowledges a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 * <p>
	 * This function then invokes reply(Message request, Message reply) to
	 * place that message on the GMSEC bus and route it to the original requestor.
	 *
	 * @param subject Subject on which to publish the message
	 * @param request The original directive message
	 * @param status  An enumeration specifying the response code to the directive request.
	 * @param fields  Supplemental information fields which may be included with the message
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given request Message is null.
	 * @throws GMSEC_Exception Thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 *
	 */
	public void acknowledgeDirectiveRequest(String subject, Message request, gmsecMIST.ResponseStatus status, java.util.List<Field> fields)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}

		m_jniConnMgr.acknowledgeDirectiveRequest(subject, request, status, fields);
	}


	/**
	 * This function creates a valid resource message, as described in the GMSEC
	 * specification. This function then publishes the message to the GMSEC bus on the 
	 * supplied subject.
	 *
	 * @param subject Subject on which to publish the message
	 * @param sampleInterval Distance between samples (in seconds)
	 * @param averageInterval Time interval over which to make a moving average of samples (in seconds)
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if a severe error occurs while attempting to
	 * publish the message.
	 */
	public void publishResourceMessage(String subject, long sampleInterval, long averageInterval)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		m_jniConnMgr.publishResourceMessage(subject, sampleInterval, averageInterval);
	}


	/**
	 * This function creates a valid resource message, as described in the GMSEC
	 * specification, and returns that message to the user.
	 *
	 * @param subject Subject with which to create the message
	 * @param sampleInterval Distance between samples (in seconds)
	 * @param averageInterval Time interval over which to make a moving average of samples (in seconds)
	 *
	 * @return A GMSEC Resource Message.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to build the Resource Message.
	 */
	public Message createResourceMessage(String subject, long sampleInterval, long averageInterval)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		return m_jniConnMgr.createResourceMessage(subject, sampleInterval, averageInterval);
	}


	/**
	 * This function starts the resource message service, which publishes 
	 * resource messages at a fixed interval as defined by this function.
	 *
	 * @param subject Subject with which to create the message.
	 * @param interval The interval, in seconds, when messages are published to the bus.
	 * @param sampleInterval  This is the the time distance between samples (in seconds).
	 * @param averageInterval Time window over which to make a moving average of samples (in seconds).
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws GMSEC_Exception Thrown if error occurs while attempting to start the service.
	 */
	public void startResourceMessageService(String subject, long interval, long sampleInterval, long averageInterval)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		m_jniConnMgr.startResourceMessageService(subject, interval, sampleInterval, averageInterval);
	}


	/**
	 * This function stops the resource message service.
	 *
	 * @return Returns true if the resource message service was running before invocation of this
	 * function, false otherwise.
	 */
	public boolean stopResourceMessageService()
	{
		return m_jniConnMgr.stopResourceMessageService();
	}


	/**
	 * This function acknowledges a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 * <p>
	 * This function then invokes reply(Message request, Message reply) to
	 * place that message on the GMSEC bus and route it to the original requestor.
	 *
	 * @param subject Subject on which to publish the message
	 * @param request The original directive message
	 * @param status  An enumeration specifying the response code to the simple service request.
	 * @param fields  Supplemental information fields which may be included with the message
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the request Message is null.
	 * @throws GMSEC_Exception Thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	public void acknowledgeSimpleService(String subject, Message request, gmsecMIST.ResponseStatus status, java.util.List<Field> fields)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (request == null)
		{
			throw new IllegalArgumentException("Request Message is null");
		}

		m_jniConnMgr.acknowledgeSimpleService(subject, request, status, fields);
	}


	/**
	 * This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 * <p>
	 * This function then invokes publish(Message msg) to place that message on the GMSEC
	 * bus. This implicitly sets the message's response flag to false, so that receivers 
	 * of the message will know a response is not necessary
	 *
	 * @param subject  Subject on which to publish the message.
	 * @param opName   The name of the operation of the service.
	 * @param opNumber The operation number for the service message is intended to convey.
	 * @param fields   A list of supplemental fields that the user wishes to include with the directive message.
	 * @param params   A list of the ServiceParam objects providing meta data for this service invocation.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the operation name string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the operation number Field is null.
	 * @throws GMSEC_Exception Thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	public void requestSimpleService(String subject, String opName, Field opNumber, java.util.List<Field> fields, java.util.List<ServiceParam> params)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (opName == null || opName.isEmpty())
		{
			throw new IllegalArgumentException("Operation Name is null or contain an empty string");
		}
		if (opNumber == null)
		{
			throw new IllegalArgumentException("Operation Number Field is null");
		}

		m_jniConnMgr.requestSimpleService(subject, opName, opNumber, fields, params);
	}


	/**
	 * This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 * <p>
	 * This function then invokes request(Message request, int timeout, 
	 * ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms = 0) to place that message
	 * on the GMSEC bus. This implicitly sets the message's response flag to true, so that
	 * receivers of the message will know a response is necessary.
	 * <p>
	 * Note: Client applications should maintain a reference to their ConnectionManagerReplyCallback object
	 * when calling this method. This will prevent the JVM garbage collector from destroying the callback
	 * before it is ever called.
	 *
	 * @param subject      Subject on which to publish the message.
	 * @param opName       The name of the operation of the service.
	 * @param opNumber     The operation number for the service message is intended to convey.
	 * @param fields       A list of and supplemental fields that the user wishes to include with the directive message.
	 * @param params       A list of the ServiceParam objects providing meta data for this service invocation.
	 * @param timeout      The time to wait before a response to the message will no longer be routed to the supplied callback.
	 * @param cb           The callback to be invoked upon reception of a response message.
	 * @param republish_ms Request message resubmission interval (in milliseconds). If set to a negative
	 *                     value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
	 *                     the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                     Config object used to create the Connection object. The minimum republish period allowed is 100ms.
	 *
	 * @throws IllegalArgumentException Thrown if the given subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given operation name string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given operation number Field is null.
	 * @throws IllegalArgumentException Thrown if the given reply callback is null.
	 * @throws GMSEC_Exception Thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	public void requestSimpleService(String subject, String opName, Field opNumber,
					java.util.List<Field> fields, java.util.List<ServiceParam> params,
					int timeout, ConnectionManagerReplyCallback cb, int republish_ms)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (opName == null || opName.isEmpty())
		{
			throw new IllegalArgumentException("Operation Name is null or contain an empty string");
		}
		if (opNumber == null)
		{
			throw new IllegalArgumentException("Operation Number Field is null");
		}
		if (cb == null)
		{
			throw new IllegalArgumentException("ConnectionManagerReplyCallback is null");
		}

		m_jniConnMgr.requestSimpleService(subject, opName, opNumber, fields, params, timeout, cb, republish_ms);
	}


	/**
	 * This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 * <p>
	 * This function then invokes request(Message request, int timeout, int republish_ms)
	 * to place that message on the GMSEC bus. This implicitly
	 * sets the message's response flag to true, so that receivers of the message will 
	 * know a response is necessary.
	 *
	 * @param subject      Subject on which to publish the message.
	 * @param opName       The name of the operation of the service.
	 * @param opNumber     The operation number for the service message is intended to convey.
	 * @param fields       A list of and supplemental fields that the user wishes to include with the directive message.
	 * @param params       A list of the ServiceParam objects providing meta data for this service invocation.
	 * @param timeout      The time to wait before a response to the message will no longer be routed to the supplied callback.
	 * @param republish_ms Request message resubmission interval (in milliseconds). If set to a negative
	 *                     value (eg. REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
	 *                     the period will default to 60000ms, unless the user has provided an alternate time period via the
	 *                     Config object used to create the Connection object.  The minimum republish period allowed is 100ms.
	 *
	 * @return A handle to a message received in response to the directive request message. This method
	 * will return null if a message is not received in the proscribed period.
	 *
	 * @throws IllegalArgumentException Thrown if the given subject string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given operation name string is null, or contains an empty-string.
	 * @throws IllegalArgumentException Thrown if the given operation number Field is null.
	 * @throws GMSEC_Exception Thrown if the message cannot be validated, or if other severe error
	 * occurs while attempting to publish the message.
	 */
	 public Message requestSimpleService(String subject, String opName, Field opNumber,
						java.util.List<Field> fields, java.util.List<ServiceParam> params,
						int timeout, int republish_ms)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}
		if (opName == null || opName.isEmpty())
		{
			throw new IllegalArgumentException("Operation Name is null or contain an empty string");
		}
		if (opNumber == null)
		{
			throw new IllegalArgumentException("Operation Number Field is null");
		}

		return m_jniConnMgr.requestSimpleService(subject, opName, opNumber, fields, params, timeout, republish_ms);
	}


	/**
	 * Calls shutdown routines for each middleware that has a shutdown routine registered.
	 * This method should only be called once, typically just before an application terminates. 
	 * Note the calling of this method is optional.
	 */
	public static void shutdownAllMiddlewares()
	{
		JNIConnectionManager.shutdownAllMiddlewares();
	}
}
