/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_EventCallback.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;


CxxEventCallbackProxy::CxxEventCallbackProxy(JNIEnv* env, jobject javaEventCallback)
	: jCallback(env->NewGlobalRef(javaEventCallback))
{
}

CxxEventCallbackProxy::~CxxEventCallbackProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxEventCallbackProxy::~CxxEventCallbackProxy() -- unable to attach to the current thread";
		return;
	}

	jenv->DeleteGlobalRef(jCallback);
}

void CxxEventCallbackProxy::onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
{
	if (!jCallback)
	{
		GMSEC_ERROR << "Unable to locate JNI Event Callback.";
		return;
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxEventCallbackProxy::onEvent() -- unable to attach to the current thread";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodEventCallbackOnEvent;
	jobject   jniConnection  = jenv->GetObjectField(jCallback, Cache::getCache().fieldEventCallbackJNIConnection);
	jobject   jConnection    = jenv->GetObjectField(jniConnection, Cache::getCache().fieldConnection_extconn);

	if (!jConnection)
	{
		GMSEC_ERROR << "CxxEventCallbackProxy::onEvent() -- unable to locate JNI Connection";
		return;
	}

	jstring statusString = makeJavaString(jenv, status.getReason());

	if (!jvmOk(jenv, "CxxEventCallbackProxy.onEvent: makeJavaString(jenv, status)") || !statusString)
	{
		GMSEC_WARNING << "CxxEventCallbackProxy::onEvent() -- Unable to create status jstring.";
		return;
	}

	jobject jStatus = jenv->NewObject(Cache::getCache().classJNIStatus,
			Cache::getCache().methodStatusInitIJString,
			(jint) status.getClass(), (jint) status.getCode(), statusString, status.getCustomCode());

	if (!jvmOk(jenv, "CxxEventCallbackProxy.onEvent: create JNIStatus") || jStatus == NULL)
	{
		GMSEC_WARNING << "CxxEventCallbackProxy::onEvent() -- Unable to create JNIStatus.";
		return;
	}

	jenv->CallVoidMethod(jCallback, callbackMethod, jConnection, jStatus, convertEvent(jenv, event));

	jvmOk(jenv, "CxxEventCallbackProxy.onEvent");

	jenv->DeleteLocalRef(jStatus);
	jenv->DeleteLocalRef(statusString);
}


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1EventCallback
  (JNIEnv *jenv, jclass jcls, jobject jEventCallback)
{
	EventCallback* proxy = 0;

	try
	{
		proxy = new CxxEventCallbackProxy(jenv, jEventCallback);
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(proxy);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1EventCallback
  (JNIEnv *jenv, jclass jcls, jlong eventCallbackPtr, jobject jEventCallback)
{
	CxxEventCallbackProxy* proxy = reinterpret_cast<CxxEventCallbackProxy*>(eventCallbackPtr);

	delete proxy;
}


#ifdef __cplusplus
}
#endif
