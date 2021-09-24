/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file StringField.h
 *
 * @brief Specialized Field class that can be used to store a string value.
 */

#ifndef GMSEC_API_STRING_FIELD_H
#define GMSEC_API_STRING_FIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
	namespace internal
	{
		class InternalStringField;
	}

/**
 * @class StringField
 * @brief Specialized Field class that can be used to store a string value.
 */
class GMSEC_API StringField : public Field
{
public:
	/**
	 * @fn StringField(const char* name, const char* value)
	 * @desc Constructor for creating a specialized Field object containing a string value.
	 * @param name - the name of the Field
	 * @param value - the string value to store
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	StringField(const char* name, const char* value);


	/**
	 * @fn StringField(const StringField& other)
	 * @desc Copy-constructor for instantiating of copy of another StringField
	 * @param other - the StringField object to replicate
	 */
	 StringField(const StringField& other);


	/**
	 * @fn ~StringField()
	 * @desc Destructor that frees all resources associated with the object.
	 */
	virtual ~StringField();


	/**
	 * @fn const char* getValue() const
	 * @desc Exposes the underlying value held by the object.
	 * @return Returns the value associated with the object.
	 */
	const char* CALL_TYPE getValue() const;


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
	StringField();
	StringField& operator=(const StringField&);

	gmsec::api::internal::InternalStringField* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
