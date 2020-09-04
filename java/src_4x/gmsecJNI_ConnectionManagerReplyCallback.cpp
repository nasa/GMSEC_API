/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_ConnectionManagerReplyCallback.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


CxxConnMgrReplyCallbackProxy::CxxConnMgrReplyCallbackProxy(JNIEnv* env, jobject javaCallback)
	: jCallback(env->NewGlobalRef(javaCallback))
{
}


CxxConnMgrReplyCallbackProxy::~CxxConnMgrReplyCallbackProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxConnMgrReplyCallbackProxy::~CxxConnMgrReplyCallbackProxy() -- unable to attach to the current thread";
		return;
	}

	jenv->DeleteGlobalRef(jCallback);
}


void CxxConnMgrReplyCallbackProxy::onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
{
	if (!jCallback)
	{
		GMSEC_WARNING << "Unable to locate JNI MIST Reply Callback.";
		return;
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxConnMgrReplyCallbackProxy::onEvent() -- unable to attach to the current thread";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodConnMgrEventCallbackOnEvent;
	jobject   jniConnMgr     = jenv->GetObjectField(jCallback, Cache::getCache().fieldReplyCallbackJNIConnMgr);
	jobject   jConnMgr       = jenv->GetObjectField(jniConnMgr, Cache::getCache().fieldConnectionManager_extConnMgr);

	if (!jConnMgr)
	{
		GMSEC_WARNING << "Unable to locate JNI ConnectionManager.";
		return;
	}

	jstring jStatusString = makeJavaString(jenv, status.getReason());

	if (!jvmOk(jenv, "JNIMistReplyCallbackProxy.onEvent") || !jStatusString)
	{
		GMSEC_WARNING << "JNIMistReplyCallbackProxy::onEvent() -- Unable to create status jstring.";
		return;
	}

	jobject jStatus = jenv->NewObject(Cache::getCache().classJNIStatus,
		Cache::getCache().methodStatusInitIJString,
		(jint) status.getClass(), (jint) status.getCode(), jStatusString, (jint) status.getCustomCode());

	if (!jvmOk(jenv, "JNIMistReplyCallbackProxy.onEvent") || !jStatus)
	{
		GMSEC_WARNING << "JNIMistReplyCallbackProxy::onEvent() -- Unable to create status jstring.";
		return;
	}

	jenv->CallVoidMethod(jCallback, callbackMethod, jConnMgr, jStatus, convertEvent(jenv, event));

	jvmOk(jenv, "CxxConnMgrReplyCallbackProxy.onEvent");

	jenv->DeleteLocalRef(jStatus);
	jenv->DeleteLocalRef(jStatusString);
}


void CxxConnMgrReplyCallbackProxy::onReply(ConnectionManager& connMgr, const Message& request, const Message& reply)
{
	if (!jCallback)
	{
		GMSEC_WARNING << "Unable to locate JNI MIST Reply Callback.";
		return;
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxConnMgrReplyCallbackProxy::onReply() -- unable to attach to the current thread";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodConnMgrReplyCallbackOnReply;
	jobject   jniConnMgr     = jenv->GetObjectField(jCallback, Cache::getCache().fieldReplyCallbackJNIConnMgr);
	jobject   jConnMgr       = jenv->GetObjectField(jniConnMgr, Cache::getCache().fieldConnectionManager_extConnMgr);

	if (!jConnMgr)
	{
		GMSEC_WARNING << "Unable to locate JNI ConnectionManager.";
		return;
	}

	jobject jRequest = createJavaMessage(jenv, request);
	jobject jReply   = createJavaMessage(jenv, reply);

	if (!jRequest)
	{
		GMSEC_ERROR << "CxxConnMgrReplyCallbackProxy::onReply() -- unable to create Java Request Message";
		return;
	}
	if (!jReply)
	{
		GMSEC_ERROR << "CxxConnMgrReplyCallbackProxy::onReply() -- unable to create Java Reply Message";
		return;
	}

	jenv->CallVoidMethod(jCallback, callbackMethod, jConnMgr, jRequest, jReply);

	jvmOk(jenv, "CxxConnMgrReplyCallbackProxy.onReply");

	jenv->DeleteLocalRef(jRequest);
	jenv->DeleteLocalRef(jReply);
}


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ConnectionManagerReplyCallback
  (JNIEnv *jenv, jclass jcls, jobject callback)
{
	ConnectionManagerReplyCallback* proxy = 0;

	try
	{
		proxy = new CxxConnMgrReplyCallbackProxy(jenv, callback);
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(proxy);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ConnectionManagerReplyCallback
  (JNIEnv *jenv, jclass jcls, jlong callbackPtr, jobject jCallback)
{
	CxxConnMgrReplyCallbackProxy* proxy = reinterpret_cast<CxxConnMgrReplyCallbackProxy*>(callbackPtr);

	delete proxy;
}


#ifdef __cplusplus
}
#endif
