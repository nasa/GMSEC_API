/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_JNI_REPLY_CALLBACK_H
#define GMSEC_JNI_REPLY_CALLBACK_H

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/ReplyCallback.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace jni
{

class CxxReplyCallbackProxy : public gmsec::api::ReplyCallback
{
public:
	CxxReplyCallbackProxy(JNIEnv* env, jobject javaReplyCallback);

	virtual ~CxxReplyCallbackProxy();

	void CALL_TYPE onEvent(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);

	void CALL_TYPE onReply(gmsec::api::Connection& conn, const gmsec::api::Message& request, const gmsec::api::Message& reply);

private:
	jobject jCallback;
};

}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec


#endif
