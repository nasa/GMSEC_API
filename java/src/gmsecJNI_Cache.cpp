/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"
#include "gmsecJNI_Callbacks.h"


Cache& Cache::getCache()
{
	static Cache theCache;
	return theCache;
}


void Cache::initialize(JNIEnv* jenv)
{
	gmsec::util::AutoMutex hold(mutex);

	if (initialized)
	{
		LOG_ERROR << "Already initialized";
		return;
	}

	// gov.nasa.gsfc.gmsecapi.jni.JNIConnection initialization
	jclass clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/jni/JNIConnection");
	classJNIConnection = clazz;
	fieldConnection_extconn = getFieldID(jenv, clazz, "extconn", "Lgov/nasa/gsfc/gmsecapi/Connection;");
	fieldConnection_swigCPtr = getFieldID(jenv, clazz, "swigCPtr", "J");
	fieldConnection_swigCMemOwn = getFieldID(jenv, clazz, "swigCMemOwn", "Z");

	// gov.nasa.gsfc.gmsecapi.jni.JNIStatus initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/jni/JNIStatus");
	classJNIStatus = clazz;
	methodStatusInitIJString = getMethod(jenv, clazz, "<init>", "(IJLjava/lang/String;)V");
	methodStatusSetCustomCode = getMethod(jenv, clazz, "SetCustomCode", "(I)V");

	// gov.nasa.gsfc.gmsecapi.jni.JNIMessage initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/jni/JNIMessage");
	classJNIMessage = clazz;
	methodMessageInitJZ = getMethod(jenv, clazz, "<init>", "(JZ)V");
	fieldMessage_swigCPtr = getFieldID(jenv, clazz, "swigCPtr", "J");
	fieldMessage_swigCMemOwn = getFieldID(jenv, clazz, "swigCMemOwn", "Z");

	// gov.nasa.gsfc.gmsecapi.Message initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/Message");
	classMessage = clazz;
	methodMessageInit = getMethod(jenv, clazz, "<init>", "()V");
	methodMessageSetImpl = getMethod(jenv, clazz, "SetImpl", "(Lgov/nasa/gsfc/gmsecapi/internal/MessageImpl;)V");

	// gov.nasa.gsfc.gmsecapi.LogHandler initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/util/LogHandler");
	methodLogHandlerOnMessage = getMethod(jenv, clazz, "OnMessage", "(Lgov/nasa/gsfc/gmsecapi/util/LogEntry;)V");
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/util/LogEntry");
	classLogEntry = clazz;
	methodLogEntryInitJZ = getMethod(jenv, clazz, "<init>", "()V");
	fieldLogLevel_value = getFieldID(jenv, clazz, "level", "I");
	fieldLogTime_value = getFieldID(jenv, clazz, "time", "D");
	fieldLogMessage_value = getFieldID(jenv, clazz, "message", "Ljava/lang/String;");

	// gov.nasa.gsfc.gmsecapi.Callback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/Callback");
	methodCallbackOnMessage = getMethod(jenv, clazz, "OnMessage", "(Lgov/nasa/gsfc/gmsecapi/Connection;Lgov/nasa/gsfc/gmsecapi/Message;)V");

	// gov.nasa.gsfc.gmsecapi.ErrorCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/ErrorCallback");
	methodErrorCallbackOnError = getMethod(jenv, clazz, "OnError", "(Lgov/nasa/gsfc/gmsecapi/Connection;Lgov/nasa/gsfc/gmsecapi/Message;Lgov/nasa/gsfc/gmsecapi/Status;)V");

	// gov.nasa.gsfc.gmsecapi.ReplyCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/ReplyCallback");
	methodReplyCallbackOnReply = getMethod(jenv, clazz, "OnReply", "(Lgov/nasa/gsfc/gmsecapi/Connection;Lgov/nasa/gsfc/gmsecapi/Message;Lgov/nasa/gsfc/gmsecapi/Message;)V");

	// gov.nasa.gsfc.gmsecapi.GMSEC_String initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_String");
	classGMSEC_String = clazz;
	fieldString_value = getFieldID(jenv, clazz, "value", "Ljava/lang/String;");

	// gov.nasa.gsfc.gmsecapi.GMSEC_Bin initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_Bin");
	classGMSEC_Bin = clazz;
	fieldBin_value = getFieldID(jenv, clazz, "value", "[B");

	// gov.nasa.gsfc.gmsecapi.GMSEC_I64 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_I64");
	classGMSEC_I64 = clazz;
	fieldI64_value = getFieldID(jenv, clazz, "value", "J");

	// gov.nasa.gsfc.gmsecapi.GMSEC_I32 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_I32");
	classGMSEC_I32 = clazz;
	fieldI32_value = getFieldID(jenv, clazz, "value", "I");

	// gov.nasa.gsfc.gmsecapi.GMSEC_U32 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_U32");
	classGMSEC_U32 = clazz;
	fieldU32_value = getFieldID(jenv, clazz, "value", "J");

	// gov.nasa.gsfc.gmsecapi.GMSEC_I16 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_I16");
	classGMSEC_I16 = clazz;
	fieldI16_value = getFieldID(jenv, clazz, "value", "S");

	// gov.nasa.gsfc.gmsecapi.GMSEC_U16 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_U16");
	classGMSEC_U16 = clazz;
	fieldU16_value = getFieldID(jenv, clazz, "value", "I");

	// gov.nasa.gsfc.gmsecapi.GMSEC_F32 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_F32");
	classGMSEC_F32 = clazz;
	fieldF32_value = getFieldID(jenv, clazz, "value", "F");

	// gov.nasa.gsfc.gmsecapi.GMSEC_F64 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_F64");
	classGMSEC_F64 = clazz;
	fieldF64_value = getFieldID(jenv, clazz, "value", "D");

	// gov.nasa.gsfc.gmsecapi.GMSEC_I8 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_I8");
	classGMSEC_I8 = clazz;
	fieldI8_value = getFieldID(jenv, clazz, "value", "B");

	// gov.nasa.gsfc.gmsecapi.GMSEC_U8 initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_U8");
	classGMSEC_U8 = clazz;
	fieldU8_value = getFieldID(jenv, clazz, "value", "I");

	// gov.nasa.gsfc.gmsecapi.GMSEC_Char initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_Char");
	classGMSEC_Char = clazz;
	fieldChar_value = getFieldID(jenv, clazz, "value", "B");

	// gov.nasa.gsfc.gmsecapi.GMSEC_Bool initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/GMSEC_Bool");
	classGMSEC_Bool = clazz;
	fieldBool_value = getFieldID(jenv, clazz, "value", "Z");

	// gov.nasa.gsfc.gmsecapi.jni.JNIConnectionManager initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/jni/JNIConnectionManager");
	classJNIConnectionManager = clazz;
	fieldConnectionManager_extConnMgr = getFieldID(jenv, clazz, "extConnMgr", "Lgov/nasa/gsfc/gmsecapi/mist/ConnectionManager;");
	fieldConnectionManager_swigCPtr = getFieldID(jenv, clazz, "swigCPtr", "J");
	fieldConnectionManager_swigCMemOwn = getFieldID(jenv, clazz, "swigCMemOwn", "Z");

	// gov.nasa.gsfc.gmsecapi.mist.ConnectionManagerCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/mist/ConnectionManagerCallback");
	methodConnMgrCallbackOnMessage = getMethod(jenv, clazz, "OnMessage", "(Lgov/nasa/gsfc/gmsecapi/mist/ConnectionManager;Lgov/nasa/gsfc/gmsecapi/Message;)V");

	// gov.nasa.gsfc.gmsecapi.mist.ConnectionManagerErrorCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/mist/ConnectionManagerErrorCallback");
	methodConnMgrCallbackOnError = getMethod(jenv, clazz, "OnError", "(Lgov/nasa/gsfc/gmsecapi/mist/ConnectionManager;Lgov/nasa/gsfc/gmsecapi/Message;Lgov/nasa/gsfc/gmsecapi/Status;)V");

	// gov.nasa.gsfc.gmsecapi.mist.ConnectionManagerReplyCallback initialization
	clazz = getClass(jenv, "gov/nasa/gsfc/gmsecapi/mist/ConnectionManagerReplyCallback");
	methodConnMgrCallbackOnReply = getMethod(jenv, clazz, "OnReply", "(Lgov/nasa/gsfc/gmsecapi/mist/ConnectionManager;Lgov/nasa/gsfc/gmsecapi/Message;Lgov/nasa/gsfc/gmsecapi/Message;)V");

	initialized = 1;
}


void Cache::addGlobalReference(jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	globalReferences.push_back(o);
}


void Cache::putConnection(gmsec::Connection* conn, jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	connections[conn] = o;
	LOG_VERBOSE << "JNI.putConnection cxx=" << conn << " java=" << o;
}


jobject Cache::getConnection(gmsec::Connection* connection)
{
	gmsec::util::AutoMutex hold(mutex);
	jobject o = connections[connection];
	if (!o)
	{
		LOG_WARNING << "JNI.getConnection: unknown connection " << connection;
	}
	return o;
}


void Cache::putCallback(gmsec::Callback* callback, jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	callbacks[callback] = o;
}


jobject Cache::getCallback(gmsec::Callback* callback)
{
	gmsec::util::AutoMutex hold(mutex);
	std::map<gmsec::Callback*, jobject>::iterator it = callbacks.find(callback);
	jobject o = NULL;
	if (it == callbacks.end())
	{
		LOG_WARNING << "JNI.getCallback: unknown callback " << callback;
	}
	else
	{
		o = it->second;
	}
	return o;
}


gmsec::Callback* Cache::getAndRemoveCallback(JNIEnv* jenv, jstring subject, jobject userCallback)
{
	gmsec::util::AutoMutex hold(mutex);

	gmsec::Callback* cb = 0;
	std::map<gmsec::Callback*, jobject>::iterator it = callbacks.begin();

	for (; it != callbacks.end(); ++it)
	{
		JNICallback* jcb = dynamic_cast<JNICallback*>(it->first);

		if (jcb)
		{
			jstring jcbSubject = (jstring) jcb->getSubject();

			JStringManager str1(jenv, subject);
			JStringManager str2(jenv, jcbSubject);

			if (str1.c_str() && str2.c_str() && std::string(str1.c_str()) == std::string(str2.c_str()) &&
			    jenv->IsSameObject(jcb->getCallback(), userCallback) == JNI_TRUE)
			{
				cb = it->first;
				break;
			}
		}
	}

	if (cb)
	{
		LOG_DEBUG << "Found callback " << it->first << "; removing it.";
		callbacks.erase(it);
		LOG_DEBUG << "We now have " << callbacks.size() << " entries in callbacks.";
	}
	else
	{
		LOG_WARNING << "JNI.getCallback: unknown callback " << userCallback;
	}

	return cb;
}


void Cache::destroyCallbacks(JNIEnv* jenv, jobject jConnection)
{
	gmsec::util::AutoMutex hold(mutex);

	repeat:
	for (std::map<gmsec::Callback*, jobject>::iterator it = callbacks.begin(); it != callbacks.end(); ++it)
	{
		JNICallback* jcb = dynamic_cast<JNICallback*>(it->first);

		if (jcb)
		{
			if (jenv->IsSameObject(jcb->getConnection(), jConnection) == JNI_TRUE)
			{
				LOG_DEBUG << "Destroying callback associated with connection object.";
				delete it->first;
				callbacks.erase(it);
				goto repeat;
			}
		}
	}
}


void Cache::putLogHandler(gmsec::util::LogHandler* handler, jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	logHandlers[handler] = o;
}


jobject Cache::getLogHandler(gmsec::util::LogHandler* handler)
{
	gmsec::util::AutoMutex hold(mutex);
	jobject o = logHandlers[handler];
	if (!o)
	{
		LOG_WARNING << "JNI.getLogHandler: unknown handler " << handler;
	}
	return o;
}


void Cache::putReplyCallback(gmsec::ReplyCallback* callback, jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	replyCallbacks[callback] = o;
	LOG_VERBOSE << "JNI.putReplyCallback: ReplyCallback=" << callback << " jobject=" << o;
}


jobject Cache::getReplyCallback(gmsec::ReplyCallback* callback)
{
	gmsec::util::AutoMutex hold(mutex);
	jobject o = replyCallbacks[callback];
	if (!o)
	{
		LOG_WARNING << "JNI.getReplyCallback: unknown callback " << callback;
	}
	return o;
}


void Cache::putErrorCallback(gmsec::ErrorCallback* callback, jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	errorCallbacks[callback] = o;
}


jobject Cache::getErrorCallback(gmsec::ErrorCallback* callback)
{
	gmsec::util::AutoMutex hold(mutex);
	jobject o = errorCallbacks[callback];
	if (!o)
	{
		LOG_WARNING << "JNI.getErrorCallback: unknown callback " << callback;
	}
	return o;
}


// For ConnectionManagerCallbacks
//
void Cache::putConnMgrCallback(gmsec::mist::ConnectionManagerCallback* callback, jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	mistCallbacks[callback] = o;
}


jobject Cache::getConnMgrCallback(gmsec::mist::ConnectionManagerCallback* callback)
{
	gmsec::util::AutoMutex hold(mutex);
	std::map<gmsec::mist::ConnectionManagerCallback*, jobject>::iterator it = mistCallbacks.find(callback);
	jobject o = NULL;
	if (it == mistCallbacks.end())
	{
		LOG_WARNING << "JNI.getCallback: unknown MIST callback " << callback;
	}
	else
	{
		o = it->second;
	}
	return o;
}


gmsec::mist::ConnectionManagerCallback* Cache::getAndRemoveConnMgrCallback(JNIEnv* jenv, jstring subject, jobject userCallback)
{
	gmsec::util::AutoMutex hold(mutex);

	gmsec::mist::ConnectionManagerCallback* cb = 0;
	std::map<gmsec::mist::ConnectionManagerCallback*, jobject>::iterator it = mistCallbacks.begin();

	for (; it != mistCallbacks.end(); ++it)
	{
		JNIMistCallback* jcb = dynamic_cast<JNIMistCallback*>(it->first);

		if (jcb)
		{
			jstring jcbSubject = (jstring) jcb->getSubject();

			JStringManager str1(jenv, subject);
			JStringManager str2(jenv, jcbSubject);

			if (str1.c_str() && str2.c_str() && std::string(str1.c_str()) == std::string(str2.c_str()) &&
			    jenv->IsSameObject(jcb->getCallback(), userCallback) == JNI_TRUE)
			{
				cb = it->first;
				break;
			}
		}
	}

	if (cb)
	{
		LOG_DEBUG << "Found callback " << it->first << "; removing it.";
		delete it->first;
		mistCallbacks.erase(it);
		LOG_DEBUG << "We now have " << mistCallbacks.size() << " entries in MIST Callbacks.";
	}
	else
	{
		LOG_WARNING << "JNI.getAndRemoveMistCallback: unknown callback " << userCallback;
	}

	return cb;
}


void Cache::destroyConnMgrCallbacks(JNIEnv* jenv, jobject jConnectionManager)
{
	gmsec::util::AutoMutex hold(mutex);

	repeat:
	for (std::map<gmsec::mist::ConnectionManagerCallback*, jobject>::iterator it = mistCallbacks.begin(); it != mistCallbacks.end(); ++it)
	{
		JNIMistCallback* jcb = dynamic_cast<JNIMistCallback*>(it->first);

		if (jcb)
		{
			if (jenv->IsSameObject(jcb->getConnMgr(), jConnectionManager) == JNI_TRUE)
			{
				LOG_DEBUG << "Destroying callback associated with connection manager object.";
				delete it->first;
				mistCallbacks.erase(it);
				goto repeat;
			}
		}
	}
}


// For ConnectionManagerErrorCallbacks
//
void Cache::putConnMgrErrorCallback(gmsec::mist::ConnectionManagerErrorCallback* callback, jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	mistErrorCallbacks[callback] = o;
}


jobject Cache::getConnMgrErrorCallback(gmsec::mist::ConnectionManagerErrorCallback* callback)
{
	gmsec::util::AutoMutex hold(mutex);
	std::map<gmsec::mist::ConnectionManagerErrorCallback*, jobject>::iterator it = mistErrorCallbacks.find(callback);
	jobject o = NULL;
	if (it == mistErrorCallbacks.end())
	{
		LOG_WARNING << "JNI.getCallback: unknown MIST Error Callback " << callback;
	}
	else
	{
		o = it->second;
	}
	return o;
}



// For ConnectionManagerReplyCallbacks
//
void Cache::putConnMgrReplyCallback(gmsec::mist::ConnectionManagerReplyCallback* callback, jobject o)
{
	gmsec::util::AutoMutex hold(mutex);
	mistReplyCallbacks[callback] = o;
}


jobject Cache::getConnMgrReplyCallback(gmsec::mist::ConnectionManagerReplyCallback* callback)
{
	gmsec::util::AutoMutex hold(mutex);
	std::map<gmsec::mist::ConnectionManagerReplyCallback*, jobject>::iterator it = mistReplyCallbacks.find(callback);
	jobject o = NULL;
	if (it == mistReplyCallbacks.end())
	{
		LOG_WARNING << "JNI.getCallback: unknown MIST Reply Callback " << callback;
	}
	else
	{
		o = it->second;
	}
	return o;
}
