/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





package gov.nasa.gsfc.gmsecapi.jni;

import gov.nasa.gsfc.gmsecapi.mist.ConnectionManager;
import gov.nasa.gsfc.gmsecapi.mist.ConnectionManagerCallback;
import gov.nasa.gsfc.gmsecapi.mist.ConnectionManagerReplyCallback;
import gov.nasa.gsfc.gmsecapi.mist.ConnectionManagerErrorCallback;

import gov.nasa.gsfc.gmsecapi.Callback;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.gmsecAPI;
import gov.nasa.gsfc.gmsecapi.GMSEC_UShort;

import gov.nasa.gsfc.gmsecapi.internal.MessageImpl;


public class JNIConnectionManager
{
	private ConnectionManager extConnMgr;
	public long               swigCPtr;
	public boolean            swigCMemOwn;

	private long[]     m_fieldPtrs = null;
	private JNIField[] m_jniFields = null;
	private int        m_fieldCount = 0;


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
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNIConnectionManager obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIConnectionManager(ConnectionManager connMgr, Config config, int version, boolean validate)
	{
		extConnMgr = connMgr;

        JNIConfig jcfg = Config.getInternal(config);
		swigCPtr = gmsecJNI.new_ConnectionManager(JNIConfig.getCPtr(jcfg), jcfg, version, validate);
		swigCMemOwn = (swigCPtr != 0);
	}


	public JNIConnectionManager(ConnectionManager connMgr, Config config)
	{
		extConnMgr = connMgr;

        JNIConfig jcfg = Config.getInternal(config);
		swigCPtr = gmsecJNI.new_ConnectionManager(JNIConfig.getCPtr(jcfg), jcfg);
		swigCMemOwn = (swigCPtr != 0);
	}


	public Status SetStandardFields(Field[] standardFields)
	{
		getValidFields(standardFields);

		long cPtr = gmsecJNI.ConnectionManager_SetStandardFields(swigCPtr, this, m_fieldPtrs, m_jniFields, m_fieldCount);

		return new JNIStatus(cPtr, true);
	}


	public Status Initialize()
	{
		long cPtr = gmsecJNI.ConnectionManager_Initialize(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status Cleanup()
	{
		long cPtr = gmsecJNI.ConnectionManager_Cleanup(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status RegisterErrorCallback(String event, ConnectionManagerErrorCallback cb)
	{
		long cPtr = gmsecJNI.ConnectionManager_RegisterErrorCallback(swigCPtr, this, event, cb);
		return new JNIStatus(cPtr, true);
	}


	public Status Subscribe(String subject)
	{
		long cPtr = gmsecJNI.ConnectionManager_Subscribe(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}


	public Status Subscribe(String subject, ConnectionManagerCallback cb)
	{
		long cPtr = gmsecJNI.ConnectionManager_Subscribe(swigCPtr, this, subject, cb);
		return new JNIStatus(cPtr, true);
	}


	public Status Subscribe(String subject, Config config)
	{
        JNIConfig jcfg = Config.getInternal(config);
		long cPtr = gmsecJNI.ConnectionManager_Subscribe(swigCPtr, this, subject, JNIConfig.getCPtr(jcfg), jcfg);
		return new JNIStatus(cPtr, true);
	}


	public Status UnSubscribe(String subject)
	{
		long cPtr = gmsecJNI.ConnectionManager_UnSubscribe(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}


	public Status UnSubscribe(String subject, ConnectionManagerCallback cb)
	{
		long cPtr = gmsecJNI.ConnectionManager_UnSubscribe(swigCPtr, this, subject, cb);
		return new JNIStatus(cPtr, true);
	}


	public Status CreateMessage(String subject, int kind, Message msg)
	{
		JNIMessage jmsg = new JNIMessage();

		long cPtr = gmsecJNI.ConnectionManager_CreateMessage(swigCPtr, this, subject, kind, jmsg);

		msg.SetImpl(jmsg);

		return new JNIStatus(cPtr, true);
	}


	public Status ConvertMessage(Message in, Message out)
	{
		JNIMessage jmsg_in  = (JNIMessage) in.GetImpl();
		JNIMessage jmsg_out = (JNIMessage) out.GetImpl();

		long cPtr = gmsecJNI.ConnectionManager_ConvertMessage(swigCPtr, this, JNIMessage.getCPtr(jmsg_in), jmsg_in, JNIMessage.getCPtr(jmsg_out), jmsg_out);

		out.SetImpl(jmsg_out);

		return new JNIStatus(cPtr, true);
	}


	public Status ConvertCleanup(Message in, Message out)
	{
		JNIMessage jmsg_in  = (JNIMessage) in.GetImpl();
		JNIMessage jmsg_out = (JNIMessage) out.GetImpl();

		long cPtr = gmsecJNI.ConnectionManager_ConvertCleanup(swigCPtr, this, JNIMessage.getCPtr(jmsg_in), jmsg_in, JNIMessage.getCPtr(jmsg_out), jmsg_out);

		out.SetImpl(jmsg_out);

		return new JNIStatus(cPtr, true);
	}


	public Status CloneMessage(Message in, Message out)
	{
		JNIMessage jmsg_in  = (JNIMessage) in.GetImpl();
		JNIMessage jmsg_out = (JNIMessage) out.GetImpl();

		long cPtr = gmsecJNI.ConnectionManager_CloneMessage(swigCPtr, this, JNIMessage.getCPtr(jmsg_in), jmsg_in, JNIMessage.getCPtr(jmsg_out), jmsg_out);

		out.SetImpl(jmsg_out);

		return new JNIStatus(cPtr, true);
	}


	public Status DestroyMessage(Message msg)
	{
		MessageImpl mi = msg.GetImpl();

		if (mi instanceof JNIMessage)
		{
			JNIMessage jmsg = (JNIMessage) mi;

			// We only allow message we own to be destroyed.
			if (jmsg.swigCMemOwn)
			{
				long cPtr = gmsecJNI.ConnectionManager_DestroyMessage(swigCPtr, this, JNIMessage.getCPtr(jmsg), jmsg);

				jmsg.swigCPtr    = 0;
				jmsg.swigCMemOwn = false;

				msg.SetImpl(null);

				return new JNIStatus(cPtr, true);
			}
			else
			{
				return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message not owned");
			}
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	public Status Publish(Message msg)
	{
		MessageImpl mi = msg.GetImpl();

		if (mi instanceof JNIMessage)
		{
			JNIMessage jmsg = (JNIMessage) mi;

			long cPtr = gmsecJNI.ConnectionManager_Publish(swigCPtr, this, JNIMessage.getCPtr(jmsg), jmsg);

			return new JNIStatus(cPtr, true);
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	public Status Publish(Message msg, Config config)
	{
		MessageImpl mi = msg.GetImpl();

		if (mi instanceof JNIMessage)
		{
			JNIMessage jmsg = (JNIMessage) mi;
        	JNIConfig jcfg  = Config.getInternal(config);

			long cPtr = gmsecJNI.ConnectionManager_Publish(swigCPtr, this, JNIMessage.getCPtr(jmsg), jmsg, JNIConfig.getCPtr(jcfg), jcfg);

			return new JNIStatus(cPtr, true);
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	public Status Request(Message request, int timeout, ConnectionManagerReplyCallback cb)
	{
		MessageImpl req_impl = request.GetImpl();

		if (req_impl instanceof JNIMessage)
		{
			JNIMessage jreq = (JNIMessage) req_impl;

			long cPtr = gmsecJNI.ConnectionManager_Request(swigCPtr, this, JNIMessage.getCPtr(jreq), jreq, timeout, cb);

			return new JNIStatus(cPtr, true);
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	public Status Request(Message request, int timeout, Message reply)
	{
		MessageImpl req_impl = request.GetImpl();

		if (req_impl instanceof JNIMessage)
		{
			JNIMessage jreq = (JNIMessage) req_impl;
			JNIMessage jrep = new JNIMessage();

			long cPtr = gmsecJNI.ConnectionManager_Request(swigCPtr, this, JNIMessage.getCPtr(jreq), jreq, timeout, jrep);

			reply.SetImpl(jrep);

			return new JNIStatus(cPtr, true);
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	public Status Reply(Message request, Message reply)
	{
		MessageImpl req_impl = request.GetImpl();
		MessageImpl rep_impl = reply.GetImpl();

		if (req_impl instanceof JNIMessage && rep_impl instanceof JNIMessage)
		{
			JNIMessage jreq = (JNIMessage) req_impl;
			JNIMessage jrep = (JNIMessage) rep_impl;

			long cPtr = gmsecJNI.ConnectionManager_Reply(swigCPtr, this, JNIMessage.getCPtr(jreq), jreq, JNIMessage.getCPtr(jrep), jrep);

			return new JNIStatus(cPtr, true);
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	public Status StartAutoDispatch()
	{
		long cPtr = gmsecJNI.ConnectionManager_StartAutoDispatch(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status StopAutoDispatch()
	{
		long cPtr = gmsecJNI.ConnectionManager_StopAutoDispatch(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status StopAutoDispatch(boolean waitForCompletion)
	{
		long cPtr = gmsecJNI.ConnectionManager_StopAutoDispatch(swigCPtr, this, waitForCompletion);
		return new JNIStatus(cPtr, true);
	}


	public Status GetNextMsg(Message msg, int timeout)
	{
		JNIMessage jmsg = new JNIMessage();

		long cPtr = gmsecJNI.ConnectionManager_GetNextMsg(swigCPtr, this, JNIMessage.getCPtr(jmsg), jmsg, timeout);

		msg.SetImpl(jmsg);

		return new JNIStatus(cPtr, true);
	}


	public Status DispatchMsg(Message msg)
	{
		MessageImpl msg_impl = msg.GetImpl();

		if (msg_impl instanceof JNIMessage)
		{
			JNIMessage jmsg = (JNIMessage) msg_impl;

			long cPtr = gmsecJNI.ConnectionManager_DispatchMsg(swigCPtr, this, JNIMessage.getCPtr(jmsg), jmsg);

			return new JNIStatus(cPtr, true);
		}

		return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_INVALID_MESSAGE, "Message is of invalid type.");
	}


	public Status GetLastDispatcherStatus()
	{
		long cPtr = gmsecJNI.ConnectionManager_GetLastDispatcherStatus(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status ExcludeSubject(String subject)
	{
		long cPtr = gmsecJNI.ConnectionManager_ExcludeSubject(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}


	public Status RemoveExcludedSubject(String subject)
	{
		long cPtr = gmsecJNI.ConnectionManager_RemoveExcludedSubject(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}


	public Status GetHeartbeatMessage(Field[] standardFields, Message msg, String subject)
	{
		getValidFields(standardFields);

		JNIMessage jmsg = new JNIMessage();

		long cPtr = gmsecJNI.ConnectionManager_GetHeartbeatMessage(swigCPtr, this, m_fieldPtrs, m_jniFields, m_fieldCount, JNIMessage.getCPtr(jmsg), jmsg, subject);

		msg.SetImpl(jmsg);

		return new JNIStatus(cPtr, true);
	}


	public Status StartHeartbeatService(Field[] standardFields, String subject)
	{
		getValidFields(standardFields);

		long cPtr = gmsecJNI.ConnectionManager_StartHeartbeatService(swigCPtr, this, m_fieldPtrs, m_jniFields, m_fieldCount, subject);

		return new JNIStatus(cPtr, true);
	}


	public Status ChangeComponentStatus(Field componentStatus)
	{
		JNIField jfld = new JNIField(componentStatus);
		long cPtr = gmsecJNI.ConnectionManager_ChangeComponentStatus(swigCPtr, this, JNIField.getCPtr(jfld), jfld);
		return new JNIStatus(cPtr, true);
	}


	public Status ChangeComponentInfo(Field componentInfo)
	{
		JNIField jfld = new JNIField(componentInfo);
		long cPtr = gmsecJNI.ConnectionManager_ChangeComponentInfo(swigCPtr, this, JNIField.getCPtr(jfld), jfld);
		return new JNIStatus(cPtr, true);
	}


	public Status ChangeCPUMemory(Field cpuMemory)
	{
		JNIField jfld = new JNIField(cpuMemory);
		long cPtr = gmsecJNI.ConnectionManager_ChangeCPUMemory(swigCPtr, this, JNIField.getCPtr(jfld), jfld);
		return new JNIStatus(cPtr, true);
	}


	public Status ChangeCPUUtil(Field cpuUtil)
	{
		JNIField jfld = new JNIField(cpuUtil);
		long cPtr = gmsecJNI.ConnectionManager_ChangeCPUUtil(swigCPtr, this, JNIField.getCPtr(jfld), jfld);
		return new JNIStatus(cPtr, true);
	}


	public Status StopHeartbeatService()
	{
		long cPtr = gmsecJNI.ConnectionManager_StopHeartbeatService(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status GetLogMessage(Field[] standardFields, Message msg, String subject)
	{
		getValidFields(standardFields);

		JNIMessage jmsg = new JNIMessage();

		long cPtr = gmsecJNI.ConnectionManager_GetLogMessage(swigCPtr, this, m_fieldPtrs, m_jniFields, m_fieldCount, JNIMessage.getCPtr(jmsg), jmsg, subject);

		msg.SetImpl(jmsg);

		return new JNIStatus(cPtr, true);
	}


	public Status SetLoggingDefaults(Field[] standardFields, String subject)
	{
		getValidFields(standardFields);

		long cPtr = gmsecJNI.ConnectionManager_SetLoggingDefaults(swigCPtr, this, m_fieldPtrs, m_jniFields, m_fieldCount, subject);

		return new JNIStatus(cPtr, true);
	}


	public Status QuickLog(String msg, Field severity)
	{
		JNIField jfld = new JNIField(severity);
		long cPtr = gmsecJNI.ConnectionManager_QuickLog(swigCPtr, this, msg, JNIField.getCPtr(jfld), jfld);
		return new JNIStatus(cPtr, true);
	}


	private void getValidFields(Field[] fields)
	{
		m_fieldPtrs  = null;
		m_jniFields  = null;
		m_fieldCount = 0;

		if (fields == null)
		{
			return;
		}

		for (int i = 0; i < fields.length; ++i)
		{
			if (fields[i] != null)
			{
				GMSEC_UShort type = new GMSEC_UShort();
				Status status = fields[i].GetType(type);

				if (!status.IsError() && type.Get() != gmsecAPI.GMSEC_TYPE_UNSET)
				{
					++m_fieldCount;
				}
			}
		}

		if (m_fieldCount > 0)
		{
			m_fieldPtrs = new long[m_fieldCount];
			m_jniFields = new JNIField[m_fieldCount];

			for (int i = 0, count = 0; i < fields.length; ++i)
			{
				if (fields[i] != null)
				{
					GMSEC_UShort type = new GMSEC_UShort();
					Status status = fields[i].GetType(type);

					if (!status.IsError() && type.Get() != gmsecAPI.GMSEC_TYPE_UNSET)
					{
						JNIField jfld = new JNIField(fields[i]);

						m_fieldPtrs[ count ] = JNIField.getCPtr(jfld);
						m_jniFields[ count ] = jfld;

						++count;
					}
				}
			}
		}
	}
}
