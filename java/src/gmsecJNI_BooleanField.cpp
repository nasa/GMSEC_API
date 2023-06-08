/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// BooleanField class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/field/BooleanField.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Errors.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1BooleanField
  (JNIEnv *jenv, jclass jcls, jstring jName, jboolean jData, jboolean jIsHeader)
{
	BooleanField* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "BooleanField") && name.c_str())
		{
			created = new BooleanField(name.c_str(), jData == JNI_TRUE, jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1BooleanField_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jBooleanFieldPtr, jobject jBooleanField)
{
	BooleanField* created = 0;

	try
	{
		BooleanField* field = JNI_JLONG_TO_BOOLEANFIELD(jBooleanFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "BooleanField reference is null");
		}
		else
		{
			created = new BooleanField(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_BooleanField_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jBooleanFieldPtr, jobject jBooleanField)
{
	jboolean result = JNI_FALSE;

	try
	{
		BooleanField* field = JNI_JLONG_TO_BOOLEANFIELD(jBooleanFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "BooleanField reference is null");
		}
		else
		{
			result = (field->getValue() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


#ifdef __cplusplus
}
#endif
