/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec5/internal/InternalMessageSpecification.h>

#include <gmsec5/FieldSpecification.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


InternalMessageSpecification::InternalMessageSpecification(const std::string& schemaID,
                                                           const std::string& subjectTemplate,
                                                           List<FieldSpecification*>& fieldSpecs)
	: m_schemaID(schemaID),
	  m_subjectTemplate(subjectTemplate),
	  m_fieldSpecs(fieldSpecs)
{
}


InternalMessageSpecification::~InternalMessageSpecification()
{
	for (List<FieldSpecification*>::iterator it = m_fieldSpecs.begin(); it != m_fieldSpecs.end(); ++it)
	{
		delete *it;
	}
	m_fieldSpecs.clear();
}


const char* InternalMessageSpecification::getSchemaID() const
{
	return m_schemaID.c_str();
}


const char* InternalMessageSpecification::getSubjectTemplate() const
{
	return m_subjectTemplate.c_str();
}


const List<FieldSpecification*>& InternalMessageSpecification::getFieldSpecifications() const
{
	return m_fieldSpecs;
}
