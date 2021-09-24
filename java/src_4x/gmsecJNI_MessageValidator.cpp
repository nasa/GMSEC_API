/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_MessageValidator.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::jni;


CxxMessageValidatorProxy::CxxMessageValidatorProxy(JNIEnv* env, jobject javaMessageValidator)
	: jValidator(env->NewGlobalRef(javaMessageValidator))
{
}

CxxMessageValidatorProxy::~CxxMessageValidatorProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CxxMessageValidatorProxy::~CxxMessageValidatorProxy() -- unable to attach to the current thread";
		return;
	}

	jenv->DeleteGlobalRef(jValidator);
}

Status CxxMessageValidatorProxy::validateMessage(const Message& msg)
{
	if (!jValidator)
	{
		return Status(MIST_ERROR, UNINITIALIZED_OBJECT, "CxxMessageValidatorProxy::validateMessage() -- Unable to locate JNI Message Validator.");
	}

	AutoJEnv aje;
	JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		return Status(MIST_ERROR, UNINITIALIZED_OBJECT, "CxxMessageValidatorProxy::validateMessage() -- Unable to attach to the current thread");
	}

	jmethodID validateMethod = Cache::getCache().methodMessageValidatorValidateMessage;
	jobject   jMessage       = createJavaMessage(jenv, msg);

	if (!jMessage)
	{
		return Status(MIST_ERROR, UNINITIALIZED_OBJECT, "CxxMessageValidatorProxy::validateMessage() -- Unable to create Java Message");
	}

	jobject jStatus = jenv->CallObjectMethod(jValidator, validateMethod, jMessage);

	jvmOk(jenv, "CxxMessageValidatorProxy.validateMessage");

	jobject jStatusClass  = jenv->CallObjectMethod(jStatus, Cache::getCache().methodStatusGetClass);
	jobject jStatusCode   = jenv->CallObjectMethod(jStatus, Cache::getCache().methodStatusGetCode);
	jint    jStatusCustom = (jint) jenv->CallIntMethod(jStatus, Cache::getCache().methodStatusGetCustomCode);
	jstring jStatusReason = (jstring) jenv->CallObjectMethod(jStatus, Cache::getCache().methodStatusGetReason);

	JStringManager reason(jenv, jStatusReason);

	StatusClass statusClass = (StatusClass) jenv->CallIntMethod(jStatusClass, Cache::getCache().methodStatusClassGetValue);
	StatusCode  statusCode  = (StatusCode) jenv->CallIntMethod(jStatusCode, Cache::getCache().methodStatusCodeGetValue);
	GMSEC_I32   customCode  = (GMSEC_I32) jStatusCustom;

	Status status(statusClass, statusCode, reason.c_str(), customCode);

	jenv->DeleteLocalRef(jStatusReason);
	jenv->DeleteLocalRef(jStatus);
	jenv->DeleteLocalRef(jMessage);

	return status;
}


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MessageValidator
  (JNIEnv *jenv, jclass jcls, jobject jMessageValidator)
{
	MessageValidator* proxy = 0;

	try
	{
		proxy = new CxxMessageValidatorProxy(jenv, jMessageValidator);
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(proxy);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1MessageValidator
  (JNIEnv *jenv, jclass jcls, jlong jMessageValidatorPtr, jobject jMessageValidator)
{
	CxxMessageValidatorProxy* proxy = reinterpret_cast<CxxMessageValidatorProxy*>(jMessageValidatorPtr);

	delete proxy;
}


#ifdef __cplusplus
}
#endif
