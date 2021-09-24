/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST DeviceParam class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Fields.h>
#include <gmsec4/mist/DeviceParam.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1DeviceParam
  (JNIEnv *jenv, jclass jcls, jstring jName, jstring jTimestamp, jlong jFieldPtr, jobject jField)
{
	jlong created = 0;

	try
	{
		JStringManager name(jenv, jName);
		JStringManager timestamp(jenv, jTimestamp);

		if (jvmOk(jenv, "DeviceParam"))
		{
			Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				created = JNI_POINTER_TO_JLONG(new DeviceParam(name.c_str(), timestamp.c_str(), *field));
			}
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1DeviceParam_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jDeviceParamPtr, jobject jDeviceParam)
{
	jlong created = 0;

	try
	{
		DeviceParam* param = JNI_JLONG_TO_DEVICE_PARAM(jDeviceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceParam reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new DeviceParam(*param));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1DeviceParam
  (JNIEnv *jenv, jclass jcls, jlong jDeviceParamPtr, jobject jDeviceParam)
{
	try
	{
		DeviceParam* param = JNI_JLONG_TO_DEVICE_PARAM(jDeviceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceParam reference is null");
		}
		else
		{
			delete param;
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceParam_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jDeviceParamPtr, jobject jDeviceParam)
{
	jstring jName = 0;

	try
	{
		DeviceParam* param = JNI_JLONG_TO_DEVICE_PARAM(jDeviceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceParam reference is null");
		}
		else
		{
			const char* name = param->getName();

			jName = makeJavaString(jenv, name);

			jvmOk(jenv, "DeviceParam getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceParam_1GetTimestamp
  (JNIEnv *jenv, jclass jcls, jlong jDeviceParamPtr, jobject jDeviceParam)
{
	jstring jTimestamp = 0;

	try
	{
		DeviceParam* param = JNI_JLONG_TO_DEVICE_PARAM(jDeviceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceParam reference is null");
		}
		else
		{
			const char* timestamp = param->getTimestamp();

			jTimestamp = makeJavaString(jenv, timestamp);

			jvmOk(jenv, "DeviceParam getName");
		}
	}
	JNI_CATCH

	return jTimestamp;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceParam_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jDeviceParamPtr, jobject jDeviceParam)
{
	jlong jField = 0;

	try
	{
		DeviceParam* param = JNI_JLONG_TO_DEVICE_PARAM(jDeviceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceParam reference is null");
		}
		else
		{
			const Field& field = param->getValue();

			jField = JNI_POINTER_TO_JLONG(&field);
		}
	}
	JNI_CATCH

	return jField;
}


#ifdef __cplusplus
}
#endif
