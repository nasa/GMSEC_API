
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Field.h
 *
 *  @brief This file contains the class for managing message fields independent of middleware.
 *
**/

#ifndef gmsec_Field_h
#define gmsec_Field_h


#include <gmsec/Status.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

namespace internal
{
class BaseField;
class FieldBuddy;
}


/** @class Field
 * @brief This object is a container for GMSEC Fields.  The methods of this class allow
 * the construction and manipulation of a standard GMSEC Field.
 *
 * Example creation and use:
 * @code
 * //create field
 * Field fd;
 *
 * //set field name & value
 * result = fd.SetName("char_field");
 * if( result.isError() )
 *	//handle error
 * result = fd.Set((GMSEC_CHAR)'c');
 * if( result.isError() )
 *	//handle error
 *
 * //add to message (will copy)
 * result = msg->AddField(fd);
 * if( result.isError() )
 *	//handle error
 * @endcode
*/
class GMSEC_API Field
{
public:
	/** @fn Field()
	 * @brief Create an "UNSET" field
	 */
	GMSEC_DEPRECATED Field();

	/** @fn Field(const char *name, GMSEC_CHAR value)
	 * @brief Create a named field of type GMSEC_CHAR
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_CHAR value);

	/** @fn Field(const char *name, GMSEC_I8 value)
	 * @brief Create a named field of type GMSEC_I8
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_I8 value);

	/** @fn Field(const char *name, GMSEC_U8 value)
	 * @brief Create a named field of type GMSEC_U8
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_U8 value);

	/** @fn Field(const char *name, GMSEC_BOOL value)
	 * @brief Create a named field of type GMSEC_BOOL
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_BOOL value);

	/** @fn Field(const char *name, GMSEC_I16 value)
	 * @brief Create a named field of type GMSEC_I16
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_I16 value);

	/** @fn Field(const char *name, GMSEC_U16 value)
	 * @brief Create a named field of type GMSEC_U16
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_U16 value);

	/** @fn Field(const char *name, GMSEC_I32 value)
	 * @brief Create a named field of type GMSEC_I32
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_I32 value);

	/** @fn Field(const char *name, GMSEC_U32 value)
	 * @brief Create a named field of type GMSEC_U32
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_U32 value);

	/** @fn Field(const char *name, GMSEC_U64 value)
	 * @brief Create a named field of type GMSEC_U64
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_U64 value);

	/** @fn Field(const char *name, GMSEC_F32 value)
	 * @brief Create a named field of type GMSEC_F32
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_F32 value);

	/** @fn Field(const char *name, GMSEC_F64 value)
	 * @brief Create a named field of type GMSEC_F64
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_F64 value);

	/** @fn Field(const char *name, GMSEC_STRING value)
	 * @brief Create a named field of type GMSEC_STRING
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_STRING value);

	/** @fn Field(const char *name, GMSEC_STR value)
	 * @brief Create a named field of type GMSEC_STR
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_STR value);

	/** @fn Field(const char *name, GMSEC_BIN value, GMSEC_U32 size)
	 * @brief Create a named field of type GMSEC_BIN
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 * @param size - size of the field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_BIN value, GMSEC_U32 size);

	/** @fn Field(const char *name, GMSEC_BLOB value, GMSEC_U32 size)
	 * @brief Create a named field of type GMSEC_BLOB
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 * @param size - size of the field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_BLOB value, GMSEC_U32 size);

	/** @fn Field(const char *name, GMSEC_I64 value)
	 * @brief Create a named field of type GMSEC_I64
	 *
	 * @param name - name of field to set
	 * @param value - value of field to set
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_I64 value);

	/** @fn Field(const char *name, GMSEC_TYPE ftype, const char *value)
	 * @brief Create a named field of specified type, value parsed from the value string
	 *
	 * @param name - name of field to set
	 * @param ftype - target field type to set
	 * @param value - string to parse as the type in "ftype"
	 */
	GMSEC_DEPRECATED Field(const char *name, GMSEC_TYPE ftype, const char *value);

	/** @fn Field(Field &fd)
	 * @brief (safe) deep copy constructor
	 */
	GMSEC_DEPRECATED Field(Field &fd);

	/** @fn ~Field()
	 * @brief Destructor.
	 */
	~Field();

	/** @fn operator=( Field &fd )
	 * @brief (safe) deep copy asignment operator
	 */
	Field &operator=(Field &fd);

	/** @fn GetName(const char *&name)
	 * @brief This function returns the name of this field. This scope is the
	 * lifetime of the Field object, or until the next %GetName() call.
	 *
	 * @param name - out parameter, the name of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetName(const char *&name);

	/** @fn SetName(const char *name)
	 * @brief This function sets the field name without changing type/value.
	 *
	 * @param name - the name of this field to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetName(const char *name);

	/** @fn SetType(GMSEC_TYPE ftype)
	 * @brief This function sets the type for this field
	 *
	 * @param ftype - type of the field to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetType(GMSEC_TYPE ftype);
	/** @fn GetType(GMSEC_TYPE &ftype)
	 * @brief This Function gets the type identifier for this field.
	 *
	 * @param ftype - out parameter, type of the field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetType(GMSEC_TYPE &ftype);

	/** @fn isHeader()
	 * @brief This function determines if the field is a header field.
	 *
	 * @note This function has been deprecated; use IsHeader() instead.
	 *
	 * @return status - true if this is a header field.
	 */
	bool CALL_TYPE isHeader() const;

	/** @fn IsHeader()
	 * @brief This function determines if the field is a header field.
	 *
	 * @return status - true if this is a header field.
	 */
	bool CALL_TYPE IsHeader() const;

	/** @fn setHeader()
	 * @brief This function sets whether or not the field is a header field.
	 *
	 * @note This function has been deprecated; use SetHeader() instead.
	 *
	 * @note Header fields are a mechanism for exposing GMSEC fields to the middleware.  For
	 * instance, they allow the middleware server to check a message signature (which is stored
	 * as a header field).  
	 *
	 * @param isHeader - boolean indicating whether this is a header field.
	 */
	void CALL_TYPE setHeader(bool isHeader);

	/** @fn SetHeader()
	 * @brief This function sets whether or not the field is a header field.
	 *
	 * @note Header fields are a mechanism for exposing GMSEC fields to the middleware.  For
	 * instance, they allow the middleware server to check a message signature (which is stored
	 * as a header field).  
	 *
	 * @param isHeader - boolean indicating whether this is a header field.
	 */
	void CALL_TYPE SetHeader(bool isHeader);

	/** @fn Get(T& value)
	 * @brief Templated method that returns the value as the specified type T for this field.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	template <typename T> Status CALL_TYPE Get(T& value)
	{
		return GetValue(value);
	}

	/** @fn Set(T value)
	 * @brief Templated method that sets the value & type T of this field.
	 *
	 * @param value - value of type to set
	 * @return status - result of the operation
	 */
	template <typename T> Status CALL_TYPE Set(T value)
	{
		return SetValue(value);
	}

	/** @fn GetValue(GMSEC_CHAR &value)
	 * @brief This function returns the value as GMSEC_CHAR type.
	 * If the value can't be coerced into a GMSEC_CHAR type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_CHAR &value);

	/** @fn GetValue(GMSEC_I8 &value)
	 * @brief This function returns the value as GMSEC_I8 type.
	 * If the value can't be coerced into a GMSEC_I8 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_I8 &value);

	/** @fn GetValue(GMSEC_U8 &value)
	 * @brief This function returns the value as GMSEC_U8 type.
	 * If the value can't be coerced into a GMSEC_U8 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_U8 &value);

	/** @fn GetValue(GMSEC_BOOL &value)
	 * @brief This function returns the value as GMSEC_BOOL type.
	 * If the value can't be coerced into a GMSEC_BOOL type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_BOOL &value);

	/** @fn GetValue(GMSEC_I16 &value)
	 * @brief This function returns the value as GMSEC_I16 type.
	 * If the value can't be coerced into a GMSEC_I16 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_I16 &value);

	/** @fn GetValue(GMSEC_U16 &value)
	 * @brief This function returns the value as GMSEC_U16 type.
	 * If the value can't be coerced into a GMSEC_U16 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_U16 &value);

	/** @fn GetValue(GMSEC_I32 &value)
	 * @brief This function returns the value as GMSEC_I32 type.
	 * If the value can't be coerced into a GMSEC_I32 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_I32 &value);

	/** @fn GetValue(GMSEC_U32 &value)
	 * @brief This function returns the value as GMSEC_U32 type.
	 * If the value can't be coerced into a GMSEC_U32 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_U32 &value);

	/** @fn GetValue(GMSEC_I64 &value)
	 * @brief This function returns the value as GMSEC_I64 type.
	 * If the value can't be coerced into a GMSEC_I64 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_I64 &value);

	/** @fn GetValue(GMSEC_U64 &value)
	 * @brief This function returns the value as GMSEC_U64 type.
	 * If the value can't be coerced into a GMSEC_U64 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_U64 &value);

	/** @fn GetValue(GMSEC_F32 &value)
	 * @brief This function returns the value as GMSEC_F32 type.
	 * If the value can't be coerced into a GMSEC_F32 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_F32 &value);

	/** @fn GetValue(GMSEC_F64 &value)
	 * @brief This function returns the value as GMSEC_F64 type.
	 * If the value can't be coerced into a GMSEC_F64 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetValue(GMSEC_F64 &value);

	/** @fn GetValue(GMSEC_STR &value)
	 * @brief This function returns the value as GMSEC_STR type.
	 * If the value can't be coerced into a GMSEC_STR type, an error will be returned.
	 *
	 * @note This function allocates memory that is attached to this object.
	 * the client does not need to clean up, but it must be copied to
	 * be stored anywhere. The scope is the lifetime of this Field object.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 *
	 */
	Status CALL_TYPE GetValue(GMSEC_STR &value);

	/** @fn GetValue(GMSEC_STRING &value)
	 * @brief This function returns the value as GMSEC_STRING type.
	 * If the value can't be coerced into a GMSEC_STRING type, an error will be returned.
	 *
	 * @note This function allocates memory that is attached to this object.
	 * the client does not need to clean up, but it must be copied to
	 * be stored anywhere. The scope is the lifetime of this Field object.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 *
	 */
	Status CALL_TYPE GetValue(GMSEC_STRING &value);

	/** @fn GetValue(GMSEC_BLOB &value, GMSEC_U32 &size)
	 * @brief This function returns the value as GMSEC_BLOB type.
	 * If the value can't be coerced into a GMSEC_BLOB type, an error will be returned.
	 *
	 * @note This function allocates memory that is attached to this object.
	 * the client does not need to clean up, but it must be copied to
	 * be stored anywhere. The scope is the lifetime of this Field object.
	 *
	 * @param value - out parameter, value of this field
	 * @param size - out parameter, the length of the array in "value"
	 * @return status - result of the operation
	 *
	 */
	Status CALL_TYPE GetValue(GMSEC_BLOB &value, GMSEC_U32 &size);

	/** @fn GetValue(GMSEC_BIN *value, GMSEC_U32 &size)
	 * @brief This function returns the value as GMSEC_BIN type.
	 * If the value can't be coerced into a GMSEC_BIN type, an error will be returned.
	 *
	 * @note This function allocates memory that is attached to this object.
	 * the client does not need to clean up, but it must be copied to
	 * be stored anywhere. The scope is the lifetime of this Field object.
	 *
	 * @param value - out parameter, value of this field
	 * @param size - out parameter, the length of the array in "value"
	 * @return status - result of the operation
	 *
	 */
	Status CALL_TYPE GetValue(GMSEC_BIN *value, GMSEC_U32 &size);

#ifdef GMSEC_COMPOUND_FIELDS
	/** @fn GetIterator(FieldIterator &iterator)
	 * @brief This function returns an iterator for the internal fields.
	 * If this field is not a compound field, an error will be returned.
	 *
	 * @param iterator - out parameter, iterator for this field
	 * @return status - result of the operation
	 *
	 */
	Status CALL_TYPE GetIterator(FieldIterator &iterator);
#endif

	/** @fn SetValue(GMSEC_CHAR value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_CHAR to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_CHAR value);

	/** @fn SetValue(GMSEC_I8 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_I8 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_I8 value);

	/** @fn SetValue(GMSEC_U8 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_U8 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_U8 value);

	/** @fn SetValue(GMSEC_BOOL value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_BOOL to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_BOOL value);

	/** @fn SetValue(GMSEC_I16 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_I16 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_I16 value);

	/** @fn SetValue(GMSEC_U16 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_U16 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_U16 value);

	/** @fn SetValue(GMSEC_I32 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_I32 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_I32 value);

	/** @fn SetValue(GMSEC_U32 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_U32 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_U32 value);

	/** @fn SetValue(GMSEC_I64 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_I64 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_I64 value);

	/** @fn SetValue(GMSEC_U64 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_U64 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_U64 value);

	/** @fn SetValue(GMSEC_F32 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_F32 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_F32 value);

	/** @fn SetValue(GMSEC_F64 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_F64 to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_F64 value);

	/** @fn SetValue(GMSEC_STR value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_STR to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_STR value);

	/** @fn SetValue(GMSEC_BLOB value, GMSEC_U32 size)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_BLOB to set
	 * @param size - size of the array in "value"
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_BLOB value, GMSEC_U32 size);

	/** @fn SetValue(GMSEC_STRING value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_STRING to set
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_STRING value);

	/** @fn SetValue(GMSEC_BIN value, GMSEC_U32 size)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_BIN to set
	 * @param size - size of the array in "value"
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_BIN value, GMSEC_U32 size);

	/** @fn SetValue(GMSEC_TYPE ftype, const char *value)
	 * @brief This function sets the value of type specified with value
	 * parsed from the string
	 *
	 * @param ftype - type of field to set
	 * @param value - string to parse as the type in "ftype"
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetValue(GMSEC_TYPE ftype, const char *value);

#ifdef GMSEC_COMPOUND_FIELDS
	/** @fn AddField(Field &f)
	 * @brief This function adds a sub-field to a compound field.  If this
	 * Field is not compound, it results in an error.
	 *
	 * @param field - field to add to this compound field
	 * @return status - result of the operation
	 */
	Status CALL_TYPE AddField(Field &field);
#endif /* GMSEC_COMPOUND_FIELDS */

	/** @fn UnSet()
	 * @brief This clears the message to "UNSET" state.
	 *
	 * @return status - result of the operation
	 */
	Status CALL_TYPE UnSet();

	/* Overloaded operators to simplify comparison of field values
	*****************************************************************/

	/** @fn operator== (Field &other)
	 * @brief Overloaded equals operator.
	 * @param other - other Field object to compare against
	 * @return bool - returns true if fields are the same; otherwise returns false.
	 */
	bool operator== (Field &other);

	/** @fn operator!= (Field &other)
	 * @brief Overloaded not equals operator.
	 * @param other - other Field object to compare against
	 * @return bool - returns true if this field is dissimilar from the other; otherwise returns false.
	 */
	bool operator!= (Field &other);

	/** @fn operator> (Field &other)
	 * @brief Overloaded greater than operator.
	 * @param other - other Field object to compare against
	 * @return bool - returns true if this field has a value greater than the other field; otherwise returns false.
	 */
	bool operator> (Field &other);

	/** @fn operator< (Field &other)
	 * @brief Overloaded less than operator.
	 * @param other - other Field object to compare against
	 * @return bool - returns true if this field has a value less than the other field; otherwise returns false.
	 */
	bool operator< (Field &other);

	/** @fn operator>= (Field &other)
	 * @brief Overloaded greater than or equals operator.
	 * @param other - other Field object to compare against
	 * @return bool - returns true if this field has a value greater than or equal to the other field; otherwise returns false.
	 */
	bool operator>= (Field &other);

	/** @fn operator<= (Field &other)
	 * @brief Overloaded less than or equals operator.
	 * @param other - other Field object to compare against
	 * @return bool - returns true if this field has a value less than or equal to the other field; otherwise returns false.
	 */
	bool operator<= (Field &a);

	/** @fn LookupType(const char *ftype)
	 * @brief Lookup a standard field type identifier by string name. The valid strings
	 * correspond to the last part of the identifier names. ex. "BOOL" => GMSEC_TYPE_BOOL
	 *
	 * @param ftype - string that matches a known GMSEC field type
	 * @return GMSEC field type identifier, or GMSEC_UNSET
	 *
	 * @sa GMSEC_TYPE
	 */
	GMSEC_DEPRECATED static GMSEC_TYPE LookupType(const char *ftype);

	/** @fn ToXML( const char *&outbuf )
	 * @brief This function formats the field value to an XML tag. The buffer
	 * returned is attached to this Field object and has the same scope,
	 * or until the next %ToXML() call.
	 *
	 * @param outbuf - out parameter, field XML
	 * @return status - result of the operation
	 *
	 * @sa Message::ToXML()
	 */
	Status CALL_TYPE ToXML(const char *&outbuf);

	/** @fn FromXML(const char* xml)
	 * @brief This function will set type and value into a Field as specified by the passed in
	 * XML string. The format is compatible with what is used by ToXML().
	 *
	 * @param xml - XML string to parse
	 * @return status - result of the operation
	 */
	Status CALL_TYPE FromXML(const char* xml);

	/** @fn const char* ToJSON()
	 * @brief This function formats the field value to a JSON object.
	 *
	 * @return A JSON formatted string.
	 *
	 * @sa Message::ToJSON()
	 */
	const char* CALL_TYPE ToJSON();

private:
	friend class internal::FieldBuddy;

	Field(internal::BaseField *internal);

	internal::BaseField *ptr;

};

} // namespace gmsec

#endif /* gmsec_Field_h */

