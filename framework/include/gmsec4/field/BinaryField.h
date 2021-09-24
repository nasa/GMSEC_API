/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BinaryField.h
 *
 * @brief Specialized Field class that can be used to retain raw data.
 */

#ifndef GMSEC_API_BINARYFIELD_H
#define GMSEC_API_BINARYFIELD_H

#include <gmsec4/field/Field.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>

#include <stddef.h>  // for size_t


namespace gmsec
{
namespace api
{
	namespace internal
	{
		class InternalBinaryField;
	}


/**
 * @class BinaryField
 * @brief Specialized Field class that can be used to retain raw data.
 */
class GMSEC_API BinaryField : public Field
{
public:
	/**
	 * @fn BinaryField(const char* name, const GMSEC_BIN blob, size_t length)
	 * @desc Constructor for creating a specialized Field object containing raw data.
	 * @param name - the name of the Field
	 * @param blob - the raw data
	 * @param length - the length (size) of the raw data
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	BinaryField(const char* name, const GMSEC_BIN blob, size_t length);


	/**
	 * @fn BinaryField(const BinaryField& other)
	 * @desc Copy-constructor for instantiating of copy of another BinaryField
	 * @param other - the BinaryField object to replicate
	 */
	BinaryField(const BinaryField& other);


	/**
	 * @fn ~BinaryField()
	 * @desc Destructor that frees all resources associated with the object.
	 */
	virtual ~BinaryField();


	/**
	 * @fn GMSEC_BIN getValue() const
	 * @desc Exposes the underlying data held by the object.
	 * @return Returns a pointer to the underlying data held by this object.
	 */
	GMSEC_BIN CALL_TYPE getValue() const;


	/**
	 * @fn size_t getLength() const
	 * @desc Accessor to obtain the length of the raw data held by the object.
	 * @return The length (size) of the data held by this object.
	 */
	size_t CALL_TYPE getLength() const;


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
	BinaryField();
	BinaryField& operator=(const BinaryField&);

	gmsec::api::internal::InternalBinaryField* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
