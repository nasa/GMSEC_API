/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.*;

import gov.nasa.gsfc.gmsec.api5.util.*;

import gov.nasa.gsfc.gmsec.api5.jni.field.JNIField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIBinaryField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIBooleanField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNICharField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII16Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII32Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII64Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII8Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIF32Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIF64Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIStringField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU16Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU32Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU64Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU8Field;

import gov.nasa.gsfc.gmsec.api5.jni.util.JNILogHandler;
import gov.nasa.gsfc.gmsec.api5.jni.util.JNITimeSpec;

import java.util.List;


/** @class gmsecJNI
 *
 * @brief This file declares all native JNI methods that are interfacing with the
 * functions in the GMSEC API library.
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
	public final static native long new_Config_Data(String jarg1, int type);
	public final static native long new_Config_Copy(long jarg1, JNIConfig config);
	public final static native void delete_Config(long jarg1, JNIConfig config);
	public final static native void Config_AddValue(long jarg1, JNIConfig config, String jarg2, String jarg3);
	public final static native boolean Config_ClearValue(long jarg1, JNIConfig config, String jarg2);
	public final static native void Config_Clear(long jarg1, JNIConfig config);
	public final static native String[] Config_KeySet(long cfg, JNIConfig config);
	public final static native String[] Config_Values(long cfg, JNIConfig config);
	public final static native String Config_GetValue(long jarg1, JNIConfig config, String jarg2);
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
	public final static native long Config_GetFromFile(String filename, String configName);
	public final static native String Config_ToJSON(long cfg, JNIConfig config);


	// ConfigEntry native class functions
	//
	public final static native void delete_ConfigEntry(long jarg1, JNIConfigEntry entry);
	public final static native String ConfigEntry_GetName(long jarg1, JNIConfigEntry entry);
	public final static native long ConfigEntry_GetConfig(long jarg1, JNIConfigEntry entry);


	// ConfigFile native class functions
	//
	public final static native long new_ConfigFile();
	public final static native long new_ConfigFileUsingConfig(long jarg1, JNIConfig config);
	public final static native void delete_ConfigFile(long jarg1, JNIConfigFile configFile);
	public final static native void ConfigFile_Load(long jarg1, JNIConfigFile configFile, String filePath);
	public final static native void ConfigFile_Save(long jarg1, JNIConfigFile configFile, String filePath, boolean compact);
	public final static native void ConfigFile_FromXML(long jarg1, JNIConfigFile configFile, String xml);
	public final static native String ConfigFile_ToXML(long ptr, JNIConfigFile cfile);
	public final static native boolean ConfigFile_IsLoaded(long jarg1, JNIConfigFile configFile);
	public final static native void ConfigFile_AddConfig(long jarg1, JNIConfigFile configFile, String name, long jarg2, JNIConfig config);
	public final static native long ConfigFile_LookupConfig(long jarg1, JNIConfigFile configFile, String name);
	public final static native boolean ConfigFile_RemoveConfig(long jarg1, JNIConfigFile configFile, String name);
	public final static native void ConfigFile_AddMessage(long jarg1, JNIConfigFile configFile, String name, long jarg2, JNIMessage msg);
	public final static native long ConfigFile_LookupMessage(long jarg1, JNIConfigFile configFile, String name);
	public final static native boolean ConfigFile_RemoveMessage(long jarg1, JNIConfigFile configFile, String name);
	public final static native String ConfigFile_LookupSubscription(long jarg1, JNIConfigFile configFile, String name);
	public final static native void ConfigFile_AddSubscriptionEntry(long jarg1, JNIConfigFile configFile, long entryPtr, JNISubscriptionEntry entry);
	public final static native long ConfigFile_LookupSubscriptionEntry(long jarg1, JNIConfigFile configFile, String name);
	public final static native boolean ConfigFile_RemoveSubscriptionEntry(long jarg1, JNIConfigFile configFile, String name);
	public final static native long ConfigFile_GetIterator(long jarg1, JNIConfigFile configFile);


	// ConfigFileIterator native class functions
	//
	public final static native boolean ConfigFileIterator_HasNextConfig(long jarg1, JNIConfigFileIterator iter);
	public final static native boolean ConfigFileIterator_HasNextMessage(long jarg1, JNIConfigFileIterator iter);
	public final static native boolean ConfigFileIterator_HasNextSubscription(long jarg1, JNIConfigFileIterator iter);
	public final static native long ConfigFileIterator_NextConfig(long jarg1, JNIConfigFileIterator iter);
	public final static native long ConfigFileIterator_NextMessage(long jarg1, JNIConfigFileIterator iter);
	public final static native long ConfigFileIterator_NextSubscription(long jarg1, JNIConfigFileIterator iter);
	public final static native void ConfigFileIterator_Reset(long jarg1, JNIConfigFileIterator iter);


	// Connection native class functions
	//
    public final static native long Connection_Create(long jarg1, JNIConfig config, JNIConnection conn);
    public final static native long Connection_CreateWithFactory(long jarg1, JNIConfig config, long jarg2, JNIMessageFactory factory, JNIConnection conn);
    public final static native void Connection_Destroy(long jarg1, JNIConnection connection);
	public final static native String Connection_GetAPIVersion();
	public final static native void Connection_Connect(long jarg1, JNIConnection connection);
	public final static native void Connection_Disconnect(long jarg1, JNIConnection connection);
	public final static native String Connection_GetLibraryName(long jarg1, JNIConnection connection);
	public final static native String Connection_GetLibraryVersion(long jarg1, JNIConnection connection);
	public final static native long Connection_GetMessageFactory(long jarg1, JNIConnection connection);
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
	public final static native long HeartbeatGenerator_Create(long jarg1, JNIConfig config, int pubRate, long[] fldPtrs, JNIField[] flds, int numFields);
    public final static native void HeartbeatGenerator_Destroy(long jarg1, JNIHeartbeatGenerator hbGen);
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
	public final static native long new_Message();
	public final static native long new_Message_Copy(long jarg1, JNIMessage msg);
	public final static native void delete_Message(long jarg1, JNIMessage msg);
	public final static native void Message_Acknowledge(long jarg1, JNIMessage msg);
	public final static native void Message_Destroy(long jarg1, JNIMessage jmsg);
	public final static native String Message_GetSchemaID(long jarg1, JNIMessage msg);
	public final static native int Message_GetVersion(long jarg1, JNIMessage msg);
	public final static native int Message_GetSchemaLevel(long jarg1, JNIMessage msg);
	public final static native Object Message_IsCompliant(long jarg1, JNIMessage msg);
	public final static native void Message_RegisterMessageValidator(long jarg1, JNIMessage msg, long jarg2);
	public final static native boolean MessageSetFieldValueString(long jarg1, JNIMessage jMistMsg, String fieldName, String value);
	public final static native boolean MessageSetFieldValueInt(long jarg1, JNIMessage jMistMsg, String fieldName, int value);
	public final static native boolean MessageSetFieldValueLong(long jarg1, JNIMessage jMistMsg, String fieldName, long value);
	public final static native boolean MessageSetFieldValueDouble(long jarg1, JNIMessage jMistMsg, String fieldName, double value);
	public final static native void Message_SetConfig(long jarg1, JNIMessage msg, long cfgPtr, JNIConfig config);
	public final static native long Message_GetConfig(long jarg1, JNIMessage msg);
	public final static native void Message_SetSubject(long jarg1, JNIMessage msg, String subject);
	public final static native String Message_GetSubject(long jarg1, JNIMessage msg);
	public final static native void Message_SetKind(long jarg1, JNIMessage msg, int jKind);
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
	public final static native String Message_GetStringValue(long jarg1, JNIMessage msg, String fieldName);
	public final static native boolean Message_GetBooleanValue(long jarg1, JNIMessage msg, String fieldName);
	public final static native short Message_GetI16Value(long jarg1, JNIMessage msg, String fieldName);
	public final static native int Message_GetI32Value(long jarg1, JNIMessage msg, String fieldName);
	public final static native long Message_GetI64Value(long jarg1, JNIMessage msg, String fieldName);
	public final static native U16 Message_GetU16Value(long jarg1, JNIMessage msg, String fieldName);
	public final static native U32 Message_GetU32Value(long jarg1, JNIMessage msg, String fieldName);
	public final static native U64 Message_GetU64Value(long jarg1, JNIMessage msg, String fieldName);
	public final static native double Message_GetF64Value(long jarg1, JNIMessage msg, String fieldName);
	public final static native boolean Message_HasField(long jarg1, JNIMessage msg, String fieldName);
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


	// MessageFactory class functions
	//
	public final static native long MessageFactory_Create();
	public final static native long MessageFactory_CreateUsingConfig(long jarg1, JNIConfig jConfig);
	public final static native void MessageFactory_Delete(long jarg1, JNIMessageFactory jMsgFactory);
	public final static native void MessageFactory_SetStandardFields(long jarg1, JNIMessageFactory jMsgFactory, long[] fldPtrs, JNIField[] flds, int numFields);
	public final static native void MessageFactory_ClearStandardFields(long jarg1, JNIMessageFactory jMsgFactory);
	public final static native void MessageFactory_SetMessageConfig(long jarg1, JNIMessageFactory jMsgFactory, long jarg2, JNIConfig jMsgConfig);
	public final static native long MessageFactory_CreateMessage(long jarg1, JNIMessageFactory jMsgFactory);
	public final static native long MessageFactory_CreateMessage(long jarg1, JNIMessageFactory jMsgFactory, String schemaID);
	public final static native long MessageFactory_FromData(long jarg1, JNIMessageFactory jMsgFactory, String data, int type);
	public final static native long MessageFactory_GetSpecification(long jarg1, JNIMessageFactory jMsgFactory);
	public final static native void MessageFactory_RegisterMessageValidator(long jarg1, JNIMessageFactory jMsgFactory, long jarg2);


	// MessageEntry native class functions
	//
	public final static native void delete_MessageEntry(long jarg1, JNIMessageEntry entry);
	public final static native String MessageEntry_GetName(long jarg1, JNIMessageEntry entry);
	public final static native long MessageEntry_GetMessage(long jarg1, JNIMessageEntry entry);


	// MessageFieldIterator native class functions
	//
	public final static native boolean MessageFieldIterator_HasNext(long jarg1, JNIMessageFieldIterator iter);
	public final static native long MessageFieldIterator_Next(long jarg1, JNIMessageFieldIterator iter) throws GmsecException;
	public final static native void MessageFieldIterator_Reset(long jarg1, JNIMessageFieldIterator iter);


	// MessageValidator native class functions
	//
	public final static native long new_MessageValidator(Object validator);
	public final static native void delete_MessageValidator(long jarg1, JNIMessageValidator jValidator);


	// ReplyCallback native class functions
	//
	public final static native long new_ReplyCallback(Object replyCallback);
	public final static native void delete_ReplyCallback(long jarg1, JNIReplyCallback jcb);


	// ResourceGenerator native class functions
	//
	public final static native long ResourceGenerator_Create(long jarg1, JNIConfig config, int pubRate, int sampleInterval, int averageInterval, long[] fldPtrs, JNIField[] flds, int numFields);
    public final static native void ResourceGenerator_Destroy(long jarg1, JNIResourceGenerator rsrcGen);
	public final static native boolean ResourceGenerator_Start(long jarg1, JNIResourceGenerator rsrcgen);
	public final static native boolean ResourceGenerator_Stop(long jarg1, JNIResourceGenerator rsrcgen);
	public final static native boolean ResourceGenerator_IsRunning(long jarg1, JNIResourceGenerator rsrcgen);
	public final static native boolean ResourceGenerator_SetField(long jarg1, JNIResourceGenerator rsrcgen, long jarg2, JNIField field);
	public final static native long ResourceGenerator_CreateResourceMessage(long jarg1, JNIMessageFactory factory, int sampleInterval, int averageInterval);


	// Status native class functions
	//
	public final static native long new_Status();
	public final static native long new_Status_Set(int jarg1, int jarg2, String jarg3);
	public final static native long new_Status_Set(int jarg1, int jarg2, String jarg3, int jarg4);
	public final static native long new_Status_Copy(long jarg1, JNIStatus status);
	public final static native void delete_Status(long jarg1, JNIStatus status);
	public final static native int Status_HasError(long jarg1, JNIStatus status);
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
	public final static native long new_SubscriptionEntry(String name, String pattern);
	public final static native void delete_SubscriptionEntry(long jarg1, JNISubscriptionEntry entry);
	public final static native String SubscriptionEntry_GetName(long jarg1, JNISubscriptionEntry entry);
	public final static native String SubscriptionEntry_GetSubject(long jarg1, JNISubscriptionEntry entry);
	public final static native String SubscriptionEntry_GetPattern(long jarg1, JNISubscriptionEntry entry);
	public final static native void SubscriptionEntry_AddExcludedPattern(long jarg1, JNISubscriptionEntry entry, String pattern);
	public final static native boolean SubscriptionEntry_HasNextExcludedPattern(long jarg1, JNISubscriptionEntry entry);
	public final static native String SubscriptionEntry_NextExcludedPattern(long jarg1, JNISubscriptionEntry entry);


	// Base Field native class functions
	public final static native int Field_GetType(long fieldPtr, JNIField field);
	public final static native String Field_GetName(long fieldPtr, JNIField field);
	public final static native String Field_ToXML(long fieldPtr, JNIField field);
	public final static native String Field_ToJSON(long fieldPtr, JNIField field);
	public final static native String Field_GetStringValue(long fieldPtr, JNIField field);
	public final static native boolean Field_GetBooleanValue(long fieldPtr, JNIField field);
	public final static native short Field_GetI16Value(long fieldPtr, JNIField field);
	public final static native int Field_GetI32Value(long fieldPtr, JNIField field);
	public final static native long Field_GetI64Value(long fieldPtr, JNIField field);
	public final static native U16 Field_GetU16Value(long fieldPtr, JNIField field);
	public final static native U32 Field_GetU32Value(long fieldPtr, JNIField field);
	public final static native U64 Field_GetU64Value(long fieldPtr, JNIField field);
	public final static native double Field_GetF64Value(long fieldPtr, JNIField field);
	public final static native boolean Field_IsHeader(long fieldPtr, JNIField field);
	public final static native void delete_Field(long fieldPtr, JNIField field);


	// BinaryField native class functions
	public final static native long new_BinaryField(String name, byte[] data, long dataLength, boolean isHeader);
	public final static native long new_BinaryField_Copy(long fieldPtr, JNIBinaryField field);
	public final static native byte[] BinaryField_GetValue(long fieldPtr, JNIBinaryField field);


	// BooleanField native class functions
	public final static native long new_BooleanField(String name, boolean data, boolean isHeader);
	public final static native long new_BooleanField_Copy(long fieldPtr, JNIBooleanField field);
	public final static native boolean BooleanField_GetValue(long fieldPtr, JNIBooleanField field);


	// CharField native class functions
	public final static native long new_CharField(String name, char data, boolean isHeader);
	public final static native long new_CharField_Copy(long fieldPtr, JNICharField field);
	public final static native char CharField_GetValue(long fieldPtr, JNICharField field);


	// F32Field native class functions
	public final static native long new_F32Field(String name, float data, boolean isHeader);
	public final static native long new_F32Field_Copy(long fieldPtr, JNIF32Field field);
	public final static native float F32Field_GetValue(long fieldPtr, JNIF32Field field);


	// F64Field native class functions
	public final static native long new_F64Field(String name, double data, boolean isHeader);
	public final static native long new_F64Field_Copy(long fieldPtr, JNIF64Field field);
	public final static native double F64Field_GetValue(long fieldPtr, JNIF64Field field);


	// I8Field native class functions
	public final static native long new_I8Field(String name, byte data, boolean isHeader);
	public final static native long new_I8Field_Copy(long fieldPtr, JNII8Field field);
	public final static native byte I8Field_GetValue(long fieldPtr, JNII8Field field);


	// I16Field native class functions
	public final static native long new_I16Field(String name, short data, boolean isHeader);
	public final static native long new_I16Field_Copy(long fieldPtr, JNII16Field field);
	public final static native short I16Field_GetValue(long fieldPtr, JNII16Field field);


	// I32Field native class functions
	public final static native long new_I32Field(String name, int data, boolean isHeader);
	public final static native long new_I32Field_Copy(long fieldPtr, JNII32Field field);
	public final static native int I32Field_GetValue(long fieldPtr, JNII32Field field);


	// I64Field native class functions
	public final static native long new_I64Field(String name, long data, boolean isHeader);
	public final static native long new_I64Field_Copy(long fieldPtr, JNII64Field field);
	public final static native long I64Field_GetValue(long fieldPtr, JNII64Field field);


	// U8Field native class functions
	public final static native long new_U8Field(String name, int data, boolean isHeader);
	public final static native long new_U8Field_Copy(long fieldPtr, JNIU8Field field);
	public final static native int U8Field_GetValue(long fieldPtr, JNIU8Field field);


	// U16Field native class functions
	public final static native long new_U16Field(String name, int data, boolean isHeader);
	public final static native long new_U16Field_Copy(long fieldPtr, JNIU16Field field);
	public final static native int U16Field_GetValue(long fieldPtr, JNIU16Field field);


	// U32Field native class functions
	public final static native long new_U32Field(String name, long data, boolean isHeader);
	public final static native long new_U32Field_Copy(long fieldPtr, JNIU32Field field);
	public final static native long U32Field_GetValue(long fieldPtr, JNIU32Field field);


	// U64Field native class functions
	public final static native long new_U64Field(String name, String data, boolean isHeader);
	public final static native long new_U64Field_Copy(long fieldPtr, JNIU64Field field);
	public final static native String U64Field_GetValue(long fieldPtr, JNIU64Field field);


	// StringField native class functions
	public final static native long new_StringField(String name, String data, boolean isHeader);
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
	public final static native void TimeUtil_Millisleep(int ms);
	public final static native long TimeUtil_GetCurrentTime();
	public final static native double TimeUtil_GetCurrentTime_s();
	public final static native long TimeUtil_ConvertTimeString(String timeString);
	public final static native String TimeUtil_FormatTime(long seconds, long nanoseconds, int subseconds);


	// SchemaIDIterator native class functions
	//
	public final static native boolean SchemaIDIterator_HasNext(long jarg1, JNISchemaIDIterator jIter);
	public final static native String SchemaIDIterator_Next(long jarg1, JNISchemaIDIterator jIter);
	public final static native void SchemaIDIterator_Reset(long jarg1, JNISchemaIDIterator jIter);


	// Specification native class functions
	//
	public final static native long new_Specification(long jarg1, JNIConfig jConfig);
	public final static native long new_Specification_Copy(long jarg1, JNISpecification jSpec);
	public final static native void delete_Specification(long jarg1, JNISpecification jSpec);
	public final static native long Specification_GetSchemaIDIterator(long jarg1, JNISpecification jSpec);
	public final static native int Specification_GetVersion(long jarg1, JNISpecification jSpec);
	public final static native int Specification_GetSchemaLevel(long jarg1, JNISpecification jSpec);
	public final static native List<MessageSpecification> Specification_GetMessageSpecifications(long jarg1, JNISpecification jSpec);
	public final static native List<String> Specification_GetHeaderFieldNames(long jarg1, JNISpecification jSpec, String schemaName);
}
