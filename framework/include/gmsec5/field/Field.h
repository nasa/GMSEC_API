/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Field.h
 *
 * @brief Base Field class inherited by all field types.
 */

#ifndef GMSEC_API5_FIELD_H
#define GMSEC_API5_FIELD_H

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{
	class InternalField;
	class FieldBuddy;
}

/**
 * @class Field
 * @brief Base class for all field types.
 */
class GMSEC_API Field
{
public:
	/**
	 * @enum class Type
	 *
	 * @brief The various types of GMSEC Fields that are supported.
	 *
	 * @note Field numbering was chosen to match that of API 3.x
	 * and is preserved so that message encoding/decoding between
	 * APIs 3.x, 4.x, and 5.x are possible.
	 */
	enum class Type
	{
		CHAR   = 1,    ///< Field containing a character value
		BOOL   = 2,    ///< Field containing a boolean value
		I16    = 3,    ///< Field containing a signed 16-bit value
		U16    = 4,    ///< Field containing an unsigned 16-bit value
		I32    = 5,    ///< Field containing a signed 32-bit value
		U32    = 6,    ///< Field containing an unsigned 32-bit value
		F32    = 7,    ///< Field containing a 16-bit float value
		F64    = 8,    ///< Field containing a 64-bit float value
		STRING = 9,    ///< Field containing a string
		BINARY = 10,   ///< Field containing a raw data
		I8     = 20,   ///< Field containing a signed 8-bit value
		U8     = 21,   ///< Field containing an unsigned 8-bit value
		I64    = 22,   ///< Field containing a signed 64-bit value
		U64    = 23    ///< Field containing an unsigned 64-bit value
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
	 * @fn Type getType() const
	 * @brief Returns the type of the Field object.
	 * @return Type enumerated value.
	 *
	 * @sa Type
	 */
	Type CALL_TYPE getType() const;


	/**
	 * @fn bool isHeader() const
	 * @brief Returns whether the Field represents a header field.
	 * @return Returns true if a header field; false otherwise.
	 */
	bool CALL_TYPE isHeader() const;


	/**
	 * @fn bool isTracking() const
	 * @brief Returns whether the Field represents a GMSEC API tracking field.
	 * @return Returns true if a tracking field; false otherwise.
	 */
	bool CALL_TYPE isTracking() const;


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
	 * @fn const char* getStringValue() const
	 * @brief Attempts to convert the field value into string representation.
	 * @return Returns the field value as a string.
	 */
	const char* CALL_TYPE getStringValue() const;


	/**
	 * @fn bool getBooleanValue() const
	 * @brief Attempts to convert the field value into boolean representation.
	 * @note Any non-zero value will be interpreted as true, otherwise as false.
	 * @return Returns the field value as a boolean value.
	 * @throw A GmsecException is thrown if the field cannot successfully be converted to a boolean.
	 */
	bool CALL_TYPE getBooleanValue() const;


	/**
	 * @fn GMSEC_I16 getI16Value() const
	 * @brief Attempts to convert the field value into a signed 16-bit integer number representation.
	 * @return Returns the field value as an integer.
	 * @throws A GmsecException is thrown if the field cannot be successfully converted to an integer.
	 */
	GMSEC_I16 CALL_TYPE getI16Value() const;


	/**
	 * @fn GMSEC_I32 getI32Value() const
	 * @brief Attempts to convert the field value into a signed 32-bit integer number representation.
	 * @return Returns the field value as an integer.
	 * @throws A GmsecException is thrown if the field cannot be successfully converted to an integer.
	 */
	GMSEC_I32 CALL_TYPE getI32Value() const;


	/**
	 * @fn GMSEC_I64 getI64Value() const
	 * @brief Attempts to convert the field value into a signed 64-bit integer number representation.
	 * @return Returns the field value as an integer.
	 * @throws A GmsecException is thrown if the field cannot be successfully converted to an integer.
	 */
	GMSEC_I64 CALL_TYPE getI64Value() const;


	/**
	 * @fn GMSEC_U16 getU16Value() const
	 * @brief Attempts to convert the field value into an unsigned 16-bit integer number representation.
	 * @return Returns the field value as an unsigned integer.
	 * @throws A GmsecException is thrown if the field cannot be successfully converted to an unsigned integer.
	 */
	GMSEC_U16 CALL_TYPE getU16Value() const;


	/**
	 * @fn GMSEC_U32 getU32Value() const
	 * @brief Attempts to convert the field value into an unsigned 32-bit integer number representation.
	 * @return Returns the field value as an unsigned integer.
	 * @throws A GmsecException is thrown if the field cannot be successfully converted to an unsigned integer.
	 */
	GMSEC_U32 CALL_TYPE getU32Value() const;


	/**
	 * @fn GMSEC_U64 getU64Value() const
	 * @brief Attempts to convert the field value into an unsigned 64-bit integer number representation.
	 * @return Returns the field value as an unsigned integer.
	 * @throws A GmsecException is thrown if the field cannot be successfully converted to an unsigned integer.
	 */
	GMSEC_U64 CALL_TYPE getU64Value() const;


	/**
	 * @fn GMSEC_F64 getF64Value() const
	 * @brief Attempts to convert the field value into a 64-bit floating point number representation.
	 * @return Returns the field value as a floating point number.
	 * @throws A GmsecException is thrown if the field cannot be successfully converted to a double.
	 */
	GMSEC_F64 CALL_TYPE getF64Value() const;


protected:
	/** @cond */
	void CALL_TYPE registerChild(gmsec::api5::internal::InternalField* child);

	Field();
	/** @endcond */

private:
	// defined, but not implemented
	Field(const Field& other);

	friend class gmsec::api5::internal::InternalField;
	friend class gmsec::api5::internal::FieldBuddy;

	gmsec::api5::internal::InternalField* m_internal;
};

} //namespace api5
} //namespace gmsec

#endif
