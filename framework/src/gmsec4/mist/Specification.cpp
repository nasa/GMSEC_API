/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Specification.cpp
 *
 *  @brief This file contains a class for managing MessageTemplates and validating Messages.
 *
**/

#include <gmsec4/mist/Specification.h>

#include <gmsec4/internal/mist/InternalSpecification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;


Specification::Specification()
	: m_internal(new internal::InternalSpecification(Config()))
{
}


Specification::Specification(const Config& config)
	: m_internal(new internal::InternalSpecification(config))
{
}


Specification::Specification(const Specification& other)
	: m_internal(new internal::InternalSpecification(*(other.m_internal)))
{
}


Specification::~Specification()
{
	delete m_internal;
}


void Specification::validateMessage(const Message& msg)
{
	m_internal->validateMessage(msg);
}


SchemaIDIterator& Specification::getSchemaIDIterator()
{
	return m_internal->getSchemaIDIterator();
}


unsigned int Specification::getVersion() const
{
	return m_internal->getVersion();
}


Specification::SchemaLevel Specification::getSchemaLevel() const
{
	return m_internal->getSchemaLevel();
}


const util::DataList<MessageSpecification*>& Specification::getMessageSpecifications() const
{
	return m_internal->getMessageSpecifications();
}


void Specification::registerMessageValidator(MessageValidator* validator)
{
	m_internal->registerMessageValidator(validator);
}


void Specification::registerMessageValidator(GMSEC_MessageValidator* validator)
{
	m_internal->registerMessageValidator(validator);
}


const char* Specification::getTemplateXML(const char *subject, const char *schemaID)
{
	return m_internal->getTemplateXML(subject, schemaID);
}
