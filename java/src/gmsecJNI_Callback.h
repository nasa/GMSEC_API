/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_JNI_CALLBACK_H
#define GMSEC_JNI_CALLBACK_H

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Callback.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	class Connection;
	class Message;

namespace jni
{

class CxxCallbackProxy : public gmsec::api5::Callback
{
public:
	CxxCallbackProxy(JNIEnv* env, jobject javaCallback);

	virtual ~CxxCallbackProxy();

	void CALL_TYPE onMessage(gmsec::api5::Connection& conn, const gmsec::api5::Message& msg);

private:
	jobject jCallback;
};

}  // end namespace jni
}  // end namespace api5
}  // end namespace gmsec

#endif
