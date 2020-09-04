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

#include <gmsec4/mist/Specification.h>

#include <gmsec4/internal/mist/InternalSpecification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;


Specification::Specification(const Config& config)
	: m_iSpec(new internal::InternalSpecification(config))
{
}


Specification::Specification(const Specification& other)
	: m_iSpec(new internal::InternalSpecification(*(other.m_iSpec)))
{
}


Specification::~Specification()
{
	delete m_iSpec;
}


void Specification::validateMessage(const Message& msg)
{
	m_iSpec->validateMessage(msg);
}


SchemaIDIterator& Specification::getSchemaIDIterator()
{
	return m_iSpec->getSchemaIDIterator();
}


unsigned int Specification::getVersion() const
{
	return m_iSpec->getVersion();
}


const char* Specification::getTemplateXML(const char *subject, const char *schemaID)
{
	return m_iSpec->getTemplateXML(subject, schemaID);
}
