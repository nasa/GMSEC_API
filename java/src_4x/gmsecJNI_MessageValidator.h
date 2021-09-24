/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_JNI_MESSAGE_VALIDATOR_H
#define GMSEC_JNI_MESSAGE_VALIDATOR_H


#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/mist/MessageValidator.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace jni
{

class CxxMessageValidatorProxy : public gmsec::api::mist::MessageValidator
{
public:
	CxxMessageValidatorProxy(JNIEnv* env, jobject javaMessageValidator);

	virtual ~CxxMessageValidatorProxy();

	gmsec::api::Status CALL_TYPE validateMessage(const gmsec::api::Message& msg);

private:
	jobject jValidator;
};

}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec


#endif
