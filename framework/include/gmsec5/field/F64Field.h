/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F64Field.h
 *
 * @brief Specialized Field class that can be used to store a 64-bit floating point value.
 */

#ifndef GMSEC_API5_F64FIELD_H
#define GMSEC_API5_F64FIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
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
	 * @brief Constructor for creating a specialized Field object containing a 64-bit floating point value.
	 * @param name - the name of the Field
	 * @param value - the floating point value to store
	 * @param isHeader - used to indicate if Field is a header field (default is false)
	 * @throw A GmsecException is thrown if the name is NULL, or is an empty string.
	 */
	F64Field(const char* name, GMSEC_F64 value, bool isHeader = false);


	/**
	 * @fn F64Field(const F64Field& other)
	 * @brief Copy-constructor for instantiating of copy of another F64Field
	 * @param other - the F64Field object to replicate
	 */
	 F64Field(const F64Field& other);


	/**
	 * @fn ~F64Field()
	 * @brief Destructor that frees all resources associated with the object.
	 */
	virtual ~F64Field();


	/**
	 * @fn GMSEC_F64 getValue() const
	 * @brief Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_F64 CALL_TYPE getValue() const;


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
	F64Field();
	F64Field& operator=(const F64Field&);

	gmsec::api5::internal::InternalF64Field* m_internal;
};

} //namespace api5
} //namespace gmsec

#endif
