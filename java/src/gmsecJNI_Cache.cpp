/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/util/Log.h>

#include <iostream>

using namespace gmsec::api5;
using namespace gmsec::api5::util;
using namespace gmsec::api5::jni;


Cache& Cache::getCache()
{
	static Cache theCache;
	return theCache;
}


void Cache::initialize(JNIEnv* jenv)
{
	AutoMutex hold(mutex);

	if (initialized)
	{
		GMSEC_ERROR << "Already initialized";
		return;
	}

	jclass clazz;

	//Note: Method signatures acquired using javap -s <classfile>

	// gov.nasa.gsfc.gmsec.api5.Callback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/Callback");
	methodCallbackOnMessage = getMethod(jenv, clazz, "onMessage", "(Lgov/nasa/gsfc/gmsec/api5/Connection;Lgov/nasa/gsfc/gmsec/api5/Message;)V");
    fieldCallbackJNIConnection = getFieldID(jenv, clazz, "m_jniConnection", "Lgov/nasa/gsfc/gmsec/api5/jni/JNIConnection;");

	// gov.nasa.gsfc.gmsec.api5.EventCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/EventCallback");
	methodEventCallbackOnEvent = getMethod(jenv, clazz, "onEvent", "(Lgov/nasa/gsfc/gmsec/api5/Connection;Lgov/nasa/gsfc/gmsec/api5/Status;Lgov/nasa/gsfc/gmsec/api5/Connection$Event;)V");
    fieldEventCallbackJNIConnection = getFieldID(jenv, clazz, "m_jniConnection", "Lgov/nasa/gsfc/gmsec/api5/jni/JNIConnection;");

	// gov.nasa.gsfc.gmsec.api5.ReplyCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/ReplyCallback");
	methodReplyCallbackOnReply = getMethod(jenv, clazz, "onReply", "(Lgov/nasa/gsfc/gmsec/api5/Connection;Lgov/nasa/gsfc/gmsec/api5/Message;Lgov/nasa/gsfc/gmsec/api5/Message;)V");
    fieldReplyCallbackJNIConnection = getFieldID(jenv, clazz, "m_jniConnection", "Lgov/nasa/gsfc/gmsec/api5/jni/JNIConnection;");

	// gov.nasa.gsfc.gmsec.api5.MessageValidator initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/MessageValidator");
	methodMessageValidatorValidateMessage = getMethod(jenv, clazz, "validateMessage", "(Lgov/nasa/gsfc/gmsec/api5/Message;)Lgov/nasa/gsfc/gmsec/api5/Status;");

	// gov.nasa.gsfc.gmsec.api5.jni.JNIConnection initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/jni/JNIConnection");
	classJNIConnection = clazz;
	fieldConnection_extconn = getFieldID(jenv, clazz, "extconn", "Lgov/nasa/gsfc/gmsec/api5/Connection;");
	fieldConnection_swigCPtr = getFieldID(jenv, clazz, "swigCPtr", "J");
	fieldConnection_swigCMemOwn = getFieldID(jenv, clazz, "swigCMemOwn", "Z");

	// gov.nasa.gsfc.gmsec.api5.jni.JNIStatus initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/jni/JNIStatus");
	classJNIStatus = clazz;
	methodStatusInitIJString  = getMethod(jenv, clazz, "<init>", "(IILjava/lang/String;I)V");

	// gov.nasa.gsfc.gmsec.api5.Status initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/Status");
	classStatus = clazz;
    methodStatusGetClass      = getMethod(jenv, clazz, "getClassification", "()I");
    methodStatusGetCode       = getMethod(jenv, clazz, "getCode", "()I");
    methodStatusGetReason     = getMethod(jenv, clazz, "getReason", "()Ljava/lang/String;");
    methodStatusGetCustomCode = getMethod(jenv, clazz, "getCustomCode", "()I");

	// gov.nasa.gsfc.gmsec.api5.jni.JNIMessage initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/jni/JNIMessage");
	classJNIMessage = clazz;
	methodMessageInitJZ = getMethod(jenv, clazz, "<init>", "(JZ)V");
	fieldMessage_swigCPtr = getFieldID(jenv, clazz, "swigCPtr", "J");
	fieldMessage_swigCMemOwn = getFieldID(jenv, clazz, "swigCMemOwn", "Z");

	// gov.nasa.gsfc.gmsec.api5.LogHandler initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/util/LogHandler");
	methodLogHandlerOnMessage = getMethod(jenv, clazz, "onMessage", "(Lgov/nasa/gsfc/gmsec/api5/util/LogEntry;)V");

	// gov.nasa.gsfc.gmsec.api5.LogEntry initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/util/LogEntry");
	classLogEntry = clazz;
	methodLogEntryInitJZ = getMethod(jenv, clazz, "<init>", "()V");
	fieldLogLevel_value = getFieldID(jenv, clazz, "level", "Lgov/nasa/gsfc/gmsec/api5/util/LogLevel;");
	fieldLogTime_value = getFieldID(jenv, clazz, "time", "Lgov/nasa/gsfc/gmsec/api5/util/TimeSpec;");
	fieldLogFile_value = getFieldID(jenv, clazz, "fileName", "Ljava/lang/String;");
	fieldLogLine_value = getFieldID(jenv, clazz, "lineNumber", "I");
	fieldLogMessage_value = getFieldID(jenv, clazz, "message", "Ljava/lang/String;");

	// gov.nasa.gsfc.gmsec.api5.LogLevel initialization
	clazz                = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/util/LogLevel");
	classLogLevel        = clazz;
    fieldLogLevelNONE    = jenv->GetStaticFieldID(clazz, "NONE", "Lgov/nasa/gsfc/gmsec/api5/util/LogLevel;");
    fieldLogLevelERROR   = jenv->GetStaticFieldID(clazz, "ERROR", "Lgov/nasa/gsfc/gmsec/api5/util/LogLevel;");
    fieldLogLevelSECURE  = jenv->GetStaticFieldID(clazz, "SECURE", "Lgov/nasa/gsfc/gmsec/api5/util/LogLevel;");
    fieldLogLevelWARNING = jenv->GetStaticFieldID(clazz, "WARNING", "Lgov/nasa/gsfc/gmsec/api5/util/LogLevel;");
    fieldLogLevelINFO    = jenv->GetStaticFieldID(clazz, "INFO", "Lgov/nasa/gsfc/gmsec/api5/util/LogLevel;");
    fieldLogLevelVERBOSE = jenv->GetStaticFieldID(clazz, "VERBOSE", "Lgov/nasa/gsfc/gmsec/api5/util/LogLevel;");
    fieldLogLevelDEBUG   = jenv->GetStaticFieldID(clazz, "DEBUG", "Lgov/nasa/gsfc/gmsec/api5/util/LogLevel;");

	// gov.nasa.gsfc.gmsec.api5.Message initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/Message");
	classMessage = clazz;
	methodMessageInit = getMethod(jenv, clazz, "<init>", "(Lgov/nasa/gsfc/gmsec/api5/jni/JNIMessage;)V");

	// gov.nasa.gsfc.gmsec.api5.TimeSpec initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/util/TimeSpec");
	classTimeSpec = clazz;
	methodTimeSpecInitJZ = getMethod(jenv, clazz, "<init>", "()V");
	fieldSeconds_value = getFieldID(jenv, clazz, "seconds", "J");
	fieldNanoseconds_value = getFieldID(jenv, clazz, "nanoseconds", "J");

	// gov.nasa.gsfc.gmsec.api5.U8 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/U8");
    classU8 = clazz;
    methodU8Init = getMethod(jenv, clazz, "<init>", "(I)V");

	// gov.nasa.gsfc.gmsec.api5.U16 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/U16");
    classU16 = clazz;
    methodU16Init = getMethod(jenv, clazz, "<init>", "(I)V");

	// gov.nasa.gsfc.gmsec.api5.U32 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/U32");
    classU32 = clazz;
    methodU32Init = getMethod(jenv, clazz, "<init>", "(J)V");

	// gov.nasa.gsfc.gmsec.api5.U64 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/U64");
    classU64 = clazz;
    methodU64Init = getMethod(jenv, clazz, "<init>", "(Ljava/lang/String;)V");

	// gov.nasa.gsfc.gmsec.api5.Connection$Event
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api5/Connection$Event");

    jfieldID jField = 0;
	jobject  jEvent = 0;

    jField = jenv->GetStaticFieldID(clazz, "DISPATCHER_ERROR_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::DISPATCHER_ERROR_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "REQUEST_TIMEOUT_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::REQUEST_TIMEOUT_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "CONNECTION_SUCCESSFUL_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::CONNECTION_SUCCESSFUL_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "CONNECTION_BROKEN_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::CONNECTION_BROKEN_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "CONNECTION_RECONNECT_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::CONNECTION_RECONNECT_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "CONNECTION_EXCEPTION_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::CONNECTION_EXCEPTION_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "WSMQ_ASYNC_STATUS_CHECK_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::WSMQ_ASYNC_STATUS_CHECK_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "MSG_PUBLISH_FAILURE_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::MSG_PUBLISH_FAILURE_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "INVALID_MESSAGE_EVENT", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::INVALID_MESSAGE_EVENT] = addGlobalReference(jenv, jEvent);

    jField = jenv->GetStaticFieldID(clazz, "ALL_EVENTS", "Lgov/nasa/gsfc/gmsec/api5/Connection$Event;");
	jEvent = jenv->GetStaticObjectField(clazz, jField);
    connectionEvents[Connection::Event::ALL_EVENTS] = addGlobalReference(jenv, jEvent);

	initialized = 1;
}


jobject Cache::addGlobalReference(JNIEnv* jenv, jobject o)
{
	AutoMutex hold(mutex);
	jobject global = jenv->NewGlobalRef(o);
	globalReferences.push_back(global);
	return global;
}


void Cache::putConnection(Connection* conn, jobject jConnection)
{
	AutoMutex hold(mutex);

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "Cache::putConnection() -- unable to attach to the current thread";
		return;
	}

	jobject o = jenv->NewGlobalRef(jConnection);

	GMSEC_VERBOSE << "JNI.putConnection cxx=" << conn << " java=" << o;

	connections[conn] = o;
}


void Cache::removeConnection(Connection* conn)
{
	AutoMutex hold(mutex);

	std::map<Connection*, jobject>::iterator it = connections.find(conn);

	if (it != connections.end())
	{
		AutoJEnv aje;
		JNIEnv* jenv = aje.getEnv();

		if (!jenv)
		{
			GMSEC_ERROR << "Cache::removeConnection() -- unable to attach to the current thread";
			return;
		}

		jobject o = it->second;

		GMSEC_VERBOSE << "JNI.removeConnection cxx=" << conn << " java=" << o;

		jenv->DeleteGlobalRef(o);

		connections.erase(it);
	}
}


jobject Cache::getConnection(Connection* connection)
{
	AutoMutex hold(mutex);
	jobject o = connections[connection];
	if (!o)
	{
		GMSEC_WARNING << "JNI.getConnection: unknown connection " << connection;
	}
	return o;
}
