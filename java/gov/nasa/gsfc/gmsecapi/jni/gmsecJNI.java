/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




package gov.nasa.gsfc.gmsecapi.jni;

import gov.nasa.gsfc.gmsecapi.*;
import gov.nasa.gsfc.gmsecapi.internal.*;
import gov.nasa.gsfc.gmsecapi.mist.*;
import gov.nasa.gsfc.gmsecapi.util.*;

/** @class gmsecJNI
 *
 * @brief This file declares all native JNI methods that are interfacing with the
 * functions in the GMSEC API library.
 *
 */
class gmsecJNI
{
	// This loads the jni library
	static
	{
		final String GMSEC_JNI_KEY = "gmsec.jni.library";

		String libpath = System.getProperty(GMSEC_JNI_KEY);
		if (libpath != null)
		{
			System.load(libpath);
		}
		else
		{
			System.loadLibrary("gmsec_jni");
		}

		initialize();
		
		// Get java version 
		String version = System.getProperty("java.version");
		if (version != null)
		{
			int major = 0, minor = 0;
			int firstDot = version.indexOf('.');
			try
			{
				major = Integer.parseInt(version.substring(0, firstDot));
				minor = Integer.parseInt(version.substring((firstDot + 1), version.indexOf('.', (firstDot + 1))));
			}
			catch (NumberFormatException e)
			{
				// commented out for Fortify
				// e.printStackTrace();
			}

			// If version is greater than 1.4, then enable thread detach in JNI
			setEnableDetach((major > 1 || minor > 4));
		}
	}

	private final static native void initialize();

	private final static native void setEnableDetach(boolean enableDetach);

	// Status native class functions
	public final static native long new_Status();

	public final static native long new_Status_Set(int jarg1, long jarg2, String jarg3);

	public final static native long new_Status_Copy(long jarg1, JNIStatus status);

	public final static native void delete_Status(long jarg1, JNIStatus status);

	public final static native String Status_GetString(long jarg1, JNIStatus status);

	public final static native String Status_Get(long jarg1, JNIStatus status);

	public final static native int Status_GetClass(long jarg1, JNIStatus status);

	public final static native long Status_GetCode(long jarg1, JNIStatus status);

	public final static native int Status_GetCustomCode(long jarg1, JNIStatus status);

	public final static native int Status_isError(long jarg1, JNIStatus status);
	
	public final static native int Status_IsError(long jarg1, JNIStatus status);

	public final static native void Status_SetString(long jarg1, JNIStatus status, String jarg2);

	public final static native void Status_SetClass(long jarg1, JNIStatus status, int jarg2);

	public final static native void Status_SetCode(long jarg1, JNIStatus status, long jarg2);

	public final static native void Status_SetCustomCode(long jarg1, JNIStatus status, int jarg2);

	public final static native void Status_Set(long jarg1, JNIStatus status, int jarg2, long jarg3, String jarg4);

	public final static native void Status_ReSet(long jarg1, JNIStatus status);

	// ConfigFile native class functions
	public final static native long new_ConfigFile(JNIConfigFile configFile);

	public final static native long new_ConfigFile(JNIConfigFile configFile, String filepath);

	public final static native long ConfigFile_Load(long jarg1, JNIConfigFile configFile);

	public final static native long ConfigFile_Load(long jarg1, JNIConfigFile configFile, String filePath);

	public final static native long ConfigFile_Save(long jarg1, JNIConfigFile configFile, String filePath, boolean compact);

	public final static native void delete_ConfigFile(long jarg1, JNIConfigFile configFile);

	public final static native long ConfigFile_FromXML(long jarg1, JNIConfigFile configFile, String xml);

	public final static native boolean ConfigFile_IsLoaded(long jarg1, JNIConfigFile configFile);

	public final static native long ConfigFile_LookupConfig(long jarg1, JNIConfigFile configFile, String name, long cptr, JNIConfig jconfig);

	public final static native long ConfigFile_LookupMessage(long jarg1, JNIConfigFile configFile, String name, long mptr, JNIMessage jmessage);

	public final static native long ConfigFile_ToXML(long ptr, JNIConfigFile cfile, GMSEC_String xml);

	public final static native long ConfigFile_LookupSubscription(long jarg1, JNIConfigFile configFile, String name, GMSEC_String pattern);

	public final static native long ConfigFile_AddSubscription(long jarg1, JNIConfigFile configFile, String name, String subscription);

	public final static native long ConfigFile_AddConfig(long jarg1, JNIConfigFile configFile, long cptr, String name, Config config);

	public final static native long ConfigFile_AddMessage(long jarg1, JNIConfigFile configFile, long mptr, String name, Message message);

	public final static native long ConfigFile_AddCustomXML(long jarg1, JNIConfigFile configFile, String xml);

	public final static native long ConfigFile_RemoveSubscription(long jarg1, JNIConfigFile configFile, String name);

	public final static native long ConfigFile_RemoveConfig(long jarg1, JNIConfigFile configFile, String name);

	public final static native long ConfigFile_RemoveMessage(long jarg1, JNIConfigFile configFile, String name);

	public final static native long ConfigFile_RemoveCustomXML(long jarg1, JNIConfigFile configFile, String xml);

	// Config native class functions
	public final static native long new_Config();

	public final static native long new_Config_String(String jarg1[]);

	public final static native long new_Config_Copy(long jarg1, JNIConfig config);

	public final static native void delete_Config(long jarg1, JNIConfig config);

	public final static native long Config_AddValue(long jarg1, JNIConfig config, String jarg2, String jarg3);

	public final static native long Config_ClearValue(long jarg1, JNIConfig config, String jarg2);

	public final static native long Config_GetValue(long jarg1, JNIConfig config, String jarg2, GMSEC_String jarg3);

	public final static native long Config_Clear(long jarg1, JNIConfig config);

	public final static native long Config_GetFirst(long jarg, JNIConfig config1, GMSEC_String jarg2, GMSEC_String jarg3);

	public final static native long Config_GetNext(long jarg1, JNIConfig config, GMSEC_String jarg2, GMSEC_String jarg3);

	public final static native long Config_ToXML(long cfg, JNIConfig config, GMSEC_String xml);

	public final static native long Config_FromXML(long cfg, JNIConfig config, String xml);

	// ConnectionFactory native class functions
	// gone public final static native long ConnectionFactory_Create_ConnectionType(long jarg1, JNIConfig config, long jarg2, JNIConnection jarg3);
	public final static native long ConnectionFactory_Create(long jarg1, JNIConfig config, JNIConnection jarg2);

	public final static native long ConnectionFactory_Destroy(long jarg1, JNIConnection connection);

	public final static native String ConnectionFactory_GetAPIVersion();

	// Message native class functions
	public final static native void delete_Message(long jarg1, JNIMessage message);

	public final static native boolean Message_isValid(long jarg1, JNIMessage message);
	
	public final static native boolean Message_IsValid(long jarg1, JNIMessage message);

	public final static native long Message_SetKind(long jarg1, JNIMessage message, int jarg2);

	public final static native long Message_GetKind(long jarg1, JNIMessage message, GMSEC_UShort jarg2);

	public final static native long Message_SetSubject(long jarg1, JNIMessage message, String jarg2);

	public final static native long Message_GetSubject(long jarg1, JNIMessage message, GMSEC_String jarg2);

	public final static native long Message_AddField(long jarg1, JNIMessage message, long jarg2, JNIField field);

	public final static native long Message_GetField(long jarg1, JNIMessage message, String jarg2, long jarg, JNIField field3);

	public final static native long Message_GetFieldCount(long jarg1, JNIMessage message, GMSEC_Long jarg2);

	public final static native long Message_GetFirstField(long jarg1, JNIMessage message, long jarg, JNIField field2);

	public final static native long Message_GetNextField(long jarg1, JNIMessage message, long jarg2, JNIField field);

	public final static native long Message_SetConfig(long jarg1, JNIMessage message, long jarg2, JNIConfig config);

	public final static native long Message_ToXML(long jarg1, JNIMessage message, GMSEC_String jarg2);

	public final static native long Message_FromXML(long jarg1, JNIMessage message, String jarg2);

	public final static native String Message_ToJSON(long jarg1, JNIMessage message);

	public final static native long Message_FromJSON(long jarg1, JNIMessage message, String jarg2);

	public final static native String Message_GetTime(long jarg, JNIMessage message1);

	public final static native long Message_GetMSGSize(long jarg1, JNIMessage message, GMSEC_ULong jarg3);

	public final static native long Message_ClearFields(long jarg, JNIMessage message1);

	public final static native long Message_ClearField(long jarg1, JNIMessage message, String jarg2);

	/**
	 * The Set/GetTracking functions are left here in case someday they are supported within the C++ binding of the GMSEC API.
	 *
	public final static native void Message_SetTracking(long jarg1, JNIMessage message, short jarg2);
	public final static native void Message_SetTrackingNode(long jarg1, JNIMessage message, short jarg2);
	public final static native void Message_SetTrackingProcessId(long jarg1, JNIMessage message, short jarg2);
	public final static native void Message_SetTrackingConnectionId(long jarg1, JNIMessage message, short jarg2);
	public final static native void Message_SetTrackingUserName(long jarg1, JNIMessage message, short jarg2);
	public final static native void Message_SetTrackingPublishTime(long jarg1, JNIMessage message, short jarg2);
	public final static native void Message_SetTrackingUniqueId(long jarg1, JNIMessage message, short jarg2);
	public final static native void Message_SetTrackingMwInfo(long jarg1, JNIMessage message, short jarg2);

	public final static native short Message_GetTracking(long jarg1, JNIMessage message);
	public final static native short Message_GetTrackingNode(long jarg1, JNIMessage message);
	public final static native short Message_GetTrackingProcessId(long jarg1, JNIMessage message);
	public final static native short Message_GetTrackingConnectionId(long jarg1, JNIMessage message);
	public final static native short Message_GetTrackingUserName(long jarg1, JNIMessage message);
	public final static native short Message_GetTrackingPublishTime(long jarg1, JNIMessage message);
	public final static native short Message_GetTrackingUniqueId(long jarg1, JNIMessage message);
	public final static native short Message_GetTrackingMwInfo(long jarg1, JNIMessage message);
	 */

	// Connection native class functions
	public final static native void delete_Connection(long jarg1, JNIConnection connection);

	public final static native long Connection_Connect(long jarg1, JNIConnection connection);

	public final static native long Connection_Disconnect(long jarg1, JNIConnection connection);

	// taken out of version 1.2 of Connection.h (may need to implement GetLibraryRootName method soon)
	// public final static native int Connection_GetType(long jarg1);

	public final static native long Connection_Subscribe(long jarg1, JNIConnection connection, String jarg2);

	public final static native long Connection_Subscribe_Via_Callback(long jarg1, JNIConnection connection, String jarg2, Object jarg3);

	public final static native long Connection_UnSubscribe(long jarg1, JNIConnection connection, String jarg2);

	public final static native long Connection_UnSubscribe_Via_Callback(long jarg1, JNIConnection connection, String jarg2, Object cb);

	public final static native long Connection_RegisterErrorCallback(long jarg1, JNIConnection connection, String jarg2, Object cb);

	public final static native long Connection_CreateMessage(long conn, JNIConnection connection, JNIMessage msg);

	public final static native long Connection_CreateMessage(long jarg1, JNIConnection connection, String jarg2, int jarg3, JNIMessage jarg4); //{{EAC

	public final static native long Connection_CreateMessageCFG(long jarg1, JNIConnection connection, String jarg2, int jarg3, JNIMessage jarg4, long jarg5, JNIConfig config);

	public final static native long Connection_DestroyMessage(long jarg1, JNIConnection connection, long jarg2, JNIMessage message);

	public final static native long Connection_Publish(long jarg1, JNIConnection connection, long jarg2, JNIMessage message);

	public final static native long Connection_Request(long jarg1, JNIConnection connection, long jarg2, JNIMessage request, int jarg3, JNIMessage jarg4);

	public final static native long Connection_Request_Via_Callback(long jConnPtr, JNIConnection connection, long jReqMsgPtr, JNIMessage request, int timeout, Object cb, boolean replyCallback);

	// Replaced by NATIVE implementation
	//public final static native long Connection_Request_Via_Callback(long jarg1, JNIConnection connection, long jarg2, JNIMessage request, int jarg3, long jarg4);
	public final static native long Connection_Reply(long jarg1, JNIConnection connection, long jarg2, JNIMessage request, long jarg3, JNIMessage reply);

	public final static native long Connection_GetNextMsg(long jarg1, JNIConnection connection, JNIMessage jarg2, int jarg3); //{{EAC

	public final static native long Connection_StartAutoDispatch(long jarg1, JNIConnection connection);

	public final static native long Connection_StopAutoDispatch(long jarg1, JNIConnection connection);

	public final static native long Connection_DispatchMsg(long jarg1, JNIConnection connection, long jarg2, JNIMessage jmsg);

	public final static native long Connection_GetLastDispatcherStatus(long jarg1, JNIConnection connection);

	public final static native long Connection_ExcludeSubject(long jarg1, JNIConnection connection, String jarg2);

	public final static native long Connection_RemoveExcludedSubject(long jarg1, JNIConnection connection, String jarg2);

	public final static native long Connection_CloneMessage(long jarg1, JNIConnection connection, long jarg2, JNIMessage in, JNIMessage jarg3);

	public final static native String Connection_GetLibraryRootName(long jarg1, JNIConnection connection);

	public final static native String Connection_GetLibraryVersion(long jarg1, JNIConnection connection);

	public final static native long Connection_GetPublishQueueMessageCount(long jarg1, JNIConnection connection);

	public final static native int Connection_IsConnected(long jarg1, JNIConnection connection);

	// Field native class functions
	public final static native long new_Field();

	//{{SC EAC changed from char type to byte type
	//    public final static native long Field_SetFieldValueCHAR(long jarg1, String jarg2, char jarg3);
	public final static native long Field_SetFieldValueCHAR(long jarg1, JNIField field, String jarg2, byte jarg3);

	public final static native long Field_SetFieldValueBOOL(long jarg1, JNIField field, String jarg2, boolean jarg3);

	public final static native long Field_SetFieldValueBYTE(long jarg1, JNIField field, String jarg2, byte jarg3);

	public final static native long Field_SetFieldValueUBYTE(long jarg1, JNIField field, String jarg2, int jarg3);

	public final static native long Field_SetFieldValueSHORT(long jarg1, JNIField field, String jarg2, short jarg3);

	public final static native long Field_SetFieldValueUSHORT(long jarg1, JNIField field, String jarg2, int jarg3);

	public final static native long Field_SetFieldValueLONG(long jarg1, JNIField field, String jarg2, int jarg3);

	public final static native long Field_SetFieldValueULONG(long jarg1, JNIField field, String jarg2, long jarg3);

	public final static native long Field_SetFieldValueFLOAT(long jarg1, JNIField field, String jarg2, float jarg3);

	public final static native long Field_SetFieldValueDOUBLE(long jarg1, JNIField field, String jarg2, double jarg3);

	public final static native long Field_SetFieldValueSTRING(long jarg1, JNIField field, String jarg2, String jarg3);

	public final static native long Field_SetFieldValueBIN(long jarg1, JNIField field, String jarg2, byte[] jarg3, long jarg4);

    public final static native long Field_SetFieldValueI8(long jarg1, JNIField field, String jarg2, byte jarg3);

    public final static native long Field_SetFieldValueU8(long jarg1, JNIField field, String jarg2, int jarg3);

	public final static native long Field_SetFieldValueU16(long jarg1, JNIField field, String jarg2, int jarg3);

	public final static native long Field_SetFieldValueI16(long jarg1, JNIField field, String jarg2, short jarg3);

	public final static native long Field_SetFieldValueU32(long jarg1, JNIField field, String jarg2, long jarg3);

	public final static native long Field_SetFieldValueI32(long jarg1, JNIField field, String jarg2, int jarg3);

	public final static native long Field_SetFieldValueI64(long jarg1, JNIField field, String jarg2, long jarg3);

	public final static native long Field_SetFieldValueF32(long jarg1, JNIField field, String jarg2, float jarg3);

	public final static native long Field_SetFieldValueF64(long jarg1, JNIField field, String jarg2, double jarg3);

	public final static native long new_Field_Copy(long jarg1, JNIField field);

	public final static native void delete_Field(long jarg1, JNIField field);

	public final static native long Field_GetName(long jarg, JNIField field1, GMSEC_String jarg2);

	public final static native long Field_SetName(long jarg1, JNIField field, String jarg2);

	public final static native long Field_SetType(long jarg1, JNIField field, int jarg2);

	public final static native long Field_GetType(long jarg1, JNIField field, GMSEC_UShort jarg2);

	public final static native long Field_GetValueBOOL(long jarg1, JNIField field, GMSEC_Bool jarg2);

	public final static native long Field_GetValueCHAR(long jarg1, JNIField field, GMSEC_Char jarg2);

	public final static native long Field_GetValueSTRING(long jarg1, JNIField field, GMSEC_String jarg2);

	public final static native long Field_GetValueBIN(long jarg1, JNIField field, GMSEC_Bin jarg2, GMSEC_ULong jarg3);

    public final static native long Field_GetValueI8(long jarg1, JNIField field, GMSEC_I8 jarg2);

    public final static native long Field_GetValueU8(long jarg1, JNIField field, GMSEC_U8 jarg2);

	public final static native long Field_GetValueU16(long jarg1, JNIField field, GMSEC_U16 jarg2);

	public final static native long Field_GetValueI16(long jarg1, JNIField field, GMSEC_I16 jarg2);

	public final static native long Field_GetValueU32(long jarg1, JNIField field, GMSEC_U32 jarg2);

	public final static native long Field_GetValueI32(long jarg1, JNIField field, GMSEC_I32 jarg2);

	public final static native long Field_GetValueI64(long jarg1, JNIField field, GMSEC_I64 jarg2);

	public final static native long Field_GetValueF32(long jarg1, JNIField field, GMSEC_F32 jarg2);

	public final static native long Field_GetValueF64(long jarg1, JNIField field, GMSEC_F64 jarg2);

	// this doesn't seem to make sense
	public final static native long Field_SetValueCHAR(long jarg1, JNIField field, byte jarg2);

	public final static native long Field_SetValueBOOL(long jarg1, JNIField field, boolean jarg2);

	public final static native long Field_SetValueSTRING(long jarg1, JNIField field, String jarg2);

	public final static native long Field_SetValueBIN(long jarg1, JNIField field, byte[] jarg2, long jarg3);

    public final static native long Field_SetValueI8(long jarg1, JNIField field, byte jarg2);

    public final static native long Field_SetValueU8(long jarg1, JNIField field, int jarg2);

	public final static native long Field_SetValueU16(long jarg1, JNIField field, int jarg2);

	public final static native long Field_SetValueI16(long jarg1, JNIField field, short jarg2);

	public final static native long Field_SetValueU32(long jarg1, JNIField field, long jarg2);

	public final static native long Field_SetValueI32(long jarg1, JNIField field, int jarg2);

	public final static native long Field_SetValueI64(long jarg1, JNIField field, long jarg2);

	public final static native long Field_SetValueF32(long jarg1, JNIField field, float jarg2);

	public final static native long Field_SetValueF64(long jarg1, JNIField field, double jarg2);

	public final static native long Field_UnSet(long jarg1, JNIField field);

	// Callback native class functions
	//..gone NATIVE - EAC 6/17/2003
	//public final static native void Callback_OnMessage(long jarg1, long jarg2, long jarg3);
	//.. gone NATIVE - EAC 6/17/2003
	//public final static native void delete_Callback(long jarg1);
	
	public final static native long new_Log();
	public final static native void delete_Log(long jarg1, JNILog log);
	public final static native void Log_SetReportingLevel(int jarg1);
	public final static native int Log_GetReportingLevel();
	public final static native void Log_RegisterHandler(LogHandler handler);
	public final static native void Log_RegisterHandler(int level, LogHandler handler);
	public final static native String Log_LogLevelToString(int jLevel);
	public final static native int Log_LogLevelFromString(String level);
	public final static native void Log_LogError(String message);
	public final static native void Log_LogWarning(String message);
	public final static native void Log_LogInfo(String message);
	public final static native void Log_LogVerbose(String message);
	public final static native void Log_LogDebug(String message);
	
	public final static native String TimeUtil_FormatTime_s(double time_s);


	// MIST related methods...
	public final static native long new_ConnectionManager(long cptr, JNIConfig config, int version, boolean validate);

	public final static native long new_ConnectionManager(long cptr, JNIConfig config);

	public final static native void delete_ConnectionManager(long jarg1, JNIConnectionManager connMgr);

	public final static native long ConnectionManager_SetStandardFields(long jarg1, JNIConnectionManager connMgr, long[] fldPtrs, JNIField[] flds, int numFields);

	public final static native long ConnectionManager_Initialize(long jarg1, JNIConnectionManager connMgr);

	public final static native long ConnectionManager_Cleanup(long jarg1, JNIConnectionManager connMgr);

	public final static native long ConnectionManager_RegisterErrorCallback(long jarg1, JNIConnectionManager connMgr, String event, Object cb);

	public final static native long ConnectionManager_Subscribe(long jarg1, JNIConnectionManager connMgr, String subject);

	public final static native long ConnectionManager_Subscribe(long jarg1, JNIConnectionManager connMgr, String subject, Object cb);

	public final static native long ConnectionManager_Subscribe(long jarg1, JNIConnectionManager connMgr, String subject, long cptr, JNIConfig config);

	public final static native long ConnectionManager_UnSubscribe(long jarg1, JNIConnectionManager connMgr, String subject);

	public final static native long ConnectionManager_UnSubscribe(long jarg1, JNIConnectionManager connMgr, String subject, Object cb);

	public final static native long ConnectionManager_CreateMessage(long jarg1, JNIConnectionManager connMgr, String subject, int kind, JNIMessage jmsg);

	public final static native long ConnectionManager_ConvertMessage(long jarg1, JNIConnectionManager connMgr, long in_msgptr, JNIMessage in_jmsg, long out_msgptr, JNIMessage out_jmsg);

	public final static native long ConnectionManager_ConvertCleanup(long jarg1, JNIConnectionManager connMgr, long in_msgptr, JNIMessage in_jmsg, long out_msgptr, JNIMessage out_jmsg);

	public final static native long ConnectionManager_CloneMessage(long jarg1, JNIConnectionManager connMgr, long in_msgptr, JNIMessage in_jmsg, long out_msgptr, JNIMessage out_jmsg);

	public final static native long ConnectionManager_DestroyMessage(long jarg1, JNIConnectionManager connMgr, long msgptr, JNIMessage jmsg);

	public final static native long ConnectionManager_Publish(long jarg1, JNIConnectionManager connMgr, long msgptr, JNIMessage jmsg);

	public final static native long ConnectionManager_Publish(long jarg1, JNIConnectionManager connMgr, long msgptr, JNIMessage jmsg, long cfgptr, JNIConfig config);

	public final static native long ConnectionManager_Request(long jarg1, JNIConnectionManager connMgr, long reqptr, JNIMessage jreq, int timeout, Object cb);

	public final static native long ConnectionManager_Request(long jarg1, JNIConnectionManager connMgr, long reqptr, JNIMessage jreq, int timeout, JNIMessage jrep);

	public final static native long ConnectionManager_Reply(long jarg1, JNIConnectionManager connMgr, long reqptr, JNIMessage jreq, long repptr, JNIMessage jrep);

	public final static native long ConnectionManager_StartAutoDispatch(long jarg1, JNIConnectionManager connMgr);

	public final static native long ConnectionManager_StopAutoDispatch(long jarg1, JNIConnectionManager connMgr);

	public final static native long ConnectionManager_StopAutoDispatch(long jarg1, JNIConnectionManager connMgr, boolean waitForCompletion);

	public final static native long ConnectionManager_GetNextMsg(long jarg1, JNIConnectionManager connMgr, long msgptr, JNIMessage jmsg, int timeout);

	public final static native long ConnectionManager_DispatchMsg(long jarg1, JNIConnectionManager connMgr, long msgptr, JNIMessage jmsg);

	public final static native long ConnectionManager_GetLastDispatcherStatus(long jarg1, JNIConnectionManager connMgr);

	public final static native long ConnectionManager_ExcludeSubject(long jarg1, JNIConnectionManager connMgr, String subject);

	public final static native long ConnectionManager_RemoveExcludedSubject(long jarg1, JNIConnectionManager connMgr, String subject);

	public final static native long ConnectionManager_GetHeartbeatMessage(long jarg1, JNIConnectionManager connMgr, long[] fldPtrs, JNIField[] flds, int numFields, long msgptr, JNIMessage jmsg, String subject);

	public final static native long ConnectionManager_StartHeartbeatService(long jarg1, JNIConnectionManager connMgr, long[] fldPtrs, JNIField[] flds, int numFields, String subject);

	public final static native long ConnectionManager_ChangeComponentStatus(long jarg1, JNIConnectionManager connMgr, long jptr, JNIField jfld);

	public final static native long ConnectionManager_ChangeComponentInfo(long jarg1, JNIConnectionManager connMgr, long jptr, JNIField jfld);

	public final static native long ConnectionManager_ChangeCPUMemory(long jarg1, JNIConnectionManager connMgr, long jptr, JNIField jfld);

	public final static native long ConnectionManager_ChangeCPUUtil(long jarg1, JNIConnectionManager connMgr, long jptr, JNIField jfld);

	public final static native long ConnectionManager_StopHeartbeatService(long jarg1, JNIConnectionManager connMgr);

	public final static native long ConnectionManager_GetLogMessage(long jarg1, JNIConnectionManager connMgr, long[] fldPtrs, JNIField[] flds, int numFields, long msgptr, JNIMessage jmsg, String subject);

	public final static native long ConnectionManager_SetLoggingDefaults(long jarg1, JNIConnectionManager connMgr, long[] fldPtrs, JNIField[] flds, int numFields, String subject);

	public final static native long ConnectionManager_QuickLog(long jarg1, JNIConnectionManager connMgr, String logmsg, long fldptr, JNIField jfld);
}
