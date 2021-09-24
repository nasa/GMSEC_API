/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_FIELD_NET_H
#define GMSEC_API_FIELD_NET_H


// C++ API native
#include <gmsec4/field/Field.h>


namespace GMSEC
{
namespace API
{


/// <summary>
/// Base class for all field types.
/// </summary>


public ref class Field abstract
{
public:
	enum class FieldType
	{
		CHAR_TYPE     = 1,
		BOOL_TYPE     = 2,
		I16_TYPE      = 3,
		U16_TYPE      = 4,
		I32_TYPE      = 5,
		U32_TYPE      = 6,
		F32_TYPE      = 7,
		F64_TYPE      = 8,
		STRING_TYPE   = 9,
		BIN_TYPE      = 10,
		I8_TYPE       = 20,
		U8_TYPE       = 21,
		I64_TYPE      = 22,
		U64_TYPE      = 23
	};


	/// <summary>brief destructor</summary>
	virtual ~Field();


	/// <summary>Returns the name of the field object</summary>
	/// <returns>A string containing the field name</returns>
	System::String^ GetName();


	/// <summary>Returns the type of the field object</summary>
	/// <returns>The field type</returns>
	[System::Obsolete("GetType is deprecated; use GetFieldType instead.")]
	FieldType GetType();


	/// <summary>Returns the type of the field object</summary>
	/// <returns>The field type</returns>
	FieldType GetFieldType();


	/// <summary>Returns whether the field object is considered a header field</summary>
	/// <returns>True if a header field; false otherwise</returns>
	bool IsHeader();


	/// <summary>Converts the field object into an XML string representation</summary>
	//  <returns>An XML string</returns>
	virtual System::String^ ToXML();


	/// <summary>Converts the field object into a JSON string representation</summary>
	/// <returns>A JSON string</returns>
	virtual System::String^ ToJSON();


	/// <summary>Attempts to convert the field value into a signed 64-bit integer number representation.</summary>
	/// <returns>Returns the field value as a signed integer.</returns>
	/// <exception cref="GMSEC_Exception">An exception is thrown if the field cannot be successfully converted to an integer.</exception>
	System::Int64 GetIntegerValue();


	/// <summary>Attempts to convert the field value into an unsigned 64-bit integer number representation.</summary>
	/// <returns>Returns the field value as an unsigned integer.</returns>
	/// <exception cref="GMSEC_Exception">An exception is thrown if the field cannot be successfully converted to an integer.</exception>
	System::UInt64 GetUnsignedIntegerValue();


	/// <summary>Attempts to convert the field value into a 64-bit floating point number representation.</summary>
	/// <returns>Returns the field value as a floating point number.</returns>
	/// <exception cref="GMSEC_Exception">An exception is thrown if the field cannot be successfully converted to a double.</exception>
	double GetDoubleValue();


	/// <summary>Attempts to convert the field value into string representation.</summary>
	/// <returns>Returns the field value as a string.</returns>
	/// <exception cref="GMSEC_Exception">An exception is thrown if the field cannot be successfully converted to a string.</exception>
	System::String^ GetStringValue();


protected:
	void RegisterChild(gmsec::api::Field* child);

	bool m_implOwned;


internal:
	gmsec::api::Field* GetChild();


private:
	gmsec::api::Field* m_child;
};

} // end namespace API
} // end namespace GMSEC

#endif
