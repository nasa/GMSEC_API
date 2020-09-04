/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_LogHandler.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api::util;
using namespace gmsec::api::jni;


CxxLogHandlerProxy::CxxLogHandlerProxy(JNIEnv* env, jobject javaLogHandler)
	: jLogHandler(env->NewGlobalRef(javaLogHandler))
{
}

CxxLogHandlerProxy::~CxxLogHandlerProxy()
{
	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	jenv->DeleteGlobalRef(jLogHandler);
}

void CxxLogHandlerProxy::onMessage(const LogEntry& entry)
{
	if (!jLogHandler)
	{
		GMSEC_ERROR << "Unable to locate JNI Log Handler.";
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
	default:
		// we should never be here; if so, then just return.
		return;
	}

	AutoJEnv aje;
	JNIEnv*  jenv = aje.getEnv();

	jmethodID callbackMethod = Cache::getCache().methodLogHandlerOnMessage;
	jobject   levelObj       = jenv->GetStaticObjectField(Cache::getCache().classLogLevel, levelField);
	jobject   timeSpecObj    = jenv->NewObject(Cache::getCache().classTimeSpec, Cache::getCache().methodTimeSpecInitJZ);
	jstring   fileStr        = jenv->NewStringUTF(entry.file);
	jint      lineNumber     = (jint) entry.line;
	jstring   msgStr         = jenv->NewStringUTF(entry.message);
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
