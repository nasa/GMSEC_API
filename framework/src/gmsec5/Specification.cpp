/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Specification.cpp
 *
 *  @brief This file contains a class for managing MessageTemplates and validating Messages.
 *
 */

#include <gmsec5/Specification.h>

#include <gmsec5/internal/InternalSpecification.h>

#include <gmsec5/Config.h>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


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


SchemaIDIterator& Specification::getSchemaIDIterator() const
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


const util::List<MessageSpecification*>& Specification::getMessageSpecifications() const
{
	return m_internal->getMessageSpecifications();
}


const util::List<const char*> CALL_TYPE Specification::getHeaderFieldNames(const char* schemaName) const
{
	return m_internal->getHeaderFieldNames(schemaName);
}
