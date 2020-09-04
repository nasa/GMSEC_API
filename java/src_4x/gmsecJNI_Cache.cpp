/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/util/Log.h>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::jni;


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

	// gov.nasa.gsfc.gmsec.api.Callback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/Callback");
	methodCallbackOnMessage = getMethod(jenv, clazz, "onMessage", "(Lgov/nasa/gsfc/gmsec/api/Connection;Lgov/nasa/gsfc/gmsec/api/Message;)V");
    fieldCallbackJNIConnection = getFieldID(jenv, clazz, "m_jniConnection", "Lgov/nasa/gsfc/gmsec/api/jni/JNIConnection;");

	// gov.nasa.gsfc.gmsec.api.EventCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/EventCallback");
	methodEventCallbackOnEvent = getMethod(jenv, clazz, "onEvent", "(Lgov/nasa/gsfc/gmsec/api/Connection;Lgov/nasa/gsfc/gmsec/api/Status;Lgov/nasa/gsfc/gmsec/api/Connection$ConnectionEvent;)V");
    fieldEventCallbackJNIConnection = getFieldID(jenv, clazz, "m_jniConnection", "Lgov/nasa/gsfc/gmsec/api/jni/JNIConnection;");

	// gov.nasa.gsfc.gmsec.api.ReplyCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/ReplyCallback");
	methodReplyCallbackOnReply = getMethod(jenv, clazz, "onReply", "(Lgov/nasa/gsfc/gmsec/api/Connection;Lgov/nasa/gsfc/gmsec/api/Message;Lgov/nasa/gsfc/gmsec/api/Message;)V");
    fieldReplyCallbackJNIConnection = getFieldID(jenv, clazz, "m_jniConnection", "Lgov/nasa/gsfc/gmsec/api/jni/JNIConnection;");

	// gov.nasa.gsfc.gmsec.api.jni.JNIConnection initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/jni/JNIConnection");
	classJNIConnection = clazz;
	fieldConnection_extconn = getFieldID(jenv, clazz, "extconn", "Lgov/nasa/gsfc/gmsec/api/Connection;");
	fieldConnection_swigCPtr = getFieldID(jenv, clazz, "swigCPtr", "J");
	fieldConnection_swigCMemOwn = getFieldID(jenv, clazz, "swigCMemOwn", "Z");

	// gov.nasa.gsfc.gmsec.api.jni.JNIStatus initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/jni/JNIStatus");
	classJNIStatus = clazz;
	methodStatusInitIJString = getMethod(jenv, clazz, "<init>", "(IILjava/lang/String;I)V");

	// gov.nasa.gsfc.gmsec.api.jni.JNIMessage initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/jni/JNIMessage");
	classJNIMessage = clazz;
	methodMessageInitJZ = getMethod(jenv, clazz, "<init>", "(JZ)V");
	fieldMessage_swigCPtr = getFieldID(jenv, clazz, "swigCPtr", "J");
	fieldMessage_swigCMemOwn = getFieldID(jenv, clazz, "swigCMemOwn", "Z");

	// gov.nasa.gsfc.gmsec.api.LogHandler initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/util/LogHandler");
	methodLogHandlerOnMessage = getMethod(jenv, clazz, "onMessage", "(Lgov/nasa/gsfc/gmsec/api/util/LogEntry;)V");

	// gov.nasa.gsfc.gmsec.api.LogEntry initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/util/LogEntry");
	classLogEntry = clazz;
	methodLogEntryInitJZ = getMethod(jenv, clazz, "<init>", "()V");
	fieldLogLevel_value = getFieldID(jenv, clazz, "level", "Lgov/nasa/gsfc/gmsec/api/util/LogLevel;");
	fieldLogTime_value = getFieldID(jenv, clazz, "time", "Lgov/nasa/gsfc/gmsec/api/util/TimeSpec;");
	fieldLogFile_value = getFieldID(jenv, clazz, "fileName", "Ljava/lang/String;");
	fieldLogLine_value = getFieldID(jenv, clazz, "lineNumber", "I");
	fieldLogMessage_value = getFieldID(jenv, clazz, "message", "Ljava/lang/String;");

	// gov.nasa.gsfc.gmsec.api.LogLevel initialization
	clazz                = getClass(jenv, "gov/nasa/gsfc/gmsec/api/util/LogLevel");
	classLogLevel        = clazz;
    fieldLogLevelNONE    = jenv->GetStaticFieldID(clazz, "NONE", "Lgov/nasa/gsfc/gmsec/api/util/LogLevel;");
    fieldLogLevelERROR   = jenv->GetStaticFieldID(clazz, "ERROR", "Lgov/nasa/gsfc/gmsec/api/util/LogLevel;");
    fieldLogLevelSECURE  = jenv->GetStaticFieldID(clazz, "SECURE", "Lgov/nasa/gsfc/gmsec/api/util/LogLevel;");
    fieldLogLevelWARNING = jenv->GetStaticFieldID(clazz, "WARNING", "Lgov/nasa/gsfc/gmsec/api/util/LogLevel;");
    fieldLogLevelINFO    = jenv->GetStaticFieldID(clazz, "INFO", "Lgov/nasa/gsfc/gmsec/api/util/LogLevel;");
    fieldLogLevelVERBOSE = jenv->GetStaticFieldID(clazz, "VERBOSE", "Lgov/nasa/gsfc/gmsec/api/util/LogLevel;");
    fieldLogLevelDEBUG   = jenv->GetStaticFieldID(clazz, "DEBUG", "Lgov/nasa/gsfc/gmsec/api/util/LogLevel;");

	// gov.nasa.gsfc.gmsec.api.Message initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/Message");
	classMessage = clazz;
	methodMessageInit = getMethod(jenv, clazz, "<init>", "(Lgov/nasa/gsfc/gmsec/api/jni/JNIMessage;)V");

	// gov.nasa.gsfc.gmsec.api.TimeSpec initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/util/TimeSpec");
	classTimeSpec = clazz;
	methodTimeSpecInitJZ = getMethod(jenv, clazz, "<init>", "()V");
	fieldSeconds_value = getFieldID(jenv, clazz, "seconds", "J");
	fieldNanoseconds_value = getFieldID(jenv, clazz, "nanoseconds", "J");

	// gov.nasa.gsfc.gmsec.api.jni.JNIConnectionManager initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/jni/mist/JNIConnectionManager");
	classJNIConnectionManager = clazz;
	fieldConnectionManager_extConnMgr = getFieldID(jenv, clazz, "extConnMgr", "Lgov/nasa/gsfc/gmsec/api/mist/ConnectionManager;");
	fieldConnectionManager_swigCPtr = getFieldID(jenv, clazz, "swigCPtr", "J");
	fieldConnectionManager_swigCMemOwn = getFieldID(jenv, clazz, "swigCMemOwn", "Z");

	// gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/mist/ConnectionManagerCallback");
	methodConnMgrCallbackOnMessage = getMethod(jenv, clazz, "onMessage", "(Lgov/nasa/gsfc/gmsec/api/mist/ConnectionManager;Lgov/nasa/gsfc/gmsec/api/Message;)V");
    fieldCallbackJNIConnMgr = getFieldID(jenv, clazz, "m_jniConnMgr", "Lgov/nasa/gsfc/gmsec/api/jni/mist/JNIConnectionManager;");

	// gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerEventCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/mist/ConnectionManagerEventCallback");
	methodConnMgrEventCallbackOnEvent = getMethod(jenv, clazz, "onEvent", "(Lgov/nasa/gsfc/gmsec/api/mist/ConnectionManager;Lgov/nasa/gsfc/gmsec/api/Status;Lgov/nasa/gsfc/gmsec/api/Connection$ConnectionEvent;)V");
    fieldEventCallbackJNIConnMgr = getFieldID(jenv, clazz, "m_jniConnMgr", "Lgov/nasa/gsfc/gmsec/api/jni/mist/JNIConnectionManager;");

	// gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerReplyCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/mist/ConnectionManagerReplyCallback");
	methodConnMgrReplyCallbackOnReply = getMethod(jenv, clazz, "onReply", "(Lgov/nasa/gsfc/gmsec/api/mist/ConnectionManager;Lgov/nasa/gsfc/gmsec/api/Message;Lgov/nasa/gsfc/gmsec/api/Message;)V");
    fieldReplyCallbackJNIConnMgr = getFieldID(jenv, clazz, "m_jniConnMgr", "Lgov/nasa/gsfc/gmsec/api/jni/mist/JNIConnectionManager;");

	// gov.nasa.gsfc.gmsec.api.U64 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsec/api/U64");
    classU64 = clazz;
    methodU64Init = getMethod(jenv, clazz, "<init>", "(Ljava/lang/String;)V");

	initialized = 1;
}


void Cache::addGlobalReference(jobject o)
{
	AutoMutex hold(mutex);
	globalReferences.push_back(o);
}


void Cache::putConnection(Connection* conn, jobject jConnection)
{
	AutoMutex hold(mutex);

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

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
