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

#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/DeviceIterator.h>

#include <gmsec4/Exception.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceIterator_1HasNext
  (JNIEnv *jenv, jclass jcls, jlong jDevIterPtr, jobject jDevIter)
{
	jboolean result = JNI_FALSE;

	try
	{
		DeviceIterator* iter = JNI_JLONG_TO_DEVICE_ITER(jDevIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceIterator reference is null");
		}
		else
		{
			result = (iter->hasNext() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceIterator_1Next
  (JNIEnv *jenv, jclass jcls, jlong jDevIterPtr, jobject jDevIter)
{
	jlong result = 0;

	try
	{
		DeviceIterator* iter = JNI_JLONG_TO_DEVICE_ITER(jDevIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceIterator reference is null");
		}
		else
		{
			try
			{
				const Device& device = iter->next();

				result = JNI_POINTER_TO_JLONG(&device);
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_DeviceIterator_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jDevIterPtr, jobject jDevIter)
{
	try
	{
		DeviceIterator* iter = JNI_JLONG_TO_DEVICE_ITER(jDevIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceIterator reference is null");
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
