/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// StringField class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/field/StringField.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1StringField
  (JNIEnv *jenv, jclass jcls, jstring jName, jstring jData, jboolean jIsHeader)
{
	StringField* created = 0;

	try
	{
		JStringManager name(jenv, jName);
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "StringField") && name.c_str() && data.c_str())
		{
			created = new StringField(name.c_str(), data.c_str(), jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1StringField_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jStringFieldPtr, jobject jStringField)
{
	StringField* field = JNI_JLONG_TO_STRINGFIELD(jStringFieldPtr);

	StringField* created = new StringField(*field);

	FieldBuddy::getInternal(*created).isTracking(field->isTracking());

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_StringField_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jStringFieldPtr, jobject jStringField)
{
	StringField* field = JNI_JLONG_TO_STRINGFIELD(jStringFieldPtr);

	// Note: makeJavaString() can throw an exception if the given value is NULL.
	// Since a StringField cannot contain a NULL field value, there's no need for
	// a try-catch block below.
	jstring result = makeJavaString(jenv, field->getValue());

	return result;
}


#ifdef __cplusplus
}
#endif
