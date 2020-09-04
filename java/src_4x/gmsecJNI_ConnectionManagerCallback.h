/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_JNI_CONNECTION_MANAGER_CALLBACK_H
#define GMSEC_JNI_CONNECTION_MANAGER_CALLBACK_H


#include "gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/mist/ConnectionManagerCallback.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace jni
{

class CxxConnMgrCallbackProxy : public gmsec::api::mist::ConnectionManagerCallback
{
public:
	CxxConnMgrCallbackProxy(JNIEnv* env, jobject javaCallback);

	virtual ~CxxConnMgrCallbackProxy();

	void CALL_TYPE onMessage(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& msg);

private:
	jobject jCallback;
};

}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec


#endif
