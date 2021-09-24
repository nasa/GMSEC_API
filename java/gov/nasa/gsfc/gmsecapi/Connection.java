/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

import gov.nasa.gsfc.gmsecapi.jni.JNIConnection;


/** @class Connection
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief The connection class provides services available on an implemented GMSEC
 * connection.  An application can create multiple connection objects with
 * different parameters and different middleware.  Creating multiple connections
 * to the same middleware is not prevented but may not be supported by all
 * middleware implementations.
 *
 * Example creation and use:
 * @code
 * Connection conn = null;
 *
 * //Create config from command line arguments
 * Config cfg = new Config(argc, argv);
 *
 * //Create the Connection
 * result = ConnectionFactory.Create(cfg, conn);
 * if( result.isError() ) 
 *  //handle error
 *
 * //Establish the connection
 * result = conn.Connect();
 * if( result.isError() )
 *  //handle error
 * @endcode
 *
 * @sa ConnectionFactory @n
 *      Config
 */
@Deprecated
public class Connection
{
	private JNIConnection m_JNIconn = null;


	public JNIConnection getImpl()
	{
		return m_JNIconn;
	}


	public void setImpl(JNIConnection JNIconn)
	{
		m_JNIconn = JNIconn;
	}


	/** @fn Connect()
	 * @brief This function establishes this connection to the middleware
	 *
	 * @return status - result of the connection operation
	 */
	public Status Connect()
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Connect();
	}


	/** @fn Disconnect()
	 * @brief This function terminates this connection to the middleware.
	 *
	 * @return status - result of the connection operation
	 */
	public Status Disconnect()
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Disconnect();
	}


	/** @fn IsConnected()
	 * @brief This function returns t/f whether the connection has
	 * been established. Note that this function will return true even if the
	 * connection has been broken and the client is attempting to reconnect,
	 * it simply reflects whether or not a Connect() call has been made on this
	 * connection.
	 *
	 * @return true if connected
	 */
	public boolean IsConnected()
	{
		return (m_JNIconn == null ? false : m_JNIconn.IsConnected());
	}


	/** @fn Subscribe(String subject)
	 * @brief This function subscribes to a particular subject or pattern. This
	 * causes middleware routing of messages that match that subject or pattern
	 * be queued for this process. Messages that are subscribed to without callback
	 * need to be pulled from the queue using GetNextMsg() and are thrown away if
	 * the auto-dispatcher is used.
	 *
	 * Example subscription patterns:
	 * @code
	 * // this will match only messages with this exact subject
	 * conn.Subscribe("GMSEC.MISSION.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages with any mission
	 * conn.Subscribe("GMSEC.*.CONST.SAT.EVT.MSG");
	 *
	 * // this will match messages that have AT LEAST ONE MORE TAG
	 * //   (will not match "GMSEC.MISSION.CONST.SAT")
	 * conn.Subscribe("GMSEC.MISSION.CONST.SAT.>");
	 *
	 * // this will match any event message
	 * conn.Subscribe("GMSEC.*.*.*.EVT.>");
	 * @endcode
	 *
	 * @note Although subscription behavior is outlines as above, the actual behavior for a particular
	 * middleware implementation MAY deviate from this behavior slightly.
	 *
	 * @param subject - subject pattern to match received messages
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.GetNextMsg(Message msg, int timeout) @n
	 *     Connection.StartAutoDispatch() @n
	 *     Connection.StopAutoDispatch()
	 */
	public Status Subscribe(String subject)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Subscribe(subject);
	}


	/** @fn Subscribe(String subject, Callback cb)
	 * @brief This function subscribes to a particular subject or pattern and
	 * associates a callback to be called when messages matching the subject
	 * or pattern are received. If all subscriptions are performed using this
	 * function then the auto-dispatcher can be used to asynchronously receive
	 * messages. If GetNextMsg() is used to pull messages then DispatchMsg()
	 * will need to be called to ensure registered Callbacks are called.
	 *
	 * <b>see Connection::Subscribe(String subject) for an explaination of subscription patterns</b>
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.Subscribe(String subject) @n
	 *     Connection.GetNextMsg(Message msg, int timeout) @n
	 *     Connection.DispatchMsg(Message msg) @n
	 *     Connection.StartAutoDispatch() @n
	 *     Connection.StopAutoDispatch()
	 */
	public Status Subscribe(String subject, Callback cb)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Subscribe(subject, cb);
	}


	/** @fn UnSubscribe(String subject)
	 * @brief This function unsubscribes to a particular subject pattern, and will stop the reception
	 * of messages that match this pattern. It will also remove the registration of any callbacks with
	 * this subject pattern.
	 *
	 * @param subject - subject pattern that was used to match received messages
	 * @return status - result of the connection operation
	 *
	 * @note This function will only unsubscribe from the subscription matching the string literal
	 * that is passed into it. It will NOT unsubscribe from matching sub-subjects.
	 *
	 * @sa Connection.Subscribe(String subject) @n
	 *     Connection.GetNextMsg(Message msg, int timeout) @n
	 *     Connection.DispatchMsg(Message msg) @n
	 *     Connection.StartAutoDispatch() @n
	 *     Connection.StopAutoDispatch()
	 */
	public Status UnSubscribe(String subject)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.UnSubscribe(subject);
	}


	/** @fn UnSubscribe(String subject, Callback cb)
	 * @brief This function unsubscribes a single callback to a particular subject pattern. It will
	 * prevent a particular callback from being called by the auto-dispatch or DispatchMsg().
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 * @return status - result of the connection operation
	 *
	 * @note If there are multiple subscriptions registered to the specified subject, then messages
	 * will still be received on said subject after calling this function. The user will need to call
	 * UnSubscribe(String subject) in order to unsubscribe from the subject.
	 *
	 * @sa Connection.Subscribe(String subject) @n
	 *     Connection.GetNextMsg(Message msg, int timeout) @n
	 *     Connection.DispatchMsg(Message msg) @n
	 *     Connection.StartAutoDispatch() @n
	 *     Connection.StopAutoDispatch()
	 */
	public Status UnSubscribe(String subject, Callback cb)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}
		if (cb == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_INVALID_CALLBACK, "Callback is NULL");
		}

		return m_JNIconn.UnSubscribe(subject, cb);
	}


	/** @fn CreateMessage(Message msg)
	 * @brief This function creates a message for this particular middleware connection.
	 * The kind & subject are set to defaults dependent upon the particular middleware implementation.
	 * 
	 * @note The newly created message MUST BE CLEANED UP with DestroyMessage() by the client application.
	 *
	 * @param msg - Message object to be filled by created message
	 * @return status - result of the connection operation
	 */
	public Status CreateMessage(Message msg)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.CreateMessage(msg);
	}


	/** @fn CreateMessage(String subject, int msgKind, Message msg)
	 * @brief This function creates a message for this particular middleware connection.
	 * The subject name for this call MUST be a valid subject name and NOT a pattern.
	 *
	 * @note The newly created message MUST BE CLEANED UP with DestroyMessage() by the client application.
	 *
	 * @param subject - subject under which this message will eventually be published/requested
	 * @param msgKind - indentifier for the intended message kind
	 * @param msg - Message object to be filled by created message
	 * @return status - result of the connection operation
	 */
	public Status CreateMessage(String subject, int msgKind, Message msg)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.CreateMessage(subject, msgKind, msg);
	}


	/** @fn CreateMessage(String subject, int msgKind, Message msg, Config config)
	 * @brief The Config object can be used to set Subject, Kind or middleware specific settings.
	 * The options are specific to middleware implementation and are listed in the <i>GMSEC User's Guide</i>
	 *
	 * @note The newly created message MUST BE CLEANED UP with DestroyMessage() by the client application.
	 *
	 * @param subject - subject under which this message will eventually be published/requested
	 * @param msgKind - indentifier for the intended message kind
	 * @param msg - Message object to be filled by created message
	 * @param config - Config object used to pass parameters to the middleware specific message implementation
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.CreateMessage(String subject, int msgKind, Message msg)
	 */
	public Status CreateMessage(String subject, int msgKind, Message msg, Config cfg)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.CreateMessage(subject, msgKind, msg, cfg);
	}


	/** @fn CloneMessage(Message in, Message out)
	 * @brief This function copies a message without knowing what type it is. This
	 * function can be used to copy a message's contents.
	 *
	 * @note The 'out' message MUST BE CLEANED UP with DestroyMessage() by the client application.
	 *
	 * @param in - message to be cloned
	 * @param out - cloned message
	 * @return status - result of the connection operation
	 *
	 * @sa Connection.DestroyMessage(Message msg)
	 */
	public Status CloneMessage(Message in, Message out)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.CloneMessage(in, out);
	}


	/** @fn DestroyMessage(Message msg)
	 * @brief This function destroys a message and cleans up any associated memory.
	 *
	 * @param msg - message to be destroyed
	 * @return status - result of the connection operation
	 */
	public Status DestroyMessage(Message msg)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.DestroyMessage(msg);
	}


	/** @fn GetNextMsg(Message msg, int timeout)
	 * @brief This function returns the next message received within the specified timeout.
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
	 * @sa Connection.Subscribe(String subject) @n
	 *     Connection.Subscribe(String subject, Callback cb) @n
	 *     Connection.DispatchMsg(Message msg) @n
	 *     Connection.StartAutoDispatch() @n
	 *     Connection.StopAutoDispatch()
	 */
	public Status GetNextMsg(Message msg, int timeout)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.GetNextMsg(msg, timeout);
	}


	/** @fn Publish(Message msg)
	 * @brief This function will publish a message to the middleware.
	 *
	 * @param msg - message to be published
	 * @return status - result of the connection operation
	 */
	public Status Publish(Message msg)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Publish(msg);
	}


	/** @fn Request(Message request, int timeout, Message reply)
	 * @brief This function will send a request, wait for the specified timeout, and return the received reply
	 * This function will block until the reply is received or the timeout is reached.
	 * The timeout value is expressed in milliseconds.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param reply - out parameter reply message if received
	 * @return status - result of the request operation
	 */
	public Status Request(Message request, int timeout, Message reply)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Request(request, timeout, reply);
	}


	/** @fn Request(Message request, int timeout, Callback cb)
	 * @brief This function will send a request asyncronously. The callback will be called for the reply
	 * if it is received within the specified timeout. This function will not block.
	 * The timeout value is expressed in milliseconds.
	 *
	 * <B>IMPORTANT:</B> The request message passed into this function will be cleaned up when the processing
	 * is complete, therefore it must be created new and not stored or cleaned up by the client program.
	 *
	 * <B>NOTE:</B> This version, and the Callback class is DEPRECATED. Please use the ReplyCallback
	 * version of this function.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param cb - Callback to call when reply is receieved
	 * @return status - result of the request operation
	 */
	public Status Request(Message request, int timeout, Callback cb)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Request(request, timeout, cb);
	}


	/** @fn Request(Message request, int timeout, ReplyCallback cb)
	 * @brief This function will send a request asyncronously. The callback will be called for the reply
	 * if it is received within the specified timeout. This function will not block.
	 * The timeout value is expressed in milliseconds.
	 *
	 * <B>IMPORTANT:</B> The request message passed into this function will be cleaned up when the processing
	 * is complete, therefore it must be created new and not stored or cleaned up by the client program.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param cb - ReplyCallback to call when reply is received
	 * @return status - result of the request operation
	 */
	public Status Request(Message request, int timeout, ReplyCallback cb)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Request(request, timeout, cb);
	}


	/** @fn Reply(Message request, Message reply)
	 * @brief This function will send a reply to a given request.
	 *
	 * @param request - the recieved request that we are responding to
	 * @param reply - the reply to be sent
	 * @return status - result of the reply operation
	 */
	public Status Reply(Message request, Message reply)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.Reply(request, reply);
	}


	/** @fn StartAutoDispatch()
	 * @brief This function will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped.
	 * If GetNextMessage() is called while the auto-dispatcher is used, the behavior will be undesireable
	 * and undefined.
	 *
	 * @return status - result of the start operation
	 *
	 * @sa Connection.Subscribe(String subject, Callback cb)
	 */
	public Status StartAutoDispatch()
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.StartAutoDispatch();
	}


	/** @fn StopAutoDispatch()
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @return status - result of the stop operation
	 */
	public Status StopAutoDispatch()
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.StopAutoDispatch();
	}


	/** @fn DispatchMsg(Message msg)
	 * @brief This function will cause the any callbacks that are registered with matching
	 * patterns to be called.
	 *
	 * @param msg - message to be dispatched
	 * @return status - result of the operation
	 *
	 * @sa Connection.GetNextMsg(Message msg, int timeout)
	 */
	public Status DispatchMsg(Message msg)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.DispatchMsg(msg);
	}


	/** @fn GetLastDispatcherStatus()
	 * @brief When running with the auto-dispatcher, it may be necessary to monitor the
	 * status as it runs within a seperate thread.  This method allows access to the
	 * last status error reported by the dispatcher.  Once the status is read, the
	 * status is cleared.
	 *
	 * @note Another way to be notified in the case of a dispatcher error is to register an
	 * error callback with the RegisterErrorCallback() function.
	 *
	 * @sa Connection.StartAutoDispatch() @n
	 *     Connection.StopAutoDispatch() @n
	 *     Connection.RegisterErrorCallack()
	 */
	public Status GetLastDispatcherStatus()
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.GetLastDispatcherStatus();
	}


	/** @fn ExcludeSubject(String subject)
	 *
	 * @brief Exclude any incoming messages with the specified subject.
	 *
	 * @param subject - subject matching that of incoming messages to be filtered
	 *
	 * @sa Connection.Subscribe(String subject) for examples of subject patterns
	 *
	 * @return a Status representing the success of the operation.
	 * 	The Status will contain a GMSEC_INVALID_SUBJECT_NAME
	 * 	error if the subject provided was null.
	 *
	 */
	public Status ExcludeSubject(String subject)
	{
		if (m_JNIconn != null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.ExcludeSubject(subject);
	}


	/** @fn RemoveExcludedSubject(String subject)
	 *
	 * @brief Remove an excluded subject, allowing incoming messages with the matching
	 * subject to once again be received.
	 *
	 * @param subject - The subject pattern to remove.
	 *
	 * @return a Status representing the success of the operation.
	 * 	The Status will contain a GMSEC_INVALID_SUBJECT_NAME
	 * 	error if the subject provided was null.
	 *
	 */
	public Status RemoveExcludedSubject(String subject)
	{
		if (m_JNIconn != null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.RemoveExcludedSubject(subject);
	}


	/** @fn RegisterErrorCallback(String event, ErrorCallback cb)
	 * @brief This function allows the registration of a callback for a particular
	 * error event. Event names are middleware dependent.
	 *
	 * All connection types implement at least following error events:
	 *
	 * "CONNECTION_DISPATCHER_ERROR" - Auto-dispatcher error callback that gets called
	 *      whenever an error occurs inside the auto-dispatcher.
	 *
	 * "CONNECTION_REQUEST_TIMEOUT" - Request w/callback error callback that gets called
	 *      whenever an error occurs while trying to process an request (ex. timeout)
	 *
	 * @param event - name of event to register
	 * @param cb - object derrived from ErrorCallback to register for this error event
	 * @return status - result of the connection operation
	 *
	 * @sa ErrorCallback
	 */
	public Status RegisterErrorCallback(String event, ErrorCallback cb)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.RegisterErrorCallback(event, cb);
	}


	public Status RemoveErrorCallback(String event, ErrorCallback cb)
	{
		if (m_JNIconn == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Connection not initialized");
		}

		return m_JNIconn.RemoveErrorCallback(event, cb);
	}


	/** @fn GetLibraryRootName()
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this connection is associated with. For example,
	 * the root library name for the icsswb middleware library is "gmsec_icsswb"
	 * and matches the name of the windows library (gmsec_icsswb.dll) and the
	 * UNIX library (libgmsec_icsswb.so).
	 *
	 * @return root library name
	 *
	 * @sa Message.GetLibraryRootName()
	 */
	public java.lang.String GetLibraryRootName()
	{
		if (m_JNIconn == null)
		{
			return "Connection not initialized";
		}

		return m_JNIconn.GetLibraryRootName();
	}


	/** @fn GetLibraryVersion()
	 * @brief This function returns a string containing the version information for
	 * this connection's associated middleware.
	 *
	 * @return library version
	 */
	public java.lang.String GetLibraryVersion()
	{
		if (m_JNIconn == null)
		{
			return "Connection not initialized";
		}
		return m_JNIconn.GetLibraryVersion();
	}


	public long GetPublishQueueMessageCount()
	{
		return (m_JNIconn == null ? 0 : m_JNIconn.GetPublishQueueMessageCount());
	}
}
