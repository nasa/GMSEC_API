
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseField.h
 *  @brief This file contains the class for managing message fields independent of middleware.
 *
 *
**/

#ifndef gmsec_internal_BaseField_h
#define gmsec_internal_BaseField_h


#include <gmsec_defs.h>
#include <gmsec/Field.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/internal/tinyxml2.h>
#include <gmsec/internal/json-forwards.h>

#include <string>


namespace gmsec
{

class GMSEC_API Field;

namespace internal
{


const int GMSEC_FLAG_BITS_VALUE = 0x8000;



/** @class BaseField
 * @brief This class implements the Field interface and holds the field data.
 * In the future, there may be alternative implementations, but for now
 * it just uses the pre-3.0 implementation.  For example, adding sub-classes
 * IntegerField, RealField would reduce the size of the objects and improve
 * performance.
 * @sa gmsec::Field
*/
class GMSEC_API BaseField
{
private:

	Field *external;

	std::string   fName;     // we use an std::string, instead of char*, for saner memory mgmt
	mutable char *fXmlCache;
	mutable char *fJsonCache;

	GMSEC_TYPE	fType;
	bool		fHeader;

	GMSEC_I8		fI8Value;
	GMSEC_U8		fU8Value;
	GMSEC_BOOL		fBoolValue;
	GMSEC_I16		fI16Value;
	GMSEC_U16		fU16Value;
	GMSEC_I32		fI32Value;
	GMSEC_U32		fU32Value;
	GMSEC_U64		fU64Value;
	GMSEC_I64		fI64Value;
	GMSEC_F32		fF32Value;
	GMSEC_F64		fF64Value;
	std::string     fStrValue;    // we use an std::string, instead of GMSEC_STR, for saner memory mgmt
	GMSEC_BLOB		fBinValue;
	GMSEC_U32		fBinSize;

#ifdef GMSEC_COMPOUND_FIELDS
	std::map<string, BaseField*> fCompoundValue;
#endif


	// internal initialization function
	void Init();

	// For use with FromXML and FromJSON
	Status setValues(const char* name, const char* type, const char* value, const char* head, const char* bits, const char* callingFunction);

public:

	/** @fn BaseField(Field *external)
	 * @brief Create an "UNSET" field
	 */
	BaseField();


	/** @fn BaseField(Field *extnernal, const BaseField &other)
	 * @brief Pseudo-copy constructor
	 */
	BaseField(const BaseField &other);

	/** @fn ~BaseField()
	 * @brief Destructor.
	 */
	virtual ~BaseField();

	Field *getExternal()
	{
		return external;
	}

	/** @fn operator=( BaseField &fd )
	 * @brief (safe) deep copy asignment operator
	 */
	virtual BaseField &operator=(const BaseField &fd);

	/** @fn GetName(const char *&name)
	 * @brief This function returns the name of this field. This scope is the
	 * lifetime of the BaseField object, or until the next %GetName() call.
	 *
	 * @param name - out parameter, the name of this field
	 * @return status - result of the operation
	 */
	virtual Status GetName(const char *&name) const;

	/** @fn SetName(const char *name)
	 * @brief This function sets the field name without changing type/value.
	 *
	 * @param name - the name of this field to set
	 * @return status - result of the operation
	 */
	virtual Status SetName(const char *name);

	/** @fn SetType(GMSEC_TYPE ftype)
	 * @brief This function sets the type for this field
	 *
	 * @param ftype - type of the field to set
	 * @return status - result of the operation
	 */
	virtual Status SetType(GMSEC_TYPE ftype);

	/** @fn GetType(GMSEC_TYPE &ftype)
	 * @brief This Function gets the type identifier for this field.
	 *
	 * @param ftype - out parameter, type of the field
	 * @return status - result of the operation
	 */
	virtual Status GetType(GMSEC_TYPE &ftype) const;

	/** @fn isHeader()
	 * @brief This function determines if this is a header field.
	 * @return status - true if this is a header field.
	 */
	virtual bool isHeader() const;

	/** @fn setHeader()
	 * @brief This function sets whether or not this is a header field.
	 */
	virtual void setHeader(bool flag);

	/** @fn GetValue(GMSEC_CHAR &value)
	 * @brief This function returns the value as GMSEC_CHAR type.
	 * If the value can't be coerced into a GMSEC_CHAR type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_CHAR &value) const;

	/** @fn GetValue(GMSEC_I8 &value)
	 * @brief This function returns the value as GMSEC_I8 type.
	 * If the value can't be coerced into a GMSEC_I8 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_I8 &value) const;

	/** @fn GetValue(GMSEC_U8 &value)
	 * @brief This function returns the value as GMSEC_U8 type.
	 * If the value can't be coerced into a GMSEC_U8 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_U8 &value) const;

	/** @fn GetValue(GMSEC_BOOL &value)
	 * @brief This function returns the value as GMSEC_BOOL type.
	 * If the value can't be coerced into a GMSEC_BOOL type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_BOOL &value) const;

	/** @fn GetValue(GMSEC_I16 &value)
	 * @brief This function returns the value as GMSEC_I16 type.
	 * If the value can't be coerced into a GMSEC_I16 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_I16 &value) const;

	/** @fn GetValue(GMSEC_U16 &value)
	 * @brief This function returns the value as GMSEC_U16 type.
	 * If the value can't be coerced into a GMSEC_U16 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_U16 &value) const;

	/** @fn GetValue(GMSEC_I32 &value)
	 * @brief This function returns the value as GMSEC_I32 type.
	 * If the value can't be coerced into a GMSEC_I32 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_I32 &value) const;

	/** @fn GetValue(GMSEC_U32 &value)
	 * @brief This function returns the value as GMSEC_U32 type.
	 * If the value can't be coerced into a GMSEC_U32 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_U32 &value) const;

	/** @fn GetValue(GMSEC_I64 &value)
	 * @brief This function returns the value as GMSEC_I64 type.
	 * If the value can't be coerced into a GMSEC_I64 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_I64 &value) const;

	/** @fn GetValue(GMSEC_U64 &value)
	 * @brief This function returns the value as GMSEC_U64 type.
	 * If the value can't be coerced into a GMSEC_U64 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_U64 &value) const;

	/** @fn GetValue(GMSEC_F32 &value)
	 * @brief This function returns the value as GMSEC_F32 type.
	 * If the value can't be coerced into a GMSEC_F32 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_F32 &value) const;

	/** @fn GetValue(GMSEC_F64 &value)
	 * @brief This function returns the value as GMSEC_F64 type.
	 * If the value can't be coerced into a GMSEC_F64 type, an error will be returned.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 */
	virtual Status GetValue(GMSEC_F64 &value) const;

	/** @fn GetValue(GMSEC_STR &value)
	 * @brief This function returns the value as GMSEC_STR type.
	 * If the value can't be coerced into a GMSEC_STR type, an error will be returned.
	 *
	 * @note This function allocates memory that is attached to this object.
	 * the client does not need to clean up, but it must be copied to
	 * be stored anywhere. The scope is the lifetime of this BaseField object.
	 *
	 * @param value - out parameter, value of this field
	 * @return status - result of the operation
	 *
	 */
	virtual Status GetValue(GMSEC_STR &value) const;

	/** @fn GetValue(GMSEC_BLOB &value, GMSEC_U32 &size)
	 * @brief This function returns the value as GMSEC_BLOB type.
	 *
	 * @param value - out parameter, value of this field
	 * @param size - out parameter, the length of the array in "value"
	 * @return status - result of the operation
	 *
	 */
	virtual Status GetValue(GMSEC_BLOB &value, GMSEC_U32 &size) const;

	/** @fn GetValue(GMSEC_BIN *value, GMSEC_U32 &size)
	 * @brief This function returns the value as GMSEC_BIN type.
	 *
	 * @param value - out parameter, value of this field
	 * @param size - out parameter, the length of the array in "value"
	 * @return status - result of the operation
	 *
	 */
	virtual Status GetValue(GMSEC_BIN *value, GMSEC_U32 &size) const;

#ifdef GMSEC_COMPOUND_FIELDS
	/** @fn GetValue(BaseFieldIterator &iterator)
	 * @brief This function returns an iterator for the internal fields.
	 * If this field is not a compound field, an error will be returned.
	 *
	 * @param iterator - out parameter, iterator for this field
	 * @return status - result of the operation
	 *
	 */
	virtual Status GetIterator(BaseFieldIterator &iterator) const;
#endif

	/** @fn SetValue(GMSEC_CHAR value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_CHAR to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_CHAR value);

	/** @fn SetValue(GMSEC_I8 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_I8 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_I8 value);

	/** @fn SetValue(GMSEC_U8 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_U8 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_U8 value);

	/** @fn SetValue(GMSEC_BOOL value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_BOOL to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_BOOL value);

	/** @fn SetValue(GMSEC_I16 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_I16 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_I16 value);

	/** @fn SetValue(GMSEC_U16 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_U16 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_U16 value);

	/** @fn SetValue(GMSEC_I32 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_I32 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_I32 value);

	/** @fn SetValue(GMSEC_U32 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_U32 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_U32 value);

	/** @fn SetValue(GMSEC_I64 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_I64 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_I64 value);

	/** @fn SetValue(GMSEC_U64 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_U64 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_U64 value);

	/** @fn SetValue(GMSEC_F32 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_F32 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_F32 value);

	/** @fn SetValue(GMSEC_F64 value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_F64 to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_F64 value);

	/** @fn SetValue(GMSEC_STR value)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_STR to set
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_STR value);

	/** @fn SetValue(GMSEC_BLOB value, GMSEC_U32 size)
	 * @brief This function sets the value & type of this field.
	 *
	 * @param value - value of type GMSEC_BLOB to set
	 * @param size - size of the array in "value"
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_BLOB value, GMSEC_U32 size);

	/** @fn SetValue(GMSEC_TYPE ftype, const char *value)
	 * @brief This function sets the value of type specified with value
	 * parsed from the string
	 *
	 * @param ftype - type of field to set
	 * @param value - string to parse as the type in "ftype"
	 * @return status - result of the operation
	 */
	virtual Status SetValue(GMSEC_TYPE ftype, const char *value);

#ifdef GMSEC_COMPOUND_FIELDS
	/** @fn AddField(BaseField &f)
	 * @brief This function adds a sub-field to a compound field.  If this
	 * BaseField is not compound, it results in an error.
	 *
	 * @param field - field to add to this compound field
	 * @return status - result of the operation
	 */
	virtual Status AddField(BaseField &field);
#endif /* GMSEC_COMPOUND_FIELDS */

	/** @fn UnSet()
	 * @brief This clears the message to "UNSET" state.
	 *
	 * @return status - result of the operation
	 */
	virtual Status UnSet();

	/* Overloaded operators to simplify comparison of field values
	*****************************************************************/

	/** @fn operator== (BaseField &a)
	 * @brief Overloaded equals operator.
	 */
	bool operator== (const BaseField &a) const;

	/** @fn operator< (BaseField &a)
	 * @brief Overloaded less than operator.
	 */
	bool operator< (const BaseField &a) const;


	/** @fn ToXML( const char *&outbuf )
	 * @brief This function formats the field value to an XML tag. The buffer
	 * returned is attached to this BaseField object and has the same scope,
	 * or until the next %ToXML() call.
	 *
	 * @param outbuf - out parameter, field XML
	 * @return status - result of the operation
	 *
	 * @sa Message::ToXML()
	 */
	virtual Status ToXML(const char *&outbuf) const;


	/** @fn FromXML(const char* xml)
	* @brief This function will set type and value into a Field as specified by the passed in
	* XML string. The format is compatible with what is used by ToXML().
	*
	* @param xml - XML string to parse
	* @return status - result of the operation
	*/
	virtual Status FromXML(const char* xml);


	/** @fn FromXML(tinyxml2::XMLElement *element)
	* @brief This function will set type and value into a Field as specified by the passed in
	* by tinyxml2::XMLElement object.
	*
	* @param element - tinyxml2::Element to parse
	* @return status - result of the operation
	*/
	Status FromXML(tinyxml2::XMLElement *element);


	const char* CALL_TYPE ToJSON();


	// For internal use only!
	//
	Status FromJSON(const Json::Value &object);
};



/**
* @class FieldBuddy
* @brief Provides exchange between internal and external fields.
*/
class FieldBuddy
{
public:

	FieldBuddy(BaseField *internal)
			: field(internal)
	{
	}

	~FieldBuddy()
	{
		field.ptr = NULL;
	}

	Field *ptr()
	{
		return &field;
	}

	Field &ref()
	{
		return field;
	}

	static BaseField *getInternal(Field *field)
	{
		return field->ptr;
	}

	static void destroy(Field *field)
	{
		delete field;
	}


private:

	// Declared, but not implemented.
	FieldBuddy(const FieldBuddy &);
	FieldBuddy &operator=(const FieldBuddy &);

	Field field;

};


} // namespace internal
} // namespace gmsec

#endif	// gmsec_internal_BaseField_h


