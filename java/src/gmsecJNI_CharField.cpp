/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// CharField class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/field/CharField.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Errors.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1CharField
  (JNIEnv *jenv, jclass jcls, jstring jName, jchar jData, jboolean jIsHeader)
{
	CharField* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "CharField") && name.c_str())
		{
			created = new CharField(name.c_str(), static_cast<char>(jData), jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1CharField_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jCharFieldPtr, jobject jCharField)
{
	CharField* field = JNI_JLONG_TO_CHARFIELD(jCharFieldPtr);

	CharField* created = new CharField(*field);

	FieldBuddy::getInternal(*created).isTracking(field->isTracking());

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jchar JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_CharField_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jCharFieldPtr, jobject jCharField)
{
	CharField* field = JNI_JLONG_TO_CHARFIELD(jCharFieldPtr);

	jchar result = static_cast<jchar>(field->getValue());

	return result;
}


#ifdef __cplusplus
}
#endif
