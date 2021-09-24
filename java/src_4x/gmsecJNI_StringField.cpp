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

#include <gmsec4/field/StringField.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1StringField
  (JNIEnv *jenv, jclass jcls, jstring jName, jstring jData)
{
	StringField* created = 0;

	try
	{
		JStringManager name(jenv, jName);
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "StringField") && name.c_str() && data.c_str())
		{
			created = new StringField(name.c_str(), data.c_str());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1StringField_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jStringFieldPtr, jobject jStringField)
{
	StringField* created = 0;

	try
	{
		StringField* field = JNI_JLONG_TO_STRINGFIELD(jStringFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "StringField reference is null");
		}
		else
		{
			created = new StringField(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_StringField_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jStringFieldPtr, jobject jStringField)
{
	jstring result;

	try
	{
		StringField* field = JNI_JLONG_TO_STRINGFIELD(jStringFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "StringField reference is null");
		}
		else
		{
			result = makeJavaString(jenv, field->getValue());
		}
	}
	JNI_CATCH

	return result;
}


#ifdef __cplusplus
}
#endif
