/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// BinaryField class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/field/BinaryField.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Errors.h>

#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1BinaryField
  (JNIEnv *jenv, jclass jcls, jstring jName, jbyteArray jData, jlong jDataLength, jboolean jIsHeader)
{
	BinaryField* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "BinaryField") && name.c_str())
		{
			JByteArrayManager byteMgr(jenv, jData);

			created = new BinaryField(name.c_str(),
			                          reinterpret_cast<const GMSEC_U8*>(byteMgr.jbytes()),
			                          static_cast<size_t>(jDataLength),
			                          jIsHeader == JNI_TRUE);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1BinaryField_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jBinaryFieldPtr, jobject jBinaryField)
{
	BinaryField* field = JNI_JLONG_TO_BINARYFIELD(jBinaryFieldPtr);

	BinaryField* created = new BinaryField(*field);

	FieldBuddy::getInternal(*created).isTracking(field->isTracking());

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jbyteArray JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_BinaryField_1GetValue
  (JNIEnv *jenv, jclass jcls, jlong jBinaryFieldPtr, jobject jBinaryField)
{
	BinaryField* field = JNI_JLONG_TO_BINARYFIELD(jBinaryFieldPtr);

	GMSEC_U8 const* value  = field->getValue();
	size_t          length = field->getLength();

	jbyteArray array = jenv->NewByteArray(static_cast<jsize>(length));

	if (array != NULL)
	{
		jenv->SetByteArrayRegion(array, 0, static_cast<jsize>(length), reinterpret_cast<jbyte const*>(value));
	}

	return array;
}


#ifdef __cplusplus
}
#endif
