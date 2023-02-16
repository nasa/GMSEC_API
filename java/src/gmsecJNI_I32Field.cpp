/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// I32Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/field/I32Field.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1I32Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jint jData, jboolean jIsHeader)
{
	I32Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "I32Field") && name.c_str())
		{
			created = new I32Field(name.c_str(), static_cast<GMSEC_I32>(jData), jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1I32Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jI32FieldPtr, jobject jI32Field)
{
	I32Field* created = 0;

	try
	{
		I32Field* field = JNI_JLONG_TO_I32FIELD(jI32FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "I32Field reference is null");
		}
		else
		{
			created = new I32Field(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_I32Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jI32FieldPtr, jobject jI32Field)
{
	jint result = 0;

	try
	{
		I32Field* field = JNI_JLONG_TO_I32FIELD(jI32FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "I32Field reference is null");
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
