/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// F64Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/field/F64Field.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1F64Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jdouble jData, jboolean jIsHeader)
{
	F64Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "F64Field") && name.c_str())
		{
			created = new F64Field(name.c_str(), static_cast<GMSEC_F64>(jData), jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1F64Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jF64FieldPtr, jobject jF64Field)
{
	F64Field* field = JNI_JLONG_TO_F64FIELD(jF64FieldPtr);

	F64Field* created = new F64Field(*field);

	FieldBuddy::getInternal(*created).isTracking(field->isTracking());

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_F64Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jF64FieldPtr, jobject jF64Field)
{
	F64Field* field = JNI_JLONG_TO_F64FIELD(jF64FieldPtr);

	jdouble result = field->getValue();

	return result;
}


#ifdef __cplusplus
}
#endif
