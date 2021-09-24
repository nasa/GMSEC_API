/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef GMSEC_JNI_CALLBACK_H
#define GMSEC_JNI_CALLBACK_H

#include <gmsec/Callback.h>
#include <gmsec/ErrorCallback.h>
#include <gmsec/ReplyCallback.h>

#include <gmsec/mist/ConnectionManagerCallback.h>
#include <gmsec/mist/ConnectionManagerErrorCallback.h>
#include <gmsec/mist/ConnectionManagerReplyCallback.h>

#include <gmsec/util/Log.h>

#include <jni.h>


class JNIMistCallback : public gmsec::mist::ConnectionManagerCallback
{
private:
    jobject myConnMgr;
    jobject mySubject;
    jobject myCallback;

public:
    JNIMistCallback(JNIEnv* env, jobject jConnection, jstring jSubject, jobject jCallback);

    virtual ~JNIMistCallback();

    jobject CALL_TYPE getConnMgr() const;

    jobject CALL_TYPE getSubject() const;

    jobject CALL_TYPE getCallback() const;

    void CALL_TYPE OnMessage(gmsec::mist::ConnectionManager* connMgr, gmsec::Message* message);
};


class JNIMistErrorCallback : public gmsec::mist::ConnectionManagerErrorCallback
{
private:
    jobject myConnMgr;
    jobject myCallback;

public:
    JNIMistErrorCallback(JNIEnv* env, jobject jConnection, jobject jCallback);

    virtual ~JNIMistErrorCallback();

    jobject CALL_TYPE getConnMgr() const;

    jobject CALL_TYPE getCallback() const;

    void CALL_TYPE OnError(gmsec::mist::ConnectionManager* connMgr, gmsec::Message* message, gmsec::Status* status, const char* event);
};


class JNIMistReplyCallback : public gmsec::mist::ConnectionManagerReplyCallback
{
private:
    jobject myConnMgr;
    jobject myCallback;

public:
    JNIMistReplyCallback(JNIEnv* env, jobject jConnection, jobject jCallback);

    virtual ~JNIMistReplyCallback();

    jobject CALL_TYPE getConnMgr() const;

    jobject CALL_TYPE getCallback() const;

    void CALL_TYPE OnError(gmsec::mist::ConnectionManager* connMgr, gmsec::Message* message, gmsec::Status* status, const char* unused);

    void CALL_TYPE OnReply(gmsec::mist::ConnectionManager* connMgr, gmsec::Message* request, gmsec::Message* reply);
};


class JNICallback : public gmsec::Callback
{
private:
	jobject myConnection;
	jobject mySubject;
	jobject myCallback;

public:
	JNICallback(JNIEnv* env, jobject jConnection, jobject jCallback);

	JNICallback(JNIEnv* env, jobject jConnection, jstring jSubject, jobject jCallback);

	virtual ~JNICallback();

	jobject CALL_TYPE getConnection() const;

	jobject CALL_TYPE getSubject() const;

	jobject CALL_TYPE getCallback() const;

	void CALL_TYPE OnMessage(gmsec::Connection* connection, gmsec::Message* message);
};


class JNIReplyCallback : public gmsec::ReplyCallback
{
public:
	JNIReplyCallback(JNIEnv* env, jobject jCallback);

	void CALL_TYPE OnError(gmsec::Connection* connection, gmsec::Message* message, gmsec::Status* status, const char* event);

	void CALL_TYPE OnReply(gmsec::Connection* connection, gmsec::Message* request, gmsec::Message* reply);
};


class JNIErrorCallback : public gmsec::ErrorCallback
{
public:
	//DMW TODO: Dead code??
	JNIErrorCallback();

	JNIErrorCallback(JNIEnv* env, jobject jCallback);

	virtual void CALL_TYPE OnError(gmsec::Connection* connection, gmsec::Message* message, gmsec::Status* status, const char* event);
};


class JNILogHandler : public gmsec::util::LogHandler
{
public:
	JNILogHandler(JNIEnv* env, jobject jLogHandler);

	void CALL_TYPE OnMessage(const gmsec::util::LogEntry& entry);
};

#endif
