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

#include <gmsec4/field/I16Field.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1I16Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jshort jData)
{
	I16Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "I16Field") && name.c_str())
		{
			created = new I16Field(name.c_str(), (GMSEC_I16) jData);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1I16Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jI16FieldPtr, jobject jI16Field)
{
	I16Field* created = 0;

	try
	{
		I16Field* field = JNI_JLONG_TO_I16FIELD(jI16FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "I16Field reference is null");
		}
		else
		{
			created = new I16Field(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_I16Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jI16FieldPtr, jobject jI16Field)
{
	jshort result = 0;

	try
	{
		I16Field* field = JNI_JLONG_TO_I16FIELD(jI16FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "I16Field reference is null");
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
