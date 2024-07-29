/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file CustomMessageValidator.h
 *
 * @brief Internal custom Message Validator that is used in support of the C binding.
 */

#include <gmsec5/internal/CustomMessageValidator.h>

#include <gmsec5/Message.h>

#include <gmsec5/c/status.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


CustomMessageValidator::CustomMessageValidator(GMSEC_MessageValidator* validator)
	: m_validator(validator)
{
}


CustomMessageValidator::~CustomMessageValidator()
{
	// Nothing to do.
}


Status CustomMessageValidator::validateMessage(const Message& message)
{
	Message&      tmpMsg = const_cast<Message&>(message);
	GMSEC_Message msg    = reinterpret_cast<GMSEC_Message>(&tmpMsg);
	GMSEC_Status  status = statusCreate();

	// Invoke custom validation function
	(*m_validator)(msg, status);

	Status result = *(reinterpret_cast<Status*>(status));

	statusDestroy(&status);

	return result;
}


GMSEC_MessageValidator* CustomMessageValidator::getValidator() const
{
	return m_validator;
}
