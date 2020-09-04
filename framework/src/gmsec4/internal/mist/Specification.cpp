/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Specification.cpp
 *
 *  @brief This file contains a class for managing MessageTemplates and validating Messages.
 *
**/

#include <gmsec4/internal/mist/Specification.h>
#include <gmsec4/internal/mist/InternalSpecification.h>
#include <gmsec4/internal/mist/Context.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


Specification::Specification()
	: m_iSpec(new InternalSpecification())
{
}


Specification::~Specification()
{
	delete m_iSpec;
	m_iSpec = 0;
}


Status Specification::loadTemplate(const char* id, const char* path)
{
	return m_iSpec->loadTemplate(id, path);
}


Status Specification::load(const char* basePath, unsigned int version)
{
	return m_iSpec->load(basePath, version);
}


Status Specification::apply(Message& message, Context& context) const
{
	return m_iSpec->apply(message, context);
}


Status Specification::instantiate(Message& message, Context& context) const
{
	return m_iSpec->instantiate(message, context);
}


Status Specification::validate(Message& message, Context& context) const
{
	return m_iSpec->validate(message, context);
}


Status Specification::registerTemplate(const char* id, MessageTemplate* newTemplate)
{
	return m_iSpec->registerTemplate(id, newTemplate);
}


void Specification::getTemplateIds(DataList<std::string>& templateIDs) const
{
	std::list<std::string> ids = m_iSpec->getTemplateIds();

	for (std::list<std::string>::const_iterator it = ids.begin(); it != ids.end(); ++it)
	{
		templateIDs.push_back(*it);
	}
}


Status Specification::lookupTemplate(const Message& message, std::string& templateID)
{
	Status status;

	try {
		templateID = m_iSpec->lookupTemplate(message);
	}
	catch (Exception& e) {
		status.set(e.getErrorClass(), e.getErrorCode(), e.getErrorMessage());
	}

	return status;
}
