/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_ReplyCallback.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;


CxxReplyCallbackProxy::CxxReplyCallbackProxy(JNIEnv* env, jobject javaReplyCallback)
	: jCallback(env->NewGlobalRef(javaReplyCallback))
{
}

CxxReplyCallbackProxy::~CxxReplyCallbackProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxReplyCallbackProxy::~CxxReplyCallbackProxy() -- unable to attach to the current thread";
		return;
	}

	jenv->DeleteGlobalRef(jCallback);
}

void CxxReplyCallbackProxy::onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
{
	if (!jCallback)
	{
		GMSEC_ERROR << "Unable to locate JNI Reply Callback.";
		return;
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxReplyCallbackProxy::onEvent() -- unable to attach to the current thread";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodEventCallbackOnEvent;
	jobject   jniConnection  = jenv->GetObjectField(jCallback, Cache::getCache().fieldReplyCallbackJNIConnection);
	jobject   jConnection    = jenv->GetObjectField(jniConnection, Cache::getCache().fieldConnection_extconn);

	if (!jConnection)
	{
		GMSEC_ERROR << "CxxReplyCallbackProxy::onEvent() -- unable to locate JNI Connection";
		return;
	}

	jstring statusString = makeJavaString(jenv, status.getReason());

	if (!jvmOk(jenv, "CxxReplyCallbackProxy.onEvent: makeJavaString(jenv, status)") || !statusString)
	{
		GMSEC_WARNING << "CxxReplyCallbackProxy::onEvent() -- Unable to create status jstring.";
		return;
	}

	jobject jStatus = jenv->NewObject(Cache::getCache().classJNIStatus,
			Cache::getCache().methodStatusInitIJString,
			(jint) status.getClass(), (jint) status.getCode(), statusString, status.getCustomCode());

	if (!jvmOk(jenv, "CxxReplyCallbackProxy.onEvent: create JNIStatus") || jStatus == NULL)
	{
		GMSEC_WARNING << "CxxReplyCallbackProxy::onEvent() -- Unable to create JNIStatus.";
		return;
	}

	jenv->CallVoidMethod(jCallback, callbackMethod, jConnection, jStatus, convertEvent(jenv, event));

	jvmOk(jenv, "CxxReplyCallbackProxy.onEvent");

	jenv->DeleteLocalRef(jStatus);
	jenv->DeleteLocalRef(statusString);
}

void CxxReplyCallbackProxy::onReply(Connection& conn, const Message& request, const Message& reply)
{
	if (!jCallback)
	{
		GMSEC_ERROR << "Unable to locate JNI Reply Callback.";
		return;
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxReplyCallbackProxy::onReply() -- unable to attach to the current thread";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodReplyCallbackOnReply;
	jobject   jniConnection  = jenv->GetObjectField(jCallback, Cache::getCache().fieldReplyCallbackJNIConnection);
	jobject   jConnection    = jenv->GetObjectField(jniConnection, Cache::getCache().fieldConnection_extconn);

	if (!jConnection)
	{
		GMSEC_ERROR << "CxxReplyCallbackProxy::onReply() -- unable to locate JNI Connection";
		return;
	}

	jobject jRequest = createJavaMessage(jenv, request);
	jobject jReply   = createJavaMessage(jenv, reply);

	if (!jRequest)
	{
		GMSEC_ERROR << "CxxReplyCallbackProxy::onReply() -- unable to create Java Request Message";
		return;
	}
	if (!jReply)
	{
		GMSEC_ERROR << "CxxReplyCallbackProxy::onReply() -- unable to create Java Reply Message";
		return;
	}

	jenv->CallVoidMethod(jCallback, callbackMethod, jConnection, jRequest, jReply);

	jvmOk(jenv, "CxxReplyCallbackProxy.onReply");

	jenv->DeleteLocalRef(jRequest);
	jenv->DeleteLocalRef(jReply);
}


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ReplyCallback
  (JNIEnv *jenv, jclass jcls, jobject jReplyCallback)
{
	ReplyCallback* proxy = 0;

	try
	{
		proxy = new CxxReplyCallbackProxy(jenv, jReplyCallback);
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(proxy);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ReplyCallback
  (JNIEnv *jenv, jclass jcls, jlong replyCallbackPtr, jobject jReplyCallback)
{
	CxxReplyCallbackProxy* proxy = reinterpret_cast<CxxReplyCallbackProxy*>(replyCallbackPtr);

	delete proxy;
}


#ifdef __cplusplus
}
#endif
