/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalSchemaIDIterator.h
 *
 * @brief Defines a class that supports the iteration of Specification's loaded schema.
 * 
 */

#ifndef GMSEC_API_INTERNAL_SCHEMA_ID_ITERATOR_H
#define GMSEC_API_INTERNAL_SCHEMA_ID_ITERATOR_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/internal/mist/InternalSpecification.h>


namespace gmsec
{
namespace api
{
namespace mist
{

namespace internal
{

class GMSEC_API InternalSchemaIDIterator
{
public:
	InternalSchemaIDIterator(InternalSpecification& spec);

	~InternalSchemaIDIterator();

	bool CALL_TYPE hasNext();

	const char* CALL_TYPE next();

	void CALL_TYPE reset();

private:
	InternalSpecification&	m_spec;
	bool					m_ready;
	std::string				m_nextID;
};

} // namespace mist
} // namespace internal
} // namespace api
} // namespace gmsec

#endif
