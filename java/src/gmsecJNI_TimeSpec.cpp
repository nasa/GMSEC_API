/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// TimeSpec class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5_defs.h>


using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1TimeSpec
  (JNIEnv *jenv, jclass jcls)
{
	GMSEC_TimeSpec* spec = new GMSEC_TimeSpec();

	spec->seconds     = 0L;
	spec->nanoseconds = 0L;

	return JNI_POINTER_TO_JLONG(spec);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1TimeSpec
  (JNIEnv *jenv, jclass jcls, jlong jTimeSpecPtr, jobject jTimeSpec)
{
	GMSEC_TimeSpec* spec = JNI_JLONG_TO_TIMESPEC(jTimeSpecPtr);

	delete spec;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_TimeSpec_1GetSeconds
  (JNIEnv *jenv, jclass jcls, jlong jTimeSpecPtr, jobject jTimeSpec)
{
	GMSEC_TimeSpec* spec = JNI_JLONG_TO_TIMESPEC(jTimeSpecPtr);

	jlong seconds = spec->seconds;

	return seconds;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_TimeSpec_1GetNanoseconds
  (JNIEnv *jenv, jclass jcls, jlong jTimeSpecPtr, jobject jTimeSpec)
{
	GMSEC_TimeSpec* spec = JNI_JLONG_TO_TIMESPEC(jTimeSpecPtr);

	jlong nanos = spec->nanoseconds;

	return nanos;
}


#ifdef __cplusplus
}
#endif
