/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Context.cpp
 *
 *  @brief This file contains a class to store the results of a Message validation
 *
**/

#include <gmsec4/internal/mist/Context.h>
#include <gmsec4/internal/mist/InternalContext.h>

#include <gmsec4/Exception.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


Context::Context()
	: ptr(new InternalContext)
{
}


Context::Context(const char* templateId, bool validate)
	: ptr(new InternalContext(templateId, validate))
{
}


Context::~Context()
{
	delete ptr;
	ptr = 0;
}


const char* Context::getTemplateId() const
{
	return ptr->getTemplateId();
}


void Context::reset(const char* templateId)
{
	ptr->reset(templateId);
}


void Context::validateOnApply(bool validate)
{
	ptr->validateOnApply(validate);
}


bool Context::getValidate() const
{
	return ptr->getValidate();
}


bool Context::visited(const char* name) const
{
	return ptr->visited(name);
}


size_t Context::getStatusCount()
{
	return ptr->getStatusCount();
}


ContextStatus Context::getStatus(size_t index)
{
	return ptr->getStatus(index);
}


void Context::resetContext(InternalContext* bc)
{
	if (!bc)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "InternalContext cannot be NULL");
	}

	const std::string templateID = ptr->getTemplateId();
	const bool        validate   = ptr->getValidate();

	delete ptr;

	ptr = bc;

	ptr->reset(templateID.c_str());
	ptr->validateOnApply(validate);
}
