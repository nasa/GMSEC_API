/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U32Field.h
 *
 * @brief Specialized Field class that can be used to store an unsigned 32-bit value.
 */

#ifndef GMSEC_API5_U32FIELD_H
#define GMSEC_API5_U32FIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
	namespace internal
	{
		class InternalU32Field;
	}

/**
 * @class U32Field
 * @brief Specialized Field class that can be used to store an unsigned 32-bit value.
 */
class GMSEC_API U32Field : public Field
{
public:
	/**
	 * @fn U32Field(const char* name, GMSEC_U32 value)
	 * @brief Constructor for creating a specialized Field object containing an unsigned 32-bit value.
	 * @param name - the name of the Field
	 * @param value - the value to store
	 * @param isHeader - used to indicate if Field is a header field (default is false)
	 * @throw A GmsecException is thrown if the name is NULL, or is an empty string.
	 */
	U32Field(const char* name, GMSEC_U32 value, bool isHeader = false);


	/**
	 * @fn U32Field(const U32Field& other)
	 * @brief Copy-constructor for instantiating of copy of another U32Field
	 * @param other - the U32Field object to replicate
	 */
	 U32Field(const U32Field& other);


	/**
	 * @fn ~U32Field()
	 * @brief Destructor that frees all resources associated with the object.
	 */
	virtual ~U32Field();


	/**
	 * @fn GMSEC_U32 getValue() const
	 * @brief Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_U32 CALL_TYPE getValue() const;


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
	U32Field();
	U32Field& operator=(const U32Field&);

	gmsec::api5::internal::InternalU32Field* m_internal;
};

} //namespace api5
} //namespace gmsec

#endif
