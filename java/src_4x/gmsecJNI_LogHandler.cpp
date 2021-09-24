/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_LogHandler.h"

#include <gmsec4/internal/util/InternalLog.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <iostream>


using namespace gmsec::api::jni;
using namespace gmsec::api::util;


CxxLogHandlerProxy::CxxLogHandlerProxy(JNIEnv* env, jobject javaLogHandler)
	: jLogHandler(env->NewGlobalRef(javaLogHandler))
{
}

CxxLogHandlerProxy::~CxxLogHandlerProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (jenv)
	{
		jenv->DeleteGlobalRef(jLogHandler);
	}
}

void CxxLogHandlerProxy::onMessage(const LogEntry& entry)
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	if (jenv)
	{
		if (!jLogHandler)
		{
			using namespace gmsec::api::internal;

			LogEntry warning = { __FILE__, __LINE__, logWARNING, TimeUtil::getCurrentTime(), "No reference to JNI Log Handler is available" };

			std::cerr << InternalLog::prepareLogMessage(warning) << std::endl;

			return;
		}

		jfieldID levelField = NULL;

		switch (entry.level)
		{
			case logNONE:    levelField = Cache::getCache().fieldLogLevelNONE; break;
			case logERROR:   levelField = Cache::getCache().fieldLogLevelERROR; break;
			case logSECURE:  levelField = Cache::getCache().fieldLogLevelSECURE; break;
			case logWARNING: levelField = Cache::getCache().fieldLogLevelWARNING; break;
			case logINFO:    levelField = Cache::getCache().fieldLogLevelINFO; break;
			case logVERBOSE: levelField = Cache::getCache().fieldLogLevelVERBOSE; break;
			case logDEBUG:   levelField = Cache::getCache().fieldLogLevelDEBUG; break;
			default:         return; // we should never be here; if so, then just return.
		}

		std::string file = (entry.file ? entry.file : "<unknown>");

#ifdef WIN32
		file = file.substr(file.rfind("\\") + 1);
#else
		file = file.substr(file.rfind("/") + 1);
#endif

		jmethodID callbackMethod = Cache::getCache().methodLogHandlerOnMessage;
		jobject   levelObj       = jenv->GetStaticObjectField(Cache::getCache().classLogLevel, levelField);
		jobject   timeSpecObj    = jenv->NewObject(Cache::getCache().classTimeSpec, Cache::getCache().methodTimeSpecInitJZ);
		jstring   fileStr        = makeJavaString(jenv, file.c_str());
		jint      lineNumber     = (jint) entry.line;
		jstring   msgStr         = makeJavaString(jenv, entry.message);
		jobject   logEntryObj    = jenv->NewObject(Cache::getCache().classLogEntry, Cache::getCache().methodLogEntryInitJZ);

		jenv->SetLongField(timeSpecObj, Cache::getCache().fieldSeconds_value,     (jlong) entry.time.seconds);
		jenv->SetLongField(timeSpecObj, Cache::getCache().fieldNanoseconds_value, (jlong) entry.time.nanoseconds);

		jenv->SetObjectField(logEntryObj, Cache::getCache().fieldLogLevel_value,   levelObj);
		jenv->SetObjectField(logEntryObj, Cache::getCache().fieldLogTime_value,    timeSpecObj);
		jenv->SetObjectField(logEntryObj, Cache::getCache().fieldLogFile_value,    fileStr);
		jenv->SetIntField(logEntryObj, Cache::getCache().fieldLogLine_value,       lineNumber);
		jenv->SetObjectField(logEntryObj, Cache::getCache().fieldLogMessage_value, msgStr);

		jenv->CallVoidMethod(jLogHandler, callbackMethod, logEntryObj);

		jvmOk(jenv, "JNILogHandler.onMessage");

		jenv->DeleteLocalRef(logEntryObj);
	}
	else
	{
		// We should never be here; however, when dealing with ActiveMQ CMS, and during
		// the period when a connection failover occurs, something weird happens to the
		// JavaVM which prevents us from attaching to the current running thread (see
		// AutoJEnv constructor), thus not allowing us to reference objects related to
		// logging (e.g. classLogLevel).
		//
		// To mitigate this weird situation, we will prepare the log message and directly
		// display it to standard-error.
		//
		using namespace gmsec::api::internal;

		LogEntry warning = { __FILE__, __LINE__, logWARNING, TimeUtil::getCurrentTime(), "Unable to log to Java log handler" };

		std::cerr << InternalLog::prepareLogMessage(warning) << "\n"
		          << InternalLog::prepareLogMessage(entry)
		          << std::endl;
	}
}


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1LogHandler
  (JNIEnv *jenv, jclass jcls, jobject logHandler)
{
	LogHandler* proxy = 0;

	try
	{
		proxy = new CxxLogHandlerProxy(jenv, logHandler);
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(proxy);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1LogHandler
  (JNIEnv *jenv, jclass jcls, jlong logHandlerPtr, jobject jLogHandler)
{
	CxxLogHandlerProxy* proxy = reinterpret_cast<CxxLogHandlerProxy*>(logHandlerPtr);

	delete proxy;
}


#ifdef __cplusplus
}
#endif
