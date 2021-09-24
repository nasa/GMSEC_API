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

#include <gmsec4/field/BinaryField.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1BinaryField
  (JNIEnv *jenv, jclass jcls, jstring jName, jbyteArray jData, jlong jDataLength)
{
	BinaryField* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "BinaryField") && name.c_str())
		{
			JByteArrayManager byteMgr(jenv, jData);

			created = new BinaryField(name.c_str(), (GMSEC_BIN) byteMgr.jbytes(), (size_t) jDataLength);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1BinaryField_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jBinaryFieldPtr, jobject jBinaryField)
{
	BinaryField* created = 0;

	try
	{
		BinaryField* field = JNI_JLONG_TO_BINARYFIELD(jBinaryFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "BinaryField reference is null");
		}
		else
		{
			created = new BinaryField(*field);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jbyteArray JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_BinaryField_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jBinaryFieldPtr, jobject jBinaryField)
{
	jbyteArray array = NULL;

	try
	{
		BinaryField* field = JNI_JLONG_TO_BINARYFIELD(jBinaryFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "BinaryField reference is null");
		}
		else
		{
			GMSEC_BIN value  = field->getValue();
			size_t    length = field->getLength();

			array = jenv->NewByteArray((jsize) length);

			jenv->SetByteArrayRegion(array, 0, (jsize) length, (jbyte*) value);
		}
	}
	JNI_CATCH

	return array;
}


#ifdef __cplusplus
}
#endif
