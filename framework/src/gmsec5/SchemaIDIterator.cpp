/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec5/SchemaIDIterator.h>

#include <gmsec5/internal/InternalSpecification.h>
#include <gmsec5/internal/InternalSchemaIDIterator.h>


using namespace gmsec::api5;


SchemaIDIterator::SchemaIDIterator(internal::InternalSpecification& spec)
	: m_internal(new internal::InternalSchemaIDIterator(spec))
{
}


SchemaIDIterator::~SchemaIDIterator()
{
	delete m_internal;
}


bool SchemaIDIterator::hasNext() const
{
	return m_internal->hasNext();
}


const char* SchemaIDIterator::next() const
{
	return m_internal->next();
}


void SchemaIDIterator::reset()
{
	m_internal->reset();
}
