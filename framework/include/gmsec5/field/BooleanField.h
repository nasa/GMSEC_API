/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BooleanField.h
 *
 * @brief Specialized Field class that can be used to store a boolean value.
 */

#ifndef GMSEC_API5_BOOLEANFIELD_H
#define GMSEC_API5_BOOLEANFIELD_H

#include <gmsec5/field/Field.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
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
	 * @brief Constructor for creating a specialized Field object containing a boolean value.
	 * @param name - the name of the Field
	 * @param value - the boolean value to store
	 * @param isHeader - used to indicate if Field is a header field (default is false)
	 * @throw A GmsecException is thrown if the name is NULL, or is an empty string.
	 */
	BooleanField(const char* name, bool value, bool isHeader = false);


	/**
	 * @fn BooleanField(const BooleanField& other)
	 * @brief Copy-constructor for instantiating of copy of another BooleanField
	 * @param other - the BooleanField object to replicate
	 */
	BooleanField(const BooleanField& other);


	/**
	 * @fn ~BooleanField()
	 * @brief Destructor that frees all resources associated with the object.
	 */
	virtual ~BooleanField();


	/**
	 * @fn bool getValue() const
	 * @brief Exposes the underlying boolean value held by the object.
	 * @return Returns the boolean value associated with the object.
	 */
	bool CALL_TYPE getValue() const;


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
	BooleanField();
	BooleanField& operator=(const BooleanField&);

	gmsec::api5::internal::InternalBooleanField* m_internal;
};

} //namespace api5
} //namespace gmsec

#endif
