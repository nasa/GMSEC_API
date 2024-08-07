/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I16Field.h
 *
 * @brief Specialized Field class that can be used to store a signed 16-bit value.
 */

#ifndef GMSEC_API5_I16FIELD_H
#define GMSEC_API5_I16FIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
	namespace internal
	{
		class InternalI16Field;
	}

/**
 * @class I16Field
 * @brief Specialized Field class that can be used to store a signed 16-bit value.
 */
class GMSEC_API I16Field : public Field
{
public:
	/**
	 * @fn I16Field(const char* name, GMSEC_I16 value, bool isHeader = false)
	 * @brief Constructor for creating a specialized Field object containing a signed 16-bit value.
	 * @param name - the name of the Field
	 * @param value - the value to store
	 * @param isHeader - used to indicate if Field is a header field (default is false)
	 * @throw A GmsecException is thrown if the name is NULL, or is an empty string, or the field name is otherwise not compliant.
	 */
	I16Field(const char* name, GMSEC_I16 value, bool isHeader = false);


	/**
	 * @fn I16Field(const I16Field& other)
	 * @brief Copy-constructor for instantiating of copy of another I16Field
	 * @param other - the I16Field object to replicate
	 */
	 I16Field(const I16Field& other);


	/**
	 * @fn ~I16Field()
	 * @brief Destructor that frees all resources associated with the object.
	 */
	virtual ~I16Field();


	/**
	 * @fn GMSEC_I16 getValue() const
	 * @brief Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_I16 CALL_TYPE getValue() const;


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
	I16Field();
	I16Field& operator=(const I16Field&);

	gmsec::api5::internal::InternalI16Field* m_internal;
};

} //namespace api5
} //namespace gmsec

#endif
