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

#include <gmsec4/field/F64Field.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1F64Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jdouble jData)
{
	F64Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "F64Field") && name.c_str())
		{
			created = new F64Field(name.c_str(), (GMSEC_F64) jData);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1F64Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jF64FieldPtr, jobject jF64Field)
{
	F64Field* created = 0;

	try
	{
		F64Field* field = JNI_JLONG_TO_F64FIELD(jF64FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "F64Field reference is null");
		}
		else
		{
			created = new F64Field(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_F64Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jF64FieldPtr, jobject jF64Field)
{
	jdouble result = 0;

	try
	{
		F64Field* field = JNI_JLONG_TO_F64FIELD(jF64FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "F64Field reference is null");
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
