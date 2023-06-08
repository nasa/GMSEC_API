/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file CustomMessageValidator.h
 *
 * @brief Internal custom Message Validator that is used in support of the C binding.
 */

#ifndef GMSEC_API5_INTERNAL_CUSTOM_MESSAGE_VALIDATOR_H
#define GMSEC_API5_INTERNAL_CUSTOM_MESSAGE_VALIDATOR_H

#include <gmsec5/Status.h>
#include <gmsec5/MessageValidator.h>

#include <gmsec5/c/message.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Message;

namespace internal
{

class CustomMessageValidator : public gmsec::api5::MessageValidator
{
public:
    CustomMessageValidator(GMSEC_MessageValidator* validator);

	virtual ~CustomMessageValidator();

    virtual Status validateMessage(const gmsec::api5::Message& message);

	GMSEC_MessageValidator* getValidator() const;

private:
    GMSEC_MessageValidator* m_validator;
};


}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec

#endif
