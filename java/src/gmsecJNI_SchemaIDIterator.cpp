/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// SchemaIDIterator class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/SchemaIDIterator.h>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SchemaIDIterator_1HasNext
  (JNIEnv *jenv, jclass jcls, jlong jSchemaIDIterPtr, jobject jSchemaIDIter)
{
	SchemaIDIterator* iter = JNI_JLONG_TO_SCHEMA_ID_ITER(jSchemaIDIterPtr);

	jboolean result = (iter->hasNext() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SchemaIDIterator_1Next
  (JNIEnv *jenv, jclass jcls, jlong jSchemaIDIterPtr, jobject jSchemaIDIter)
{
	SchemaIDIterator* iter = JNI_JLONG_TO_SCHEMA_ID_ITER(jSchemaIDIterPtr);

	jstring id = 0;

	try
	{
		const char* tmp = iter->next();
		id = makeJavaString(jenv, tmp);
		jvmOk(jenv, "SchemaIDIterator.next");
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return id;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SchemaIDIterator_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jSchemaIDIterPtr, jobject jSchemaIDIter)
{
	SchemaIDIterator* iter = JNI_JLONG_TO_SCHEMA_ID_ITER(jSchemaIDIterPtr);

	iter->reset();
}


#ifdef __cplusplus
}
#endif
