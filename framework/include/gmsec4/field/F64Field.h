/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F64Field.h
 *
 * @brief Specialized Field class that can be used to store a 64-bit floating point value.
 */

#ifndef GMSEC_API_F64FIELD_H
#define GMSEC_API_F64FIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
	namespace internal
	{
		class InternalF64Field;
	}

/**
 * @class F64Field
 * @brief Specialized Field class that can be used to store a 64-bit floating point value.
 */
class GMSEC_API F64Field : public Field
{
public:
	/**
	 * @fn F64Field(const char* name, GMSEC_F64 value)
	 * @desc Constructor for creating a specialized Field object containing a 64-bit floating point value.
	 * @param name - the name of the Field
	 * @param value - the floating point value to store
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	F64Field(const char* name, GMSEC_F64 value);


	/**
	 * @fn F64Field(const F64Field& other)
	 * @desc Copy-constructor for instantiating of copy of another F64Field
	 * @param other - the F64Field object to replicate
	 */
	 F64Field(const F64Field& other);


	/**
	 * @fn ~F64Field()
	 * @desc Destructor that frees all resources associated with the object.
	 */
	virtual ~F64Field();


	/**
	 * @fn GMSEC_F64 getValue() const
	 * @desc Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_F64 CALL_TYPE getValue() const;


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
	F64Field();
	F64Field& operator=(const F64Field&);

	gmsec::api::internal::InternalF64Field* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
