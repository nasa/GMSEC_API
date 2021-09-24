/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#include "gmsecJNI_Callbacks.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"


//
// JNIMistCallback
//
JNIMistCallback::JNIMistCallback(JNIEnv* env, jobject jConnMgr, jstring jSubject, jobject jCallback)
	: myConnMgr(env->NewGlobalRef(jConnMgr)),
	  mySubject(env->NewGlobalRef(jSubject)),
	  myCallback(env->NewGlobalRef(jCallback))
{
	Cache::getCache().putConnMgrCallback(this, myCallback);
}


JNIMistCallback::~JNIMistCallback()
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jenv->DeleteGlobalRef(myConnMgr);

	if (mySubject != 0) {
		jenv->DeleteGlobalRef(mySubject);
	}

	jenv->DeleteGlobalRef(myCallback);
}


jobject JNIMistCallback::getConnMgr() const
{
	return myConnMgr;
}


jobject JNIMistCallback::getSubject() const
{
	return mySubject;
}


jobject JNIMistCallback::getCallback() const
{
	return myCallback;
}


void JNIMistCallback::OnMessage(gmsec::mist::ConnectionManager* connMgr, gmsec::Message* message)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject jCallback = Cache::getCache().getConnMgrCallback(this);
	jobject jConnMgr = this->getConnMgr();

	if (!jCallback)
	{
		LOG_WARNING << "Unable to locate JNI MIST Callback.";
		return;
	}

	if (!jConnMgr)
	{
		LOG_WARNING << "Unable to locate JNI ConnectionManager.";
		return;
	}

	jobject javaMessage = createJavaMessage(jenv, message);

	if (!javaMessage)
	{
		LOG_WARNING << "Unable to create Java Message";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodConnMgrCallbackOnMessage;

	jobject javaConnMgr = jenv->GetObjectField(jConnMgr, Cache::getCache().fieldConnectionManager_extConnMgr);

	jenv->CallVoidMethod(jCallback, callbackMethod, javaConnMgr, javaMessage);

	jvmOk(jenv, "JNIMistCallback.OnMessage");
}



//
// JNIMistErrorCallback
//
JNIMistErrorCallback::JNIMistErrorCallback(JNIEnv* env, jobject jConnMgr, jobject jCallback)
	: myConnMgr(env->NewGlobalRef(jConnMgr)),
	  myCallback(env->NewGlobalRef(jCallback))
{
	Cache::getCache().putConnMgrErrorCallback(this, myCallback);
}


JNIMistErrorCallback::~JNIMistErrorCallback()
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jenv->DeleteGlobalRef(myConnMgr);

	jenv->DeleteGlobalRef(myCallback);
}


jobject JNIMistErrorCallback::getConnMgr() const
{
	return myConnMgr;
}


jobject JNIMistErrorCallback::getCallback() const
{
	return myCallback;
}


void JNIMistErrorCallback::OnError(gmsec::mist::ConnectionManager* connMgr, gmsec::Message* message, gmsec::Status* status, const char* event)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject jCallback = Cache::getCache().getConnMgrErrorCallback(this);
	jobject jConnMgr = this->getConnMgr();

	if (!jCallback)
	{
		LOG_WARNING << "Unable to locate JNI MIST Error Callback.";
		return;
	}

	if (!jConnMgr)
	{
		LOG_WARNING << "Unable to locate JNI ConnectionManager.";
		return;
	}

	jobject javaMessage = createJavaMessage(jenv, message);

	if (!javaMessage)
	{
		LOG_WARNING << "Unable to create Java Message";
		return;
	}

    jstring statusString = makeJavaString(jenv, status->GetString());

    if (!jvmOk(jenv, "JNIMistErrorCallback.OnError") || !statusString)
    {   
        LOG_DEBUG << "JNIMistErrorCallback.OnError: Unable to create status jstring.";
        return;
    }   

    jobject jStatus = jenv->NewObject(Cache::getCache().classJNIStatus,
            Cache::getCache().methodStatusInitIJString,
            (jshort) status->GetClass(),
            (jlong) status->GetCode(), statusString);

    if (!jvmOk(jenv, "JNIMistErrorCallback.OnError") || !jStatus)
    {   
        LOG_DEBUG << "JNIMistErrorCallback.OnError: Unable to create status jstring.";
        return;
    }   

    jenv->CallVoidMethod(jStatus, Cache::getCache().methodStatusSetCustomCode, status->GetCustomCode());

	jmethodID callbackMethod = Cache::getCache().methodConnMgrCallbackOnError;

	jobject javaConnMgr = jenv->GetObjectField(jConnMgr, Cache::getCache().fieldConnectionManager_extConnMgr);

	jenv->CallVoidMethod(jCallback, callbackMethod, javaConnMgr, javaMessage, jStatus);

	jvmOk(jenv, "JNIMistErrorCallback.OnError");
}



//
// JNIMistReplyCallback
//
JNIMistReplyCallback::JNIMistReplyCallback(JNIEnv* env, jobject jConnMgr, jobject jCallback)
	: myConnMgr(env->NewGlobalRef(jConnMgr)),
	  myCallback(env->NewGlobalRef(jCallback))
{
	Cache::getCache().putConnMgrReplyCallback(this, myCallback);
}


JNIMistReplyCallback::~JNIMistReplyCallback()
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jenv->DeleteGlobalRef(myConnMgr);

	jenv->DeleteGlobalRef(myCallback);
}


jobject JNIMistReplyCallback::getConnMgr() const
{
	return myConnMgr;
}


jobject JNIMistReplyCallback::getCallback() const
{
	return myCallback;
}


void JNIMistReplyCallback::OnReply(gmsec::mist::ConnectionManager* connMgr, gmsec::Message* request, gmsec::Message* reply)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject jCallback = Cache::getCache().getConnMgrReplyCallback(this);
	jobject jConnMgr = this->getConnMgr();

	if (!jCallback)
	{
		LOG_WARNING << "Unable to locate JNI MIST Reply Callback.";
		return;
	}

	if (!jConnMgr)
	{
		LOG_WARNING << "Unable to locate JNI ConnectionManager.";
		return;
	}

	jobject javaRequestMsg = createJavaMessage(jenv, request);

	if (!javaRequestMsg) {
		LOG_WARNING << "Unable to create Java Request Message";
		return;
	}

	jobject javaReplyMsg = createJavaMessage(jenv, reply);

	if (!javaReplyMsg) {
		LOG_WARNING << "Unable to create Java Reply Message";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodConnMgrCallbackOnReply;

	jobject javaConnMgr = jenv->GetObjectField(jConnMgr, Cache::getCache().fieldConnectionManager_extConnMgr);

	jenv->CallVoidMethod(jCallback, callbackMethod, javaConnMgr, javaRequestMsg, javaReplyMsg);

	jvmOk(jenv, "JNIMistReplCallback.OnReply");
}


void JNIMistReplyCallback::OnError(gmsec::mist::ConnectionManager* connMgr, gmsec::Message* message, gmsec::Status* status, const char* event)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject jCallback = Cache::getCache().getConnMgrReplyCallback(this);
	jobject jConnMgr = this->getConnMgr();

	if (!jCallback)
	{
		LOG_WARNING << "Unable to locate JNI MIST Reply Callback.";
		return;
	}

	if (!jConnMgr)
	{
		LOG_WARNING << "Unable to locate JNI ConnectionManager.";
		return;
	}

	jobject javaMessage = createJavaMessage(jenv, message);

	if (!javaMessage)
	{
		LOG_WARNING << "Unable to create Java Message";
		return;
	}

    jstring statusString = makeJavaString(jenv, status->GetString());

    if (!jvmOk(jenv, "JNIMistReplyCallback.OnError") || !statusString)
    {   
        LOG_DEBUG << "JNIMistReplyCallback.OnError: Unable to create status jstring.";
        return;
    }   

    jobject jStatus = jenv->NewObject(Cache::getCache().classJNIStatus,
            Cache::getCache().methodStatusInitIJString,
            (jshort) status->GetClass(),
            (jlong) status->GetCode(), statusString);

    if (!jvmOk(jenv, "JNIMistReplyCallback.OnError") || !jStatus)
    {   
        LOG_DEBUG << "JNIMistReplyCallback.OnError: Unable to create status jstring.";
        return;
    }   

    jenv->CallVoidMethod(jStatus, Cache::getCache().methodStatusSetCustomCode, status->GetCustomCode());

	jmethodID callbackMethod = Cache::getCache().methodConnMgrCallbackOnError;

	jobject javaConnMgr = jenv->GetObjectField(jConnMgr, Cache::getCache().fieldConnectionManager_extConnMgr);

	jenv->CallVoidMethod(jCallback, callbackMethod, javaConnMgr, javaMessage, jStatus);

	jvmOk(jenv, "JNIMistReplyCallback.OnError");
}



//
// JNICallback
//
JNICallback::JNICallback(JNIEnv* env, jobject jConnection, jobject jCallback)
	: myConnection(env->NewGlobalRef(jConnection)),
	  mySubject(0),
	  myCallback(env->NewGlobalRef(jCallback))
{
	Cache::getCache().putCallback(this, myCallback);
}


JNICallback::JNICallback(JNIEnv* env, jobject jConnection, jstring jSubject, jobject jCallback)
	: myConnection(env->NewGlobalRef(jConnection)),
	  mySubject(env->NewGlobalRef(jSubject)),
	  myCallback(env->NewGlobalRef(jCallback))
{
	Cache::getCache().putCallback(this, myCallback);
}

JNICallback::~JNICallback()
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jenv->DeleteGlobalRef(myConnection);

	if (mySubject != 0) {
		jenv->DeleteGlobalRef(mySubject);
	}

	jenv->DeleteGlobalRef(myCallback);
}


jobject JNICallback::getConnection() const
{
	return myConnection;
}


jobject JNICallback::getSubject() const
{
	return mySubject;
}


jobject JNICallback::getCallback() const
{
	return myCallback;
}


void JNICallback::OnMessage(gmsec::Connection* connection, gmsec::Message* message)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject jCallback = Cache::getCache().getCallback(this);
	jmethodID callbackMethod = Cache::getCache().methodCallbackOnMessage;
	jobject jniConnection = Cache::getCache().getConnection(connection);
	if (!jCallback || !jniConnection)
	{
		LOG_WARNING << "Unable to locate JNI Connection/Callback.";
		return;
	}

	jobject javaMessage = createJavaMessage(jenv, message);
	if (!javaMessage)
	{
		LOG_WARNING << "Unable to create Java Message";
		return;
	}

	jobject javaConn = jenv->GetObjectField(jniConnection, Cache::getCache().fieldConnection_extconn);

	jenv->CallVoidMethod(jCallback, callbackMethod, javaConn, javaMessage);

	jvmOk(jenv, "JNICallback.OnMessage");
}


//
// JNIReplyCallback
//
JNIReplyCallback::JNIReplyCallback(JNIEnv* env, jobject jCallback)
{
	Cache::getCache().putReplyCallback(this, env->NewGlobalRef(jCallback));
}


void JNIReplyCallback::OnError(gmsec::Connection* connection, gmsec::Message* message, gmsec::Status* status, const char* event)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject javaRequest = createJavaMessage(jenv, message);
	if (!jvmOk(jenv, "JNIReplyCallback.OnError") || !javaRequest)
	{
		LOG_WARNING << "JNIReplyCallback.OnError: unable to create Java request";
		return;
	}

	jobject jCallback = Cache::getCache().getReplyCallback(this);
	jmethodID errorMethod = Cache::getCache().methodErrorCallbackOnError;
	jobject jConnection = Cache::getCache().getConnection(connection);
	jstring statusString = makeJavaString(jenv, status->GetString());
	if (!jvmOk(jenv, "JNIReplyCallback.OnError") || !statusString)
	{
		LOG_DEBUG << "JNIReplyCallback.OnError: Unable to create status jstring.";
		return;
	}

	jobject jExtConn = jenv->GetObjectField(jConnection, Cache::getCache().fieldConnection_extconn);

	jobject statusObj = jenv->NewObject(Cache::getCache().classJNIStatus,
			Cache::getCache().methodStatusInitIJString,
			(jshort) status->GetClass(),
			(jlong) status->GetCode(), statusString);
	if (!jvmOk(jenv, "JNIReplyCallback.OnError") || !statusObj)
	{
		LOG_DEBUG << "JNIReplyCallback.OnError: Unable to create status jstring.";
		return;
	}
	jenv->CallVoidMethod(statusObj, Cache::getCache().methodStatusSetCustomCode, status->GetCustomCode());

	jenv->CallVoidMethod(jCallback, errorMethod, jExtConn, javaRequest, statusObj);
	jvmOk(jenv, "JNIReplyCallback.OnError");
}


void JNIReplyCallback::OnReply(gmsec::Connection* connection, gmsec::Message* request, gmsec::Message* reply)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject javaRequest = createJavaMessage(jenv, request);
	if (!jvmOk(jenv, "JNIReplyCallback.OnReply") || !javaRequest)
	{
		LOG_WARNING << "JNIReplyCallback.OnReply: Couldn't create Java request message.";
		return;
	}

	jobject javaReply = createJavaMessage(jenv, reply);
	if (!jvmOk(jenv, "JNIReplyCallback.OnReply") || !javaReply)
	{
		LOG_DEBUG << "JNIReplyCallback.OnReply: Couldn't create Java reply message.";
		return;
	}

	jobject jCallback = Cache::getCache().getReplyCallback(this);
	jmethodID callbackMethod = Cache::getCache().methodReplyCallbackOnReply;
	jobject jConnection = Cache::getCache().getConnection(connection);

	jobject jExtConn = jenv->GetObjectField(jConnection, Cache::getCache().fieldConnection_extconn);

	jenv->CallVoidMethod(jCallback, callbackMethod, jExtConn, javaRequest, javaReply);
	jvmOk(jenv, "JNIReplyCallback.OnReply");
}


//
// JNIErrorCallback
//
//DMW TODO: Dead code??
JNIErrorCallback::JNIErrorCallback()
{
	Cache::getCache().putErrorCallback(this, NULL);
}


JNIErrorCallback::JNIErrorCallback(JNIEnv* env, jobject jCallback)
{
	Cache::getCache().putErrorCallback(this, env->NewGlobalRef(jCallback));
}


void JNIErrorCallback::OnError(gmsec::Connection* connection, gmsec::Message* message, gmsec::Status* status, const char* event)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject jCallback = Cache::getCache().getErrorCallback(this);
	if (!jCallback)
	{
		LOG_WARNING << "No callback associated with " << event << ".";
		return;
	}

	jobject jniConnection = Cache::getCache().getConnection(connection);
	if (!jCallback || !jniConnection)
	{
		LOG_DEBUG << "Unable to locate JNI Connection/Callback.";
		return;
	}

	jobject javaConn = jenv->GetObjectField(jniConnection, Cache::getCache().fieldConnection_extconn);

	jobject javaMessage = createJavaMessage(jenv, message);
	if (!javaMessage)
	{
		LOG_DEBUG << "Unable to create Java Message";
		return;
	}

	jstring statusString = makeJavaString(jenv, status->GetString());
	if (!jvmOk(jenv, "JNIErrorCallback.OnError: makeJavaString(jenv, status)") || !statusString)
	{
		LOG_DEBUG << "JNIErrorCallback: Unable to create status jstring.";
		return;
	}

	jobject statusObj = jenv->NewObject(Cache::getCache().classJNIStatus,
			Cache::getCache().methodStatusInitIJString,
			(jshort) status->GetClass(),
			(jlong) status->GetCode(), statusString);
	if (!jvmOk(jenv, "JNIErrorCallback.OnError: create JNIStatus") || statusObj == NULL)
	{
		LOG_DEBUG << "JNIErrorCallback: Unable to create JNIStatus.";
		return;
	}
	jenv->CallVoidMethod(statusObj, Cache::getCache().methodStatusSetCustomCode, status->GetCustomCode());

	jenv->CallVoidMethod(jCallback, Cache::getCache().methodErrorCallbackOnError, javaConn, javaMessage, statusObj);
	jvmOk(jenv, "JNIErrorCallback.OnError");
}



//
// JNILogHandler
//
JNILogHandler::JNILogHandler(JNIEnv *env, jobject jLogHandler)
{
	Cache::getCache().putLogHandler(this, env->NewGlobalRef(jLogHandler));
}


void JNILogHandler::OnMessage(const gmsec::util::LogEntry& entry)
{
	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	jobject jLogHandler = Cache::getCache().getLogHandler(this);
	jmethodID callbackMethod = Cache::getCache().methodLogHandlerOnMessage;
	jobject entryObj = jenv->NewObject(Cache::getCache().classLogEntry, Cache::getCache().methodLogEntryInitJZ);
	jenv->SetIntField(entryObj, Cache::getCache().fieldLogLevel_value, (jint) entry.level);
	jenv->SetDoubleField(entryObj, Cache::getCache().fieldLogTime_value, (jdouble) entry.time);

	jstring s = makeJavaString(jenv, entry.message);
	jenv->SetObjectField(entryObj, Cache::getCache().fieldLogMessage_value, s);
	jenv->CallVoidMethod(jLogHandler, callbackMethod, entryObj);
	jvmOk(jenv, "JNILogHandler.OnMessage");
}
