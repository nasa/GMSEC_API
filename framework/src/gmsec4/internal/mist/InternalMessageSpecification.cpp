/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/InternalMessageSpecification.h>

#include <gmsec4/mist/FieldSpecification.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::util;


InternalMessageSpecification::InternalMessageSpecification(const char* schemaID, DataList<FieldSpecification*>& fieldSpecs)
	: m_schemaID(schemaID),
	  m_fieldSpecs(fieldSpecs)
{
}


InternalMessageSpecification::~InternalMessageSpecification()
{
	for (DataList<FieldSpecification*>::iterator it = m_fieldSpecs.begin(); it != m_fieldSpecs.end(); ++it)
	{
		delete *it;
	}
	m_fieldSpecs.clear();
}


const char* InternalMessageSpecification::getSchemaID() const
{
	return m_schemaID.c_str();
}


const DataList<FieldSpecification*>& InternalMessageSpecification::getFieldSpecifications() const
{
	return m_fieldSpecs;
}
