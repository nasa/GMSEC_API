/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/mist/SchemaIDIterator.h>

#include <gmsec4/internal/mist/InternalSpecification.h>
#include <gmsec4/internal/mist/InternalSchemaIDIterator.h>

namespace gmsec
{
namespace api
{
namespace mist
{

SchemaIDIterator::SchemaIDIterator(mist::internal::InternalSpecification& spec)
	: m_iter(new internal::InternalSchemaIDIterator(spec))
{
}

SchemaIDIterator::~SchemaIDIterator()
{
	delete m_iter;
}

bool SchemaIDIterator::hasNext() const
{
	return m_iter->hasNext();
}


const char* SchemaIDIterator::next() const
{
	return m_iter->next();
}


void SchemaIDIterator::reset()
{
	m_iter->reset();
}

} // namespace mist
} // namespace api
} // namespace gmsec