/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U8Field.h
 *
 * @brief Specialized Field class that can be used to store an unsigned 8-bit value.
 */

#ifndef GMSEC_API5_U8FIELD_H
#define GMSEC_API5_U8FIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
	namespace internal
	{
		class InternalU8Field;
	}

/**
 * @class U8Field
 * @brief Specialized Field class that can be used to store an unsigned 8-bit value.
 */
class GMSEC_API U8Field : public Field
{
public:
	/**
	 * @fn U8Field(const char* name, GMSEC_U8 value)
	 * @brief Constructor for creating a specialized Field object containing an unsigned 8-bit value.
	 * @param name - the name of the Field
	 * @param value - the value to store
	 * @param isHeader - used to indicate if Field is a header field (default is false)
	 * @throw A GmsecException is thrown if the name is NULL, or is an empty string.
	 */
	U8Field(const char* name, GMSEC_U8 value, bool isHeader = false);


	/**
	 * @fn U8Field(const U8Field& other)
	 * @brief Copy-constructor for instantiating of copy of another U8Field
	 * @param other - the U8Field object to replicate
	 */
	 U8Field(const U8Field& other);


	/**
	 * @fn ~U8Field()
	 * @brief Destructor that frees all resources associated with the object.
	 */
	virtual ~U8Field();


	/**
	 * @fn GMSEC_U8 getValue() const
	 * @brief Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_U8 CALL_TYPE getValue() const;


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
	U8Field();
	U8Field& operator=(const U8Field&);

	gmsec::api5::internal::InternalU8Field* m_internal;
};

} //namespace api5
} //namespace gmsec

#endif
