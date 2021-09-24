/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef GMSEC_JNI_CACHE_H
#define GMSEC_JNI_CACHE_H

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"

#include <gmsec/util/Mutex.h>

#include <map>
#include <vector>


// Forward declarations
namespace gmsec
{
	class Callback;
	class Connection;
	class ErrorCallback;
	class ReplyCallback;

	namespace mist
	{
		class ConnectionManager;
		class ConnectionManagerCallback;
		class ConnectionManagerErrorCallback;
		class ConnectionManagerReplyCallback;
	};

	namespace util
	{
		class LogHandler;
	};
};


class Cache
{
private:
	bool initialized;

	gmsec::util::Mutex mutex;

	std::vector<jobject>                           globalReferences;
	std::map<gmsec::Connection*, jobject>          connections;
	std::map<gmsec::Callback*, jobject>            callbacks;
	std::map<gmsec::util::LogHandler*, jobject>    logHandlers;
	std::map<gmsec::ReplyCallback*, jobject>       replyCallbacks;
	std::map<gmsec::ErrorCallback*, jobject>       errorCallbacks;

	std::map<gmsec::mist::ConnectionManagerCallback*, jobject>       mistCallbacks;
	std::map<gmsec::mist::ConnectionManagerErrorCallback*, jobject>  mistErrorCallbacks;
	std::map<gmsec::mist::ConnectionManagerReplyCallback*, jobject>  mistReplyCallbacks;

	Cache() {}

	// Not implemented;
	Cache(const Cache& other);
	Cache& operator=(const Cache& other);

public:
	static Cache& getCache();

	void initialize(JNIEnv* jenv);

	void addGlobalReference(jobject o);

	void putConnection(gmsec::Connection* conn, jobject o);

	jobject getConnection(gmsec::Connection* connection);

	void putCallback(gmsec::Callback* callback, jobject o);

	jobject getCallback(gmsec::Callback* callback);

	gmsec::Callback* getAndRemoveCallback(JNIEnv* jenv, jstring subject, jobject userCallback);

	void destroyCallbacks(JNIEnv* jenv, jobject jConnection);

	void putLogHandler(gmsec::util::LogHandler* handler, jobject o);

	jobject getLogHandler(gmsec::util::LogHandler* handler);

	void putReplyCallback(gmsec::ReplyCallback* callback, jobject o);

	jobject getReplyCallback(gmsec::ReplyCallback* callback);

	void putErrorCallback(gmsec::ErrorCallback* callback, jobject o);

	jobject getErrorCallback(gmsec::ErrorCallback* callback);


	// For ConnectionManagerCallbacks
	//
	void putConnMgrCallback(gmsec::mist::ConnectionManagerCallback* callback, jobject o);

	jobject getConnMgrCallback(gmsec::mist::ConnectionManagerCallback* callback);

	gmsec::mist::ConnectionManagerCallback* getAndRemoveConnMgrCallback(JNIEnv* jenv, jstring subject, jobject userCallback);

	void destroyConnMgrCallbacks(JNIEnv* jenv, jobject jConnection);


	// For ConnectionManagerErrorCallbacks
	//
	void putConnMgrErrorCallback(gmsec::mist::ConnectionManagerErrorCallback* callback, jobject o);

	jobject getConnMgrErrorCallback(gmsec::mist::ConnectionManagerErrorCallback* callback);


	// For ConnectionManagerReplyCallbacks
	//
	void putConnMgrReplyCallback(gmsec::mist::ConnectionManagerReplyCallback* callback, jobject o);

	jobject getConnMgrReplyCallback(gmsec::mist::ConnectionManagerReplyCallback* callback);


	// Only initialize should change these!  I am too lazy to write getters
	// for each of them.

	// JNIConnection references
	jclass classJNIConnection;
	jfieldID fieldConnection_extconn;
	jfieldID fieldConnection_swigCPtr;
	jfieldID fieldConnection_swigCMemOwn;

	// JNIStatus references
	jclass classJNIStatus;
	jmethodID methodStatusInitIJString;
	jmethodID methodStatusSetCustomCode;

	// JNIMessage references
	jclass classJNIMessage;
	jmethodID methodMessageInitJZ;
	jfieldID fieldMessage_swigCPtr;
	jfieldID fieldMessage_swigCMemOwn;

	// LogHandler references
	jmethodID methodLogHandlerOnMessage;
	jclass classLogEntry;
	jmethodID methodLogEntryInitJZ;
	jfieldID fieldLogLevel_value;
	jfieldID fieldLogTime_value;
	jfieldID fieldLogMessage_value;

	// Message references
	jclass classMessage;
	jmethodID methodMessageInit;
	jmethodID methodMessageSetImpl;

	// Callback methods
	jmethodID methodCallbackOnMessage;
	jmethodID methodReplyCallbackOnReply;
	jmethodID methodErrorCallbackOnError;

	// MIST Callback methods
	jmethodID methodConnMgrCallbackOnMessage;
	jmethodID methodConnMgrCallbackOnReply;
	jmethodID methodConnMgrCallbackOnError;

	// GMSEC_String
	jclass classGMSEC_String;
	jfieldID fieldString_value;

	// GMSEC_Bin
	jclass classGMSEC_Bin;
	jfieldID fieldBin_value;

	// GMSEC_I64
	jclass classGMSEC_I64;
	jfieldID fieldI64_value;

	// GMSEC_I32
	jclass classGMSEC_I32;
	jfieldID fieldI32_value;

	// GMSEC_U32
	jclass classGMSEC_U32;
	jfieldID fieldU32_value;

	// GMSEC_I16
	jclass classGMSEC_I16;
	jfieldID fieldI16_value;

	// GMSEC_U16
	jclass classGMSEC_U16;
	jfieldID fieldU16_value;

	// GMSEC_F32
	jclass classGMSEC_F32;
	jfieldID fieldF32_value;

	// GMSEC_F64
	jclass classGMSEC_F64;
	jfieldID fieldF64_value;

	// GMSEC_I8
	jclass classGMSEC_I8;
	jfieldID fieldI8_value;

	// GMSEC_U8
	jclass classGMSEC_U8;
	jfieldID fieldU8_value;

	// GMSEC_Char
	jclass classGMSEC_Char;
	jfieldID fieldChar_value;

	// GMSEC_Bool
	jclass classGMSEC_Bool;
	jfieldID fieldBool_value;

	// for detaching thread of JVM version is greater than 1.4
	jboolean enableDetach;

	// JNIConnectionManager references
	jclass classJNIConnectionManager;
	jfieldID fieldConnectionManager_extConnMgr;
	jfieldID fieldConnectionManager_swigCPtr;
	jfieldID fieldConnectionManager_swigCMemOwn;
};

#endif
