/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_JNI_CUSTOM_SPECIFICATION_H
#define GMSEC_JNI_CUSTOM_SPECIFICATION_H

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/mist/Specification.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
	class Message;

namespace jni
{

class CustomSpecification : public gmsec::api::mist::Specification
{
public:
	CustomSpecification(const Config& config);

	CustomSpecification(const Specification& other);

	virtual ~CustomSpecification();

	virtual void CALL_TYPE validateMessage(const Message& msg);

	void registerObserver(JNIEnv* env, jobject jSpec);

private:
	jobject jSpec;
};

}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec

#endif
