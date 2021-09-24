/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BooleanField.h
 *
 * @brief Specialized Field class that can be used to store a boolean value.
 */

#ifndef GMSEC_API_BOOLEANFIELD_H
#define GMSEC_API_BOOLEANFIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
	namespace internal
	{
		class InternalBooleanField;
	}

/**
 * @class BooleanField
 * @brief Specialized Field class that can be used to store a boolean value.
 */
class GMSEC_API BooleanField : public Field
{
public:
	/**
	 * @fn BooleanField(const char* name, bool value)
	 * @desc Constructor for creating a specialized Field object containing a boolean value.
	 * @param name - the name of the Field
	 * @param value - the boolean value to store
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	BooleanField(const char* name, bool value);


	/**
	 * @fn BooleanField(const BooleanField& other)
	 * @desc Copy-constructor for instantiating of copy of another BooleanField
	 * @param other - the BooleanField object to replicate
	 */
	BooleanField(const BooleanField& other);


	/**
	 * @fn ~BooleanField()
	 * @desc Destructor that frees all resources associated with the object.
	 */
	virtual ~BooleanField();


	/**
	 * @fn bool getValue() const
	 * @desc Exposes the underlying boolean value held by the object.
	 * @return Returns the boolean value associated with the object.
	 */
	bool CALL_TYPE getValue() const;


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
	BooleanField();
	BooleanField& operator=(const BooleanField&);

	gmsec::api::internal::InternalBooleanField* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
