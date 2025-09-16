/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec5/MessageSpecification.h>

#include <gmsec5/internal/InternalMessageSpecification.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


MessageSpecification::MessageSpecification(const char* schemaID, const char* subjectTemplate, List<FieldSpecification*>& fieldSpecs)
	: m_internal(new InternalMessageSpecification(schemaID, subjectTemplate, fieldSpecs))
{
}


MessageSpecification::~MessageSpecification()
{
	delete m_internal;
}


const char* MessageSpecification::getSchemaID() const
{
	return m_internal->getSchemaID();
}


const char* MessageSpecification::getSubjectTemplate() const
{
	return m_internal->getSubjectTemplate();
}


const List<FieldSpecification*>& MessageSpecification::getFieldSpecifications() const
{
	return m_internal->getFieldSpecifications();
}
