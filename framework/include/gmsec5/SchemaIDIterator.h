/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SchemaIDIterator.h
 *
 * @brief Defines a class that supports the iteration of a Specification's loaded schema.
 * 
 */

#ifndef GMSEC_API_SCHEMA_ID_ITERATOR_H
#define GMSEC_API_SCHEMA_ID_ITERATOR_H

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace internal
{
	class InternalSpecification;
	class InternalSchemaIDIterator;
}


/** @class SchemaIDIterator
 * 
 * @brief Supports the iteration of a Specification's loaded schema templates.
 *
 * @sa Specification::getSchemaIDIterator()
 */
class GMSEC_API SchemaIDIterator
{
public:
	/*
	 * @fn ~SchemaIDIterator()
	 *
	 * @brief Destructor.
	 */
	~SchemaIDIterator();

	/**
	 * @fn bool hasNext() const
	 *
	 * @brief Provides information as to whether there are additional Schema IDs that can be referenced using next().
	 *
	 * @return True if additional fields are available, false otherwise.
	 */
    bool CALL_TYPE hasNext() const;


	/**
	 * @fn const char* next() const
	 *
	 * @brief Returns a reference to the next available Schema ID.
	 *
	 * @return A Schema ID string.
	 *
	 * @throw GmsecException is thrown if the iterator has reached the end (i.e. there are no more IDs).
	 */
    const char* CALL_TYPE next() const;


	/**
	 * @fn void reset()
	 *
	 * @brief Resets the interator to the beginning of the Schema list that is maintained by the Specification.
	 */
	void CALL_TYPE reset();

private:
	friend class gmsec::api5::internal::InternalSpecification;

	SchemaIDIterator(internal::InternalSpecification& spec);

	//Declared, not implemented
	SchemaIDIterator(const SchemaIDIterator&);

	gmsec::api5::internal::InternalSchemaIDIterator* m_internal;
};

} // namespace api5
} // namespace gmsec

#endif
