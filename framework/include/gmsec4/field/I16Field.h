/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I16Field.h
 *
 * @brief Specialized Field class that can be used to store a signed 16-bit value.
 */

#ifndef GMSEC_API_I16FIELD_H
#define GMSEC_API_I16FIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
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
	 * @fn I16Field(const char* name, GMSEC_I16 value)
	 * @desc Constructor for creating a specialized Field object containing a signed 16-bit value.
	 * @param name - the name of the Field
	 * @param value - the value to store
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	I16Field(const char* name, GMSEC_I16 value);


	/**
	 * @fn I16Field(const I16Field& other)
	 * @desc Copy-constructor for instantiating of copy of another I16Field
	 * @param other - the I16Field object to replicate
	 */
	 I16Field(const I16Field& other);


	/**
	 * @fn ~I16Field()
	 * @desc Destructor that frees all resources associated with the object.
	 */
	virtual ~I16Field();


	/**
	 * @fn GMSEC_I16 getValue() const
	 * @desc Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_I16 CALL_TYPE getValue() const;


	/**
	 * @fn const char* toXML() const;
	 * @desc Convenience method that returns the XML string representation of this object.
	 * @return An XML string.
	 */
	virtual const char* CALL_TYPE toXML() const;


	/**
	 * @fn const char* toJSON() const;
	 * @desc Convenience method that returns the JSON string representation of this object.
	 * @return A JSON string.
	 */
	virtual const char* CALL_TYPE toJSON() const;

private:
	// Defined, but not implemented
	I16Field();
	I16Field& operator=(const I16Field&);

	gmsec::api::internal::InternalI16Field* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
