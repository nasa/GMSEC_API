/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// U64Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/field/U64Field.h>

#include <sstream>
#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1U64Field
  (JNIEnv *jenv, jclass jcls, jstring jName, jstring jData, jboolean jIsHeader)
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

			created = new U64Field(name.c_str(), value, jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1U64Field_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jU64FieldPtr, jobject jU64Field)
{
	U64Field* field = JNI_JLONG_TO_U64FIELD(jU64FieldPtr);

	U64Field* created = new U64Field(*field);

	FieldBuddy::getInternal(*created).isTracking(field->isTracking());

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_U64Field_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jU64FieldPtr, jobject jU64Field)
{
	U64Field* field = JNI_JLONG_TO_U64FIELD(jU64FieldPtr);

	GMSEC_U64 value = field->getValue();

	std::ostringstream oss;
	oss << value;

	// Note: makeJavaString() can throw an exception if the given value is NULL.
	// Since a string is always passed, there's no need for a try-catch block.
	jstring data = makeJavaString(jenv, oss.str().c_str());

	return data;
}


#ifdef __cplusplus
}
#endif
