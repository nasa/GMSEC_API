/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST ServiceParam class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Fields.h>
#include <gmsec4/mist/ServiceParam.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ServiceParam
  (JNIEnv *jenv, jclass jcls, jstring jName, jlong jFieldPtr, jobject jField)
{
	jlong created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "ServiceParam"))
		{
			Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				created = JNI_POINTER_TO_JLONG(new ServiceParam(name.c_str(), *field));
			}
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ServiceParam_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jServiceParamPtr, jobject jServiceParam)
{
	jlong created = 0;

	try
	{
		ServiceParam* param = JNI_JLONG_TO_SERVICE_PARAM(jServiceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ServiceParam reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new ServiceParam(*param));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ServiceParam
  (JNIEnv *jenv, jclass jcls, jlong jServiceParamPtr, jobject jServiceParam)
{
	try
	{
		ServiceParam* param = JNI_JLONG_TO_SERVICE_PARAM(jServiceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ServiceParam reference is null");
		}
		else
		{
			delete param;
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ServiceParam_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jServiceParamPtr, jobject jServiceParam)
{
	jstring jName = 0;

	try
	{
		ServiceParam* param = JNI_JLONG_TO_SERVICE_PARAM(jServiceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ServiceParam reference is null");
		}
		else
		{
			const char* name = param->getName();

			jName = makeJavaString(jenv, name);

			jvmOk(jenv, "ServiceParam getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ServiceParam_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jServiceParamPtr, jobject jServiceParam)
{
	jlong jField = 0;

	try
	{
		ServiceParam* param = JNI_JLONG_TO_SERVICE_PARAM(jServiceParamPtr);

		if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ServiceParam reference is null");
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
