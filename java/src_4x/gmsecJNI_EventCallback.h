/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_JNI_EVENT_CALLBACK_H
#define GMSEC_JNI_EVENT_CALLBACK_H


#include "gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/EventCallback.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace jni
{

class CxxEventCallbackProxy : public gmsec::api::EventCallback
{
public:
	CxxEventCallbackProxy(JNIEnv* env, jobject javaEventCallback);

	virtual ~CxxEventCallbackProxy();

	void CALL_TYPE onEvent(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);

private:
	jobject jCallback;
};

}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec


#endif
