/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U64Field.h
 *
 * @brief Specialized Field class that can be used to store an unsigned 64-bit value.
 */

#ifndef GMSEC_API5_U64FIELD_H
#define GMSEC_API5_U64FIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
	namespace internal
	{
		class InternalU64Field;
	}

/**
 * @class U64Field
 * @brief Specialized Field class that can be used to store an unsigned 64-bit value.
 */
class GMSEC_API U64Field : public Field
{
public:
	/**
	 * @fn U64Field(const char* name, GMSEC_U64 value, bool isHeader = false)
	 * @brief Constructor for creating a specialized Field object containing an unsigned 64-bit value.
	 * @param name - the name of the Field
	 * @param value - the value to store
	 * @param isHeader - used to indicate if Field is a header field (default is false)
	 * @throw A GmsecException is thrown if the name is NULL, or is an empty string, or the field name is otherwise not compliant.
	 */
	U64Field(const char* name, GMSEC_U64 value, bool isHeader = false);


	/**
	 * @fn U64Field(const U64Field& other)
	 * @brief Copy-constructor for instantiating of copy of another U64Field
	 * @param other - the U64Field object to replicate
	 */
	 U64Field(const U64Field& other);


	/**
	 * @fn ~U64Field()
	 * @brief Destructor that frees all resources associated with the object.
	 */
	virtual ~U64Field();


	/**
	 * @fn GMSEC_U64 getValue() const
	 * @brief Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_U64 CALL_TYPE getValue() const;


	/**
	 * @fn const char* toXML() const;
	 * @brief Convenience method that returns the XML string representation of this object.
	 * @return An XML string.
	 */
	virtual const char* CALL_TYPE toXML() const;


	/**
	 * @fn const char* toJSON() const;
	 * @brief Convenience method that returns the JSON string representation of this object.
	 * @return A JSON string.
	 */
	virtual const char* CALL_TYPE toJSON() const;

private:
	// Defined, but not implemented
	U64Field();
	U64Field& operator=(const U64Field&);

	gmsec::api5::internal::InternalU64Field* m_internal;
};

} //namespace api5
} //namespace gmsec

#endif
