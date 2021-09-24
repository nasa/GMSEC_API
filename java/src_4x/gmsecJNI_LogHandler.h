/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_JNI_LOG_HANDLER_H
#define GMSEC_JNI_LOG_HANDLER_H


#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/util/LogHandler.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace jni
{

class CxxLogHandlerProxy : public gmsec::api::util::LogHandler
{
public:
	CxxLogHandlerProxy(JNIEnv* env, jobject javaLogHandler);

	virtual ~CxxLogHandlerProxy();

	void CALL_TYPE onMessage(const gmsec::api::util::LogEntry& entry);

private:
	jobject jLogHandler;
};

}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec


#endif
