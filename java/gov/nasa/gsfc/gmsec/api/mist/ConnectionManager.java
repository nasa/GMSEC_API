/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ConnectionManager.java
 *
 *  @brief The ConnectionManager is designed to provide several high-level management
 *  functions for operation of a GMSEC connection object. These management functions 
 *  include the application of message content validation, the operation of a
 *  standard service for producing heartbeats, and a set of functions which streamline 
 *  log message production.
 *
**/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.gmsecAPI;
import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.Connection;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;
import gov.nasa.gsfc.gmsec.api.Status;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnectionManager;



/**
 * @class ConnectionManager
 *
 * @brief This class wraps the GMSEC Connection object, supplying both normal
 * middleware interaction functions and some additional quality of service. 
 * This is accomplished through standard services for producing Log and 
 * Heartbeat messages, as well as functionality for validating messages. 
 * 
 * If the object is configured to validate messages, all messages sent 
 * from this ConnectionManager via either Publish or Request will be compared
 * to an available schema definition. If a template definition for the message
 * in question is available, it will be validated. The message will not be sent
 * over the middleware if validation fails. Messages without a template
 * definition can be sent as well.
 *
 * @sa Connection
 * @sa Config
*/
public class ConnectionManager
{
	private JNIConnectionManager m_jniConnMgr = null;


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
	 * @fn ConnectionManager(Config cfg)
	 *
	 * @brief Constructor - Initializes the ConnectionManager instance without attempting a
	 * connection to the middleware.
	 *
	 * @note Message validation will be disabled, unless the configuration option
	 * GMSEC-MSG-CONTENT-VALIDATE is set to true.  If GMSEC-MSG-CONTENT-VALIDATE is set to true,
	 * then messages will be validated against the default version of the GMSEC Interface Specification
	 * Document (ISD).  The version of the ISD can be changed by specifying the configuration option
	 * GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
	 *
	 * @param cfg - The config object used to construct the underlying GMSEC connection object
	 *
	 * @throws An IllegalArgumentException is thrown if the given Config object is null.
	 */
	public ConnectionManager(Config cfg) throws IllegalArgumentException, GMSEC_Exception
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniConnMgr = new JNIConnectionManager(this, cfg);
	}


	/**
	 * @fn ConnectionManager(Config cfg, boolean validate)
	 *
	 * @brief Constructor - Initializes the ConnectionManager instance without attempting a
	 * connection to the middleware.
	 *
	 * @note If message validation is enabled, then messages will be validated against the default version
	 * of the GMSEC Interface Specification Document (ISD).  The version of the ISD can be changed by
	 * specifying the configuration option GMSEC-SPECIFICATION-VERSION to a valid value (e.g. 201400).
	 *
	 * @param cfg - The config object used to construct the underlying GMSEC connection object
	 * @param validate - flag to indicate whether the ConnectionManager should validate messages
	 *
	 * @throws An IllegalArgumentException is thrown if the given Config object is null.
	 */
	public ConnectionManager(Config cfg, boolean validate) throws IllegalArgumentException, GMSEC_Exception
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniConnMgr = new JNIConnectionManager(this, cfg, validate);
	}


	/**
	 * @fn ConnectionManager(Config cfg, boolean validate, int version)
	 *
	 * @brief Constructor - Initializes the ConnectionManager instance without attempting a
	 * connection to the middleware.
	 *
	 * @param cfg - The config object used to construct the underlying GMSEC connection object
	 * @param validate - flag to indicate whether the ConnectionManager should validate messages
	 * @param version - the version of the GMSEC message specification to be used in validating
	 * messages (e.g. gmsecMIST.GMSEC_ISD_CURRENT)
	 *
	 * @throws An IllegalArgumentException is thrown if the given Config object is null.
	 */
	public ConnectionManager(Config cfg, boolean validate, int version) throws IllegalArgumentException, GMSEC_Exception
	{
		if (cfg == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniConnMgr = new JNIConnectionManager(this, cfg, validate, version);
	}


	/**
	 * @fn void initialize()
	 *
	 * @desc Uses the Config object supplied in the constructor to establish a connection with the
	 * defined GMSEC middleware server.
	 * The underlying connection object is created and connected in one operation, 
	 * returning an error status if either operation is a failure. Once this call
	 * successfully returns, the ConnectionManager is ready for message operations.
	 *
	 * @throws A GMSEC_Exception is thrown if the configuration information cannot be used to deduce
	 * a Connection type, or if there is a failure to connect to the middleware server.
	 */
	public void initialize() throws GMSEC_Exception
	{
		m_jniConnMgr.initialize();
	}


	/**
	 * @fn void cleanup()
	 *
	 * @desc Cleanup - This function disconnects and destroys the underlying Connection object,
	 * returning an error status if this operation is not successful.
	 */
	public void cleanup()
	{
		m_jniConnMgr.cleanup();
	}


	/**
	 * @fn String getLibraryVersion()
	 *
	 * @desc getLibraryVersion - This function returns the library version,
	 * returning an error status if this operation is not successful. 
	 */
	public String getLibraryVersion() throws GMSEC_Exception
	{
		return m_jniConnMgr.getLibraryVersion();
	}


	/**
	 * @fn Specification getSpecification()
	 *
	 * @desc Returns the Specification object associated with the Connection Manager.
	 */
	public Specification getSpecification() throws GMSEC_Exception
	{
		return m_jniConnMgr.getSpecification();
	}


	/**
	 * @fn void setStandardFields(java.util.List<Field> standardFields)
	 *
	 * @brief Sets the internal list of fields which are to be automatically placed in all messages
	 * sent by this ConnectionManager. 
	 * Internal copies of the Fields are made, ownership is not retained by the 
	 * ConnectionManager. The supplied set of fields will not be validated here, validation
	 * occurs at the time a message is to be published. 
	 *
	 * @param standardFields - The array of fields to be copied to the internal set of fields to
	 * be appended to all messages.
	 */
	public void setStandardFields(java.util.List<Field> standardFields) throws GMSEC_Exception
	{
		if (standardFields != null && standardFields.size() > 0)
		{
			m_jniConnMgr.setStandardFields(standardFields);
		}
	}


	/**
	 * @fn void addStandardFields(Message msg)
	 *
	 * @brief Adds the standard fields (if any) to the given Message object.
	 *
	 * @param msg - A Message object
	 *
	 * @sa setStandardFields()
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
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
	 * @fn void registerEventCallback(Connection.ConnectionEvent event, ConnectionManagerEventCallback cb)
	 *
	 * @brief Registers the given callback for the specified event.
	 *
	 * @param event - type of event to register
	 * @param cb - object derived from EventCallback to register for this error event
	 *
	 * @throws An IllegalArgumentException is thrown if the given callback object is null.
	 * @throws A GMSEC_Exception if the Connection Manager has not been initialized.
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
	 * @fn SubscriptionInfo subscribe(String subject)
	 *
	 * @brief This method subscribes to a particular subject or pattern and if provided.
	 *
	 * @param subject - subject pattern to match received messages
	 *
	 * @return A SubscriptionInfo object is returned; this should be used when unsubscribing.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 *
	 * @sa unsubscribe(SubscriptionInfo)
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
	 * @fn SubscriptionInfo subscribe(String subject, ConnectionManagerCallback cb)
	 *
	 * @brief This method subscribes to a particular subject or pattern and if provided.  If a
	 * callback is given, then it will be associated with the subscription.
	 * If all subscriptions are setup to use a callback, then the auto-dispatcher
	 * should be used to asynchronously receive messages.  An alternative is to use receive() to
	 * receive messages, and then to forward them to callbacks using dispatch().
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 *
	 * @return A SubscriptionInfo object is returned; this should be used when unsubscribing.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the callback is null.
	 *
	 * @sa unsubscribe(SubscriptionInfo)
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
	 * @fn SubscriptionInfo subscribe(String subject, Config config)
	 *
	 * @brief This method subscribes to a particular subject or pattern and uses
	 * the provided config object to enable or disable middleware-level subscription
	 * functionalities (e.g. ActiveMQ Durable Consumer).
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config - config object to be used for subscription operation
	 *
	 * @return A SubscriptionInfo object is returned; this should be used when unsubscribing.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the config object is null.
	 *
	 * @sa unsubscribe(SubscriptionInfo)
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
	 * @fn SubscriptionInfo subscribe(String subject, Config config, ConnectionManagerCallback cb)
	 *
	 * @brief This method subscribes to a particular subject or pattern and if provided.  If a
	 * callback is given, then it will be associated with the subscription.
	 * This method also uses the provided config object to enable or disable middleware-level
	 * subscription functionalities (e.g. ActiveMQ Durable Consumer).
	 * If all subscriptions are setup to use a callback, then the auto-dispatcher
	 * should be used to asynchronously receive messages.  An alternative is to use receive() to
	 * receive messages, and then to forward them to callbacks using dispatch().
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config - config object to be used for subscription operation
	 * @param cb - callback to be called when message is received
	 *
	 * @return A SubscriptionInfo object is returned; this should be used when unsubscribing.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the callback is null.
	 *
	 * @sa unsubscribe(SubscriptionInfo)
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
	 * @fn void unsubscribe(SubscriptionInfo info)
	 *
	 * @brief This method unsubscribes to a particular subject or pattern, and will stop the reception
	 * or messages that match this pattern.  It will also remove the registration of any callbacks
	 * with this subject pattern.
	 *
	 * @param info - the SubscriptionInfo object that was returned from a call to subscribe()
	 *
	 * @throws An IllegalArgumentException is thrown if the SubscriptionInfo object is null.
	 * @throws A GMSEC_Exception is thrown if the SubscriptionInfo object originated from a different
	 * ConnectionManager object.
	 *
	 * @sa subscribe()
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
	 * @fn void publish(Message msg)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the message will be validated before it is published.
	 *
	 * @param msg - The GMSEC message to be published.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 * @throws A GMSEC_Exception is thrown if the message fails validation, or if any other severe
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
	 * @fn void publish(Message msg, Config config)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the message will be validated before it is published.  The given
	 * configuration object is applied to the message.
	 *
	 * @param msg - The GMSEC message to be published
	 * @param config - config object to be used by the publish operation
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 * @throws An IllegalArgumentException is thrown if the given Config object is null.
	 * @throws A GMSEC_Exception is thrown if the message fails validation, or if any other severe
	 * error occurs with sending the message.
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

		m_jniConnMgr.publish(msg, config);
	}


	/**
	 * @fn void request(Message request, int timeout, ConnectionManagerReplyCallback cb, int republish_ms)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 *
	 * @note Client applications should maintain a reference to their ConnectionManagerReplyCallback object
	 * when calling this method.  This will prevent the JVM garbage collector from destroying the callback
	 * before it is ever called.
	 *
	 * @param request      - message to be sent
	 * @param timeout      - maximum time to wait for reply (in milliseconds)
	 * @param cb           - callback to call when reply is received
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set  to a negative value (eg. -1)
	 *                       it will never republish a request message.  If set to 0, the period will default to 60000ms,
	 *                       unless the user has provided an alternate time period via the Config object used to create
	 *                       the ConnectionManager object.  The minimum republish period allowed is 100ms.
	 *
	 * @throws An IllegalArgumentException is thrown if the given request Message object is null.
	 * @throws An IllegalArgumentException is thrown if the given callback object is null.
	 * @throws A GMSEC_Exception is thrown if the request message fails validation, or if any other severe
	 * error occurs with issuing the request.
	 *
	 * @sa cancelRequest()
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
	 * @fn Message request(Message request, int timeout, int republish_ms)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 *
	 * @param request      - message to be sent
	 * @param timeout      - maximum time to wait for reply (in milliseconds)
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set  to a negative value (eg. -1)
	 *                       it will never republish a request message.  If set to 0, the period will default to 60000ms,
	 *                       unless the user has provided an alternate time period via the Config object used to create
	 *                       the ConnectionManager object.  The minimum republish period allowed is 100ms.
	 *
	 * @return Reply message (if received)
	 *
	 * @throws An IllegalArgumentException is thrown if the given request Message object is null.
	 * @throws A GMSEC_Exception is thrown if the request message fails validation, or if any other severe
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
	 * @fn void cancelRequest(ConnectionManagerReplyCallback cb)
	 *
	 * @brief This function can be used to cancel a pending request that is associated with
	 * the given ConnectionManagerReplyCallback object.
	 *
	 * @param cb - The ConnectionManagerReplyCallback to disassociate from a pending request.
	 *
	 * @throws An IllegalArgumentException is thrown if the given callback object is null.
	 *
	 * @sa void request(Message request, int timeout, ConnectionManagerReplyCallback cb, int republish_ms)
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
	 * @fn void reply(Message request, Message reply)
	 *
	 * @brief If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * Otherwise the request message will be validated before it is issued.
	 *
	 * @param request - the pending request message
	 * @param reply - the reply message to be sent
	 *
	 * @throws An IllegalArgumentException is thrown if the given request Message object is null.
	 * @throws An IllegalArgumentException is thrown if the given reply Message object is null.
	 * @throws A GMSEC_Exception is thrown if the either message fails validation, or if any other severe
	 * error occurs with issuing the reply.
	 *
	 * @sa ConnectionManager::request
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
	 * @fn void dispatch(Message msg)
	 *
	 * @brief Attempts to dispatch the given message to callbacks that have been previously
	 * registered.
	 *
	 * @param msg - message to be dispatched
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 * @throws A GMSEC_Exception is thrown if a severe error occurs while dispatching the message.
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
	 * @fn Message receive()
	 *
	 * @brief This method is a pass-through to the underlying connection.
	 * This function returns the next message available which contains a subject that
	 * has been previously subscribed to.  Until a message is received, this method
	 * will block.
	 *
	 * The received messages are determined by the %subscribe() function(s), but
	 * %dispatchMsg() needs to be called messages received from this method to
	 * ensure all registered callbacks are executed.
	 *
	 * This method <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @return The next received message.
	 *
	 * @throws A GMSEC_Exception is thrown if a severe error occurs while attempting to receive a message.
	 *
	 * @sa ConnectionManager::subscribe
	 */
	public Message receive() throws GMSEC_Exception
	{
		return receive(gmsecAPI.WAIT_FOREVER);
	}


	/**
	 * @fn Message receive(int timeout)
	 *
	 * @brief This method is a pass-through to the underlying connection.
	 * This method returns the next message received within the specified timeout.
	 *
	 * The received messages are determined by the %subscribe() function(s), but
	 * %dispatchMsg() needs to be called messages received from this function to
	 * ensure all registered callbacks are executed.
	 *
	 * This method <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @param timeout - the maximum time to block waiting for a message, in milliseconds
	 *
	 * @return The next received message, if any.  If a timeout occurs, null is returned.
	 *
	 * @throws A GMSEC_Exception is thrown if a severe error occurs while attempting to receive a message.
	 *
	 * @sa ConnectionManager::subscribe
	 */
	public Message receive(int timeout) throws GMSEC_Exception
	{
		return m_jniConnMgr.receive(timeout);
	}


	/**
	 * @fn boolean startAutoDispatch()
	 *
	 * @desc This method will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages will be dropped.
	 * If receive() is called while the auto-dispatcher is used, the behavior will be undesireable
	 * and undefined.
	 *
	 * @return True of the dispatcher started successfully; false otherwise.
	 *
	 * @throws A GMSEC_Exception is thrown if the ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::subscribe() @n
	 *     ConnectionManager::stopAutoDispatch()
	 */
	public boolean startAutoDispatch() throws GMSEC_Exception
	{
		return m_jniConnMgr.startAutoDispatch();
	}


	/**
	 * @fn boolean stopAutoDispatch()
	 *
	 * @desc This function will stop the auto dispatch thread.
	 *
	 * @return True of the dispatcher stopped successfully; false otherwise.
	 *
	 * @throws A GMSEC_Exception is thrown if the ConnectionManager has not been initialized.
	 *
	 * @sa ConnectionManager::startAutoDispatch()
	 */
	public boolean stopAutoDispatch() throws GMSEC_Exception
	{
		return m_jniConnMgr.stopAutoDispatch(true);
	}


	/**
	 * @fn boolean stopAutoDispatch(boolean waitForCompletion)
	 *
	 * @desc This function will stop the auto dispatch thread.
	 *
	 * @param waitForCompletion - boolean value to indicate whether or not to wait
	 * for the dispatcher to finish operations.
	 *
	 * @sa ConnectionManager::startAutoDispatch()
	 */
	public boolean stopAutoDispatch(boolean waitForCompletion) throws GMSEC_Exception
	{
		return m_jniConnMgr.stopAutoDispatch(waitForCompletion);
	}


	/**
	 * @fn void excludeSubject(String subject)
	 *
	 * @desc Exclude any incoming messages with the specified subject.
	 *
	 * @param subject - The subject pattern to look for in incoming messages.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 *
	 * @sa removeExcludedSubject()
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
	 * @fn void removeExcludedSubject(String subject)
	 *
	 * @desc Remove an excluded subject, allowing incoming messages with the matching
	 *  subject to once again be received.
	 *
	 * @param subject - The subject pattern to remove.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 *
	 * @sa excludeSubject()
	 */
	public void removeExcludedSubject(String subject) throws IllegalArgumentException, GMSEC_Exception
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contain an empty string");
		}

		m_jniConnMgr.removeExcludedSubject(subject);
	}


	/***************Start Heartbeat Section************************************************/


	/**
	 * @fn Message createHeartbeatMessage(String subject, java.util.List<Field> heartbeatFields)
	 *
	 * @brief This method creates a message and passes ownership to the user. This message is populated with
	 * the standard set of required and optional heartbeat fields, as well as the required common fields defined
	 * in setStandardFields(). If validation is enabled for this ConnectionManager and neither the common fields
	 * from setStandardFields(), nor the fields supplied in the first argument of this function are sufficient to
	 * complete a set of fields required by validation, an error will be returned.
	 *
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param subject - The topic (subject) that will be applied to the returned messages.
	 * @param heartbeatFields - Set of fields to be added to the heartbeat
	 *
	 * @return A C2CX Heartbeat Message
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
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
	 * @fn void startHeartbeatService(String subject, java.util.List<Field> heartbeatFields)
	 *
	 * @brief This method creates an archetypal heartbeat message from the concatenation of the standard fields
	 * applied globally to this ConnectionManager and to this function. This message will then be validated,
	 * if the "validate" flag is set. If there is an error, an Exception will be thrown. If it has not been set,
	 * the message will be published at an interval supplied by the "PUB-RATE" field regardless of validation
	 * results. If no "PUB-RATE" has been defined, the service will default to the GMSEC standard 30 second
	 * heartbeat interval.
	 *
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param subject - subject to set as the default for heartbeat messages
	 * @param heartbeatFields - Set of fields to be added to the published heartbeat messages
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws A GMSEC_Exception may be thrown if the Heartbeat Service is already running.
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
	 * @fn void stopHeartbeatService()
	 *
	 * @desc This method terminates opertaion of the heartbeat service and cleans up related resources.
	 * It will be called by the global connection manager "cleanup" if not done manually by the user.
	 */
	public void stopHeartbeatService() throws GMSEC_Exception
	{
		m_jniConnMgr.stopHeartbeatService();
	}


	/**
	 * @fn void changeComponentStatus(Field componentStatus)
	 *
	 * @brief This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an Exception is thrown.
	 *
	 * @param componentStatus - Field containing updated information for the Heartbeat Message
	 *
	 * @throws An IllegalArgumentException is thrown if the given Field is null.
	 * @throws A GMSEC_Exception is thrown if the given Field is not already present in the Heartbeat
	 * Message, or if by adding it to the Heartbeat Message, the message itself becomes invalid.
	 */
	public Status changeComponentStatus(Field componentStatus)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (componentStatus == null)
		{
			throw new IllegalArgumentException("Field is null");
		}

		return m_jniConnMgr.changeComponentStatus(componentStatus);
	}


	/**
	 * @fn void changeComponentInfo(Field componentInfo)
	 *
	 * @brief This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an Exception is thrown.
	 *
	 * @param componentInfo - Field containing updated information for the Heartbeat Message
	 *
	 * @throws An IllegalArgumentException is thrown if the given Field is null.
	 * @throws A GMSEC_Exception is thrown if the given Field is not already present in the Heartbeat
	 * Message, or if by adding it to the Heartbeat Message, the message itself becomes invalid.
	 */
	public Status changeComponentInfo(Field componentInfo)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (componentInfo == null)
		{
			throw new IllegalArgumentException("Field is null");
		}

		return m_jniConnMgr.changeComponentInfo(componentInfo);
	}


	/**
	 * @fn void changeCPUMemory(Field cpuMemory)
	 *
	 * @brief This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an Exception is thrown.
	 *
	 * @param cpuMemory - Field containing updated information for the Heartbeat Message
	 *
	 * @throws An IllegalArgumentException is thrown if the given Field is null.
	 * @throws A GMSEC_Exception is thrown if the given Field is not already present in the Heartbeat
	 * Message, or if by adding it to the Heartbeat Message, the message itself becomes invalid.
	 */
	public Status changeCPUMemory(Field cpuMemory)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (cpuMemory == null)
		{
			throw new IllegalArgumentException("Field is null");
		}

		return m_jniConnMgr.changeCPUMemory(cpuMemory);
	}


	/**
	 * @fn void changeCPUUtil(Field cpuUtil)
	 *
	 * @brief This method updates the field which is included within the Heartbeat Message being
	 * published by the Heartbeat Service.  If validatiion is enabled, the message will then be
	 * re-validated with the modified field set. Should the validation pass, the change will be
	 * persisted to the heartbeat service, otherwise an Exception is thrown.
	 *
	 * @param cpuUtil- Field containing updated information for the Heartbeat Message
	 *
	 * @throws An IllegalArgumentException is thrown if the given Field is null.
	 * @throws A GMSEC_Exception is thrown if the given Field is not already present in the Heartbeat
	 * Message, or if by adding it to the Heartbeat Message, the message itself becomes invalid.
	 */
	public Status changeCPUUtil(Field cpuUtil)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (cpuUtil == null)
		{
			throw new IllegalArgumentException("Field is null");
		}

		return m_jniConnMgr.changeCPUUtil(cpuUtil);
	}


	/***************Start Log Section*****************************************************/


	/**
	 * @fn Message createLogMessage(String subject, java.util.List<Field> logFields)
	 *
	 * @brief This method creates a Log Message and passes ownership to the user. This message is populated
	 * with the standard set of required and optional log fields, as well as the required common fields
	 * defined in setStandardFields. The message is not validated at this time, as MSG-TEXT and SEVERITY
	 * fields must be set by the user at the time the message is to be sent.
	 *
	 * This message automatically is generated with MESSAGE-TYPE and MESSAGE-SUBTYPE
	 * correctly set to the GMSEC Message Standard requirements, regardless of user
	 * input. EVENT-TIME defaults to the time which this function was invoked; the user
	 * may override this time once the message has been returned.
	 *
	 * @param subject - The topic (subject) to apply to the returned message.
	 * @param logFields - Set of fields to be added to the log message.
	 *
	 * @return A Log Message.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
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
	 * @fn void setLoggingDefaults(String subject, java.util.List<Field> logFields)
	 *
	 * @brief This method sets the standard default fields which will be added to a new log message when publishLog
	 * is called.
	 *
	 * @param subject - subject to use for log messages
	 * @param logFields - Set of fields to be added to log messages.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
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
	 * @fn void publishLog(String logMessage, Field severity)
	 *
	 * @brief This method creates a log message comprised of the global fields from the entire ConnectionManager,
	 * the fields set in setLoggingDefaults, and the log text and severity fields as supplied as arguments to
	 * this function. If validation is enabled, the message will be published. If validation is enabled, the
	 * message will be validated.  If validation of the message fails, then an Exception is thrown.
	 *
	 * This function automatically populates the message with the correct
	 * MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set automatically
	 * to the time of function invocation. 
	 *
	 * @param logMessage - the message to log
	 * @param severity - the Field object defining the severity of the log message.
	 *
	 * @throws An IllegalArgumentException is thrown if the message string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given severity Field object is null.
	 * @throws A GMSEC_Exception is thrown if the Log Message cannot be validated, or if other severe error
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


	/***************Start Directive Section************************************************/


	/**
	 * @fn requestDirective(String subject, Field directiveString, java.util.List<Field> fields)
	 *
	 * @brief This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes publish(Message msg) to place that message on the GMSEC
	 * bus. This implicitly sets the message's response flag to false, so that receivers 
	 * of the message will know a response is not necessary
	 *
	 * @param subject - subject on which to publish the message
	 * @param directiveString - a field containing the string directive that this
	 * message is intended to convey
	 * @param fields - a list of and supplemental fields that the user wishes to include
	 * with the directive message.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given directive string Field object is null.
	 * @throws A GMSEC_Exception is thrown if the message cannot be validated, or if other severe error
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
	 * @fn requestDirective(String subject, Field directiveString,
	 *		java.util.List<Field> fields, int timeout,
	 *		ConnectionManagerReplyCallback cb, int republish_ms)
	 *
	 * @brief This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes request(Message request, GMSEC_I32 timeout, 
	 * ConnectionManagerReplyCallback cb, int republish_ms to place that message
	 * on the GMSEC bus. This implicitly sets the message's response flag to true, so that
	 * receivers of the message will know a response is necessary
	 *
	 * @note Client applications should maintain a reference to their ConnectionManagerReplyCallback object
	 * when calling this method.  This will prevent the JVM garbage collector from destroying the callback
	 * before it is ever called.
	 *
	 * @param subject         - subject on which to publish the message
	 * @param directiveString - a field containing the string directive that this message is intended to convey
	 * @param fields          - a list of and supplemental fields that the user wishes to include with the directive message.
	 * @param timeout         - The time to wait before a response to the message will no longer be routed to the supplied callback
	 * @param cb              - the callback to be invoked upon reception of a response message
	 * @param republish_ms    - request message resubmission interval (in milliseconds). If set  to a negative value (eg. -1)
	 *                          it will never republish a request message.  If set to 0, the period will default to 60000ms,
	 *                          unless the user has provided an alternate time period via the Config object used to create
	 *                          the ConnectionManager object.  The minimum republish period allowed is 100ms.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given directive string Field object is null.
	 * @throws An IllegalArgumentException is thrown if the given reply callback object is null.
	 * @throws A GMSEC_Exception is thrown if the message cannot be validated, or if other severe error
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
	 * @fn Message requestDirective(String subject, Field directiveString, 
	 *			java.util.List<Field> fields, int timeout, int republish_ms)
	 *
	 * @brief This function creates a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes request(Message request, int timeout, 
	 * int republish_ms) to place that message on the GMSEC bus. This implicitly
	 * sets the message's response flag to true, so that receivers of the message will 
	 * know a response is necessary
	 *
	 * @param subject         - subject on which to publish the message
	 * @param directiveString - a field containing the string directive that this message is intended to convey
	 * @param fields          - a list of and supplemental fields that the user wishes to include with the directive message.
	 * @param timeout         - The time to wait before a response to the message will no longer be routed to the supplied callback
	 * @param republish_ms    - request message resubmission interval (in milliseconds). If set  to a negative value (eg. -1)
	 *                          it will never republish a request message.  If set to 0, the period will default to 60000ms,
	 *                          unless the user has provided an alternate time period via the Config object used to create
	 *                          the ConnectionManager object.  The minimum republish period allowed is 100ms.
	 *
	 * @return A handle to a message received in response to the directive request message. This method 
	 * will return null if a message is not received in the proscribed period.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given directive string Field object is null.
	 * @throws A GMSEC_Exception is thrown if the message cannot be validated, or if other severe error
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
	 * @fn void acknowledgeDirectiveRequest(String subject, Message request, 
	 *				gmsecMIST.ResponseStatus status, java.util.List<Field> fields)
	 *
	 * @brief This function acknowledges a Directive message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes reply(Message request, Message reply) to
	 * place that message on the GMSEC bus and route it to the original requestor
	 *
	 * @param subject - subject on which to publish the message
	 * @param request - the original directive message
	 * @param status  - an enumeration specifying the response code to the directive request.
	 * @param fields - supplemental information fields which may be included with the message
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given request Message is null.
	 * @throws A GMSEC_Exception is thrown if the message cannot be validated, or if other severe error
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
	 * @fn void publishResourceMessage(String subject, long sampleInterval, long averageInterval)
	 *
	 * @brief This function creates a valid resource message, as described in the GMSEC
	 * specification. This function then publishes the message to the GMSEC bus on the 
	 * supplied subject.
	 *
	 * @param subject - subject on which to publish the message
	 * @param sampleInterval - distance between samples (in seconds)
	 * @param averageInterval - time interval over which to make a moving average of samples (in seconds)
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws A GMSEC_Exception is thrown if a severe error occurs while attempting to
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
	 * @fn Message createResourceMessage(String subject, long sampleInterval, long averageInterval)
	 *
	 * @brief This function creates a valid resource message, as described in the GMSEC
	 * specification, and returns that message to the user.
	 *
	 * @param subject - subject with which to create the message
	 * @param sampleInterval - distance between samples (in seconds)
	 * @param averageInterval - time interval over which to make a moving average of samples (in seconds)
	 *
	 * @return A GMSEC resource Message.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
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
	 * @fn void startResourceMessageService(Sring subject, long interval, long sampleInterval, long averageInterval)
	 *
	 * @brief This function starts the resource message service, which publishes 
	 * resource messages at a fixed interval as defined by this function.
	 *
	 * @param subject - subject with which to create the message
	 * @param intervalSeconds - This the interval, in seconds, where messages are published to the bus.
	 * @param sampleInterval - This is the the time distance between samples (in seconds).
	 * @param averageInterval - time window over which to make a moving average of samples (in seconds)
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
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
	 * @fn boolean stopResourceMessageService()
	 *
	 * @brief This function stops the resource message service
	 *
	 * @return Returns true if the resource message service was running before invocation of this
	 * function, false otherwise.
	 */
	public boolean stopResourceMessageService()
	{
		return m_jniConnMgr.stopResourceMessageService();
	}


	/***************Start Simple Service Request Message Section***********************************/


	/**
	 * @fn void acknowledgeSimpleService(String subject, Message request, gmsecMIST.ResponseStatus status, java.util.List<Field> fields)
	 *
	 * @brief This function acknowledges a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes reply(Message request, Message reply) to
	 * place that message on the GMSEC bus and route it to the original requestor
	 *
	 * @param subject - subject on which to publish the message
	 * @param request - the original directive message
	 * @param status  - an enumeration specifying the response code to the simple service request.
	 * @param fields  - supplemental information fields which may be included with the message
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the request Message is null.
	 * @throws A GMSEC_Exception is thrown if the message cannot be validated, or if other severe error
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
	 * @fn void requestSimpleService(String subject, String opName, Field opNumber, java.util.List<Field> fields, java.util.List<ServiceParam> params)
	 *
	 * @brief This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes publish(Message msg) to place that message on the GMSEC
	 * bus. This implicitly sets the message's response flag to false, so that receivers 
	 * of the message will know a response is not necessary
	 *
	 * @param subject  - subject on which to publish the message
	 * @param opName   - the name of the operation of the service
	 * @param opNumber - the operation number for the service message is intended to convey
	 * @param fields   - a list of supplemental fields that the user wishes to include with the directive message.
	 * @param params   - a list of the ServiceParam objects providing meta data for this service invocation
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the operation name string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the operation number Field is null.
	 * @throws A GMSEC_Exception is thrown if the message cannot be validated, or if other severe error
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
	 * @fn void requestSimpleService(String subject, String opName, Field opNumber,
	 *				java.util.List<Field> fields, java.util.List<ServiceParam> params,
	 *				int timeout, ConnectionManagerReplyCallback cb, int republish_ms)
	 *
	 * @brief This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes request(Message request, int timeout, 
	 * ConnectionManagerReplyCallback* cb, GMSEC_I32 republish_ms = 0) to place that message
	 * on the GMSEC bus. This implicitly sets the message's response flag to true, so that
	 * receivers of the message will know a response is necessary
	 *
	 * @note Client applications should maintain a reference to their ConnectionManagerReplyCallback object
	 * when calling this method.  This will prevent the JVM garbage collector from destroying the callback
	 * before it is ever called.
	 *
	 * @param subject      - subject on which to publish the message
	 * @param opName       - the name of the operation of the service
	 * @param opNumber     - the operation number for the service message is intended to convey
	 * @param fields       - a list of and supplemental fields that the user wishes to include with the directive message.
	 * @param params       - a list of the ServiceParam objects providing meta data for this service invocation
	 * @param timeout      - The time to wait before a response to the message will no longer be routed to the supplied callback
	 * @param cb           - the callback to be invoked upon reception of a response message
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set  to a negative value (eg. -1)
	 *                       it will never republish a request message.  If set to 0, the period will default to 60000ms,
	 *                       unless the user has provided an alternate time period via the Config object used to create
	 *                       the ConnectionManager object.  The minimum republish period allowed is 100ms.
	 *
	 * @throws An IllegalArgumentException is thrown if the given subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given operation name string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given operation number Field is null.
	 * @throws An IllegalArgumentException is thrown if the given reply callback is null.
	 * @throws A GMSEC_Exception is thrown if the message cannot be validated, or if other severe error
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
	 * @fn Message requestSimpleService(String subject, String opName, Field opNumber,
	 *					java.util.List<Field> fields, java.util.List<ServiceParam> params,
	 *					int timeout, int republish_ms)
	 *
	 * @brief This function creates a Simple Service message, per the description in the GMSEC
	 * interface specification document. The message is populated with the standard
	 * fields for this connection, as well as required versioning and message description
	 * information as described in the GMSEC interface specification document.
	 *
	 * This function then invokes request(const Message& request, GMSEC_I32 timeout, 
	 * GMSEC_I32 republish_ms = 0) to place that message on the GMSEC bus. This implicitly
	 * sets the message's response flag to true, so that receivers of the message will 
	 * know a response is necessary
	 *
	 * @param subject      - subject on which to publish the message
	 * @param opName       - the name of the operation of the service
	 * @param opNumber     - the operation number for the service message is intended to convey
	 * @param fields       - a list of and supplemental fields that the user wishes to include with the directive message.
	 * @param params       - a list of the ServiceParam objects providing meta data for this service invocation
	 * @param timeout      - The time to wait before a response to the message will no longer be routed to the supplied callback
	 * @param republish_ms - request message resubmission interval (in milliseconds). If set  to a negative value (eg. -1)
	 *                       it will never republish a request message.  If set to 0, the period will default to 60000ms,
	 *                       unless the user has provided an alternate time period via the Config object used to create
	 *                       the ConnectionManager object.  The minimum republish period allowed is 100ms.
	 *
	 * @return A handle to a message received in response to the directive request message. This method
	 * will return null if a message is not received in the proscribed period.
	 *
	 * @throws An IllegalArgumentException is thrown if the given subject string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given operation name string is null, or contains an empty-string.
	 * @throws An IllegalArgumentException is thrown if the given operation number Field is null.
	 * @throws A GMSEC_Exception is thrown if the message cannot be validated, or if other severe error
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
}
