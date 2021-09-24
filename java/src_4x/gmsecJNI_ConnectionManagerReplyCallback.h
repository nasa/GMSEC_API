/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_JNI_CONNECTION_MANAGER_REPLY_CALLBACK_H
#define GMSEC_JNI_CONNECTION_MANAGER_REPLY_CALLBACK_H


#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/mist/ConnectionManagerReplyCallback.h>


namespace gmsec
{
namespace api
{
namespace jni
{

class CxxConnMgrReplyCallbackProxy : public gmsec::api::mist::ConnectionManagerReplyCallback
{
public:
	CxxConnMgrReplyCallbackProxy(JNIEnv* env, jobject javaCallback);

	virtual ~CxxConnMgrReplyCallbackProxy();

	void CALL_TYPE onEvent(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);

	void CALL_TYPE onReply(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& request, const gmsec::api::Message& reply);

private:
	jobject jCallback;
};

}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec


#endif
