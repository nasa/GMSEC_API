/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U32Field.h
 *
 * @brief Specialized Field class that can be used to store an unsigned 32-bit value.
 */

#ifndef GMSEC_API_U32FIELD_H
#define GMSEC_API_U32FIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
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
	 * @desc Constructor for creating a specialized Field object containing an unsigned 32-bit value.
	 * @param name - the name of the Field
	 * @param value - the value to store
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	U32Field(const char* name, GMSEC_U32 value);


	/**
	 * @fn U32Field(const U32Field& other)
	 * @desc Copy-constructor for instantiating of copy of another U32Field
	 * @param other - the U32Field object to replicate
	 */
	 U32Field(const U32Field& other);


	/**
	 * @fn ~U32Field()
	 * @desc Destructor that frees all resources associated with the object.
	 */
	virtual ~U32Field();


	/**
	 * @fn GMSEC_U32 getValue() const
	 * @desc Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	GMSEC_U32 CALL_TYPE getValue() const;


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
	U32Field();
	U32Field& operator=(const U32Field&);

	gmsec::api::internal::InternalU32Field* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
