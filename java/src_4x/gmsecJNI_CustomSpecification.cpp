/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_CustomSpecification.h"
#include "gmsecJNI_Cache.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


CustomSpecification::CustomSpecification(const Config& config)
	: Specification(config),
	  jSpec(0)
{
}


CustomSpecification::CustomSpecification(const Specification& other)
	: Specification(other),
	  jSpec(0)
{
}


CustomSpecification::~CustomSpecification()
{
	if (jSpec != 0)
	{
		AutoJEnv aje;
		JNIEnv* jenv = aje.getEnv();

		if (!jenv)
		{
			GMSEC_ERROR << "CustomSpecification::~CustomSpecification() -- unable to attach to the current thread";
			return;
		}

		jenv->DeleteGlobalRef(jSpec);
	}
}


void CustomSpecification::validateMessage(const Message& msg)
{
    if (jSpec == 0)
    {
        GMSEC_ERROR << "Unable to locate JNI Specification.";
        return;
    }

    AutoJEnv aje;
    JNIEnv* jenv = aje.getEnv();

	if (!jenv)
	{
		GMSEC_ERROR << "CustomSpecification::validateMessage() -- unable to attach to the current thread";
		return;
	}

	jclass clazz = jenv->GetObjectClass(jSpec);

    if (clazz == 0)
    {
        GMSEC_ERROR << "CustomSpecification::validateMessage() -- unable to deduce specific Specification class";
        return;
    }

	jmethodID validateMessageMethod = jenv->GetMethodID(clazz, "validateMessage", "(Lgov/nasa/gsfc/gmsec/api/Message;)V");

    if (validateMessageMethod == 0)
    {
        GMSEC_ERROR << "CustomSpecification::validateMessage() -- unable to deduce Specification class' validateMessage method";
        return;
    }

    jobject jMessage = createJavaMessage(jenv, msg);

    if (jMessage == 0)
    {
        GMSEC_ERROR << "CustomSpecification::validateMessage() -- unable to create Java Message";
        return;
    }

	jenv->CallVoidMethod(jSpec, validateMessageMethod, jMessage);

	jenv->DeleteLocalRef(jMessage);

	if (jenv->ExceptionCheck())
	{
		jthrowable e = jenv->ExceptionOccurred();

		jenv->ExceptionClear();

		// Handle Java GMSEC_Exception
		jclass    clazz1        = jenv->GetObjectClass(e);
		jmethodID jErrMsgMethod = jenv->GetMethodID(clazz1, "getErrorMessage", "()Ljava/lang/String;");
		jstring   jErrMsg       = (jstring) jenv->CallObjectMethod(e, jErrMsgMethod);

		if (jErrMsg != 0)
		{
			JStringManager errMsg(jenv, jErrMsg);

			throw gmsec::api::Exception(MIST_ERROR, MESSAGE_FAILED_VALIDATION, errMsg.c_str());
		}
	}

	jvmOk(jenv, "CustomSpecification.validateMessage");
}


void CustomSpecification::registerObserver(JNIEnv* env, jobject spec)
{
	jSpec = env->NewGlobalRef(spec);
}
