/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// F32Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/field/F32Field.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1F32Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jfloat jData, jboolean jIsHeader)
{
	F32Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "F32Field") && name.c_str())
		{
			created = new F32Field(name.c_str(), static_cast<GMSEC_F32>(jData), jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1F32Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jF32FieldPtr, jobject jF32Field)
{
	F32Field* created = 0;

	try
	{
		F32Field* field = JNI_JLONG_TO_F32FIELD(jF32FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "F32Field reference is null");
		}
		else
		{
			created = new F32Field(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jfloat JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_F32Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jF32FieldPtr, jobject jF32Field)
{
	jfloat result = 0;

	try
	{
		F32Field* field = JNI_JLONG_TO_F32FIELD(jF32FieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "F32Field reference is null");
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
