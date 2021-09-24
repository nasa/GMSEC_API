/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





package gov.nasa.gsfc.gmsecapi.jni;

import gov.nasa.gsfc.gmsecapi.Callback;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ErrorCallback;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.ReplyCallback;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.gmsecAPI;

import gov.nasa.gsfc.gmsecapi.internal.MessageImpl;


public class JNIConnection
{
	private Connection extconn;
	public long swigCPtr;
	public boolean swigCMemOwn;

	private static final String MESSAGE_CLASS_NAME = "gov.nasa.gsfc.gmsecapi.jni.JNIMessage";


	protected JNIConnection(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr = cPtr;
		swigCMemOwn = cMemoryOwn;
	}


	/**
	 * @brief This construct creates a uninitialized connection, to use
	 * it must be attached to a middleware with the ConnectionFactory.Create() method.
	 * 
	 */
	public JNIConnection()
	{
		this(0, false);
	}


	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Connection(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNIConnection obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	/**
	 * @brief Establishes the connection to the middleware
	 * @return Status - the resulting state of operation
	 */
	public Status Connect()
	{
		long cPtr = gmsecJNI.Connection_Connect(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief Termines the middleware connection
	 * @return Status - the resulting state of operation
	 */
	public Status Disconnect()
	{
		long cPtr = gmsecJNI.Connection_Disconnect(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	/**
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
	 * //	(will not match "GMSEC.MISSION.CONST.SAT")
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
	 * @sa Connection.GetNextMsg() @n
	 *     Connection.StartAutoDispatch() @n
	 *     Connection.StopAutoDispatch()
	 */
	public synchronized Status Subscribe(String subject)
	{
		if (subject == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_INVALID_SUBJECT_NAME,
				"Null Subject");
		}
		long cPtr = gmsecJNI.Connection_Subscribe(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function subscribes to a particular subject or pattern and associates a callback to
	 * be called when messages matching the subject or pattern are received. If all subscriptions are
	 * performed using this function then the auto-dispatcher can be used to asynchronously receive
	 * messages. If GetNextMsg() is used to pull messages then DispatchMsg() will need to be called to
	 * ensure registered Callbacks are called. 
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 * @return Status - the resulting status of the operation
	 */
	public synchronized Status Subscribe(String subject, Callback cb)
	{
		if (subject == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_INVALID_SUBJECT_NAME,
				"Null Subject");
		}
		if (cb == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_INVALID_CALLBACK,
				"Null Callback");
		}
		long cPtr = gmsecJNI.Connection_Subscribe_Via_Callback(swigCPtr, this, subject, cb);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function unsubscribes to a particular subject pattern, and will stop the reception 
	 * of messages that match this pattern. It will also remove the registration of any callbacks with
	 * this subject pattern.
	 *
	 * @param subject - the subject name that is currently registered
	 * @return Status - the resulting status of the operation
	 */
	public synchronized Status UnSubscribe(String subject)
	{
		if (subject == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_INVALID_SUBJECT_NAME,
				"Null Subject");
		}
		long cPtr = gmsecJNI.Connection_UnSubscribe(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function unsubscribes a single callback to a particular subject pattern, and will not
	 * unsubscribe the reception of the message. It will prevent a particular callback from being called
	 * by the auto-dispatch or DispatchMsg(), but the message will still be received for GetNextMsg(). 
	 *
	 * @param subject - the subject name that is currently registered
	 * @param cb - the callback to be unsubscribed
	 * @return Status - the resulting status of the operation
	 */
	public synchronized Status UnSubscribe(String subject, Callback cb)
	{
		if (subject == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_INVALID_SUBJECT_NAME,
				"Null Subject");
		}
		long cPtr = gmsecJNI.Connection_UnSubscribe_Via_Callback(swigCPtr, this, subject, cb);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function allows the registration of a callback for a particular error event. Event
	 * names are middleware dependent.
	 * All connection types implement at least following error events:
	 *
	 *	"CONNECTION_DISPATCHER_ERROR" - Auto-dispatcher error callback that gets called whenever an error occurs inside the auto-dispatcher.
	 *
	 *	"CONNECTION_REQUEST_TIMEOUT" - Request w/callback error callback that gets called whenever an error occurs while trying to process an request (ex. timeout)
	 *
	 * @param event - name of event to register
	 * @param cb - object derived from ErrorCallback to register for this event
	 * @return Status - the resulting status of the operation
	 */
	public synchronized Status RegisterErrorCallback(String event, ErrorCallback cb)
	{
		if (event == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_STATUS_OTHER_ERROR,
				"Null Event");
		}
		long cPtr = gmsecJNI.Connection_RegisterErrorCallback(swigCPtr, this, event, cb);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function allows the removal of a callback for a particular error event. Event
	 * names are middleware dependent.
	 * All connection types implement at least following error events:
	 *
	 *	"CONNECTION_DISPATCHER_ERROR" - Auto-dispatcher error callback that gets called whenever
	 *      an error occurs inside the auto-dispatcher.
	 *
	 *	"CONNECTION_REQUEST_TIMEOUT" - Request w/callback error callback that gets called whenever
	 *      an error occurs while trying to process an request (ex. timeout)
	 *
	 * @param event - name of event currently registered
	 * @param cb - object derived from ErrorCallback to remove for this event
	 * @return Status - the resulting status of the operation
	 */
	public synchronized Status RemoveErrorCallback(String event, ErrorCallback cb)
	{
		//TODO: To implement or not to implement?  That is the question!
		return new Status();
	}


	/**
	 * @brief This function is used by a subscriber to pull the next received message
	 * from the incoming queue.   In order for any registered callbacks to be
	 * dispatched for this message, the dispatchMsg() function must be called
	 * for the returning message
	 *
	 * @param msg - the next message received.
	 * @param timeout - determines how long to wait in the case no messages are
	 *                  immediatly available.  A zero timeout value indicates to
	 *                  return immediately and a negative timout value indicates
	 *                  to wait forever.
	 * @return Status - the resulting status of the operation
	 */
	public Status GetNextMsg(Message msg, int timeout)
	{
		if (msg == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		JNIMessage jmsg = new JNIMessage();
		long cPtr = gmsecJNI.Connection_GetNextMsg(swigCPtr, this, jmsg, timeout);
		Status result = new JNIStatus(cPtr, true);
		msg.SetImpl(jmsg);
		return result;
	}


	/**
	 * @brief This will create a message, attached to this connection that uses defaults
	 * for subject and kind.
	 */
	public Status CreateMessage(Message msg)
	{
		if (msg == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		JNIMessage jmsg = new JNIMessage();
		long cPtr = gmsecJNI.Connection_CreateMessage(swigCPtr, this, jmsg);
		Status result = new JNIStatus(cPtr, true);
		msg.SetImpl(jmsg);
		return result;
	}


	/**
	 * @brief To control message type, messages must be initialized and destroyed by the connection.
	 *
	 * @param subject - the subject field of the message
	 * @param msgKind - the type of message 
	 * @param msg - a Message object to be initialized
	 * @return Status - the resulting status of the operation
	 * 
	 * @sa gmsecAPI.GMSEC_MSG_UNSET @n
	 * 		gmsecAPI.GMSEC_MSG_PUBLISH @n
	 * 		gmsecAPI.GMSEC_MSG_REQUEST @n
	 * 		gmsecAPI.GMSEC_MSG_REPLY @n
	 */
	public Status CreateMessage(String subject, int msgKind, Message msg)
	{
		if (subject == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_INVALID_SUBJECT_NAME,
				"Null Subject");
		}
		if (msg == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		JNIMessage jmsg = new JNIMessage();
		long cPtr = gmsecJNI.Connection_CreateMessage(swigCPtr, this, subject, msgKind, jmsg);
		Status result = new JNIStatus(cPtr, true);
		msg.SetImpl(jmsg);
		return result;
	}


	/**
	 * @brief To control message type, messages must be initialized and destroyed by the connection.
	 *
	 * @param subject - the subject field of the message
	 * @param msgKind - the type of message 
	 * @param msg - a Message object to be initialized
	 * @param cfg - config object for the message
	 * @return Status - the resulting status of the operation
	 * 
	 * @sa gmsecAPI.GMSEC_MSG_UNSET @n
	 * 		gmsecAPI.GMSEC_MSG_PUBLISH @n
	 * 		gmsecAPI.GMSEC_MSG_REQUEST @n
	 * 		gmsecAPI.GMSEC_MSG_REPLY @n
	 */
	public Status CreateMessage(String subject, int msgKind, Message msg, Config cfg)
	{
		if (subject == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_INVALID_SUBJECT_NAME,
				"Null Subject");
		}
		if (msg == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		if (cfg == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_INVALID_CONFIG,
				"Null Config");
		}
		JNIMessage jmsg = new JNIMessage();
		JNIConfig jcfg = new JNIConfig(cfg);
		long cPtr = gmsecJNI.Connection_CreateMessageCFG(swigCPtr, this, subject, msgKind, jmsg, JNIConfig.getCPtr(jcfg), jcfg);
		Status result = new JNIStatus(cPtr, true);
		msg.SetImpl(jmsg);
		return result;
	}


	/**
	 * @brief Reclaim a message
	 *
	 * @param msg - the message to be reclaimed.  Once reclaimed, the message is invalid
	 * @return Status - the resulting status of the operation
	 */
	public Status DestroyMessage(Message msg)
	{
		if (msg == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		MessageImpl mi = msg.GetImpl();
		if (mi instanceof JNIMessage)
		{
			JNIMessage jmsg = (JNIMessage)mi;

			// We only allow Messages that we own to be destroyed.
			if (jmsg.swigCMemOwn == true)
			{
				long cPtr = gmsecJNI.Connection_DestroyMessage(swigCPtr, this, JNIMessage.getCPtr(jmsg), jmsg);
				JNIStatus status = new JNIStatus(cPtr, true);
				jmsg.swigCPtr = 0;
				jmsg.swigCMemOwn = false;
				msg.SetImpl(null);
				return status;
			}
			else
			{
				return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message not owned");
			}
		}
		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	/**
	 * @brief Publish a message on the connection using the Message's subject
	 * @param msg - the message to be published
	 * @return Status - the resulting status of the operation
	 */
	public Status Publish(Message msg)
	{
		if (msg == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		MessageImpl mi = msg.GetImpl();
		if (mi instanceof JNIMessage)
		{
			JNIMessage jmsg = (JNIMessage)mi;

			long cPtr = gmsecJNI.Connection_Publish(swigCPtr, this, JNIMessage.getCPtr(jmsg), jmsg);
			return new JNIStatus(cPtr, true);
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	/**
	 * @brief Send a request message on the connection and handle the resulting reply.  This
	 * function will block until the reply is received or the supplied timeout has
	 * expired.  When a callback is provided, it will be called when the reply is
	 * received.
	 *
	 * @param reqeust - the request message to be published
	 * @param timeout - the maximum time to wait for the reply before returning.  A zero
	 *                  timeout value indicates to return immediately and a negative timeout
	 *                  value indicates to wait forever.
	 * @param reply - the received reply message
	 * @return Status - the resulting status of the operation
	 */
	public Status Request(Message request, int timeout, Message reply)
	{
		if (request == null || reply == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		MessageImpl requestImpl = request.GetImpl();
		if (requestImpl instanceof JNIMessage)
		{
			JNIMessage jRequest = (JNIMessage)requestImpl;
			JNIMessage jReply = new JNIMessage();
			long cPtr = gmsecJNI.Connection_Request(swigCPtr, this, JNIMessage.getCPtr(jRequest), jRequest, timeout, jReply);
			Status result = new JNIStatus(cPtr, true);
			reply.SetImpl(jReply);
			return result;
		}
		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	/**
	 * default request dispatcher implementation
	 * 
	 * @deprecated
	 */
	public Status Request(Message request, int timeout, Callback cb)
	{
		if (request == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		MessageImpl requestImpl = request.GetImpl();
		if (requestImpl instanceof JNIMessage)
		{
			JNIMessage jRequest = (JNIMessage)requestImpl;
			long cPtr = gmsecJNI.Connection_Request_Via_Callback(swigCPtr, this, JNIMessage.getCPtr(jRequest), jRequest, timeout, cb, false);
			return new JNIStatus(cPtr, true);
		}
		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");

	}


	/**
	 * Base Request/Reply Functionality
	 */
	public Status Request(Message request, int timeout, ReplyCallback cb)
	{
		if (request == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		MessageImpl requestImpl = request.GetImpl();
		if (requestImpl instanceof JNIMessage)
		{
			JNIMessage jRequest = (JNIMessage)requestImpl;
			long cPtr = gmsecJNI.Connection_Request_Via_Callback(swigCPtr, this, JNIMessage.getCPtr(jRequest), jRequest, timeout, cb, true);
			return new JNIStatus(cPtr, true);
		}
		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");

	}


	/**
	 * @brief This function is used by the receiver of a request to send the reply
	 * message back to the sender
	 *
	 * @param request - the request message that this reply is in response to
	 * @param reply - the reply message to send
	 * @return Status - the resulting status of the operation
	 */
	public Status Reply(Message request, Message reply)
	{
		if (request == null || reply == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		MessageImpl mreq = request.GetImpl();
		MessageImpl mrpl = reply.GetImpl();
		if (mreq instanceof JNIMessage && mrpl instanceof JNIMessage)
		{
			JNIMessage jreq = (JNIMessage)mreq;
			JNIMessage jrpl = (JNIMessage)mrpl;
			long cPtr = gmsecJNI.Connection_Reply(swigCPtr, this, JNIMessage.getCPtr(jreq), jreq, JNIMessage.getCPtr(jrpl), jrpl);
			return new JNIStatus(cPtr, true);
		}
		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	public Status StartAutoDispatch()
	{
		long cPtr = gmsecJNI.Connection_StartAutoDispatch(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status StopAutoDispatch()
	{
		long cPtr = gmsecJNI.Connection_StopAutoDispatch(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status DispatchMsg(Message msg)
	{
		if (msg == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}

		MessageImpl mi = msg.GetImpl();

		if (mi instanceof JNIMessage)
		{
			JNIMessage jin = (JNIMessage) mi;
			long cPtr = gmsecJNI.Connection_DispatchMsg(swigCPtr, this, jin.swigCPtr, jin);
			return new JNIStatus(cPtr, true);
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	/**
	 * @brief If using the auto-dispatcher, returns the last status code of the last message automatically dispatched.
	 * @return Status
	 */
	public Status GetLastDispatcherStatus()
	{
		long cPtr = gmsecJNI.Connection_GetLastDispatcherStatus(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status ExcludeSubject(String subject)
	{
		long cPtr = gmsecJNI.Connection_ExcludeSubject(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}


	public Status RemoveExcludedSubject(String subject)
	{
		long cPtr = gmsecJNI.Connection_RemoveExcludedSubject(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief Create a deep-copy of a message
	 * @param in source message
	 * @param out target message
	 * @return Status
	 */
	public Status CloneMessage(Message in, Message out)
	{
		if (in == null || out == null)
		{
			return new Status(
				gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR,
				gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
				"Null Message");
		}
		MessageImpl mi = in.GetImpl();
		if (mi instanceof JNIMessage)
		{
			JNIMessage jin = (JNIMessage)mi;
			JNIMessage jmsg = new JNIMessage();
			out.SetImpl(jmsg);
			long cPtr = gmsecJNI.Connection_CloneMessage(swigCPtr, this, jin.swigCPtr, jin, jmsg);
			return new JNIStatus(cPtr, true);
		}
		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	/**
	* @brief Return the library root name
	* @return String containing the library root name
	*/
	public java.lang.String GetLibraryRootName()
	{
		return gmsecJNI.Connection_GetLibraryRootName(swigCPtr, this);
	}


	/**
	* @brief Return the library version
	* @return String containing the library version
	*/
	public java.lang.String GetLibraryVersion()
	{
		return gmsecJNI.Connection_GetLibraryVersion(swigCPtr, this);
	}


	public long GetPublishQueueMessageCount()
	{
		return gmsecJNI.Connection_GetPublishQueueMessageCount(swigCPtr, this);
	}


	/**
	 * @brief Return the connection status
	 * @return true if the connection is connected, false otherwise
	 */
	public boolean IsConnected()
	{
		return gmsecJNI.Connection_IsConnected(swigCPtr, this) != 0;
	}


	void setExternal(Connection conn)
	{
		extconn = conn;
	}
}
