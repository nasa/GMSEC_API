/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I64Field.h
 *
 * @brief Specialized Field class that can be used to store a signed 64-bit value.
 */

#ifndef GMSEC_API5_I64FIELD_H
#define GMSEC_API5_I64FIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
	namespace internal
	{
		class InternalI64Field;
	}

/**
 * @class I64Field
 * @brief Specialized Field class that can be used to store a signed 64-bit value.
 */
class GMSEC_API I64Field : public Field
{
public:
	/**
	 * @fn I64Field(const char* name, GMSEC_I64 value, bool isHeader = false)
	 * @brief Constructor for creating a specialized Field object containing a signed 64-bit value.
	 * @param name - the name of the Field
	 * @param value - the value to store
	 * @param isHeader - used to indicate if Field is a header field (default is false)
	 * @throw A GmsecException is thrown if the name is NULL, or is an empty string, or the field name is otherwise not compliant.
	 */
	I64Field(const char* name, GMSEC_I64 value, bool isHeader = false);


	/**
	 * @fn I64Field(const I64Field& other)
	 * @brief Copy-constructor for instantiating of copy of another I64Field
	 * @param other - the I64Field object to replicate
	 */
	 I64Field(const I64Field& other);


	/**
	 * @fn ~I64Field()
	 * @brief Destructor that frees all resources associated with the object.
	 */
	virtual ~I64Field();


	/**
	 * @fn GMSEC_I64 getValue() const
	 * @brief Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_I64 CALL_TYPE getValue() const;


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
	I64Field();
	I64Field& operator=(const I64Field&);

	gmsec::api5::internal::InternalI64Field* m_internal;
};

} //namespace api5
} //namespace gmsec

#endif
