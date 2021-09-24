/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_Callback.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;


CxxCallbackProxy::CxxCallbackProxy(JNIEnv* env, jobject javaCallback)
	: jCallback(env->NewGlobalRef(javaCallback))
{
}


CxxCallbackProxy::~CxxCallbackProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxCallbackProxy::~CxxCallbackProxy() -- unable to attach to the current thread";
		return;
	}

	jenv->DeleteGlobalRef(jCallback);
}


void CxxCallbackProxy::onMessage(Connection& conn, const Message& msg)
{
	if (!jCallback)
	{
		GMSEC_ERROR << "Unable to locate JNI Callback.";
		return;
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxCallbackProxy::onMessage() -- unable to attach to the current thread";
		return;
	}

	jmethodID callbackMethod = Cache::getCache().methodCallbackOnMessage;
	jobject   jniConnection  = jenv->GetObjectField(jCallback, Cache::getCache().fieldCallbackJNIConnection);
	jobject   jConnection    = jenv->GetObjectField(jniConnection, Cache::getCache().fieldConnection_extconn);

	if (!jConnection)
	{
		GMSEC_ERROR << "CxxCallbackProxy::onMessage() -- unable to locate JNI Connection";
		return;
	}

	jobject jMessage = createJavaMessage(jenv, msg);

	if (!jMessage)
	{
		GMSEC_ERROR << "CxxCallbackProxy::onMessage() -- unable to create Java Message";
		return;
	}

	jenv->CallVoidMethod(jCallback, callbackMethod, jConnection, jMessage);

	jvmOk(jenv, "CxxCallbackProxy.onMessage");

	jenv->DeleteLocalRef(jMessage);
}


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Callback
  (JNIEnv *jenv, jclass jcls, jobject callback)
{
	Callback* proxy = 0;

	try
	{
		proxy = new CxxCallbackProxy(jenv, callback);
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(proxy);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Callback
  (JNIEnv *jenv, jclass jcls, jlong callbackPtr, jobject jCallback)
{
	CxxCallbackProxy* proxy = reinterpret_cast<CxxCallbackProxy*>(callbackPtr);

	delete proxy;
}


#ifdef __cplusplus
}
#endif
