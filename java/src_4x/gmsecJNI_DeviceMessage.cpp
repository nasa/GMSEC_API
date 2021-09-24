/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST DeviceMessage class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Config.h>
#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/DeviceIterator.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/mist/message/DeviceMessage.h>

#include <gmsec4/util/Deprecated.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1DeviceMessage__Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jlong jSpecPtr, jobject jSpec)
{
	jlong jCreated = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "DeviceMessage"))
			{
				DeviceMessage* devMsg = new DeviceMessage(subject.c_str(), *spec);

				jCreated = JNI_POINTER_TO_JLONG(devMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1DeviceMessage__Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jlong jConfigPtr, jobject jConfig, jlong jSpecPtr, jobject jSpec)
{
	jlong jCreated = 0;

	try
	{
		Config*        config = JNI_JLONG_TO_CONFIG(jConfigPtr);
		Specification* spec   = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "DeviceMessage"))
			{
				DeviceMessage* devMsg = new DeviceMessage(subject.c_str(), *config, *spec);

				jCreated = JNI_POINTER_TO_JLONG(devMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1DeviceMessage__Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jstring jData)
{
	jlong jCreated = 0;

	try
	{
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "DeviceMessage"))
		{
			DeviceMessage* devMsg = new DeviceMessage(data.c_str());

			jCreated = JNI_POINTER_TO_JLONG(devMsg);
		}
	}
	JNI_CATCH

	return jCreated;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1DeviceMessage__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec, jstring jData)
{
	jlong jCreated = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			JStringManager data(jenv, jData);

			if (jvmOk(jenv, "DeviceMessage"))
			{
				DeviceMessage* devMsg = new DeviceMessage(*spec, data.c_str());

				jCreated = JNI_POINTER_TO_JLONG(devMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1DeviceMessage_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jOtherPtr, jobject jOther)
{
	jlong jCreated = 0;

	try
	{
		DeviceMessage* other = JNI_JLONG_TO_DEVICE_MSG(jOtherPtr);

		if (!other)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceMessage reference is null");
		}
		else
		{
			DeviceMessage* devMsg = new DeviceMessage(*other);

			jCreated = JNI_POINTER_TO_JLONG(devMsg);
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceMessage_1AddDevice
  (JNIEnv *jenv, jclass jcls, jlong jDevMsgPtr, jobject jDevMsg, jlong jDevPtr, jobject jDev)
{
	try
	{
		DeviceMessage* devMsg = JNI_JLONG_TO_DEVICE_MSG(jDevMsgPtr);
		Device*        device = JNI_JLONG_TO_DEVICE(jDevPtr);

		if (!devMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceMessage reference is null");
		}
		else if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			devMsg->addDevice(*device);
		}
	}
	JNI_CATCH
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceMessage_1GetNumDevices
  (JNIEnv *jenv, jclass jcls, jlong jDevMsgPtr, jobject jDevMsg)
{
	jint jNum = 0;

	try
	{
		DeviceMessage* devMsg = JNI_JLONG_TO_DEVICE_MSG(jDevMsgPtr);

		if (!devMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceMessage reference is null");
		}
		else
		{
			jNum = (jint) devMsg->getNumDevices();
		}
	}
	JNI_CATCH

	return jNum;
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceMessage_1GetDevice
  (JNIEnv *jenv, jclass jcls, jlong jDevMsgPtr, jobject jDevMsg, jint jIndex)
{
	jlong jDevice = 0;

	try
	{
		DeviceMessage* devMsg = JNI_JLONG_TO_DEVICE_MSG(jDevMsgPtr);

		if (!devMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceMessage reference is null");
		}
		else
		{
			const Device& device = devMsg->getDevice((size_t) jIndex);

			jDevice = JNI_POINTER_TO_JLONG(new Device(device));
		}
	}
	JNI_CATCH

	return jDevice;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceMessage_1GetDeviceIterator
  (JNIEnv *jenv, jclass jcls, jlong jDevMsgPtr, jobject jDevMsg)
{
	jlong jIterator = 0;

	try
	{
		DeviceMessage* devMsg = JNI_JLONG_TO_DEVICE_MSG(jDevMsgPtr);

		if (!devMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceMessage reference is null");
		}
		else
		{
			DeviceIterator& iter = devMsg->getDeviceIterator();

			jIterator = JNI_POINTER_TO_JLONG(&iter);
		}
	}
	JNI_CATCH

	return jIterator;
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceMessage_1ConvertMessage
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jlong jCreated = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			DeviceMessage devMsg = DeviceMessage::convertMessage(*msg);

			jCreated = JNI_POINTER_TO_JLONG(new DeviceMessage(devMsg));
		}
	}
	JNI_CATCH

	return jCreated;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


#ifdef __cplusplus
}
#endif
