/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_JNI_LOG_HANDLER_H
#define GMSEC_JNI_LOG_HANDLER_H


#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/util/LogHandler.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace jni
{

class CxxLogHandlerProxy : public gmsec::api5::util::LogHandler
{
public:
	CxxLogHandlerProxy(JNIEnv* env, jobject javaLogHandler);

	virtual ~CxxLogHandlerProxy();

	void CALL_TYPE onMessage(const gmsec::api5::util::LogEntry& entry);

private:
	jobject jLogHandler;
};

}  // end namespace jni
}  // end namespace api5
}  // end namespace gmsec


#endif
