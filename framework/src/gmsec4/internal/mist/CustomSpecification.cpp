/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file CustomSpecification.h
 *
 * @brief Internal custom extension of Specification that is used in support of the C binding.
 * 
 */

#include <gmsec4/internal/mist/CustomSpecification.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/c/status.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


CustomSpecification::CustomSpecification(Specification* spec, GMSEC_SpecificationValidateMessage* validateMsg)
	: Specification(*spec),
	  m_validateMsg(validateMsg)
{
}


void CustomSpecification::validateMessage(const Message& message)
{
	Message& tmpMsg = const_cast<Message&>(message);

	GMSEC_Specification spec   = reinterpret_cast<GMSEC_Specification>(this);
	GMSEC_Message       msg    = reinterpret_cast<GMSEC_Message>(&tmpMsg);
	GMSEC_Status        status = statusCreate();

	(*m_validateMsg)(spec, msg, status);

	Status result = *(reinterpret_cast<Status*>(status));

	statusDestroy(&status);

	if (result.isError())
	{
		throw Exception(result);
	}
}
