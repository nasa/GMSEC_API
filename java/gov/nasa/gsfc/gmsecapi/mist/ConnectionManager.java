/*
 * Copyright 2007-2021 United States Government as represented by the
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
 */

package gov.nasa.gsfc.gmsecapi.mist;

import gov.nasa.gsfc.gmsecapi.gmsecAPI;

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;

import gov.nasa.gsfc.gmsecapi.jni.JNIConnectionManager;


/** @class ConnectionManager
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This class wraps the GMSEC Connection object, supplying both normal
 * middleware interaction functions and some additional quality of service. 
 * This is accomplished through standard services for producing Log and 
 * Heartbeat messages, as well as functionality for validating messages. 
 * 
 * If the object is configured to validate messages, all messages sent 
 * from this ConnectionManager via either Publish or Request will be compared
 * to an available schema definition. If a definition for the message in question
 * is available, it will be validated. The message will not be sent over the
 * middleware if validation fails. 
 *
 * The following messages are currently supported, and the Fields required for 
 * the ConnectionManager to recognize them are listed as well.
 *
 * GMSEC Heartbeat Message
 * MESSAGE-TYPE = "MSG"
 * MESSAGE-SUBTYPE = "C2CX"
 * C2CX-SUBTYPE = "HB"
 *
 * GMSEC Log Message
 * MESSAGE-TYPE = "MSG"
 * MESSAGE-SUBTYPE = "LOG"
 *
 * @sa Connection @n
 *      Config
 */
@Deprecated
public class ConnectionManager
{
	private JNIConnectionManager m_jniConnMgr = null;


	/** @fn ConnectionManager(Config config, int version, boolean validate)
	 *
	 * @brief Constructor - Initializes the ConnectionManager instance without
	 * attempting a connection to the middleware.
	 *
	 * @param cfg - The config object used to construct the underlying 
	 * GMSEC connection object
	 * @param version - the version of the GMSEC message specification to be used
	 * in validating messages
	 * @param validate - boolean flag to indicate whether the ConnectionManager
	 * should validate messages produced
	 */
	public ConnectionManager(Config config, int version, boolean validate)
	{
		if (config != null)
		{
			m_jniConnMgr = new JNIConnectionManager(this, config, version, validate);
		}
	}


	/** @fn ConnectionManager(Config config)
	 *
	 * @brief Constructor - Initializes the ConnectionManager instance without
	 * attempting a connection to the middleware.
	 *
	 * @param config - The config object used to construct the underlying 
	 * GMSEC connection object. Values are pulled from the config object to determine
	 * whether or not validation is required and which specification version should
	 * be used. The config name for validation is "GMSEC-MSG-CONTENT-VALIDATE" and the
	 * name for specification version is "GMSEC-SPECIFICATION-VERSION". The default
	 * value for specification if one is not specified is the "CURRENT" specification
	 * and the default value for the validation flag is "false".
	 */
	public ConnectionManager(Config config)
	{
		if (config != null)
		{
			m_jniConnMgr = new JNIConnectionManager(this, config);
		}
	}


	/** @fn SetStandardFields(Field[] standardFields)
	 *
	 * @brief SetStandardFields - SetStandardFields sets the internal list of fields
	 * which are to be automatically placed in all messages sent by this ConnectionManager. 
	 * Internal copies of the Fields are made, ownership is not retained by the 
	 * ConnectionManager. The supplied set of fields will not be validated here, validation
	 * occurs at the time a message is to be published. 
	 *
	 * @param standardFields - The array of fields to be copied to the internal
	 * set of fields to be appended to all messages.
	 */
	public Status SetStandardFields(Field[] standardFields)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (standardFields == null || standardFields.length == 0)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Standard Field is null or is empty");
		}

		return m_jniConnMgr.SetStandardFields(standardFields);
	}


	/** @fn Initialize()
	 *
	 * @brief Initialize - Uses the config object supplied in the 
	 * constructor to establish a connection with the defined GMSEC middleware server.
	 * The underlying connection object is created and connected in one operation, 
	 * returning an error status if either operation is a failure. Once this call
	 * successfully returns, the ConnectionManager is ready for messagin operations.
	 *
	 * Behind the scenes, the ConnectionManager is calling the
	 * ConnectionFactory::CreateConnection function and Connection::Connect.
	 * If either operation fails, the results will be propagated to the user.
	 *
	 * @sa Connection.Connect() @n
	 */
	public Status Initialize()
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		return m_jniConnMgr.Initialize();
	}


	/** @fn Cleanup()
	 *
	 * @brief Cleanup - This function disconnects and destroys the 
	 * underlying Connection object, returning an error status if this operation is
	 * not successful.
	 *
	 * Behind the scenes, the ConnectionManager is calling the
	 * Connection::Disconnect and ConnectionFactory::DestroyConnection function.
	 * If either operation fails, the results will be propagated to the user.
	 *
	 * @sa Connection.Disconnect() @n
	 */
	public Status Cleanup()
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		return m_jniConnMgr.Cleanup();
	}


	/** @fn RegisterErrorCallback(String event, ConnectionManagerErrorCallback cb)
	 *
	 * @brief This is a pass-through function to the underlying connection.
	 *
	 * @param event - name of event to register
	 * @param cb - object derrived from ConnectionManagerErrorCallback to register for this error event
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.RegisterErrorCallback(String event, ConnectionManagerErrorCallback cb) @n
	 */
	public Status RegisterErrorCallback(String event, ConnectionManagerErrorCallback cb)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (event == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Event string is null");
		}

		if (cb == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Error Callback is null");
		}

		return m_jniConnMgr.RegisterErrorCallback(event, cb);
	}


	/** @fn Subscribe(String subject)
	 *
	 * @brief This is a pass-through function to the underlying Connection.
	 *
	 * @param subject - subject pattern to match received messages
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.Subscribe(String subject) @n
	 */
	public Status Subscribe(String subject)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		return m_jniConnMgr.Subscribe(subject);
	}


	/** @fn Subscribe(String subject, ConnectionManagerCallback cb)
	 *
	 * @brief This is a pass-through function to the underlying connection.
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 * @return status - result of the connection operation
	 */
	public Status Subscribe(String subject, ConnectionManagerCallback cb)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		if (cb == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Callback is null");
		}

		return m_jniConnMgr.Subscribe(subject, cb);
	}


	/** @fn Subscribe(String subject, Config config)
	 *
	 * @brief This is a pass-through function to the underlying connection.
	 *
	 * @param subject - subject pattern to match received messages
	 * @param config - config object to be used for subscription operation
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.Subscribe(String subject, Config config) @n
	 */
	public Status Subscribe(String subject, Config config)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		if (config == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Config is null");
		}

		return m_jniConnMgr.Subscribe(subject, config);
	}


	/** @fn UnSubscribe(String subject)
	 *
	 * @brief This is a pass-through function to the underlying connection.
	 *
	 * @param subject - subject pattern that was used to match received messages
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.UnSubscribe(String subject) @n
	 */
	public Status UnSubscribe(String subject)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		return m_jniConnMgr.UnSubscribe(subject);
	}


	/** @fn UnSubscribe(String subject, ConnectionManagerCallback cb)
	 *
	 * @brief This is a pass-through function to the underlying connection.
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 * @return status - result of the connection operation
	 */
	public Status UnSubscribe(String subject, ConnectionManagerCallback cb)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		if (cb == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Callback is null");
		}

		return m_jniConnMgr.UnSubscribe(subject, cb);
	}


	/** @fn CreateMessage(String subject, int kind, Message msg)
	 *
	 * @brief This function creates a message via a pass-through to the underlying
	 * connection, and then pre-populations that message with any fields set in the
	 * via SetStandardFields. The message is NOT validated at that time.
	 *
	 * @param subject - The GMSEC subject to be used for the message.
	 * @param kind - The GMSEC message kind
	 * @param msg - The pointer which will be modified to point to the created
	 * GMSEC message object
	 *
	 * @sa Connection.CreateMessage(String subject, int kind, Message msg)
	 */
	public Status CreateMessage(String subject, int kind, Message msg)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		return m_jniConnMgr.CreateMessage(subject, kind, msg);
	}


	/** @fn ConvertMessage(Message in, Message out)
	 *
	 * @brief This is a pass-through function to the underlying connection.
	 *
	 * @param in - message from which to convert
	 * @param out - out parameter filled with the coverted message (could be the same if conversion wasn't necessary)
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.ConvertMessage(Message in, Message out) @n
	 */
	public Status ConvertMessage(Message in, Message out)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (in == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "In-Message is null");
		}

		if (out == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Out-Message is null");
		}

		return m_jniConnMgr.ConvertMessage(in, out);
	}


	/** @fn ConvertCleanup(Message in, Message out)
	 *
	 * @brief This is a pass-through function to the underlying connection.
	 *
	 * @param in - original message from which the coversion was made
	 * @param out - message that was output by the ConvertMessage() call
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.ConvertCleanup(Message in, Message out) @n
	 */
	public Status ConvertCleanup(Message in, Message out)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (in == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "In-Message is null");
		}

		if (out == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Out-Message is null");
		}

		return m_jniConnMgr.ConvertCleanup(in, out);
	}


	/** @fn CloneMessage(Message in, Message out)
	 *
	 * @brief This is a pass-through function to the underlying connection.
	 *
	 * @param in - message to be cloned
	 * @param out - cloned message
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.CloneMessage(Message in, Message out) @n
	 */
	public Status CloneMessage(Message in, Message out)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (in == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "In-Message is null");
		}

		if (out == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Out-Message is null");
		}

		return m_jniConnMgr.CloneMessage(in, out);
	}


	/** @fn DestroyMessage(Message msg)
	 *
	 * @brief This function destroys a message and cleans up any associated memory.
	 *
	 * @param msg - message to be destroyed
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.DestroyMessage(Message msg) @n
	 */
	public Status DestroyMessage(Message msg)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		return m_jniConnMgr.DestroyMessage(msg);
	}


	/** @fn Publish(Message msg)
	 * @brief Publish - If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * However, if this BaseConnectionManager is set to validate its messages, the message
	 * will be validated. If a valid schema definition is available for the type of message
	 * and an error is detected during validation, an error Status will be returned.
	 *
	 * Any Connection errors occuring during the attempt to Publish will also
	 * be passed through.
	 *
	 * @param msg - The GMSEC message to be published.
	 *
	 * @sa Connection.Publish(Message msg) @n
	 */
	public Status Publish(Message msg)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		return m_jniConnMgr.Publish(msg);
	}


	/** @fn Publish(Message msg, Config config)
	 *
	 * @brief Publish - If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * However, if this BaseConnectionManager is set to validate its messages, the message
	 * will be validated. If a valid schema definition is available for the type of message
	 * and an error is detected during validation, an error Status will be returned.
	 *
	 * @param msg - The GMSEC message to be published
	 * @param config - config object to be used by the publish operation
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.Publish(Message msg, Config config) @n
	 */
	public Status Publish(Message msg, Config config)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		if (config == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Config is null");
		}

		return m_jniConnMgr.Publish(msg, config);
	}


	/** @fn Request(Message request, int timeout, ConnectionManagerReplyCallback cb)
	 *
	 * @brief Request - If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * However, if this BaseConnectionManager is set to validate its messages, the message
	 * will be validated. If a valid schema definition is available for the type of message
	 * and an error is detected during validation, an error Status will be returned.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param cb - ConnectionManagerReplyCallback to call when reply is receieved
	 * @return status - result of the request operation
	 *
	 * @sa Connection.Request(Message request, int timeout, RequestCallback cb) @n
	 */
	public Status Request(Message request, int timeout, ConnectionManagerReplyCallback cb)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (request == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Request message is null");
		}

		if (cb == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Reply Callback is null");
		}

		return m_jniConnMgr.Request(request, timeout, cb);
	}


	/** @fn Request(Message request, int timeout, Message reply)
	 *
	 * @brief Request - If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * However, if this BaseConnectionManager is set to validate its messages, the message
	 * will be validated. If a valid schema definition is available for the type of message
	 * and an error is detected during validation, an error Status will be returned.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param reply - out parameter reply message if received
	 * (eg. -1) it will never republish a request message. If set to 0 it will default to the user-specified default
	 * value supplied at the Connection object's creation time.
	 * @return status - result of the request operation
	 *
	 * @sa Connection.Request(Message request, int timeout, Message reply) @n
	 */
	public Status Request(Message request, int timeout, Message reply)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (request == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Request message is null");
		}

		if (reply == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Reply message is null");
		}

		return m_jniConnMgr.Request(request, timeout, reply);
	}


	/** @fn Reply(Message request, Message reply)
	 *
	 * @brief Reply - If this connection manager has been created with "validate"
	 * option disabled, this is a pass-through function to the underlying connection.
	 * However, if this BaseConnectionManager is set to validate its messages, the message
	 * will be validated. If a valid schema definition is available for the type of message
	 * and an error is detected during validation, an error Status will be returned.
	 *
	 * @param request - the recieved request that we are responding to
	 * @param reply - the reply to be sent
	 * (eg. -1) it will never republish a request message. If set to 0 it will default to the user-specified default
	 * value supplied at the Connection object's creation time.
	 * @return status - result of the reply operation
	 *
	 * @sa Connection.Reply(Message request, Message reply) @n
	 */
	public Status Reply(Message request, Message reply)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (request == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Request message is null");
		}

		if (reply == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Reply message is null");
		}

		return m_jniConnMgr.Reply(request, reply);
	}


	/** @fn StartAutoDispatch()
	 *
	 * @brief This function is a pass-through function to the underlying connection.
	 * It will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped.
	 * If GetNextMessage() is called while the auto-dispatcher is used, the behavior will be undesireable
	 * and undefined.
	 *
	 * @return status - result of the start operation
	 *
	 * @sa Connection.StartAutoDispatch() @n
	 */
	public Status StartAutoDispatch()
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		return m_jniConnMgr.StartAutoDispatch();
	}


	/** @fn StopAutoDispatch()
	 *
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @return status - result of the stop operation
	 *
	 * @sa Connection.StopAutoDispatch() @n
	 */
	public Status StopAutoDispatch()
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		return m_jniConnMgr.StopAutoDispatch();
	}


	/** @fn StopAutoDispatch(boolean waitForComplete)
	 *
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @param waitForComplete - Signal the function whether or not to wait
	 * for the dispatcher to finish operations.
	 * @return status - result of the stop operation
	 *
	 * @sa Connection.StopAutoDispatch(bool waitForComplete) @n
	 */
	public Status StopAutoDispatch(boolean waitForCompletion)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		return m_jniConnMgr.StopAutoDispatch(waitForCompletion);
	}


	/** @fn GetNextMsg(Message msg, int timeout)
	 *
	 * @brief This function is a pass-through function to the underlying connection.
	 * This function returns the next message received within the specified timeout.
	 * The received messages are determined by the %Subscribe() function(s), but
	 * %DispatchMsg() needs to be called messages received from this function to
	 * ensure all registered callbacks are executed. @n
	 * @n
	 * This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @param msg - out parameter, the next received message, if any
	 * @param timeout - the maximum time to block waiting for a message, in milliseconds
	 * @return status - result of the operation
	 *
	 * @sa Connection.GetNextMsg(Message msg, int timeout) @n
	 */
	public Status GetNextMsg(Message msg, int timeout)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		return m_jniConnMgr.GetNextMsg(msg, timeout);
	}


	/** @fn DispatchMsg(Message msg)
	 *
	 * @brief This is a pass-through function to the underlying Connection.
	 *
	 * @param msg - message to be dispatched
	 * @return status - result of the operation
	 *
	 * @sa Connection.DispatchMsg(Message msg)
	 */
	public Status DispatchMsg(Message msg)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		return m_jniConnMgr.DispatchMsg(msg);
	}


	/** @fn GetLastDispatcherStatus()
	 *
	 * @brief This is a pass-through function to the underlying Connection.
	 *
	 * @sa Connection.GetLastDispatchStatus() @n
	 *
	 */
	public Status GetLastDispatcherStatus()
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		return m_jniConnMgr.GetLastDispatcherStatus();
	}


	/** @fn ExcludeSubject(String subject)
	 *
	 * @brief This is a pass-through function to the underlying Connection.
	 *
	 * @sa Connection.ExcludeSubject(String subject) @n
	 *
	 * @return a Status representing the success of the operation.
	 * 	The Status will contain a GMSEC_INVALID_SUBJECT_NAME
	 * 	error if the subject provided was null.
	 *
	 */
	public Status ExcludeSubject(String subject)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_jniConnMgr.ExcludeSubject(subject);
	}


	/** @fn RemoveExcludedSubject(String subject)
	 *
	 * @brief This is a pass-through function to the underlying Connection.
	 *
	 * @sa Connection.RemoveExcludedSubject(String subject) @n
	 *
	 * @return a Status representing the success of the operation.
	 * 	The Status will contain a GMSEC_INVALID_SUBJECT_NAME
	 * 	error if the subject provided was null.
	 *
	 */
	public Status RemoveExcludedSubject(String subject)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_jniConnMgr.RemoveExcludedSubject(subject);
	}


	/** @fn GetHeartbeatMessage(Field[] standardFields, Message msg, String subject)
	 *
	 * @brief GetHeartbeatMessage - GetHeartbeatMessage creates a message and passes
	 * ownership to the user. This message is populated with the standard set of required
	 * and optional heartbeat fields, as well as the required common fields defined in
	 * SetStandardFields. If validation is enabled for this ConnectionManager and neither
	 * the common fields from SetStandardFields, nor the fields supplied in the first
	 * argument of this function are sufficient to complete a set of fields required by
	 * validation, an error will be returned.
	 *
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param standardFields - Set of fields to be added to the heartbeat
	 * @param msg - The pointer to be modified to point to the new message
	 * @param subject - The topic (subject) to include with the heartbeat message
	 */
	public Status GetHeartbeatMessage(Field[] standardFields, Message msg, String subject)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		return m_jniConnMgr.GetHeartbeatMessage(standardFields, msg, subject);
	}


	/** @fn StartHeartbeatService(Field[] standardFields, String subject)
	 *
	 * @brief StartHeartbeatService - This function creates an archetypal 
	 * heartbeat message from the concatenation of the standard fields
	 * applied globally to this Connection Manager and to this function. This
	 * message will then be validated, if the "validate" flag is set. If there
	 * is an error, an error status will be returned. If it has
	 * not been set, the message will be published at an interval supplied
	 * by the "PUB-RATE" field regardless of validation results. If no "PUB-RATE"
	 * has been defined, the service will default to the GMSEC standard 30 second
	 * heartbeat interval.
	 *
	 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be generated and
	 * added to the message automatically, according to the GMSEC Message Standard
	 *
	 * @param standardFields - Set of fields to be added to the heartbeat
	 * @param subject - subject to set as the default for heartbeat messages
	 */
	public Status StartHeartbeatService(Field[] standardFields, String subject)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		return m_jniConnMgr.StartHeartbeatService(standardFields, subject);
	}


	/** @fn ChangeComponentStatus(Field componentStatus)
	 *
	 * @brief ChangeComponentStatus - This function updates the field which the heartbeat
	 * service uses to indicate component status. If validate is enabled, the message
	 * will then be re-validated with the modified field set. Should the validation
	 * pass, the change will be persisted to the heartbeat service, otherwise
	 * an error will be returned.
	 *
	 * @param componentStatus - Field representing the component status
	 */
	public Status ChangeComponentStatus(Field componentStatus)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (componentStatus == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Component Status field is null");
		}

		return m_jniConnMgr.ChangeComponentStatus(componentStatus);
	}


	/** @fn ChangeComponentInfo(Field componentInfo)
	 *
	 * @brief ChangeComponentInfo - This function updates the field which the heartbeat
	 * service uses to indicate component information. If validate is enabled, the message
	 * will then be re-validated with the modified field set. Should the validation
	 * pass, the change will be persisted to the heartbeat service, otherwise
	 * an error will be returned.
	 *
	 * @param componentInfo - Field representing the component information
	 */
	public Status ChangeComponentInfo(Field componentInfo)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (componentInfo == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Component Info field is null");
		}

		return m_jniConnMgr.ChangeComponentInfo(componentInfo);
	}


	/** @fn ChangeCPUMemory(Field cpuMemory)
	 *
	 * @brief ChangeCPUMemory - This function updates the field which the heartbeat
	 * service uses to indicate CPU memory. If validate is enabled, the message
	 * will then be re-validated with the modified field set. Should the validation
	 * pass, the change will be persisted to the heartbeat service, otherwise
	 * an error will be returned.
	 *
	 * @param cpuMemory - Field representing the CPU memory
	 */
	public Status ChangeCPUMemory(Field cpuMemory)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (cpuMemory == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "CPU Memory field is null");
		}

		return m_jniConnMgr.ChangeCPUMemory(cpuMemory);
	}


	/** @fn ChangeCPUUtil(Field cpuUtil)
	 *
	 * @brief ChangeCPUUtil - This function updates the field which the heartbeat
	 * service uses to indicate CPU utilization. If validate is enabled, the message
	 * will then be re-validated with the modified field set. Should the validation
	 * pass, the change will be persisted to the heartbeat service, otherwise
	 * an error will be returned.
	 *
	 * @param cpuUtil - Field representing the CPU utilization
	 */
	public Status ChangeCPUUtil(Field cpuUtil)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (cpuUtil == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "CPU Util field is null");
		}

		return m_jniConnMgr.ChangeCPUUtil(cpuUtil);
	}


	/** @fn StopHeartbeatService()
	 *
	 * @brief StopHeartbeatService - This function terminates opertaion of the heartbeat
	 * service and cleans up related resources. It will be called by the global
	 * connection manager "cleanup" if not done manually by the user.
	 */
	public Status StopHeartbeatService()
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		return m_jniConnMgr.StopHeartbeatService();
	}


	/** @fn GetLogMessage(Field[] standardFields, Message msg, String subject)
	 *
	 * @brief GetLogMessage - GetLogMessage creates a message and passes
	 * ownership to the user. This message is populated with the standard set of required
	 * and optional log fields, as well as the required common fields defined in
	 * SetStandardFields. The message is not validated at this time, as MSG-TEXT and SEVERITY
	 * fields must be set by the user at the time the message is to be sent.
	 *
	 * This message automatically is generated with MESSAGE-TYPE and MESSAGE-SUBTYPE
	 * correctly set to the GMSEC Message Standard requirements, regardless of user
	 * input. EVENT-TIME defaults to the time which this function was invoked; the user
	 * may override this time once the message has been returned.
	 *
	 * @param standardFields - Set of fields to be added to the heartbeat
	 * @param msg - The pointer to be modified to point to the new message
	 * @param subject - The topic (subject) to include with the log message
	 */
	public Status GetLogMessage(Field[] standardFields, Message msg, String subject)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		return m_jniConnMgr.GetLogMessage(standardFields, msg, subject);
	}


	/** @fn SetLoggingDefaults(Field[] standardFields, String subject)
	 *
	 * @brief SetLoggingDefaults - This function sets the standard default
	 * fields which will be added to a new log message as part of "QuickLog"
	 *
	 * @param standardFields - Set of fields to be added to the heartbeat
	 * @param subject - subject to use for log messages
	 */
	public Status SetLoggingDefaults(Field[] standardFields, String subject)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Subject string is null");
		}

		return m_jniConnMgr.SetLoggingDefaults(standardFields, subject);
	}


	/** @fn QuickLog(String msg, Field severity)
	 *
	 * @brief QuickLog - This function creates a log message comprised of the 
	 * global fields from the entire ConnectionManager, the fields set in
	 * "SetLoggingDefaults", and the log text and severity fields as supplied
	 * as arguments to this function. If validation is enabled, the message
	 * will be published. If validation is enabled, the message will be validated,
	 * returning an argument if validation is not successful, before it
	 * is published.
	 *
	 * This function automatically populates the message with the correct
	 * MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set automatically
	 * to the time of function invocation. 
	 *
	 * @param msg - the message to log
	 * @param severity - the Field object defining the severity of the log message.
	 */
	public Status QuickLog(String msg, Field severity)
	{
		if (m_jniConnMgr == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INITIALIZATION_ERROR, "ConnectionManager has not been properly initialized");
		}

		if (msg == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Message is null");
		}

		if (severity == null)
		{
			return new Status(gmsecAPI.GMSEC_GENERAL_MIST_ERROR, gmsecAPI.GMSEC_INVALID_VALUE, "Severity field is null");
		}

		return m_jniConnMgr.QuickLog(msg, severity);
	}
}
