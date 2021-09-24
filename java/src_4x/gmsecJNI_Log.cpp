/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Log class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

using namespace gmsec::api::jni;
using namespace gmsec::api::util;


#ifdef __cplusplus
extern "C"
{
#endif


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Log_1SetReportingLevel
  (JNIEnv *jenv, jclass jcls, jint level)
{
	Log::setReportingLevel((LogLevel) level);
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Log_1GetReportingLevel
  (JNIEnv *jenv, jclass jcls)
{
	return Log::getReportingLevel();
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Log_1RegisterHandler__J
  (JNIEnv *jenv, jclass jcls, jlong jLogHandlerPtr)
{
	LogHandler* logHandler = reinterpret_cast<LogHandler*>(jLogHandlerPtr);

	if (logHandler == NULL)
	{
		GMSEC_ERROR << "LogHandler reference is null";
		return;
	}

	Log::registerHandler(logHandler);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Log_1RegisterHandler__IJ
  (JNIEnv *jenv, jclass jcls, jint jLevel, jlong jLogHandlerPtr)
{
	LogHandler* logHandler = reinterpret_cast<LogHandler*>(jLogHandlerPtr);

	if (logHandler == NULL)
	{
		GMSEC_ERROR << "LogHandler reference is null";
		return;
	}

	Log::registerHandler((LogLevel) jLevel, logHandler);
}



JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Log_1LogLevelToString
  (JNIEnv *jenv, jclass jcls, jint jLevel)
{
	jstring s = makeJavaString(jenv, Log::toString((LogLevel) jLevel));
	jvmOk(jenv, "LogLevelToString");
	return s;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Log_1LogLevelFromString
  (JNIEnv *jenv, jclass jcls, jstring jLevel)
{
	JStringManager text(jenv, jLevel);
	jvmOk(jenv, "LogLevelFromString");
	return Log::fromString(text.c_str());
}


#ifdef __cplusplus
}
#endif
