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
#include <gmsec4/util/TimeUtil.h>


using namespace gmsec::api::jni;
using namespace gmsec::api::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_TimeUtil_1GetCurrentTime
  (JNIEnv *jenv, jclass jcls)
{
	GMSEC_TimeSpec  curTime = TimeUtil::getCurrentTime();
	GMSEC_TimeSpec* spec    = new GMSEC_TimeSpec();

	spec->seconds     = curTime.seconds;
	spec->nanoseconds = curTime.nanoseconds;

	return JNI_POINTER_TO_JLONG(spec);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_TimeUtil_1FormatTime
  (JNIEnv *jenv, jclass jcls, jlong jSeconds, jlong jNanoseconds, jint jSubseconds)
{
	GMSEC_TimeSpec spec = { (long) jSeconds, (long) jNanoseconds };
	int            subs = (int) jSubseconds;
	char*          buf  = new char[GMSEC_TIME_BUFSIZE + subs];

	TimeUtil::formatTime(spec, subs, buf);

	jstring theTime = makeJavaString(jenv, buf);

	delete [] buf;

	return theTime;
}


#ifdef __cplusplus
}
#endif
