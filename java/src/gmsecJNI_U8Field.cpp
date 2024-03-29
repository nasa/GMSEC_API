/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// U8Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/field/U8Field.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1U8Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jint jData, jboolean jIsHeader)
{
	U8Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "U8Field") && name.c_str())
		{
			created = new U8Field(name.c_str(), static_cast<GMSEC_U8>(jData), jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1U8Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jU8FieldPtr, jobject jU8Field)
{
	U8Field* created = 0;

	try
	{
		U8Field* field = JNI_JLONG_TO_U8FIELD(jU8FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "U8Field reference is null");
		}
		else
		{
			created = new U8Field(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_U8Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jU8FieldPtr, jobject jU8Field)
{
	jint result = 0;

	try
	{
		U8Field* field = JNI_JLONG_TO_U8FIELD(jU8FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "U8Field reference is null");
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
