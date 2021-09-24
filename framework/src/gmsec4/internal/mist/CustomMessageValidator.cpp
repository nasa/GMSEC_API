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

#include <gmsec4/internal/mist/CustomMessageValidator.h>

#include <gmsec4/Message.h>

#include <gmsec4/c/status.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


CustomMessageValidator::CustomMessageValidator(GMSEC_MessageValidator* validator)
	: m_validator(validator)
{
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
