/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CharField.h
 *
 * @brief Specialized Field class that can be used to store a char value.
 */

#ifndef GMSEC_API_CHARFIELD_H
#define GMSEC_API_CHARFIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
	namespace internal
	{
		class InternalCharField;
	}

/**
 * @class CharField
 * @brief Specialized Field class that can be used to store a character value.
 */
class GMSEC_API CharField : public Field
{
public:
	/**
	 * @fn CharField(const char* name, GMSEC_CHAR value)
	 * @desc Constructor for creating a specialized Field object containing a character value.
	 * @param name - the name of the Field
	 * @param value - the character value to store
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	CharField(const char* name, GMSEC_CHAR value);


	/**
	 * @fn CharField(const CharField& other)
	 * @desc Copy-constructor for instantiating of copy of another CharField
	 * @param other - the CharField object to replicate
	 */
	 CharField(const CharField& other);


	/**
	 * @fn ~CharField()
	 * @desc Destructor that frees all resources associated with the object.
	 */
	virtual ~CharField();


	/**
	 * @fn GMSEC_CHAR getValue() const
	 * @desc Exposes the underlying character value held by the object.
	 * @return Returns the character value associated with the object.
	 */
	GMSEC_CHAR CALL_TYPE getValue() const;


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
	CharField();
	CharField& operator=(const CharField&);

	gmsec::api::internal::InternalCharField* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
