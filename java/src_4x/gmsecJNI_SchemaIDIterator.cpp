/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// SchemaIDIterator class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/mist/SchemaIDIterator.h>

#include <gmsec4/Exception.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SchemaIDIterator_1HasNext
  (JNIEnv *jenv, jclass jcls, jlong jSchemaIDIterPtr, jobject jSchemaIDIter)
{
	jboolean result = JNI_FALSE;

	try
	{
		SchemaIDIterator* iter = JNI_JLONG_TO_SCHEMA_ID_ITER(jSchemaIDIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SchemaIDIterator reference is null");
		}
		else
		{
			result = (iter->hasNext() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SchemaIDIterator_1Next
  (JNIEnv *jenv, jclass jcls, jlong jSchemaIDIterPtr, jobject jSchemaIDIter)
{
	jstring id = 0;

	try
	{
		SchemaIDIterator* iter = JNI_JLONG_TO_SCHEMA_ID_ITER(jSchemaIDIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SchemaIDIterator reference is null");
		}
		else
		{
			const char* tmp = iter->next();
			id = makeJavaString(jenv, tmp);
			jvmOk(jenv, "SchemaIDIterator.next");
		}
	}
	JNI_CATCH

	return id;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SchemaIDIterator_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jSchemaIDIterPtr, jobject jSchemaIDIter)
{
	try
	{
		SchemaIDIterator* iter = JNI_JLONG_TO_SCHEMA_ID_ITER(jSchemaIDIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SchemaIDIterator reference is null");
		}
		else
		{
			iter->reset();
		}
	}
	JNI_CATCH
}


#ifdef __cplusplus
}
#endif
