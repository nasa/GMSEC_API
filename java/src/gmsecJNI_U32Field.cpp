/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// U32Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/field/U32Field.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1U32Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jlong jData, jboolean jIsHeader)
{
	U32Field* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "U32Field") && name.c_str())
		{
			created = new U32Field(name.c_str(), static_cast<GMSEC_U32>(jData), jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1U32Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jU32FieldPtr, jobject jU32Field)
{
	U32Field* field = JNI_JLONG_TO_U32FIELD(jU32FieldPtr);

	U32Field* created = new U32Field(*field);

	FieldBuddy::getInternal(*created).isTracking(field->isTracking());

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_U32Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jU32FieldPtr, jobject jU32Field)
{
	U32Field* field = JNI_JLONG_TO_U32FIELD(jU32FieldPtr);

	jlong result = field->getValue();

	return result;
}


#ifdef __cplusplus
}
#endif
