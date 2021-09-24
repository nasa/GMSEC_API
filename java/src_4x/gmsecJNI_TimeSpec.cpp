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

#include <gmsec4_defs.h>


using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1TimeSpec
  (JNIEnv *jenv, jclass jcls)
{
	GMSEC_TimeSpec* spec = new GMSEC_TimeSpec();

	spec->seconds     = 0L;
	spec->nanoseconds = 0L;

	return JNI_POINTER_TO_JLONG(spec);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1TimeSpec
  (JNIEnv *jenv, jclass jcls, jlong jTimeSpecPtr, jobject jTimeSpec)
{
	try
	{
		GMSEC_TimeSpec* spec = JNI_JLONG_TO_TIMESPEC(jTimeSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "TimeSpec reference is null");
		}
		else
		{
			delete spec;
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_TimeSpec_1GetSeconds
  (JNIEnv *jenv, jclass jcls, jlong jTimeSpecPtr, jobject jTimeSpec)
{
	jlong seconds = 0;

	try
	{
		GMSEC_TimeSpec* spec = JNI_JLONG_TO_TIMESPEC(jTimeSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "TimeSpec reference is null");
		}
		else
		{
			seconds = spec->seconds;
		}
	}
	JNI_CATCH

	return seconds;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_TimeSpec_1GetNanoseconds
  (JNIEnv *jenv, jclass jcls, jlong jTimeSpecPtr, jobject jTimeSpec)
{
	jlong nanos = 0;

	try
	{
		GMSEC_TimeSpec* spec = JNI_JLONG_TO_TIMESPEC(jTimeSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "TimeSpec reference is null");
		}
		else
		{
			nanos = spec->nanoseconds;
		}
	}
	JNI_CATCH

	return nanos;
}


#ifdef __cplusplus
}
#endif
