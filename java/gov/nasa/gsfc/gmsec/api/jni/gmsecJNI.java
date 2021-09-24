/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.*;

import gov.nasa.gsfc.gmsec.api.mist.*;
import gov.nasa.gsfc.gmsec.api.mist.message.*;

import gov.nasa.gsfc.gmsec.api.util.*;

import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIBinaryField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIBooleanField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNICharField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNII16Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNII32Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNII64Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNII8Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIF32Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIF64Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIStringField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU16Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU32Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU64Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU8Field;

import gov.nasa.gsfc.gmsec.api.jni.util.JNILogHandler;
import gov.nasa.gsfc.gmsec.api.jni.util.JNITimeSpec;

import gov.nasa.gsfc.gmsec.api.jni.mist.*;
import gov.nasa.gsfc.gmsec.api.jni.mist.message.*;

import java.util.List;


/** @class gmsecJNI
 *
 * @brief This file declares all native JNI methods that are interfacing with the
 * functions in the GMSEC API library.
 *
 */
public class gmsecJNI
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
		
		// Enable thread detach in JNI
		setEnableDetach(true);
	}

	private final static native void initialize();
	private final static native void setEnableDetach(boolean enableDetach);


	// Callback native class functions
	//
	public final static native long new_Callback(Object callback);
	public final static native void delete_Callback(long jarg1, JNICallback jcb);


	// Config native class functions
	//
	public final static native long new_Config();
	public final static native long new_Config_Args(String jarg1[]);
	public final static native long new_Config_Data(String jarg1);
	public final static native long new_Config_Copy(long jarg1, JNIConfig config);
	public final static native void delete_Config(long jarg1, JNIConfig config);
	public final static native void Config_AddValue(long jarg1, JNIConfig config, String jarg2, String jarg3);
	public final static native boolean Config_ClearValue(long jarg1, JNIConfig config, String jarg2);
	public final static native void Config_Clear(long jarg1, JNIConfig config);
	public final static native String[] Config_KeySet(long cfg, JNIConfig config);
	public final static native String[] Config_Values(long cfg, JNIConfig config);
	public final static native String Config_GetValue(long jarg1, JNIConfig config, String jarg2, String jarg3);	
	public final static native boolean Config_GetBooleanValue(long jarg1, JNIConfig config, String jarg2);
	public final static native boolean Config_GetBooleanValue(long jarg1, JNIConfig config, String jarg2, boolean jarg3);
	public final static native int Config_GetIntegerValue(long jarg1, JNIConfig config, String jarg2);
	public final static native int Config_GetIntegerValue(long jarg1, JNIConfig config, String jarg2, int jarg3);
	public final static native double Config_GetDoubleValue(long jarg1, JNIConfig config, String jarg2);
	public final static native double Config_GetDoubleValue(long jarg1, JNIConfig config, String jarg2, double jarg3);
	public final static native void Config_Merge(long jarg1, JNIConfig config, long jarg2, JNIConfig otherConfig, boolean overwriteExisting);
	public final static native String Config_ToXML(long cfg, JNIConfig config);
	public final static native void Config_FromXML(long cfg, JNIConfig config, String xml);
	public final static native String Config_ToJSON(long cfg, JNIConfig config);


	// ConfigEntry native class functions
	//
	public final static native void delete_ConfigEntry(long jarg1, JNIConfigEntry entry);
	public final static native String ConfigEntry_GetName(long jarg1, JNIConfigEntry entry);
	public final static native long ConfigEntry_GetConfig(long jarg1, JNIConfigEntry entry);


	// ConfigFile native class functions
	//
	public final static native long new_ConfigFile();
	public final static native void delete_ConfigFile(long jarg1, JNIConfigFile configFile);
	public final static native void ConfigFile_Load(long jarg1, JNIConfigFile configFile, String filePath);
	public final static native void ConfigFile_Save(long jarg1, JNIConfigFile configFile, String filePath, boolean compact);
	public final static native void ConfigFile_FromXML(long jarg1, JNIConfigFile configFile, String xml);
	public final static native String ConfigFile_ToXML(long ptr, JNIConfigFile cfile);
	public final static native boolean ConfigFile_IsLoaded(long jarg1, JNIConfigFile configFile);
	public final static native long ConfigFile_LookupConfig(long jarg1, JNIConfigFile configFile, String name);
	public final static native long ConfigFile_LookupMessage(long jarg1, JNIConfigFile configFile, String name);
	public final static native String ConfigFile_LookupSubscription(long jarg1, JNIConfigFile configFile, String name);
	public final static native long ConfigFile_LookupSubscriptionEntry(long jarg1, JNIConfigFile configFile, String name);
	public final static native void ConfigFile_AddSubscription(long jarg1, JNIConfigFile configFile, String name, String subject);
	public final static native void ConfigFile_AddConfig(long jarg1, JNIConfigFile configFile, String name, long jarg2, JNIConfig config);
	public final static native void ConfigFile_AddMessage(long jarg1, JNIConfigFile configFile, String name, long jarg2, JNIMessage msg);
	public final static native void ConfigFile_AddCustomXML(long jarg1, JNIConfigFile configFile, String xml);
	public final static native boolean ConfigFile_RemoveSubscription(long jarg1, JNIConfigFile configFile, String name);
	public final static native boolean ConfigFile_RemoveConfig(long jarg1, JNIConfigFile configFile, String name);
	public final static native boolean ConfigFile_RemoveMessage(long jarg1, JNIConfigFile configFile, String name);
	public final static native boolean ConfigFile_RemoveCustomXML(long jarg1, JNIConfigFile configFile, String xml);
	public final static native long ConfigFile_GetIterator(long jarg1, JNIConfigFile configFile);


	// ConfigFileIterator native class functions
	//
	public final static native boolean ConfigFileIterator_HasNextConfig(long jarg1, JNIConfigFileIterator iter);
	public final static native boolean ConfigFileIterator_HasNextMessage(long jarg1, JNIConfigFileIterator iter);
	public final static native boolean ConfigFileIterator_HasNextSubscription(long jarg1, JNIConfigFileIterator iter);
	public final static native boolean ConfigFileIterator_HasNextCustomElement(long jarg1, JNIConfigFileIterator iter);
	public final static native long ConfigFileIterator_NextConfig(long jarg1, JNIConfigFileIterator iter);
	public final static native long ConfigFileIterator_NextMessage(long jarg1, JNIConfigFileIterator iter);
	public final static native long ConfigFileIterator_NextSubscription(long jarg1, JNIConfigFileIterator iter);
	public final static native String ConfigFileIterator_NextCustomElement(long jarg1, JNIConfigFileIterator iter);
	public final static native void ConfigFileIterator_Reset(long jarg1, JNIConfigFileIterator iter);


	// Connection native class functions
	//
	public final static native long Connection_Create(long jarg1, JNIConfig config, JNIConnection conn);
	public final static native void Connection_Destroy(long jarg1, JNIConnection connection);
	public final static native String Connection_GetAPIVersion();
	public final static native void Connection_Connect(long jarg1, JNIConnection connection);
	public final static native void Connection_Disconnect(long jarg1, JNIConnection connection);
	public final static native int Connection_GetState(long jarg1, JNIConnection connection);
	public final static native String Connection_GetLibraryRootName(long jarg1, JNIConnection connection);
	public final static native String Connection_GetLibraryVersion(long jarg1, JNIConnection connection);
	public final static native void Connection_RegisterEventCallback(long jarg1, JNIConnection connection, int event, long cbPtr);
	public final static native long Connection_Subscribe(long jarg1, JNIConnection connection, String subject);
	public final static native long Connection_Subscribe(long jarg1, JNIConnection connection, String subject, long cbPtr);
	public final static native long Connection_Subscribe(long jarg1, JNIConnection connection, String subject, long jarg2, JNIConfig config);
	public final static native long Connection_Subscribe(long jarg1, JNIConnection connection, String subject, long jarg2, JNIConfig config, long cbPtr);
	public final static native void Connection_Unsubscribe(long jarg1, JNIConnection connection, long jarg2, JNISubscriptionInfo info);
	public final static native boolean Connection_StartAutoDispatch(long jarg1, JNIConnection connection);
	public final static native boolean Connection_StopAutoDispatch(long jarg1, JNIConnection connection, boolean waitForCompletion);
	public final static native void Connection_Publish(long jarg1, JNIConnection connection, long jarg2, JNIMessage message);
	public final static native void Connection_Publish(long jarg1, JNIConnection connection, long jarg2, JNIMessage message, long jarg3, JNIConfig config);
	public final static native void Connection_Request(long jarg1, JNIConnection connection, long jarg2, JNIMessage request, int timeout, long cbPtr, int republish_ms);
	public final static native long Connection_Request(long jarg1, JNIConnection connection, long jarg2, JNIMessage request, int timeout, int republish_ms);
	public final static native void Connection_CancelRequest(long jarg1, JNIConnection connection, long cbPtr);
	public final static native void Connection_Reply(long jarg1, JNIConnection connection, long jarg2, JNIMessage request, long jarg3, JNIMessage reply);
	public final static native void Connection_Dispatch(long jarg1, JNIConnection connection, long jarg2, JNIMessage jmsg);
	public final static native long Connection_Receive(long jarg1, JNIConnection connection, int timeout);
	public final static native void Connection_ExcludeSubject(long jarg1, JNIConnection connection, String subject);
	public final static native void Connection_RemoveExcludedSubject(long jarg1, JNIConnection connection, String subject);
	public final static native String Connection_GetName(long jarg1, JNIConnection connection);
	public final static native void Connection_SetName(long jarg1, JNIConnection connection, String name);
	public final static native String Connection_GetID(long jarg1, JNIConnection connection);
	public final static native String Connection_GetMWInfo(long jarg1, JNIConnection connection);
	public final static native String Connection_GetConnectionEndpoint(long jarg1, JNIConnection connection);
	public final static native long Connection_GetPublishQueueMessageCount(long jarg1, JNIConnection connection);
	public final static native void Connection_ShutdownAllMiddlewares();
	public final static native void Connection_ShutdownMiddleware(String name);


	// EventCallback native class functions
	//
	public final static native long new_EventCallback(Object eventCallback);
	public final static native void delete_EventCallback(long jarg1, JNIEventCallback jcb);


	// HeartbeatGenerator native class functions
	//
	public final static native long new_HeartbeatGenerator(long jarg1, JNIConfig config, String subject, int pubRate);
	public final static native long new_HeartbeatGenerator(long jarg1, JNIConfig config, String subject, int pubRate, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void delete_HeartbeatGenerator(long jarg1, JNIHeartbeatGenerator hbgen);
	public final static native boolean HeartbeatGenerator_Start(long jarg1, JNIHeartbeatGenerator hbgen);
	public final static native boolean HeartbeatGenerator_Stop(long jarg1, JNIHeartbeatGenerator hbgen);
	public final static native boolean HeartbeatGenerator_IsRunning(long jarg1, JNIHeartbeatGenerator hbgen);
	public final static native void HeartbeatGenerator_ChangePublishRate(long jarg1, JNIHeartbeatGenerator hbgen, int pubRate);
	public final static native boolean HeartbeatGenerator_SetField(long jarg1, JNIHeartbeatGenerator hbgen, long jarg2, JNIField field);
	public final static native boolean HeartbeatGenerator_SetField(long jarg1, JNIHeartbeatGenerator hbgen, String fieldName, long fieldValue);
	public final static native boolean HeartbeatGenerator_SetField(long jarg1, JNIHeartbeatGenerator hbgen, String fieldName, double fieldValue);
	public final static native boolean HeartbeatGenerator_SetField(long jarg1, JNIHeartbeatGenerator hbgen, String fieldName, String fieldValue);


	// Log native class functions
	//
	public final static native void Log_SetReportingLevel(int jarg1);
	public final static native int Log_GetReportingLevel();
	public final static native void Log_RegisterHandler(long logHandlerPtr);
	public final static native void Log_RegisterHandler(int level, long logHandlerPtr);
	public final static native String Log_LogLevelToString(int jLevel);
	public final static native int Log_LogLevelFromString(String level);


	// LogHandler native class functions
	//
	public final static native long new_LogHandler(Object handler);
	public final static native void delete_LogHandler(long jarg1, JNILogHandler jHandler);


	// Message native class functions
	//
	public final static native long new_Message(String subject, int kind);
	public final static native long new_Message(String subject, int kind, long cfgPtr, JNIConfig config);
	public final static native long new_Message(String xml);
	public final static native long new_Message_Copy(long jarg1, JNIMessage msg);
	public final static native void delete_Message(long jarg1, JNIMessage msg);
	public final static native void Message_AddConfig(long jarg1, JNIMessage msg, long cfgPtr, JNIConfig config);
	public final static native long Message_GetConfig(long jarg1, JNIMessage msg);
	public final static native String Message_GetSubject(long jarg1, JNIMessage msg);
	public final static native void Message_SetSubject(long jarg1, JNIMessage msg, String subject);
	public final static native int Message_GetKind(long jarg1, JNIMessage msg);
	public final static native boolean Message_AddField(long jarg1, JNIMessage msg, long fieldPtr, JNIField field);
	public final static native boolean Message_AddFieldByteArray(long jarg1, JNIMessage msg, String name, byte[] value);
	public final static native boolean Message_AddFieldBoolean(long jarg1, JNIMessage msg, String name, boolean value);
	public final static native boolean Message_AddFieldChar(long jarg1, JNIMessage msg, String name, char value);
	public final static native boolean Message_AddFieldByte(long jarg1, JNIMessage msg, String name, byte value);
	public final static native boolean Message_AddFieldShort(long jarg1, JNIMessage msg, String name, short value);
	public final static native boolean Message_AddFieldInt(long jarg1, JNIMessage msg, String name, int value);
	public final static native boolean Message_AddFieldLong(long jarg1, JNIMessage msg, String name, long value);
	public final static native boolean Message_AddFieldFloat(long jarg1, JNIMessage msg, String name, float value);
	public final static native boolean Message_AddFieldDouble(long jarg1, JNIMessage msg, String name, double value);
	public final static native boolean Message_AddFieldString(long jarg1, JNIMessage msg, String name, String value);
	public final static native void Message_ClearFields(long jarg1, JNIMessage msg);
	public final static native boolean Message_ClearField(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetIntegerValue(long jarg1, JNIMessage msg, String fieldName);
	public final static native U64 Message_GetUnsignedIntegerValue(long jarg1, JNIMessage msg, String fieldName);
	public final static native double Message_GetDoubleValue(long jarg1, JNIMessage msg, String fieldName);
	public final static native String Message_GetStringValue(long jarg1, JNIMessage msg, String fieldName);
	public final static native long Message_GetField(long jarg1, JNIMessage msg, String name);
	public final static native int Message_GetFieldType(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetBinaryField(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetBooleanField(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetCharField(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetF32Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetF64Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetI8Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetI16Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetI32Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetI64Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetU8Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetU16Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetU32Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetU64Field(long jarg1, JNIMessage msg, String name);
	public final static native long Message_GetStringField(long jarg1, JNIMessage msg, String name);
	public final static native int Message_GetFieldCount(long jarg1, JNIMessage msg);
	public final static native void Message_CopyFields(long jarg1, JNIMessage msg, long toMsgPtr, JNIMessage toMsg);
	public final static native String Message_ToXML(long jarg1, JNIMessage msg);
	public final static native String Message_ToJSON(long jarg1, JNIMessage msg);
	public final static native long Message_GetSize(long jarg1, JNIMessage msg);
	public final static native long Message_GetFieldIterator(long jarg1, JNIMessage msg, int selector);


	// MessageEntry native class functions
	//
	public final static native void delete_MessageEntry(long jarg1, JNIMessageEntry entry);
	public final static native String MessageEntry_GetName(long jarg1, JNIMessageEntry entry);
	public final static native long MessageEntry_GetMessage(long jarg1, JNIMessageEntry entry);


	// MessageFieldIterator native class functions
	//
	public final static native boolean MessageFieldIterator_HasNext(long jarg1, JNIMessageFieldIterator iter);
	public final static native long MessageFieldIterator_Next(long jarg1, JNIMessageFieldIterator iter);
	public final static native void MessageFieldIterator_Reset(long jarg1, JNIMessageFieldIterator iter);


	// ReplyCallback native class functions
	//
	public final static native long new_ReplyCallback(Object replyCallback);
	public final static native void delete_ReplyCallback(long jarg1, JNIReplyCallback jcb);


	// Status native class functions
	//
	public final static native long new_Status();
	public final static native long new_Status_Set(int jarg1, int jarg2, String jarg3);
	public final static native long new_Status_Set(int jarg1, int jarg2, String jarg3, int jarg4);
	public final static native long new_Status_Copy(long jarg1, JNIStatus status);
	public final static native void delete_Status(long jarg1, JNIStatus status);
	public final static native int Status_IsError(long jarg1, JNIStatus status);
	public final static native String Status_Get(long jarg1, JNIStatus status);
	public final static native int Status_GetClassification(long jarg1, JNIStatus status);
	public final static native void Status_SetClassification(long jarg1, JNIStatus status, int jarg2);
	public final static native int Status_GetCode(long jarg1, JNIStatus status);
	public final static native void Status_SetCode(long jarg1, JNIStatus status, int jarg2);
	public final static native String Status_GetReason(long jarg1, JNIStatus status);
	public final static native void Status_SetReason(long jarg1, JNIStatus status, String jarg2);
	public final static native int Status_GetCustomCode(long jarg1, JNIStatus status);
	public final static native void Status_SetCustomCode(long jarg1, JNIStatus status, int jarg2);
	public final static native void Status_Set(long jarg1, JNIStatus status, int jarg2, int jarg3, String jarg4);
	public final static native void Status_Set(long jarg1, JNIStatus status, int jarg2, int jarg3, String jarg4, int jarg5);
	public final static native void Status_Reset(long jarg1, JNIStatus status);


	// SubscriptionEntry native class functions
	//
	public final static native void delete_SubscriptionEntry(long jarg1, JNISubscriptionEntry entry);
	public final static native String SubscriptionEntry_GetName(long jarg1, JNISubscriptionEntry entry);
	public final static native String SubscriptionEntry_GetSubject(long jarg1, JNISubscriptionEntry entry);
	public final static native String SubscriptionEntry_GetPattern(long jarg1, JNISubscriptionEntry entry);
	public final static native boolean SubscriptionEntry_HasNextExcludedPattern(long jarg1, JNISubscriptionEntry entry);
	public final static native String SubscriptionEntry_NextExcludedPattern(long jarg1, JNISubscriptionEntry entry);

	// Base Field native class functions
	public final static native int Field_GetType(long fieldPtr, JNIField field);
	public final static native String Field_GetName(long fieldPtr, JNIField field);
	public final static native void Field_SetName(long fieldPtr, JNIField field, String fieldName);
	public final static native String Field_ToXML(long fieldPtr, JNIField field);
	public final static native String Field_ToJSON(long fieldPtr, JNIField field);
	public final static native long Field_GetIntegerValue(long fieldPtr, JNIField field);
	public final static native U64 Field_GetUnsignedIntegerValue(long fieldPtr, JNIField field);
	public final static native double Field_GetDoubleValue(long fieldPtr, JNIField field);
	public final static native String Field_GetStringValue(long fieldPtr, JNIField field);
	public final static native void Field_IsHeader(long fieldPtr, JNIField field, boolean header);
	public final static native boolean Field_IsHeader(long fieldPtr, JNIField field);
	public final static native void delete_Field(long fieldPtr, JNIField field);


	// BinaryField native class functions
	public final static native long new_BinaryField(String name, byte[] data, long dataLength);
	public final static native long new_BinaryField_Copy(long fieldPtr, JNIBinaryField field);
	public final static native byte[] BinaryField_GetValue(long fieldPtr, JNIBinaryField field);


	// BooleanField native class functions
	public final static native long new_BooleanField(String name, boolean data);
	public final static native long new_BooleanField_Copy(long fieldPtr, JNIBooleanField field);
	public final static native boolean BooleanField_GetValue(long fieldPtr, JNIBooleanField field);


	// CharField native class functions
	public final static native long new_CharField(String name, char data);
	public final static native long new_CharField_Copy(long fieldPtr, JNICharField field);
	public final static native char CharField_GetValue(long fieldPtr, JNICharField field);


	// F32Field native class functions
	public final static native long new_F32Field(String name, float data);
	public final static native long new_F32Field_Copy(long fieldPtr, JNIF32Field field);
	public final static native float F32Field_GetValue(long fieldPtr, JNIF32Field field);


	// F64Field native class functions
	public final static native long new_F64Field(String name, double data);
	public final static native long new_F64Field_Copy(long fieldPtr, JNIF64Field field);
	public final static native double F64Field_GetValue(long fieldPtr, JNIF64Field field);


	// I8Field native class functions
	public final static native long new_I8Field(String name, byte data);
	public final static native long new_I8Field_Copy(long fieldPtr, JNII8Field field);
	public final static native byte I8Field_GetValue(long fieldPtr, JNII8Field field);


	// I16Field native class functions
	public final static native long new_I16Field(String name, short data);
	public final static native long new_I16Field_Copy(long fieldPtr, JNII16Field field);
	public final static native short I16Field_GetValue(long fieldPtr, JNII16Field field);


	// I32Field native class functions
	public final static native long new_I32Field(String name, int data);
	public final static native long new_I32Field_Copy(long fieldPtr, JNII32Field field);
	public final static native int I32Field_GetValue(long fieldPtr, JNII32Field field);


	// I64Field native class functions
	public final static native long new_I64Field(String name, long data);
	public final static native long new_I64Field_Copy(long fieldPtr, JNII64Field field);
	public final static native long I64Field_GetValue(long fieldPtr, JNII64Field field);


	// U8Field native class functions
	public final static native long new_U8Field(String name, int data);
	public final static native long new_U8Field_Copy(long fieldPtr, JNIU8Field field);
	public final static native int U8Field_GetValue(long fieldPtr, JNIU8Field field);


	// U16Field native class functions
	public final static native long new_U16Field(String name, int data);
	public final static native long new_U16Field_Copy(long fieldPtr, JNIU16Field field);
	public final static native int U16Field_GetValue(long fieldPtr, JNIU16Field field);


	// U32Field native class functions
	public final static native long new_U32Field(String name, long data);
	public final static native long new_U32Field_Copy(long fieldPtr, JNIU32Field field);
	public final static native long U32Field_GetValue(long fieldPtr, JNIU32Field field);


	// U64Field native class functions
	public final static native long new_U64Field(String name, String data);
	public final static native long new_U64Field_Copy(long fieldPtr, JNIU64Field field);
	public final static native String U64Field_GetValue(long fieldPtr, JNIU64Field field);


	// StringField native class functions
	public final static native long new_StringField(String name, String data);
	public final static native long new_StringField_Copy(long fieldPtr, JNIStringField field);
	public final static native String StringField_GetValue(long fieldPtr, JNIStringField field);


	// TimeSpec native class functions
	//
	public final static native long new_TimeSpec();
	public final static native void delete_TimeSpec(long timespecPtr, JNITimeSpec timespec);
	public final static native long TimeSpec_GetSeconds(long timespecPtr, JNITimeSpec timespec);
	public final static native long TimeSpec_GetNanoseconds(long timespecPtr, JNITimeSpec timespec);


	// TimeUtil native class functions
	//
	public final static native long TimeUtil_GetCurrentTime();
	public final static native String TimeUtil_FormatTime(long seconds, long nanoseconds, int subseconds);


	// MIST ConnectionManager native class functions
	//
	public final static native long new_ConnectionManager(long cptr, JNIConfig config);
	public final static native long new_ConnectionManager(long cptr, JNIConfig config, boolean validate);
	public final static native long new_ConnectionManager(long cptr, JNIConfig config, boolean validate, int version);
	public final static native void delete_ConnectionManager(long jarg1, JNIConnectionManager connMgr);
	public final static native void ConnectionManager_Initialize(long jarg1, JNIConnectionManager connMgr);
	public final static native void ConnectionManager_Cleanup(long jarg1, JNIConnectionManager connMgr);
	public final static native int ConnectionManager_GetState(long jarg1, JNIConnectionManager connMgr);
	public final static native String ConnectionManager_GetLibraryRootName(long jarg1, JNIConnectionManager connMgr);
	public final static native String ConnectionManager_GetLibraryVersion(long jarg1, JNIConnectionManager connMgr);
	public final static native long ConnectionManager_GetSpecification(long jarg1, JNIConnectionManager connMgr);
	public final static native void ConnectionManager_SetStandardFields(long jarg1, JNIConnectionManager connMgr, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native long[] ConnectionManager_GetStandardFields(long jarg1, JNIConnectionManager connMgr);
	public final static native void ConnectionManager_AddStandardFields(long jarg1, JNIConnectionManager connMgr, long msgPtr, JNIMessage msg);
	public final static native void ConnectionManager_RegisterMessageValidator(long jarg1, JNIConnectionManager connMgr, long valPtr);
	public final static native void ConnectionManager_RegisterEventCallback(long jarg1, JNIConnectionManager connMgr, int event, long cbPtr);
	public final static native long ConnectionManager_Subscribe(long jarg1, JNIConnectionManager connMgr, String subject);
	public final static native long ConnectionManager_Subscribe(long jarg1, JNIConnectionManager connMgr, String subject, long cbPtr);
	public final static native long ConnectionManager_Subscribe(long jarg1, JNIConnectionManager connMgr, String subject, long cptr, JNIConfig config);
	public final static native long ConnectionManager_Subscribe(long jarg1, JNIConnectionManager connMgr, String subject, long cptr, JNIConfig config, long cbPtr);
	public final static native void ConnectionManager_Unsubscribe(long jarg1, JNIConnectionManager connMgr, long infoPtr, JNIConnMgrSubscriptionInfo info);
	public final static native void ConnectionManager_Publish(long jarg1, JNIConnectionManager connMgr, long msgptr, JNIMessage jmsg);
	public final static native void ConnectionManager_Publish(long jarg1, JNIConnectionManager connMgr, long msgptr, JNIMessage jmsg, long cfgptr, JNIConfig config);
	public final static native void ConnectionManager_Request(long jarg1, JNIConnectionManager connMgr, long reqptr, JNIMessage jreq, int timeout, long cbPtr, int republish_ms);
	public final static native long ConnectionManager_Request(long jarg1, JNIConnectionManager connMgr, long reqptr, JNIMessage jreq, int timeout, int republish_ms);
	public final static native void ConnectionManager_CancelRequest(long jarg1, JNIConnectionManager connMgr, long cbPtr);
	public final static native void ConnectionManager_Reply(long jarg1, JNIConnectionManager connMgr, long reqptr, JNIMessage jreq, long repptr, JNIMessage jrep);
	public final static native void ConnectionManager_Dispatch(long jarg1, JNIConnectionManager connMgr, long msgptr, JNIMessage jmsg);
	public final static native long ConnectionManager_Receive(long jarg1, JNIConnectionManager connMgr, int timeout);
	public final static native boolean ConnectionManager_StartAutoDispatch(long jarg1, JNIConnectionManager connMgr);
	public final static native boolean ConnectionManager_StopAutoDispatch(long jarg1, JNIConnectionManager connMgr, boolean waitForCompletion);
	public final static native void ConnectionManager_ExcludeSubject(long jarg1, JNIConnectionManager connMgr, String subject);
	public final static native void ConnectionManager_RemoveExcludedSubject(long jarg1, JNIConnectionManager connMgr, String subject);
	public final static native String ConnectionManager_GetName(long jarg1, JNIConnectionManager connMgr);
	public final static native void ConnectionManager_SetName(long jarg1, JNIConnectionManager connMgr, String name);
	public final static native String ConnectionManager_GetID(long jarg1, JNIConnectionManager connMgr);
	public final static native String ConnectionManager_GetMWInfo(long jarg1, JNIConnectionManager connMgr);
	public final static native String ConnectionManager_GetConnectionEndpoint(long jarg1, JNIConnectionManager connMgr);
	public final static native long ConnectionManager_GetPublishQueueMessageCount(long jarg1, JNIConnectionManager connMgr);
	public final static native long ConnectionManager_CreateHeartbeatMessage(long jarg1, JNIConnectionManager connMgr, String subject, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void ConnectionManager_StartHeartbeatService(long jarg1, JNIConnectionManager connMgr, String subject, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void ConnectionManager_StopHeartbeatService(long jarg1, JNIConnectionManager connMgr);
	public final static native long ConnectionManager_SetHeartbeatServiceField(long jarg1, JNIConnectionManager connMgr, long jptr, JNIField jfld);
	public final static native long ConnectionManager_CreateLogMessage(long jarg1, JNIConnectionManager connMgr, String subject, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void ConnectionManager_SetLoggingDefaults(long jarg1, JNIConnectionManager connMgr, String subject, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void ConnectionManager_PublishLog(long jarg1, JNIConnectionManager connMgr, String logmsg, long fldptr, JNIField jfld);
	public final static native void ConnectionManager_RequestDirective(long jarg1, JNIConnectionManager connMgr, String subject, long fldptr, JNIField jfld, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void ConnectionManager_RequestDirective(long jarg1, JNIConnectionManager connMgr, String subject, long fldptr, JNIField jfld, long[] fldPtrs, JNIField[] flds, int numFields, int timeout, long cbPtr, int republish_ms);
	public final static native long ConnectionManager_RequestDirective(long jarg1, JNIConnectionManager connMgr, String subject, long fldptr, JNIField jfld, long[] fldPtrs, JNIField[] flds, int numFields, int timeout, int republish_ms);
	public final static native void ConnectionManager_AcknowledgeDirectiveRequest(long jarg1, JNIConnectionManager connMgr, String subject, long msgptr, JNIMessage jmsg, int status, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void ConnectionManager_PublishResourceMessage(long jarg1, JNIConnectionManager connMgr, String subject, long sampleInterval, long averageInterval);
	public final static native long ConnectionManager_CreateResourceMessage(long jarg1, JNIConnectionManager connMgr, String subject, long sampleInterval, long averageInterval);
	public final static native void ConnectionManager_StartResourceMessageService(long jarg1, JNIConnectionManager connMgr, String subject, long interval, long sampleInterval, long averageInterval);
	public final static native boolean ConnectionManager_StopResourceMessageService(long jarg1, JNIConnectionManager connMgr);
	public final static native void ConnectionManager_AcknowledgeSimpleService(long jarg1, JNIConnectionManager connMgr, String subject, long reqPtr, JNIMessage req, int status, long[] fieldPtrs, JNIField[] fields, int numFields);
	public final static native void ConnectionManager_RequestSimpleService(long jarg1, JNIConnectionManager connMgr, String subject, String opName, long fldPtr, JNIField opNumFld, long[] fieldPtrs, JNIField[] fields, int numFields, long[] paramPtrs, JNIServiceParam[] params, int numParams);
	public final static native void ConnectionManager_RequestSimpleService(long jarg1, JNIConnectionManager connMgr, String subject, String opName, long fldPtr, JNIField opNumFld, long[] fieldPtrs, JNIField[] fields, int numFields, long[] paramPtrs, JNIServiceParam[] params, int numParams, int timeout, long cbPtr, int republish_ms);
	public final static native long ConnectionManager_RequestSimpleService(long jarg1, JNIConnectionManager connMgr, String subject, String opName, long fldPtr, JNIField opNumFld, long[] fieldPtrs, JNIField[] fields, int numFields, long[] paramPtrs, JNIServiceParam[] params, int numParams, int timeout, int republish_ms);


	// MIST ConnectionManagerCallback native class functions
	//
	public final static native long new_ConnectionManagerCallback(Object callback);
	public final static native void delete_ConnectionManagerCallback(long jarg1, JNIConnMgrCallback jcb);


	// MIST ConnectionManagerEventCallback native class functions
	//
	public final static native long new_ConnectionManagerEventCallback(Object callback);
	public final static native void delete_ConnectionManagerEventCallback(long jarg1, JNIConnMgrEventCallback jcb);


	// MIST ConnectionManagerReplyCallback native class functions
	//
	public final static native long new_ConnectionManagerReplyCallback(Object callback);
	public final static native void delete_ConnectionManagerReplyCallback(long jarg1, JNIConnMgrReplyCallback jcb);


	// MIST Device native class functions
	//
	public final static native long new_Device(String name, int status);
	public final static native long new_Device(String name, int status, long[] paramPtrs, JNIDeviceParam[] jParams, int numParams); 
	public final static native long new_Device_Copy(long jarg1, JNIDevice jDevice);
	public final static native void delete_Device(long jarg1, JNIDevice jDevice);
	public final static native String Device_GetName(long jarg1, JNIDevice jDevice);
	public final static native boolean Device_NumberAvailable(long jarg1, JNIDevice jDevice);
	public final static native long Device_GetNumber(long jarg1, JNIDevice jDevice);
	public final static native void Device_SetNumber(long jarg1, JNIDevice jDevice, long jarg2, JNIField jField);
	public final static native boolean Device_ModelAvailable(long jarg1, JNIDevice jDevice);
	public final static native String Device_GetModel(long jarg1, JNIDevice jDevice);
	public final static native void Device_SetModel(long jarg1, JNIDevice jDevice, String model);
	public final static native boolean Device_SerialAvailable(long jarg1, JNIDevice jDevice);
	public final static native String Device_GetSerial(long jarg1, JNIDevice jDevice);
	public final static native void Device_SetSerial(long jarg1, JNIDevice jDevice, String serial);
	public final static native boolean Device_VersionAvailable(long jarg1, JNIDevice jDevice);
	public final static native String Device_GetVersion(long jarg1, JNIDevice jDevice);
	public final static native void Device_SetVersion(long jarg1, JNIDevice jDevice, String version);
	public final static native boolean Device_GroupAvailable(long jarg1, JNIDevice jDevice);
	public final static native String Device_GetGroup(long jarg1, JNIDevice jDevice);
	public final static native void Device_SetGroup(long jarg1, JNIDevice jDevice, String group);
	public final static native boolean Device_RoleAvailable(long jarg1, JNIDevice jDevice);
	public final static native String Device_GetRole(long jarg1, JNIDevice jDevice);
	public final static native void Device_SetRole(long jarg1, JNIDevice jDevice, String role);
	public final static native int Device_GetStatus(long jarg1, JNIDevice jDevice);
	public final static native void Device_SetStatus(long jarg1, JNIDevice jDevice, int status);
	public final static native boolean Device_InfoAvailable(long jarg1, JNIDevice jDevice);
	public final static native long Device_GetInfo(long jarg1, JNIDevice jDevice);
	public final static native void Device_SetInfo(long jarg1, JNIDevice jDevice, long jarg2, JNIField jField);
	public final static native long Device_GetParamCount(long jarg1, JNIDevice jDevice);
	public final static native long Device_GetParam(long jarg1, JNIDevice jDevice, long index);
	public final static native void Device_AddParam(long jarg1, JNIDevice jDevice, long jarg2, JNIDeviceParam jParam);


	// MIST DeviceIterator native class functions
	//
	public final static native boolean DeviceIterator_HasNext(long jarg1, JNIDeviceIterator iter);
	public final static native long DeviceIterator_Next(long jarg1, JNIDeviceIterator iter);
	public final static native void DeviceIterator_Reset(long jarg1, JNIDeviceIterator iter);


	// MIST DeviceMessage native class functions
	//
	public final static native long new_DeviceMessage(String subject, long jarg1, JNISpecification jSpec);
	public final static native long new_DeviceMessage(String subject, long jarg1, JNIConfig jConfig, long jarg2, JNISpecification jSpec);
	public final static native long new_DeviceMessage(String data);
	public final static native long new_DeviceMessage(long jarg1, JNISpecification jSpec, String data);
	public final static native long new_DeviceMessage_Copy(long jarg1, JNIDeviceMessage jDevMsg);
	public final static native void DeviceMessage_AddDevice(long jarg1, JNIDeviceMessage jDevMsg, long jarg2, JNIDevice jDevice);
	public final static native int DeviceMessage_GetNumDevices(long jarg1, JNIDeviceMessage jDevMsg);
	public final static native long DeviceMessage_GetDevice(long jarg1, JNIDeviceMessage jDevMsg, int index);
	public final static native long DeviceMessage_GetDeviceIterator(long jarg1, JNIDeviceMessage jDevMsg);
	public final static native long DeviceMessage_ConvertMessage(long jarg1, JNIMessage jMsg);


	// MIST DeviceParam native class functions
	//
	public final static native long new_DeviceParam(String name, String timestamp, long jarg1, JNIField jField);
	public final static native long new_DeviceParam_Copy(long jarg1, JNIDeviceParam jParam);
	public final static native void delete_DeviceParam(long jarg1, JNIDeviceParam jParam);
	public final static native String DeviceParam_GetName(long jarg1, JNIDeviceParam jParam);
	public final static native String DeviceParam_GetTimestamp(long jarg1, JNIDeviceParam jParam);
	public final static native long DeviceParam_GetValue(long jarg1, JNIDeviceParam jParam);


	// MIST MessageValidator native class functions
	//
	public final static native long new_MessageValidator(Object validator);
	public final static native void delete_MessageValidator(long jarg1, JNIMessageValidator jValidator);


	// MIST MistMessage native class functions
	//
	public final static native long new_MistMessage(String subject, String schemaID, long jarg1, JNISpecification jSpec);
	public final static native long new_MistMessageWithConfig(String subject, String schemaID, long jarg1, JNIConfig jConfig, long jarg2, JNISpecification jSpec);
	public final static native long new_MistMessageWithData(String data);
	public final static native long new_MistMessageWithSpec(long jarg1, JNISpecification jSpec, String data);
	public final static native long new_MistMessageCopy(long jarg1, JNIMistMessage jOther);
	public final static native long new_MistMessageFromMessage(long jarg1, JNIMessage jMsg, long jarg2, JNIConfig jConfig);
	public final static native void delete_MistMessage(long jarg1, JNIMistMessage jMistMsg);
	public final static native void MistMessageSetStandardFields(long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void MistMessageClearStandardFields();
	public final static native String MistMessageGetSchemaID(long jarg1, JNIMistMessage jMistMsg);
	public final static native void MistMessageSetValueString(long jarg1, JNIMistMessage jMistMsg, String fieldName, String value);
	public final static native void MistMessageSetValueLong(long jarg1, JNIMistMessage jMistMsg, String fieldName, long value);
	public final static native void MistMessageSetValueDouble(long jarg1, JNIMistMessage jMistMsg, String fieldName, double value);


	// MIST Mnemonic native class functions
	//
	public final static native long new_Mnemonic(String name, long[] samplePtrs, JNIMnemonicSample[] samples, int numSamples);
	public final static native long new_Mnemonic_Copy(long jarg1, JNIMnemonic mnemonic);
	public final static native void delete_Mnemonic(long jarg1, JNIMnemonic mnemonic);
	public final static native String Mnemonic_GetName(long jarg1, JNIMnemonic mnemonic);
	public final static native boolean Mnemonic_StatusAvailable(long jarg1, JNIMnemonic mnemonic);
	public final static native long Mnemonic_GetStatus(long jarg1, JNIMnemonic mnemonic);
	public final static native void Mnemonic_SetStatus(long jarg1, JNIMnemonic mnemonic, long jarg2, JNIField field);
	public final static native boolean Mnemonic_UnitsAvailable(long jarg1, JNIMnemonic mnemonic);
	public final static native String Mnemonic_GetUnits(long jarg1, JNIMnemonic mnemonic);
	public final static native void Mnemonic_SetUnits(long jarg1, JNIMnemonic mnemonic, String units);
	public final static native long Mnemonic_GetSampleCount(long jarg1, JNIMnemonic mnemonic);
	public final static native long Mnemonic_GetSample(long jarg1, JNIMnemonic mnemonic, long index);
	public final static native void Mnemonic_AddSample(long jarg1, JNIMnemonic mnemonic, long jarg2, JNIMnemonicSample sample);


	// MIST MnemonicIterator native class functions
	//
	public final static native boolean MnemonicIterator_HasNext(long jarg1, JNIMnemonicIterator iter);
	public final static native long MnemonicIterator_Next(long jarg1, JNIMnemonicIterator iter);
	public final static native void MnemonicIterator_Reset(long jarg1, JNIMnemonicIterator iter);


	// MIST MnemonicMessage native class functions
	//
	public final static native long new_MnemonicMessage(String subject, String schemaID, long jarg1, JNISpecification jSpec);
	public final static native long new_MnemonicMessage(String subject, String schemaID, long jarg1, JNIConfig jConfig, long jarg2, JNISpecification jSpec);
	public final static native long new_MnemonicMessage(String data);
	public final static native long new_MnemonicMessage(long jarg1, JNISpecification jSpec, String data);
	public final static native long new_MnemonicMessage_Copy(long jarg1, JNIMnemonicMessage jMnemMsg);
	public final static native void MnemonicMessage_AddMnemonic(long jarg1, JNIMnemonicMessage jMnemMsg, long jarg2, JNIMnemonic jMnem);
	public final static native int MnemonicMessage_GetNumMnemonics(long jarg1, JNIMnemonicMessage jMnemMsg);
	public final static native long MnemonicMessage_GetMnemonic(long jarg1, JNIMnemonicMessage jMnemMsg, int index);
	public final static native long MnemonicMessage_GetMnemonicIterator(long jarg1, JNIMnemonicMessage jMnemMsg);
	public final static native long MnemonicMessage_ConvertMessage(long jarg1, JNIMessage jMsg);


	// MIST MnemonicSample native class functions
	//
	public final static native long new_MnemonicSample(String timestamp, long jFieldPtr, JNIField jField);
	public final static native long new_MnemonicSample_Copy(long jarg1, JNIMnemonicSample jSample);
	public final static native void delete_MnemonicSample(long jarg1, JNIMnemonicSample jSample);
	public final static native String MnemonicSample_GetTimestamp(long jarg1, JNIMnemonicSample jSample);
	public final static native long MnemonicSample_GetRawValue(long jarg1, JNIMnemonicSample jSample);
	public final static native boolean MnemonicSample_HasEUValue(long jarg1, JNIMnemonicSample jSample);
	public final static native long MnemonicSample_GetEUValue(long jarg1, JNIMnemonicSample jSample);
	public final static native void MnemonicSample_SetEUValue(long jarg1, JNIMnemonicSample jSample, long jarg2, JNIField jField);
	public final static native boolean MnemonicSample_HasTextValue(long jarg1, JNIMnemonicSample jSample);
	public final static native String MnemonicSample_GetTextValue(long jarg1, JNIMnemonicSample jSample);
	public final static native void MnemonicSample_SetTextValue(long jarg1, JNIMnemonicSample jSample, String value);
	public final static native boolean MnemonicSample_HasFlags(long jarg1, JNIMnemonicSample jSample);
	public final static native int MnemonicSample_GetFlags(long jarg1, JNIMnemonicSample jSample);
	public final static native void MnemonicSample_SetFlags(long jarg1, JNIMnemonicSample jSample, int flags);
	public final static native boolean MnemonicSample_HasLimitEnableDisable(long jarg1, JNIMnemonicSample jSample);
	public final static native boolean MnemonicSample_GetLimitEnableDisable(long jarg1, JNIMnemonicSample jSample);
	public final static native void MnemonicSample_SetLimitEnableDisable(long jarg1, JNIMnemonicSample jSample, boolean flag);
	public final static native boolean MnemonicSample_HasLimit(long jarg1, JNIMnemonicSample jSample);
	public final static native int MnemonicSample_GetLimit(long jarg1, JNIMnemonicSample jSample);
	public final static native void MnemonicSample_SetLimit(long jarg1, JNIMnemonicSample jSample, int limit);
	public final static native boolean MnemonicSample_HasStalenessStatus(long jarg1, JNIMnemonicSample jSample);
	public final static native boolean MnemonicSample_GetStalenessStatus(long jarg1, JNIMnemonicSample jSample);
	public final static native void MnemonicSample_SetStalenessStatus(long jarg1, JNIMnemonicSample jSample, boolean staleness);
	public final static native boolean MnemonicSample_HasQuality(long jarg1, JNIMnemonicSample jSample);
	public final static native boolean MnemonicSample_GetQuality(long jarg1, JNIMnemonicSample jSample);
	public final static native void MnemonicSample_SetQuality(long jarg1, JNIMnemonicSample jSample, boolean quality);


	// MIST ProductFile native class functions
	//
	public final static native long new_ProductFile(String name, String description, String version, String format, String uri);
	public final static native long new_ProductFile(String name, String description, String version, String format, byte[] data, long size);
	public final static native long new_ProductFile_Copy(long jarg1, JNIProductFile jProductFile);
	public final static native void delete_ProductFile(long jarg1, JNIProductFile jProductFile);
	public final static native String ProductFile_GetName(long jarg1, JNIProductFile jProductFile);
	public final static native String ProductFile_GetDescription(long jarg1, JNIProductFile jProductFile);
	public final static native String ProductFile_GetVersion(long jarg1, JNIProductFile jProductFile);
	public final static native String ProductFile_GetFormat(long jarg1, JNIProductFile jProductFile);
	public final static native boolean ProductFile_IsURIAvailable(long jarg1, JNIProductFile jProductFile);
	public final static native String ProductFile_GetURI(long jarg1, JNIProductFile jProductFile);
	public final static native boolean ProductFile_ContentsAvailable(long jarg1, JNIProductFile jProductFile);
	public final static native byte[] ProductFile_GetContents(long jarg1, JNIProductFile jProductFile);


	// MIST ProductFileIterator native class functions
	//
	public final static native boolean ProductFileIterator_HasNext(long jarg1, JNIProductFileIterator iter);
	public final static native long ProductFileIterator_Next(long jarg1, JNIProductFileIterator iter);
	public final static native void ProductFileIterator_Reset(long jarg1, JNIProductFileIterator iter);


	// MIST ProductFileMessage native class functions
	//
	public final static native long new_ProductFileMessage(String subject, int status, int kind, String productType, String productSubtype, long jarg1, JNISpecification jSpec);
	public final static native long new_ProductFileMessage(String subject, int status, int kind, String productType, String productSubtype, long jarg1, JNIConfig jConfig, long jarg2, JNISpecification jSpec);
	public final static native long new_ProductFileMessage(String subject, int status, String schemaID, long jarg1, JNISpecification jSpec);
	public final static native long new_ProductFileMessage(String subject, int status, String schemaID, long jarg1, JNIConfig jConfig, long jarg2, JNISpecification jSpec);
	public final static native long new_ProductFileMessage(String data);
	public final static native long new_ProductFileMessage(long jarg1, JNISpecification jSpec, String data);
	public final static native long new_ProductFileMessage_Copy(long jarg1, JNIProductFileMessage jProductFileMsg);
	public final static native void ProductFileMessage_AddProductFile(long jarg1, JNIProductFileMessage jProductFileMsg, long jarg2, JNIProductFile jProductFile);
	public final static native int ProductFileMessage_GetNumProductFiles(long jarg1, JNIProductFileMessage jProductFileMsg);
	public final static native long ProductFileMessage_GetProductFile(long jarg1, JNIProductFileMessage jProductFileMsg, int index);
	public final static native long ProductFileMessage_GetProductFileIterator(long jarg1, JNIProductFileMessage jProductFileMsg);
	public final static native int ProductFileMessage_GetResponseStatus(long jarg1, JNIProductFileMessage jProductFileMsg);
	public final static native String ProductFileMessage_GetProductType(long jarg1, JNIProductFileMessage jProductFileMsg);
	public final static native String ProductFileMessage_GetProductSubtype(long jarg1, JNIProductFileMessage jProductFileMsg);
	public final static native long ProductFileMessage_ConvertMessage(long jarg1, JNIMessage jMsg);


	// MIST ServiceParam native class functions
	//
	public final static native long new_ServiceParam(String name, long jarg1, JNIField jField);
	public final static native long new_ServiceParam_Copy(long jarg1, JNIServiceParam jParam);
	public final static native void delete_ServiceParam(long jarg1, JNIServiceParam jParam);
	public final static native String ServiceParam_GetName(long jarg1, JNIServiceParam jParam);
	public final static native long ServiceParam_GetValue(long jarg1, JNIServiceParam jParam);


	// MIST SchemaIDIterator
	//
	public final static native boolean SchemaIDIterator_HasNext(long jarg1, JNISchemaIDIterator jIter);
	public final static native String SchemaIDIterator_Next(long jarg1, JNISchemaIDIterator jIter);
	public final static native void SchemaIDIterator_Reset(long jarg1, JNISchemaIDIterator jIter);


	// MIST Specification
	//
	public final static native long new_Specification(long jarg1, JNIConfig jConfig);
	public final static native long new_Specification_Copy(long jarg1, JNISpecification jSpec);
	public final static native void delete_Specification(long jarg1, JNISpecification jSpec);
	public final static native void Specification_ValidateMessage(long jarg1, JNISpecification jSpec, long jarg2, JNIMessage msg);
	public final static native long Specification_GetSchemaIDIterator(long jarg1, JNISpecification jSpec);
	public final static native int Specification_GetVersion(long jarg1, JNISpecification jSpec);
	public final static native int Specification_GetSchemaLevel(long jarg1, JNISpecification jSpec);
	public final static native List<MessageSpecification> Specification_GetMessageSpecifications(long jarg1, JNISpecification jSpec);
	public final static native void Specification_RegisterMessageValidator(long jarg1, JNISpecification jSpec, long valPtr);
	public final static native String Specification_GetTemplateXML(long jarg1, JNISpecification jSpec, String subject, String schemaID);
}
