/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_ConnectionManagerEventCallback.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


CxxConnMgrEventCallbackProxy::CxxConnMgrEventCallbackProxy(JNIEnv* env, jobject javaCallback)
	: jCallback(env->NewGlobalRef(javaCallback))
{
}

CxxConnMgrEventCallbackProxy::~CxxConnMgrEventCallbackProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxConnMgrEventCallbackProxy::~CxxConnMgrEventCallbackProxy() -- unable to attach to the current thread";
		return;
	}

	jenv->DeleteGlobalRef(jCallback);
}

void CxxConnMgrEventCallbackProxy::onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
{
	if (!jCallback)
	{
		GMSEC_WARNING << "Unable to locate JNI MIST Event Callback.";
		return;
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxConnMgrEventCallbackProxy::onEvent() -- unable to attach to the current thread";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodConnMgrEventCallbackOnEvent;
	jobject   jniConnMgr     = jenv->GetObjectField(jCallback, Cache::getCache().fieldEventCallbackJNIConnMgr);
	jobject   jConnMgr       = jenv->GetObjectField(jniConnMgr, Cache::getCache().fieldConnectionManager_extConnMgr);

	if (!jConnMgr)
	{
		GMSEC_WARNING << "Unable to locate JNI ConnectionManager.";
		return;
	}

	jstring jStatusString = makeJavaString(jenv, status.getReason());

	if (!jvmOk(jenv, "JNIMistEventCallbackProxy.onEvent") || !jStatusString)
	{
		GMSEC_WARNING << "JNIMistEventCallbackProxy::onEvent() -- Unable to create status jstring.";
		return;
	}

	jobject jStatus = jenv->NewObject(Cache::getCache().classJNIStatus,
		Cache::getCache().methodStatusInitIJString,
		(jint) status.getClass(), (jint) status.getCode(), jStatusString, (jint) status.getCustomCode());

	if (!jvmOk(jenv, "JNIMistEventCallbackProxy.onEvent") || !jStatus)
	{
		GMSEC_WARNING << "JNIMistEventCallbackProxy::onEvent() -- Unable to create status jstring.";
		return;
	}

	jenv->CallVoidMethod(jCallback, callbackMethod, jConnMgr, jStatus, convertEvent(jenv, event));

	jvmOk(jenv, "CxxConnMgrEventCallbackProxy.onEvent");

	jenv->DeleteLocalRef(jStatus);
}


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ConnectionManagerEventCallback
  (JNIEnv *jenv, jclass jcls, jobject callback)
{
	ConnectionManagerEventCallback* proxy = 0;

	try
	{
		proxy = new CxxConnMgrEventCallbackProxy(jenv, callback);
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(proxy);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ConnectionManagerEventCallback
  (JNIEnv *jenv, jclass jcls, jlong callbackPtr, jobject jCallback)
{
	CxxConnMgrEventCallbackProxy* proxy = reinterpret_cast<CxxConnMgrEventCallbackProxy*>(callbackPtr);

	delete proxy;
}


#ifdef __cplusplus
}
#endif
