/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_JNI_CACHE_H
#define GMSEC_JNI_CACHE_H

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"

#include <gmsec5/Connection.h>
#include <gmsec5/util/Mutex.h>

#include <map>
#include <vector>


// Forward declarations
namespace gmsec
{
	namespace api5
	{
	class Callback;
	class EventCallback;
	class MessageValidator;
	class ReplyCallback;
	}
}


namespace gmsec
{
namespace api5
{
namespace jni
{


class Cache
{
private:
	bool initialized;

	gmsec::api5::util::Mutex mutex;

	std::vector<jobject>                        globalReferences;
	std::map<gmsec::api5::Connection*, jobject> connections;

	Cache() {}

	// Not implemented;
	Cache(const Cache& other);
	Cache& operator=(const Cache& other);

public:
	static Cache& getCache();

	void initialize(JNIEnv* jenv);

	jobject addGlobalReference(JNIEnv* jenv, jobject o);

	void putConnection(gmsec::api5::Connection* conn, jobject jConnection);

	void removeConnection(gmsec::api5::Connection* conn);

	jobject getConnection(gmsec::api5::Connection* connection);


	// Only initialize should change these!  I am too lazy to write getters
	// for each of them.

	// Callback method and field references
	jmethodID methodCallbackOnMessage;
	jfieldID  fieldCallbackJNIConnection;

	// EventCallback method references
	jmethodID methodEventCallbackOnEvent;
	jfieldID  fieldEventCallbackJNIConnection;

	// ReplyCallback method references
	jmethodID methodReplyCallbackOnReply;
	jfieldID  fieldReplyCallbackJNIConnection;

	// MessageValidator method references
	jmethodID methodMessageValidatorValidateMessage;

	// JNIConnection references
	jclass classJNIConnection;
	jfieldID fieldConnection_extconn;
	jfieldID fieldConnection_swigCPtr;
	jfieldID fieldConnection_swigCMemOwn;

	// JNIStatus references
	jclass classJNIStatus;
	jmethodID methodStatusInitIJString;

	// Status references
	jclass classStatus;
	jmethodID methodStatusGetClass;
	jmethodID methodStatusGetCode;
	jmethodID methodStatusGetReason;
	jmethodID methodStatusGetCustomCode;

	// JNIMessage references
	jclass classJNIMessage;
	jmethodID methodMessageInitJZ;
	jfieldID fieldMessage_swigCPtr;
	jfieldID fieldMessage_swigCMemOwn;

	// LogHandler references
	jmethodID methodLogHandlerOnMessage;

	// LogEntry references
	jclass classLogEntry;
	jmethodID methodLogEntryInitJZ;
	jfieldID fieldLogLevel_value;
	jfieldID fieldLogTime_value;
	jfieldID fieldLogFile_value;
	jfieldID fieldLogLine_value;
	jfieldID fieldLogMessage_value;

	// LogLevel references
	jclass classLogLevel;
	jfieldID fieldLogLevelNONE;
	jfieldID fieldLogLevelERROR;
	jfieldID fieldLogLevelSECURE;
	jfieldID fieldLogLevelWARNING;
	jfieldID fieldLogLevelINFO;
	jfieldID fieldLogLevelVERBOSE;
	jfieldID fieldLogLevelDEBUG;

	// Message references
	jclass classMessage;
	jmethodID methodMessageInit;

	// TimeSpec references
	jclass classTimeSpec;
	jmethodID methodTimeSpecInitJZ;
	jfieldID fieldSeconds_value;
	jfieldID fieldNanoseconds_value;

	// U8 references
	jclass classU8;
	jmethodID methodU8Init;

	// U16 references
	jclass classU16;
	jmethodID methodU16Init;

	// U32 references
	jclass classU32;
	jmethodID methodU32Init;

	// U64 references
	jclass classU64;
	jmethodID methodU64Init;

	// Connection Event references
	std::map<gmsec::api5::Connection::Event, jobject> connectionEvents;

	// for detaching thread of JVM version is greater than 1.4
	jboolean enableDetach;
};


}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec

#endif
