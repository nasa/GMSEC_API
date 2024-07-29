/*
 * Copyright 2007-2024 United States Government as represented by the
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
	MessageFieldIterator* iter = JNI_JLONG_TO_MSG_FIELD_ITER(jMsgIterPtr);

	jboolean result = (iter->hasNext() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFieldIterator_1Next
  (JNIEnv *jenv, jclass jcls, jlong jMsgIterPtr, jobject jMsgIter)
{
	MessageFieldIterator* iter = JNI_JLONG_TO_MSG_FIELD_ITER(jMsgIterPtr);

	jlong result = 0;

	try
	{
		const Field& field = iter->next();

		result = JNI_POINTER_TO_JLONG(&field);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFieldIterator_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jMsgIterPtr, jobject jMsgIter)
{
	MessageFieldIterator* iter = JNI_JLONG_TO_MSG_FIELD_ITER(jMsgIterPtr);

	iter->reset();
}


#ifdef __cplusplus
}
#endif
