/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// TimeUtil class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5_defs.h>
#include <gmsec5/util/TimeUtil.h>


using namespace gmsec::api5::jni;
using namespace gmsec::api5::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_TimeUtil_1Millisleep
  (JNIEnv *jenv, jclass jcls, jint jMillis)
{
	int ms = static_cast<int>(jMillis);

	if (ms < 1)
	{
		ms = 1;
	}

	TimeUtil::millisleep(static_cast<unsigned int>(ms));
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_TimeUtil_1GetCurrentTime
  (JNIEnv *jenv, jclass jcls)
{
	GMSEC_TimeSpec  curTime = TimeUtil::getCurrentTime();
	GMSEC_TimeSpec* spec    = new GMSEC_TimeSpec();

	spec->seconds     = curTime.seconds;
	spec->nanoseconds = curTime.nanoseconds;

	return JNI_POINTER_TO_JLONG(spec);
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_TimeUtil_1GetCurrentTime_1s
  (JNIEnv *jenv, jclass jcls)
{
	return static_cast<jdouble>( TimeUtil::getCurrentTime_s() );
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_TimeUtil_1ConvertTimeString
  (JNIEnv *jenv, jclass jcls, jstring jTimeString)
{
	jlong jTimeSpec = 0;

	try
	{
		JStringManager timeString(jenv, jTimeString);

		if (jvmOk(jenv, "TimeUtil::convertTimeString"))
		{
			GMSEC_TimeSpec  theTime = TimeUtil::convertTimeString(timeString.c_str());
			GMSEC_TimeSpec* spec    = new GMSEC_TimeSpec();

			spec->seconds     = theTime.seconds;
			spec->nanoseconds = theTime.nanoseconds;

			jTimeSpec = JNI_POINTER_TO_JLONG(spec);
		}
	}
	JNI_CATCH

	return jTimeSpec;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_TimeUtil_1FormatTime
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
