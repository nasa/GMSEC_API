/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.Callback;
import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Connection;
import gov.nasa.gsfc.gmsec.api5.StatusClassification;
import gov.nasa.gsfc.gmsec.api5.StatusCode;
import gov.nasa.gsfc.gmsec.api5.EventCallback;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.ReplyCallback;
import gov.nasa.gsfc.gmsec.api5.SubscriptionInfo;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;


public class JNIConnection
{
	public Connection  extconn;
	public long        swigCPtr;
	public boolean     swigCMemOwn;

	private List<SubscriptionInfo> subscriptions;
	private List<EventCallback>    eventCallbacks;


	public void setExternal(Connection conn)
	{
		extconn = conn;
	}


	protected JNIConnection(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;

		subscriptions  = Collections.synchronizedList(new ArrayList<SubscriptionInfo>());
		eventCallbacks = new ArrayList<EventCallback>();
	}


	protected void finalize() throws Throwable
	{
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.Connection_Destroy(swigCPtr, this);
		}

		subscriptions.clear();
		eventCallbacks.clear();

		extconn        = null;
		swigCPtr       = 0;
		swigCMemOwn    = false;
		subscriptions  = null;
		eventCallbacks = null;
	}


	protected static long getCPtr(JNIConnection obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public static Connection create(Config cfg)
		throws GmsecException
	{
		JNIConfig jConfig = Config.getInternal(cfg);

		// The following construction of a JNIConnection object (albeit an invalid one) is
		// necessary so that we can pass it along to the JNI layer so that it can be registered
		// into the Cache. Once registered, the object can be referenced if/when necessary
		// to support callbacks.
		//
		JNIConnection jConn = new JNIConnection(0, false);

		long cPtr = gmsecJNI.Connection_Create(JNIConfig.getCPtr(jConfig), jConfig, jConn);

		Connection connection = null;

		if (cPtr != 0)
		{
			connection = new Connection(jConn);

			jConn.setExternal(connection);
		}

		return connection;
	}


	public static Connection create(Config cfg, MessageFactory factory)
		throws GmsecException
	{
		JNIConfig         jConfig  = Config.getInternal(cfg);
		JNIMessageFactory jFactory = MessageFactory.getInternal(factory);

		// The following construction of a JNIConnection object (albeit an invalid one) is
		// necessary so that we can pass it along to the JNI layer so that it can be registered
		// into the Cache. Once registered, the object can be referenced if/when necessary
		// to support callbacks.
		//
		JNIConnection jConn = new JNIConnection(0, false);

		long cPtr = gmsecJNI.Connection_CreateWithFactory(JNIConfig.getCPtr(jConfig), jConfig, JNIMessageFactory.getCPtr(jFactory), jFactory, jConn);

		Connection connection = null;

		if (cPtr != 0)
		{
			connection = new Connection(jConn);

			jConn.setExternal(connection);
		}

		return connection;
	}


	public static void destroy(Connection conn)
	{
		JNIConnection jConn = Connection.getInternal(conn);

		gmsecJNI.Connection_Destroy(JNIConnection.getCPtr(jConn), jConn);

		jConn.swigCPtr    = 0;
		jConn.swigCMemOwn = false;
	}


	public static String getAPIVersion()
	{
		return gmsecJNI.Connection_GetAPIVersion();
	}


	public void connect()
	{
		gmsecJNI.Connection_Connect(swigCPtr, this);
	}


	public void disconnect()
	{
		gmsecJNI.Connection_Disconnect(swigCPtr, this);
	}


	public String getLibraryName()
	{
		return gmsecJNI.Connection_GetLibraryName(swigCPtr, this);
	}


	public String getLibraryVersion()
	{
		return gmsecJNI.Connection_GetLibraryVersion(swigCPtr, this);
	}


	public Config getConfig()
	{
		long configPtr = gmsecJNI.Connection_GetConfig(swigCPtr, this);

		return new Config(new JNIConfig(configPtr, false));
	}


	public MessageFactory getMessageFactory()
	{
		long factoryPtr = gmsecJNI.Connection_GetMessageFactory(swigCPtr, this);

		return new MessageFactory(new JNIMessageFactory(factoryPtr, false));
	}


	public void registerEventCallback(Connection.Event event, EventCallback cb)
	{
		cb.setConnection(this);

		long cbPtr = JNIEventCallback.getCPtr(EventCallback.getInternal(cb));

		gmsecJNI.Connection_RegisterEventCallback(swigCPtr, this, event.getValue(), cbPtr);

		// We store a handle to the EventCallback, in case the user has declared it
		// as an anonymous object, so that the JVM garbage collector does not dispose
		// of it after it has been sent over the JNI layer.
		if (eventCallbacks.indexOf(cb) == -1)
		{
			eventCallbacks.add(cb);
		}
	}


	public SubscriptionInfo subscribe(String subject)
	{
		long siPtr = gmsecJNI.Connection_Subscribe(swigCPtr, this, subject);

		if (siPtr != 0)
		{
			return new SubscriptionInfo(new JNISubscriptionInfo(siPtr, false, subject, null));
		}

		return null;
	}


	public SubscriptionInfo subscribe(String subject, Callback cb)
	{
		cb.setConnection(this);

		long cbPtr = JNICallback.getCPtr(Callback.getInternal(cb));
		long siPtr = gmsecJNI.Connection_Subscribe(swigCPtr, this, subject, cbPtr);

		if (siPtr != 0)
		{
			SubscriptionInfo info = new SubscriptionInfo(new JNISubscriptionInfo(siPtr, false, subject, cb));

			// We store a handle to the SubscriptionInfo object, in case the user has declared
			// their Callback as an anonymous object.  This way we can prevent the JVM garbage
			// collector from disposing of the callback.
			subscriptions.add(info);

			return info;
		}

		return null;
	}


	public SubscriptionInfo subscribe(String subject, Config config)
	{
		JNIConfig jConfig = Config.getInternal(config);

		long siPtr = gmsecJNI.Connection_Subscribe(swigCPtr, this, subject, JNIConfig.getCPtr(jConfig), jConfig);

		if (siPtr != 0)
		{
			return new SubscriptionInfo(new JNISubscriptionInfo(siPtr, false, subject, null));
		}

		return null;
	}


	public SubscriptionInfo subscribe(String subject, Config config, Callback cb)
	{
		cb.setConnection(this);

		JNIConfig jConfig = Config.getInternal(config);
		long      cbPtr   = JNICallback.getCPtr(Callback.getInternal(cb));
		long      siPtr   = gmsecJNI.Connection_Subscribe(swigCPtr, this, subject, JNIConfig.getCPtr(jConfig), jConfig, cbPtr);

		if (siPtr != 0)
		{
			SubscriptionInfo info = new SubscriptionInfo(new JNISubscriptionInfo(siPtr, false, subject, cb));

			// We store a handle to the SubscriptionInfo object, in case the user has declared
			// their Callback as an anonymous object.  This way we can prevent the JVM garbage
			// collector from disposing of the callback.
			subscriptions.add(info);

			return info;
		}

		return null;
	}


	public void unsubscribe(SubscriptionInfo info)
	{
		JNISubscriptionInfo jSubInfo = SubscriptionInfo.getInternal(info);

		gmsecJNI.Connection_Unsubscribe(swigCPtr, this, JNISubscriptionInfo.getCPtr(jSubInfo), jSubInfo);

		// Remove the subscription from our internal list
		int index = subscriptions.indexOf(info);
		if (index != -1)
		{
			subscriptions.remove(index);
		}

		jSubInfo.swigCPtr = 0;
	}


	public boolean startAutoDispatch()
	{
		return gmsecJNI.Connection_StartAutoDispatch(swigCPtr, this);
	}


	public boolean stopAutoDispatch(boolean waitForCompletion)
	{
		return gmsecJNI.Connection_StopAutoDispatch(swigCPtr, this, waitForCompletion);
	}


	public void publish(Message msg)
	{
		JNIMessage jMsg = Message.getInternal(msg);

		gmsecJNI.Connection_Publish(swigCPtr, this, JNIMessage.getCPtr(jMsg), jMsg);
	}


	public void publish(Message msg, Config mwConfig)
	{
		JNIMessage jMsg = Message.getInternal(msg);
		JNIConfig  jCfg = Config.getInternal(mwConfig);

		gmsecJNI.Connection_Publish(swigCPtr, this, JNIMessage.getCPtr(jMsg), jMsg, JNIConfig.getCPtr(jCfg), jCfg);
	}


	public void request(Message request, int timeout, ReplyCallback cb, int republish_ms)
	{
		cb.setConnection(this);

		long cbPtr = JNIReplyCallback.getCPtr(ReplyCallback.getInternal(cb));

		JNIMessage jReqMsg = Message.getInternal(request);

		gmsecJNI.Connection_Request(swigCPtr, this, JNIMessage.getCPtr(jReqMsg), jReqMsg, timeout, cbPtr, republish_ms);
	}


	public Message request(Message request, int timeout, int republish_ms)
	{
		JNIMessage jReqMsg = Message.getInternal(request);

		long cPtr = gmsecJNI.Connection_Request(swigCPtr, this, JNIMessage.getCPtr(jReqMsg), jReqMsg, timeout, republish_ms);

		if (cPtr != 0)
		{
			return new Message(new JNIMessage(cPtr, true));
		}

		return null;
	}


	public void reply(Message request, Message reply)
	{
		JNIMessage jReqMsg = Message.getInternal(request);
		JNIMessage jRepMsg = Message.getInternal(reply);

		gmsecJNI.Connection_Reply(swigCPtr, this, JNIMessage.getCPtr(jReqMsg), jReqMsg, JNIMessage.getCPtr(jRepMsg), jRepMsg);
	}


	public void dispatch(Message msg)
	{
		JNIMessage jMsg = Message.getInternal(msg);

		gmsecJNI.Connection_Dispatch(swigCPtr, this, JNIMessage.getCPtr(jMsg), jMsg);
	}


	public Message receive()
	{
		long cPtr = gmsecJNI.Connection_Receive(swigCPtr, this, -1);

		if (cPtr != 0)
		{
			return new Message(new JNIMessage(cPtr, true));
		}

		return null;
	}


	public Message receive(int timeout)
	{
		long cPtr = gmsecJNI.Connection_Receive(swigCPtr, this, timeout);

		if (cPtr != 0)
		{
			return new Message(new JNIMessage(cPtr, true));
		}

		return null;
	}


	public void excludeSubject(String subject)
	{
		gmsecJNI.Connection_ExcludeSubject(swigCPtr, this, subject);
	}


	public void removeExcludedSubject(String subject)
	{
		gmsecJNI.Connection_RemoveExcludedSubject(swigCPtr, this, subject);
	}


	public String getName()
	{
		return gmsecJNI.Connection_GetName(swigCPtr, this);
	}


	public void setName(String name)
	{
		gmsecJNI.Connection_SetName(swigCPtr, this, name);
	}


	public String getID()
	{
		return gmsecJNI.Connection_GetID(swigCPtr, this);
	}


	public String getMWInfo()
	{
		return gmsecJNI.Connection_GetMWInfo(swigCPtr, this);
	}


	public String getConnectionEndpoint()
	{
		return gmsecJNI.Connection_GetConnectionEndpoint(swigCPtr, this);
	}


	public long getPublishQueueMessageCount()
	{
		return gmsecJNI.Connection_GetPublishQueueMessageCount(swigCPtr, this);
	}


	public static void shutdownAllMiddlewares()
	{
		gmsecJNI.Connection_ShutdownAllMiddlewares();
	}


	public static void shutdownMiddleware(String name)
	{
		gmsecJNI.Connection_ShutdownMiddleware(name);
	}
}
