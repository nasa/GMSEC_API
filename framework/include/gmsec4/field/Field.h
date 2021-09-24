/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Field.h
 *
 * @brief Base Field class inherited by all field types.
 */

#ifndef GMSEC_API_FIELD_H
#define GMSEC_API_FIELD_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{
	class InternalField;
}

/**
 * @class Field
 * @brief Base class for all field types.
 */
class GMSEC_API Field
{
public:
	/**
	 * @enum FieldType
	 *
	 * @brief The various types of GMSEC Fields that are supported.
	 *
	 * @note Field numbering was chosen to match that of API 3.x
	 */
	enum FieldType
	{
		CHAR_TYPE   = 1,    ///< Field containing a character value
		BOOL_TYPE   = 2,    ///< Field containing a boolean value
		I16_TYPE    = 3,    ///< Field containing a signed 16-bit value
		U16_TYPE    = 4,    ///< Field containing an unsigned 16-bit value
		I32_TYPE    = 5,    ///< Field containing a signed 32-bit value
		U32_TYPE    = 6,    ///< Field containing an unsigned 32-bit value
		F32_TYPE    = 7,    ///< Field containing a 16-bit float value
		F64_TYPE    = 8,    ///< Field containing a 64-bit float value
		STRING_TYPE = 9,    ///< Field containing a string
		BIN_TYPE    = 10,   ///< Field containing a raw data
		I8_TYPE     = 20,   ///< Field containing a signed 8-bit value
		U8_TYPE     = 21,   ///< Field containing an unsigned 8-bit value
		I64_TYPE    = 22,   ///< Field containing a signed 64-bit value
		U64_TYPE    = 23    ///< Field containing an unsigned 64-bit value
	};


	/**
	 * @cond
	 * @fn ~Field()
	 * @brief Destructor
	 */
	virtual ~Field();
	/** @endcond */


	/**
	 * @fn const char* getName() const
	 * @brief Returns the name associated with the Field object.
	 * @return String containing Field name.
	 */
	const char* CALL_TYPE getName() const;


	/**
	 * @fn void getName(const char* name)
	 * @brief Sets the Field name attribute to the given name string.
	 * @param name - the name of the Field
	 * @throw An Exception is thrown if the name is NULL, or is an empty string.
	 */
	void CALL_TYPE setName(const char* name);


	/**
	 * @fn FieldType getType() const
	 * @brief Returns the type of the Field object.
	 * @return FieldType enumerated value.
	 *
	 * @sa FieldType
	 */
	FieldType CALL_TYPE getType() const;


	/**
	 * @fn bool isHeader() const
	 * @brief Returns whether the Field represents a header field.
	 * @return Returns true if a header field; false otherwise.
	 */
	bool CALL_TYPE isHeader() const;


	/** @cond */
	void CALL_TYPE isHeader(bool header);
	/** @endcond */


	/**
	 * @fn const char* toXML() const = 0
	 * @brief Converts the Field object into an XML string representation.
	 * @return Returns an XML string.
	 */
	virtual const char* CALL_TYPE toXML() const = 0;


	/**
	 * @fn const char* toJSON() const = 0
	 * @brief Converts the Field object into a JSON string representation.
	 * @return Returns an JSON string.
	 */
	virtual const char* CALL_TYPE toJSON() const = 0;


	/**
	 * @fn GMSEC_I64 getIntegerValue() const
	 * @brief Attempts to convert the field value into a signed 64-bit integer number representation.
	 * @return Returns the field value as an integer.
	 * @throws An Exception is thrown if the field cannot be successfully converted to an integer.
	 */
	GMSEC_I64 CALL_TYPE getIntegerValue() const;


	/**
	 * @fn GMSEC_U64 getUnsignedIntegerValue() const
	 * @brief Attempts to convert the field value into an unsigned 64-bit integer number representation.
	 * @return Returns the field value as an unsigned integer.
	 * @throws An Exception is thrown if the field cannot be successfully converted to an unsigned integer.
	 */
	GMSEC_U64 CALL_TYPE getUnsignedIntegerValue() const;


	/**
	 * @fn GMSEC_F64 getDoubleValue() const
	 * @brief Attempts to convert the field value into a 64-bit floating point number representation.
	 * @return Returns the field value as a floating point number.
	 * @throws An Exception is thrown if the field cannot be successfully converted to a double.
	 */
	GMSEC_F64 CALL_TYPE getDoubleValue() const;


	/**
	 * @fn const char* getStringValue() const
	 * @brief Attempts to convert the field value into string representation.
	 * @return Returns the field value as a string.
	 * @throws An Exception is thrown if the field cannot be successfully converted to a string.
	 */
	const char* CALL_TYPE getStringValue() const;


	/**
	 * @fn Field* clone() const
	 *
	 * @brief Creates/returns a clone of the Field object.
	 *
	 * @return A cloned Field object.
	 *
	 * @sa Field::destroyClone()
	 */
	Field* CALL_TYPE clone() const;


	/**
	 * @fn void destroyClone(Field*& clone)
	 *
	 * @brief Destroys the (cloned) Field object.
	 *
	 * @param clone - the cloned field to destroy.
	 *
	 * @sa clone()
	 */
	static void CALL_TYPE destroyClone(Field*& clone);


protected:
	/** @cond */
	void CALL_TYPE registerChild(gmsec::api::internal::InternalField* child);

	Field();
	/** @endcond */

private:
	// defined, but not implemented
	Field(const Field& other);

	friend class gmsec::api::internal::InternalField;

	gmsec::api::internal::InternalField* m_internal;
};

} //namespace api
} //namespace gmsec

#endif
