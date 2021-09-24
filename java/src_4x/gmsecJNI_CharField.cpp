/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/field/CharField.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1CharField
  (JNIEnv *jenv, jclass jcls, jstring jName, jchar jData)
{
	CharField* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "CharField") && name.c_str())
		{
			created = new CharField(name.c_str(), (char) jData);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1CharField_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jCharFieldPtr, jobject jCharField)
{
	CharField* created = 0;

	try
	{
		CharField* field = JNI_JLONG_TO_CHARFIELD(jCharFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "CharField reference is null");
		}
		else
		{
			created = new CharField(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jchar JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_CharField_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jCharFieldPtr, jobject jCharField)
{
	jchar result = (jchar) 0;

	try
	{
		CharField* field = JNI_JLONG_TO_CHARFIELD(jCharFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "CharField reference is null");
		}
		else
		{
			result = field->getValue();
		}
	}
	JNI_CATCH

	return result;
}


#ifdef __cplusplus
}
#endif
