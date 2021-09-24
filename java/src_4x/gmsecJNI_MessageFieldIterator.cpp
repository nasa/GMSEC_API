/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/MessageFieldIterator.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MessageFieldIterator_1HasNext
  (JNIEnv *jenv, jclass jcls, jlong jMsgIterPtr, jobject jMsgIter)
{
	jboolean result = JNI_FALSE;

	try
	{
		MessageFieldIterator* iter = JNI_JLONG_TO_MSG_FIELD_ITER(jMsgIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFieldIterator reference is null");
		}
		else
		{
			result = (iter->hasNext() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MessageFieldIterator_1Next
  (JNIEnv *jenv, jclass jcls, jlong jMsgIterPtr, jobject jMsgIter)
{
	jlong result = 0;

	try
	{
		MessageFieldIterator* iter = JNI_JLONG_TO_MSG_FIELD_ITER(jMsgIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFieldIterator reference is null");
		}
		else
		{
			try
			{
				const Field& field = iter->next();

				result = JNI_POINTER_TO_JLONG(&field);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MessageFieldIterator_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jMsgIterPtr, jobject jMsgIter)
{
	try
	{
		MessageFieldIterator* iter = JNI_JLONG_TO_MSG_FIELD_ITER(jMsgIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFieldIterator reference is null");
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
