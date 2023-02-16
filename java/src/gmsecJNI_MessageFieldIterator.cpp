/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MessageFieldIterator class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/MessageFieldIterator.h>
#include <gmsec5/Fields.h>
#include <gmsec5/Message.h>

#include <gmsec5/util/Log.h>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFieldIterator_1HasNext
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFieldIterator_1Next
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
			const Field& field = iter->next();

			result = JNI_POINTER_TO_JLONG(&field);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFieldIterator_1Reset
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
