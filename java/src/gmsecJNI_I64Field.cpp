/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// I64Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/field/I64Field.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1I64Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jlong jData, jboolean jIsHeader)
{
	I64Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "I64Field") && name.c_str())
		{
			created = new I64Field(name.c_str(), static_cast<GMSEC_I64>(jData), jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1I64Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jI64FieldPtr, jobject jI64Field)
{
	I64Field* created = 0;

	try
	{
		I64Field* field = JNI_JLONG_TO_I64FIELD(jI64FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "I64Field reference is null");
		}
		else
		{
			created = new I64Field(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_I64Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jI64FieldPtr, jobject jI64Field)
{
	jlong result = 0;

	try
	{
		I64Field* field = JNI_JLONG_TO_I64FIELD(jI64FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "I64Field reference is null");
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
