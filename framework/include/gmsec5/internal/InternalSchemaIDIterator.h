/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalSchemaIDIterator.h
 *
 * @brief Defines a class that supports the iteration of Specification's loaded schema.
 */

#ifndef GMSEC_API5_INTERNAL_SCHEMA_ID_ITERATOR_H
#define GMSEC_API5_INTERNAL_SCHEMA_ID_ITERATOR_H

#include <gmsec5/internal/InternalSpecification.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{

namespace internal
{

class GMSEC_API InternalSchemaIDIterator
{
public:
	InternalSchemaIDIterator(InternalSpecification& spec);

	~InternalSchemaIDIterator();

	bool hasNext();

	const char* next();

	void reset();

private:
	InternalSpecification&	m_spec;
	bool					m_ready;
	std::string				m_nextID;
};

} // end namespace internal
} // end namespace api5
} // end namespace gmsec

#endif
