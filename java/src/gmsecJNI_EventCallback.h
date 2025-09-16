/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_JNI_EVENT_CALLBACK_H
#define GMSEC_JNI_EVENT_CALLBACK_H


#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/EventCallback.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace jni
{

class CxxEventCallbackProxy : public gmsec::api5::EventCallback
{
public:
	CxxEventCallbackProxy(JNIEnv* env, jobject javaEventCallback);

	virtual ~CxxEventCallbackProxy();

	void CALL_TYPE onEvent(gmsec::api5::Connection& conn, const gmsec::api5::Status& status, gmsec::api5::Connection::Event event);

private:
	jobject jCallback;
};

}  // end namespace jni
}  // end namespace api5
}  // end namespace gmsec


#endif
