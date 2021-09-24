/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalMessageFieldIterator.h
**/

#include <gmsec4/internal/mist/InternalSchemaIDIterator.h>
#include <gmsec4/Exception.h>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

InternalSchemaIDIterator::InternalSchemaIDIterator(InternalSpecification& spec)
	: m_spec(spec),
	  m_ready(false),
	  m_nextID()
{
	reset();
}

InternalSchemaIDIterator::~InternalSchemaIDIterator()
{
}

void InternalSchemaIDIterator::reset()
{
	m_spec.resetSchemaIndex();
}

bool InternalSchemaIDIterator::hasNext()
{
	return m_spec.hasNextID();
}

const char* InternalSchemaIDIterator::next()
{
	try
	{
		m_nextID = m_spec.nextID();
	}
	catch(Exception &e)
	{
		throw Exception(e);
	}

	return m_nextID.c_str();
}

} // namespace mist
} // namespace internal
} // namespace api
} // namespace gmsec
