/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_JNI_REPLY_CALLBACK_H
#define GMSEC_JNI_REPLY_CALLBACK_H

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/ReplyCallback.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace jni
{

class CxxReplyCallbackProxy : public gmsec::api5::ReplyCallback
{
public:
	CxxReplyCallbackProxy(JNIEnv* env, jobject javaReplyCallback);

	virtual ~CxxReplyCallbackProxy();

	void CALL_TYPE onEvent(gmsec::api5::Connection& conn, const gmsec::api5::Status& status, gmsec::api5::Connection::Event event);

	void CALL_TYPE onReply(gmsec::api5::Connection& conn, const gmsec::api5::Message& request, const gmsec::api5::Message& reply);

private:
	jobject jCallback;
};

}  // end namespace jni
}  // end namespace api5
}  // end namespace gmsec


#endif
