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

#include <gmsec4/field/U64Field.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <sstream>
#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1U64Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jstring jData)
{
	U64Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "U64Field") && name.c_str() && data.c_str())
		{
			GMSEC_U64 value;

			std::istringstream iss(data.c_str());
			iss >> value;

			created = new U64Field(name.c_str(), value);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1U64Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jU64FieldPtr, jobject jU64Field)
{
	U64Field* created = 0;

	try
	{
		U64Field* field = JNI_JLONG_TO_U64FIELD(jU64FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "U64Field reference is null");
		}
		else
		{
			created = new U64Field(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_U64Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jU64FieldPtr, jobject jU64Field)
{
	jstring data;

	try
	{
		U64Field* field = JNI_JLONG_TO_U64FIELD(jU64FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "U64Field reference is null");
		}
		else
		{
			GMSEC_U64 value = field->getValue();

			std::ostringstream oss;
			oss << value;

			data = makeJavaString(jenv, oss.str().c_str());
		}
	}
	JNI_CATCH

	return data;
}


#ifdef __cplusplus
}
#endif
