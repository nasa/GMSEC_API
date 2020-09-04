/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.Connection;
import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;
import gov.nasa.gsfc.gmsec.api.Status;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.gmsecMIST;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerEventCallback;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerReplyCallback;
import gov.nasa.gsfc.gmsec.api.mist.Device;
import gov.nasa.gsfc.gmsec.api.mist.Mnemonic;
import gov.nasa.gsfc.gmsec.api.mist.ProductFile;
import gov.nasa.gsfc.gmsec.api.mist.ServiceParam;
import gov.nasa.gsfc.gmsec.api.mist.SubscriptionInfo;

import gov.nasa.gsfc.gmsec.api.util.Log;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;



public class JNIConnectionManager
{
	private ConnectionManager extConnMgr;
	private long              swigCPtr;
	protected boolean         swigCMemOwn;

	private List<SubscriptionInfo>               subscriptions;
	private List<ConnectionManagerEventCallback> eventCallbacks;


	protected JNIConnectionManager(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;

		subscriptions  = Collections.synchronizedList(new ArrayList<SubscriptionInfo>());
		eventCallbacks = new ArrayList<ConnectionManagerEventCallback>();
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
			gmsecJNI.delete_ConnectionManager(swigCPtr, this);

			subscriptions.clear();
			eventCallbacks.clear();

			extConnMgr     = null;
			swigCPtr       = 0;
			swigCMemOwn    = false;
			subscriptions  = null;
			eventCallbacks = null;
		}
	}


	protected static long getCPtr(JNIConnectionManager obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIConnectionManager(ConnectionManager connMgr, Config cfg)
	{
		this(gmsecJNI.new_ConnectionManager(JNIConfig.getCPtr(Config.getInternal(cfg)), Config.getInternal(cfg)), true);

		extConnMgr = connMgr;
	}


	public JNIConnectionManager(ConnectionManager connMgr, Config cfg, boolean validate)
	{
		this(gmsecJNI.new_ConnectionManager(JNIConfig.getCPtr(Config.getInternal(cfg)), Config.getInternal(cfg), validate), true);

		extConnMgr = connMgr;
	}


	public JNIConnectionManager(ConnectionManager connMgr, Config cfg, boolean validate, int version)
	{
		this(gmsecJNI.new_ConnectionManager(JNIConfig.getCPtr(Config.getInternal(cfg)), Config.getInternal(cfg), validate, version), true);

		extConnMgr = connMgr;
	}


	public void initialize()
	{
		gmsecJNI.ConnectionManager_Initialize(swigCPtr, this);
	}


	public void cleanup()
	{
		gmsecJNI.ConnectionManager_Cleanup(swigCPtr, this);
	}


	public String getLibraryVersion()
	{
		return gmsecJNI.ConnectionManager_GetLibraryVersion(swigCPtr, this);
	}


	public void setStandardFields(java.util.List<Field> standardFields)
	{
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(standardFields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(standardFields);
		int        numFields  = (standardFields == null ? 0 : standardFields.size());

		gmsecJNI.ConnectionManager_SetStandardFields(swigCPtr, this, jFieldPtrs, jFields, numFields);
	}


	public void addStandardFields(Message msg)
	{
		JNIMessage jMsg = Message.getInternal(msg);

		gmsecJNI.ConnectionManager_AddStandardFields(swigCPtr, this, JNIMessage.getCPtr(jMsg), jMsg);
	}


	public void registerEventCallback(Connection.ConnectionEvent event, ConnectionManagerEventCallback cb)
	{
		cb.setConnectionManager(this);

		long cbPtr = JNIConnMgrEventCallback.getCPtr(ConnectionManagerEventCallback.getInternal(cb));

		gmsecJNI.ConnectionManager_RegisterEventCallback(swigCPtr, this, event.ordinal(), cbPtr);

		// We store a handle to the ConnectionManagerEventCallback, in case the user has declared it
		// as an anonymous object, so that the JVM garbage collector does not dispose of it after it
		// has been sent over the JNI layer.
		if (eventCallbacks.indexOf(cb) == -1)
		{
			eventCallbacks.add(cb);
		}
	}


	public SubscriptionInfo subscribe(String subject)
	{
		long cPtr = gmsecJNI.ConnectionManager_Subscribe(swigCPtr, this, subject);

		if (cPtr != 0)
		{
			return new SubscriptionInfo(new JNIConnMgrSubscriptionInfo(cPtr, false, subject, null));
		}

		return null;
	}


	public SubscriptionInfo subscribe(String subject, ConnectionManagerCallback cb)
	{
		cb.setConnectionManager(this);

		long cbPtr = JNIConnMgrCallback.getCPtr(ConnectionManagerCallback.getInternal(cb));
		long siPtr = gmsecJNI.ConnectionManager_Subscribe(swigCPtr, this, subject, cbPtr);

		if (siPtr != 0)
		{
			SubscriptionInfo info = new SubscriptionInfo(new JNIConnMgrSubscriptionInfo(siPtr, false, subject, cb));

			// We store a handle to the SubscriptionInfo object, in case the user has declared
			// their ConnectionManagerCallback as an anonymous object.  This way we can prevent
			// the JVM garbage collector from disposing of the callback.
			subscriptions.add(info);

			return info;
		}

		return null;
	}


	public SubscriptionInfo subscribe(String subject, Config config)
	{
		JNIConfig jCfg = Config.getInternal(config);

		long cPtr = gmsecJNI.ConnectionManager_Subscribe(swigCPtr, this, subject, JNIConfig.getCPtr(jCfg), jCfg);

		if (cPtr != 0)
		{
			return new SubscriptionInfo(new JNIConnMgrSubscriptionInfo(cPtr, false, subject, null));
		}

		return null;
	}


	public SubscriptionInfo subscribe(String subject, Config config, ConnectionManagerCallback cb)
	{
		cb.setConnectionManager(this);

		JNIConfig jCfg = Config.getInternal(config);

		long cbPtr = JNIConnMgrCallback.getCPtr(ConnectionManagerCallback.getInternal(cb));
		long siPtr = gmsecJNI.ConnectionManager_Subscribe(swigCPtr, this, subject, JNIConfig.getCPtr(jCfg), jCfg, cbPtr);

		if (siPtr != 0)
		{
			SubscriptionInfo info = new SubscriptionInfo(new JNIConnMgrSubscriptionInfo(siPtr, false, subject, cb));

			// We store a handle to the SubscriptionInfo object, in case the user has declared
			// their ConnectionManagerCallback as an anonymous object.  This way we can prevent
			// the JVM garbage collector from disposing of the callback.
			subscriptions.add(info);

			return info;
		}

		return null;
	}


	public void unsubscribe(SubscriptionInfo info)
	{
		JNIConnMgrSubscriptionInfo jSubInfo = SubscriptionInfo.getInternal(info);

		gmsecJNI.ConnectionManager_Unsubscribe(swigCPtr, this, JNIConnMgrSubscriptionInfo.getCPtr(jSubInfo), jSubInfo);

		// Remove the subscription from our internal list
		int index = subscriptions.indexOf(info);
		if (index != -1)
		{
			subscriptions.remove(index);
		}
	}


	public void publish(Message msg)
	{
		JNIMessage jMsg = Message.getInternal(msg);

		gmsecJNI.ConnectionManager_Publish(swigCPtr, this, JNIMessage.getCPtr(jMsg), jMsg);
	}


	public void publish(Message msg, Config config)
	{
		JNIMessage jMsg = Message.getInternal(msg);
		JNIConfig  jCfg = Config.getInternal(config);

		gmsecJNI.ConnectionManager_Publish(swigCPtr, this, JNIMessage.getCPtr(jMsg), jMsg, JNIConfig.getCPtr(jCfg), jCfg);
	}


	public void request(Message request, int timeout, ConnectionManagerReplyCallback cb, int republish_ms)
	{
		cb.setConnectionManager(this);

		long       cbPtr = JNIConnMgrReplyCallback.getCPtr(ConnectionManagerReplyCallback.getInternal(cb));
		JNIMessage jReq  = Message.getInternal(request);

		gmsecJNI.ConnectionManager_Request(swigCPtr, this, JNIMessage.getCPtr(jReq), jReq, timeout, cbPtr, republish_ms);
	}


	public Message request(Message request, int timeout, int republish_ms)
	{
		JNIMessage jReq = Message.getInternal(request);

		long cPtr = gmsecJNI.ConnectionManager_Request(swigCPtr, this, JNIMessage.getCPtr(jReq), jReq, timeout, republish_ms);

		if (cPtr != 0)
		{
			return new JNIMessage(cPtr, true);
		}

		return null;
	}


	public void cancelRequest(ConnectionManagerReplyCallback cb)
	{
		if (cb.getConnectionManager() != this)
		{
			return;
		}

		long cbPtr = JNIConnMgrReplyCallback.getCPtr(ConnectionManagerReplyCallback.getInternal(cb));

		gmsecJNI.ConnectionManager_CancelRequest(swigCPtr, this, cbPtr);
	}


	public void reply(Message request, Message reply)
	{
		JNIMessage jReq = Message.getInternal(request);
		JNIMessage jRep = Message.getInternal(reply);

		gmsecJNI.ConnectionManager_Reply(swigCPtr, this, JNIMessage.getCPtr(jReq), jReq, JNIMessage.getCPtr(jRep), jRep);
	}


	public void dispatch(Message msg)
	{
		JNIMessage jMsg = Message.getInternal(msg);

		gmsecJNI.ConnectionManager_Dispatch(swigCPtr, this, JNIMessage.getCPtr(jMsg), jMsg);
	}


	public Message receive(int timeout)
	{
		long cPtr = gmsecJNI.ConnectionManager_Receive(swigCPtr, this, timeout);

		if (cPtr != 0)
		{
			return new JNIMessage(cPtr, true);
		}

		return null;
	}


	public boolean startAutoDispatch()
	{
		return gmsecJNI.ConnectionManager_StartAutoDispatch(swigCPtr, this);
	}


	public boolean stopAutoDispatch(boolean waitForCompletion)
	{
		return gmsecJNI.ConnectionManager_StopAutoDispatch(swigCPtr, this, waitForCompletion);
	}


	public void excludeSubject(String subject)
	{
		gmsecJNI.ConnectionManager_ExcludeSubject(swigCPtr, this, subject);
	}


	public void removeExcludedSubject(String subject)
	{
		gmsecJNI.ConnectionManager_RemoveExcludedSubject(swigCPtr, this, subject);
	}


	/***************Start Heartbeat Section************************************************/


	public Message createHeartbeatMessage(String subject, java.util.List<Field> heartbeatFields)
	{
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(heartbeatFields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(heartbeatFields);
		int        numFields  = (heartbeatFields == null ? 0 : heartbeatFields.size());

		long cPtr = gmsecJNI.ConnectionManager_CreateHeartbeatMessage(swigCPtr, this, subject, jFieldPtrs, jFields, numFields);

		if (cPtr != 0)
		{
			return new JNIMessage(cPtr, true);
		}

		return null;
	}


	public void startHeartbeatService(String subject, java.util.List<Field> heartbeatFields)
	{
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(heartbeatFields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(heartbeatFields);
		int        numFields  = (heartbeatFields == null ? 0 : heartbeatFields.size());

		gmsecJNI.ConnectionManager_StartHeartbeatService(swigCPtr, this, subject, jFieldPtrs, jFields, numFields);
	}


	public void stopHeartbeatService()
	{
		gmsecJNI.ConnectionManager_StopHeartbeatService(swigCPtr, this);
	}


	public Status changeComponentStatus(Field componentStatus)
	{
		JNIField jFld = Field.getInternal(componentStatus);

		long cPtr = gmsecJNI.ConnectionManager_ChangeComponentStatus(swigCPtr, this, JNIField.getCPtr(jFld), jFld);

		return new JNIStatus(cPtr, true);
	}


	public Status changeComponentInfo(Field componentInfo)
	{
		JNIField jFld = Field.getInternal(componentInfo);

		long cPtr = gmsecJNI.ConnectionManager_ChangeComponentInfo(swigCPtr, this, JNIField.getCPtr(jFld), jFld);

		return new JNIStatus(cPtr, true);
	}


	public Status changeCPUMemory(Field cpuMemory)
	{
		JNIField jFld = Field.getInternal(cpuMemory);

		long cPtr = gmsecJNI.ConnectionManager_ChangeCPUMemory(swigCPtr, this, JNIField.getCPtr(jFld), jFld);

		return new JNIStatus(cPtr, true);
	}


	public Status changeCPUUtil(Field cpuUtil)
	{
		JNIField jFld = Field.getInternal(cpuUtil);

		long cPtr = gmsecJNI.ConnectionManager_ChangeCPUUtil(swigCPtr, this, JNIField.getCPtr(jFld), jFld);

		return new JNIStatus(cPtr, true);
	}


	/***************Start Log Section*****************************************************/


	public Message createLogMessage(String subject, java.util.List<Field> logFields)
	{
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(logFields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(logFields);
		int        numFields  = (logFields == null ? 0 : logFields.size());

		long cPtr = gmsecJNI.ConnectionManager_CreateLogMessage(swigCPtr, this, subject, jFieldPtrs, jFields, numFields);

		if (cPtr != 0)
		{
			return new JNIMessage(cPtr, true);
		}

		return null;
	}


	public void setLoggingDefaults(String subject, java.util.List<Field> logFields)
	{
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(logFields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(logFields);
		int        numFields  = (logFields == null ? 0 : logFields.size());

		gmsecJNI.ConnectionManager_SetLoggingDefaults(swigCPtr, this, subject, jFieldPtrs, jFields, numFields);
	}


	public void publishLog(String logMessage, Field severity)
	{
		JNIField jFld = Field.getInternal(severity);

		gmsecJNI.ConnectionManager_PublishLog(swigCPtr, this, logMessage, JNIField.getCPtr(jFld), jFld);
	}


	/***************Start Directive Section************************************************/


	public void requestDirective(String subject, Field directiveString, java.util.List<Field> fields)
	{
		JNIField   jFld       = Field.getInternal(directiveString);
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(fields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(fields);
		int        numFields  = (fields == null ? 0 : fields.size());

		gmsecJNI.ConnectionManager_RequestDirective(swigCPtr, this, subject,
			JNIField.getCPtr(jFld), jFld, jFieldPtrs, jFields, numFields);
	}


	public void requestDirective(String subject, Field directiveString, java.util.List<Field> fields,
					int timeout, ConnectionManagerReplyCallback cb, int republish_ms)
	{
		cb.setConnectionManager(this);

		long       cbPtr      = JNIConnMgrReplyCallback.getCPtr(ConnectionManagerReplyCallback.getInternal(cb));
		JNIField   jFld       = Field.getInternal(directiveString);
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(fields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(fields);
		int        numFields  = (fields == null ? 0 : fields.size());

		gmsecJNI.ConnectionManager_RequestDirective(swigCPtr, this, subject,
			JNIField.getCPtr(jFld), jFld, jFieldPtrs, jFields, numFields, timeout, cbPtr, republish_ms);
	}


	public Message requestDirective(String subject, Field directiveString, java.util.List<Field> fields,
		int timeout, int republish_ms)
	{
		JNIField   jFld       = Field.getInternal(directiveString);
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(fields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(fields);
		int        numFields  = (fields == null ? 0 : fields.size());

		long cPtr = gmsecJNI.ConnectionManager_RequestDirective(swigCPtr, this, subject,
			JNIField.getCPtr(jFld), jFld, jFieldPtrs, jFields, numFields, timeout, republish_ms);

		if (cPtr != 0)
		{
			return new JNIMessage(cPtr, true);
		}

		return null;
	}


	public void acknowledgeDirectiveRequest(String subject, Message request, 
					gmsecMIST.ResponseStatus status, java.util.List<Field> fields)
	{
		JNIMessage jReq       = Message.getInternal(request);
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(fields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(fields);
		int        numFields  = (fields == null ? 0 : fields.size());

		gmsecJNI.ConnectionManager_AcknowledgeDirectiveRequest(swigCPtr, this, subject,
			JNIMessage.getCPtr(jReq), jReq, status.getValue(), jFieldPtrs, jFields, numFields);
	}


	public void publishResourceMessage(String subject, long sampleInterval, long averageInterval)
	{
		gmsecJNI.ConnectionManager_PublishResourceMessage(swigCPtr, this, subject, sampleInterval, averageInterval);
	}


	public Message createResourceMessage(String subject, long sampleInterval, long averageInterval)
	{
		long cPtr = gmsecJNI.ConnectionManager_CreateResourceMessage(swigCPtr, this, subject, sampleInterval, averageInterval);

		if (cPtr != 0)
		{
			return new JNIMessage(cPtr, true);
		}

		return null;
	}


	public void startResourceMessageService(String subject, long interval, long sampleInterval, long averageInterval)
	{
		gmsecJNI.ConnectionManager_StartResourceMessageService(swigCPtr, this, subject,
			interval, sampleInterval, averageInterval);
	}


	public boolean stopResourceMessageService()
	{
		return gmsecJNI.ConnectionManager_StopResourceMessageService(swigCPtr, this);
	}


	/***************Start Simple Service Request Message Section***********************************/


	public void acknowledgeSimpleService(String subject, Message request, gmsecMIST.ResponseStatus status,
					java.util.List<Field> fields)
	{
		JNIMessage jReq       = Message.getInternal(request);
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(fields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(fields);
		int        numFields  = (fields == null ? 0 : fields.size());

		gmsecJNI.ConnectionManager_AcknowledgeSimpleService(swigCPtr, this, subject, JNIMessage.getCPtr(jReq), jReq,
			status.getValue(), jFieldPtrs, jFields, numFields);
	}


	public void requestSimpleService(String subject, String opName, Field opNumber,
					java.util.List<Field> fields, java.util.List<ServiceParam> params)
	{
		JNIField          jOpNumber  = Field.getInternal(opNumber);
		long[]            jFieldPtrs = ArrayListConverter.listToFieldPtrs(fields);
		JNIField[]        jFields    = ArrayListConverter.listToJNIFields(fields);
		int               numFields  = (fields == null ? 0 : fields.size());
		long[]            jParamPtrs = ArrayListConverter.listToServiceParamPtrs(params);
		JNIServiceParam[] jParams    = ArrayListConverter.listToJNIServiceParams(params);
		int               numParams  = (params == null ? 0 : params.size());

		gmsecJNI.ConnectionManager_RequestSimpleService(swigCPtr, this, subject, opName,
			JNIField.getCPtr(jOpNumber), jOpNumber,
			jFieldPtrs, jFields, numFields,
			jParamPtrs, jParams, numParams);
	}


	public void requestSimpleService(String subject, String opName, Field opNumber,
					java.util.List<Field> fields, java.util.List<ServiceParam> params,
					int timeout, ConnectionManagerReplyCallback cb, int republish_ms)
	{
		cb.setConnectionManager(this);

		long              cbPtr      = JNIConnMgrReplyCallback.getCPtr(ConnectionManagerReplyCallback.getInternal(cb));
		JNIField          jOpNumber  = Field.getInternal(opNumber);
		long[]            jFieldPtrs = ArrayListConverter.listToFieldPtrs(fields);
		JNIField[]        jFields    = ArrayListConverter.listToJNIFields(fields);
		int               numFields  = (fields == null ? 0 : fields.size());
		long[]            jParamPtrs = ArrayListConverter.listToServiceParamPtrs(params);
		JNIServiceParam[] jParams    = ArrayListConverter.listToJNIServiceParams(params);
		int               numParams  = (params == null ? 0 : params.size());

		gmsecJNI.ConnectionManager_RequestSimpleService(swigCPtr, this, subject, opName,
			JNIField.getCPtr(jOpNumber), jOpNumber,
			jFieldPtrs, jFields, numFields,
			jParamPtrs, jParams, numParams,
			timeout, cbPtr, republish_ms);
	}


	public Message requestSimpleService(String subject, String opName, Field opNumber,
						java.util.List<Field> fields, java.util.List<ServiceParam> params,
						int timeout, int republish_ms)
	{
		JNIField          jOpNumber  = Field.getInternal(opNumber);
		long[]            jFieldPtrs = ArrayListConverter.listToFieldPtrs(fields);
		JNIField[]        jFields    = ArrayListConverter.listToJNIFields(fields);
		int               numFields  = (fields == null ? 0 : fields.size());
		long[]            jParamPtrs = ArrayListConverter.listToServiceParamPtrs(params);
		JNIServiceParam[] jParams    = ArrayListConverter.listToJNIServiceParams(params);
		int               numParams  = (params == null ? 0 : params.size());

		long cPtr = gmsecJNI.ConnectionManager_RequestSimpleService(swigCPtr, this, subject, opName,
							JNIField.getCPtr(jOpNumber), jOpNumber,
							jFieldPtrs, jFields, numFields,
							jParamPtrs, jParams, numParams,
							timeout, republish_ms);

		if (cPtr != 0)
		{
			return new JNIMessage(cPtr, true);
		}

		return null;
	}
}
