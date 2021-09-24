/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file CustomMessageValidator.h
 *
 * @brief Internal custom Message Validator that is used in support of the C binding.
 */

#ifndef GMSEC_CUSTOM_MESSAGE_VALIDATOR_H
#define GMSEC_CUSTOM_MESSAGE_VALIDATOR_H

#include <gmsec4/Status.h>
#include <gmsec4/mist/MessageValidator.h>

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Message;

namespace mist
{
namespace internal
{


class GMSEC_API CustomMessageValidator : public gmsec::api::mist::MessageValidator
{
public:
    CustomMessageValidator(GMSEC_MessageValidator* validator);

    virtual Status CALL_TYPE validateMessage(const gmsec::api::Message& message);

private:
    GMSEC_MessageValidator* m_validator;
};


}  // end namespace internal
}  // end namespace mist
}  // end namespace api
}  // end namespace gmsec

#endif
