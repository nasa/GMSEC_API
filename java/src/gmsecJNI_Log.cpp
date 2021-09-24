/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// Log class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Callbacks.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec/util/timeutil.h>

using gmsec::util::Log;


#ifdef __cplusplus
extern "C"
{
#endif

	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1SetReportingLevel(JNIEnv *jenv, jclass jcls, jint level)
	{
		Log::SetReportingLevel((LogLevel)level);
	}


	JNIEXPORT jint JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1GetReportingLevel(JNIEnv *jenv, jclass jcls)
	{
		return Log::GetReportingLevel();
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1RegisterHandler__Lgov_nasa_gsfc_gmsecapi_util_LogHandler_2(JNIEnv *jenv, jclass jcls, jobject jLogHandler )
	{
		JNILogHandler *logHandler = new JNILogHandler(jenv, jLogHandler);
		Log::RegisterHandler(logHandler);
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1RegisterHandler__ILgov_nasa_gsfc_gmsecapi_util_LogHandler_2(JNIEnv *jenv, jclass jcls, jint level, jobject jLogHandler )
	{
		JNILogHandler *logHandler = new JNILogHandler(jenv, jLogHandler);
		Log::RegisterHandler((LogLevel)level, logHandler);
	}


	JNIEXPORT jstring JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1LogLevelToString(JNIEnv *jenv, jclass jcls, jint jLevel )
	{
		jstring s = makeJavaString(jenv, Log::ToString((LogLevel)jLevel));
		jvmOk(jenv, "LogLevelToString");
		return s;
	}


	JNIEXPORT jint JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1LogLevelFromString(JNIEnv *jenv, jclass jcls, jstring jLevel )
	{
		JStringManager text(jenv, jLevel);
		jvmOk(jenv, "LogLevelFromString");
		return Log::FromString(text.c_str());
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1LogError(JNIEnv *jenv, jclass jcls, jstring jMessage )
	{
		JStringManager text(jenv, jMessage);
		LOG_ERROR << "" << text.c_str();
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1LogWarning(JNIEnv *jenv, jclass jcls, jstring jMessage )
	{
		JStringManager text(jenv, jMessage);
		LOG_WARNING << text.c_str();
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1LogInfo(JNIEnv *jenv, jclass jcls, jstring jMessage )
	{
		JStringManager text(jenv, jMessage);
		LOG_INFO << "" <<  text.c_str();
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1LogVerbose(JNIEnv *jenv, jclass jcls, jstring jMessage )
	{
		JStringManager text(jenv, jMessage);
		LOG_VERBOSE << text.c_str();
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Log_1LogDebug(JNIEnv *jenv, jclass jcls, jstring jMessage )
	{
		JStringManager text(jenv, jMessage);
		LOG_DEBUG << "" << text.c_str();
	}


	JNIEXPORT jstring JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_TimeUtil_1FormatTime_1s(JNIEnv *jenv, jclass jcls, jdouble jTime_s )
	{
		char buffer[gmsec::util::TIME_BUFSIZE] = {0};
		gmsec::util::formatTime_s((double)jTime_s, buffer);
		jstring s = makeJavaString(jenv, buffer);
		jvmOk(jenv, "TimeUtil.FormatTime_s");
		return s;
	}

#ifdef __cplusplus
}
#endif
