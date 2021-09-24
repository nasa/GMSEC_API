/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I8Field.h
 *
 * @brief Specialized Field class that can be used to store a signed 8-bit value.
 */

#ifndef GMSEC_API_I8FIELD_H
#define GMSEC_API_I8FIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
	namespace internal
	{
		class InternalI8Field;
	}

/**
 * @class I8Field
 * @brief Specialized Field class that can be used to store a signed 8-bit value.
 */
class GMSEC_API I8Field : public Field
{
public:
	/**
	 * @fn I8Field(const char* name, GMSEC_I8 value)
	 * @desc Constructor for creating a specialized Field object containing a signed 8-bit value.
	 * @param name - the name of the Field
	 * @param value - the value to store
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	I8Field(const char* name, GMSEC_I8 value);


	/**
	 * @fn I8Field(const I8Field& other)
	 * @desc Copy-constructor for instantiating of copy of another I8Field
	 * @param other - the I8Field object to replicate
	 */
	 I8Field(const I8Field& other);


	/**
	 * @fn ~I8Field()
	 * @desc Destructor that frees all resources associated with the object.
	 */
	virtual ~I8Field();


	/**
	 * @fn GMSEC_I8 getValue() const
	 * @desc Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_I8 CALL_TYPE getValue() const;


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
	I8Field();
	I8Field& operator=(const I8Field&);

	gmsec::api::internal::InternalI8Field* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
