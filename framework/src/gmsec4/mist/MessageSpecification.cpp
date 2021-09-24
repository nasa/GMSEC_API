/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/mist/MessageSpecification.h>

#include <gmsec4/internal/mist/InternalMessageSpecification.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::util;


MessageSpecification::MessageSpecification(const char* schemaID, DataList<FieldSpecification*>& fieldSpecs)
	: m_internal(new InternalMessageSpecification(schemaID, fieldSpecs))
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


const DataList<FieldSpecification*>& MessageSpecification::getFieldSpecifications() const
{
	return m_internal->getFieldSpecifications();
}
