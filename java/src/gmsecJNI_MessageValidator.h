/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_JNI_MESSAGE_VALIDATOR_H
#define GMSEC_JNI_MESSAGE_VALIDATOR_H


#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Message.h>
#include <gmsec5/MessageValidator.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace jni
{

class CxxMessageValidatorProxy : public gmsec::api5::MessageValidator
{
public:
	CxxMessageValidatorProxy(JNIEnv* env, jobject javaMessageValidator);

	virtual ~CxxMessageValidatorProxy();

	gmsec::api5::Status CALL_TYPE validateMessage(const gmsec::api5::Message& msg);

private:
	jobject jValidator;
};

}  // end namespace jni
}  // end namespace api5
}  // end namespace gmsec

#endif
