/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_ConnectionManagerCallback.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


CxxConnMgrCallbackProxy::CxxConnMgrCallbackProxy(JNIEnv* env, jobject javaCallback)
	: jCallback(env->NewGlobalRef(javaCallback))
{
}

CxxConnMgrCallbackProxy::~CxxConnMgrCallbackProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxConnMgrCallbackProxy::~CxxConnMgrCallbackProxy() -- unable to attach to the current thread";
		return;
	}

	jenv->DeleteGlobalRef(jCallback);
}

void CxxConnMgrCallbackProxy::onMessage(ConnectionManager& connMgr, const Message& msg)
{
	if (!jCallback)
	{
		GMSEC_ERROR << "Unable to locate JNI MIST Event Callback.";
		return;
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxConnMgrCallbackProxy::onMessage() -- unable to attach to the current thread";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodConnMgrCallbackOnMessage;
	jobject   jniConnMgr     = jenv->GetObjectField(jCallback, Cache::getCache().fieldCallbackJNIConnMgr);
	jobject   jConnMgr       = jenv->GetObjectField(jniConnMgr, Cache::getCache().fieldConnectionManager_extConnMgr);

	if (!jConnMgr)
	{
		GMSEC_ERROR << "CxxConnMgrCallbackProxy::onMessage() -- unable to locate JNI ConnectionManager";
		return;
	}

	jobject jMessage = createJavaMessage(jenv, msg);

	if (!jMessage)
	{
		GMSEC_ERROR << "CxxConnMgrCallbackProxy::onMessage() -- unable to create Java Message";
		return;
	}

	jenv->CallVoidMethod(jCallback, callbackMethod, jConnMgr, jMessage);

	jvmOk(jenv, "CxxConnMgrCallbackProxy.onMessage");

	jenv->DeleteLocalRef(jMessage);
}


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ConnectionManagerCallback
  (JNIEnv *jenv, jclass jcls, jobject callback)
{
	ConnectionManagerCallback* proxy = 0;

	try
	{
		proxy = new CxxConnMgrCallbackProxy(jenv, callback);
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(proxy);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ConnectionManagerCallback
  (JNIEnv *jenv, jclass jcls, jlong callbackPtr, jobject jCallback)
{
	CxxConnMgrCallbackProxy* proxy = reinterpret_cast<CxxConnMgrCallbackProxy*>(callbackPtr);

	delete proxy;
}


#ifdef __cplusplus
}
#endif
