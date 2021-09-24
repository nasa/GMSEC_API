
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/    
#ifndef Field_Net
#define Field_Net

// managed
#include "Status_Net.h"
#include "GMSEC_Net.h"
#include "GMSECDefs_Net.h"
#include "GMSECErrors_Net.h"

// native
#include "gmsec/Field.h"

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

// Forward References
ref class Message;

/// <summary>
/// This object is a container for GMSEC Fields.  The methods of this class allow
/// the construction and manipulation of a standard GMSEC Field.
/// </summary>
/// <example>
/// <code lang="C++">
/// //Example creation and use: C++ CLI
/// //create field
/// Field^ fd = gcnew Field();
/// //set field name and value
/// Status^ result = fd->SetName("char_field");
/// if( result->IsError() ) { /* handle error */ }
/// result = fd->SetValue((sbyte)0x63);
/// if( result->IsError() ) { /* handle error */ }
/// //add to message (will copy)
/// result = msg->AddField(fd);
/// if( result->IsError() ) { /* handle error */ }
/// </code>
/// <code lang="C#">
/// //Example creation and use: C#
/// //create field
/// Field fd = new Field();
/// //set field name and value
/// Status result = fd.SetName("char_field");
/// if (result.IsError())
///     //handle error
///     result = fd.SetValue((sbyte)0x63);
/// if (result.IsError())
/// {
///     //handle error
/// }
/// //add to message (will copy)
/// result = msg.AddField(fd);
/// if (result.IsError())
/// {
///     //handle error
/// }
/// </code>
/// <code lang="VB.NET">
/// 'Example creation and use: VB
/// 'create field
/// Dim fd As New Field()
/// 'set field name and value
/// Dim result As Status
/// result = fd.SetName("char_field")
/// If (result.IsError()) Then
///     'handle error
/// End If
/// result = fd.SetValue(CType(&amp;H63, SByte))
/// If (result.IsError()) Then
///     'handle error
/// End If
/// 'add to message (will copy)
/// result = msg.AddField(fd)
/// If (result.IsError()) Then
///     'handle error
/// End If
/// </code>
/// <code lang="f#">
/// //Example creation and use: F#
/// //create field
/// let fd = new Field();
/// //set field name and value
/// let result = fd.SetName("char_field");
/// if( result.IsError() ) then () // Handle Error
/// let result2 = fd.SetValue( sbyte 0x63);
/// if( result2.IsError() ) then () // Handle Error
/// //add to message (will copy)
/// let result3 = msg.AddField(fd);
/// if( result3.IsError() ) then () // Handle Error
/// </code>
/// </example>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class Field
{
private:
	gmsec::Field * mImpl;

internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	/// <param name="field">unmanaged implementation to initialize with</param>
	Field(gmsec::Field* field);

	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::Field * GetUnmanImpl();

	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanImpl(gmsec::Field* field);

protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Field();

public:
	/// <summary>Create an "UNSET" field</summary>
	Field();
 	
	/// <summary>Create a named field of type GMSEC_CHAR</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, SByte value);
 	
	/// <summary>Create a named field of type GMSEC_BOOL</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, Boolean value);
 	
	/// <summary>Create a named field of type GMSEC_I16</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, Int16 value);
 	
	/// <summary>Create a named field of type GMSEC_U16</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, UInt16 value);
 	
	/// <summary>Create a named field of type GMSEC_I32</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, Int32 value);
 	
	/// <summary>Create a named field of type GMSEC_U32</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, UInt32 value);
		
	/// <summary>Create a named field of type GMSEC_F32</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, Single value);
 	
	/// <summary>Create a named field of type GMSEC_F64</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, Double value);
 	
	/// <summary>Create a named field of type GMSEC_STRING</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, String ^ value);
 	
	/// <summary>Create a named field of type GMSEC_BIN</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	/// <param name="size">size of the field to set</param>
	Field(const String ^ name, array<Byte>^ value, UInt32 size);
	
	/// <summary>Create a named field of type GMSEC_I64</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="value">value of field to set</param>
	Field(const String ^ name, Int64 value);

	/// <summary>Create a named field of specified type, value parsed from the value string</summary>
	/// <param name="name">name of field to set</param>
	/// <param name="ftype">target field type to set</param>
	/// <param name="value">string to parse as the type in "ftype"</param>
	Field(const String ^ name, GMSECTypeDefs ftype, const String ^ value);
	
	/// <summary>(safe) deep copy constructor</summary>
	/// <param name="field">Field to set impl with</param>
	Field(Nasa::Gmsec::Net::Field ^ field);
	
	/// <summary>Destructor</summary>
	~Field();
	
	/// <summary>(safe) deep copy asignment operator</summary>
	/// <param name="field">Field to set impl with</param>
	/// <returns>New field with copied value</returns>
	Field ^ operator=(Nasa::Gmsec::Net::Field ^ field);
 	
	/// <summary>This function returns the name of this field. This scope is the
	/// lifetime of the Field object, or until the next %GetName() call.</summary>
	/// <param name="name">out parameter, the name of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetName([Runtime::InteropServices::Out] const String ^ %name);
 	
	/// <summary>This function sets the field name without changing type/value.</summary>
	/// <param name="name">the name of this field to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetName(const String ^ name);
 	
	/// <summary>This function sets the type for this field</summary>
	/// <param name="ftype">type of the field to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetType(GMSECTypeDefs ftype);

	/// <summary>This Function gets the type identifier for this field.</summary>
	/// <param name="ftype">out parameter, type of the field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetType([Runtime::InteropServices::Out] GMSECTypeDefs% ftype);
 	
	/// <summary>This function determines if the field is a header field.</summary>
	/// <returns>status - true if this is a header field.</returns>
	bool IsHeader();
	
	/// <summary>This function sets whether or not the field is a header field.</summary>
	/// <param name="isHeader">if set to header field</param>
	void SetHeader(bool isHeader);

	/// <summary>This function returns the value as GMSEC_CHAR type.
	/// If the value can't be coerced into a GMSEC_CHAR type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] SByte% value);
	
	/// <summary>This function returns the value as GMSEC_BOOL type.
	/// If the value can't be coerced into a GMSEC_BOOL type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] Boolean% value);
	
	/// <summary>This function returns the value as GMSEC_BIN type.
	/// If the value can't be coerced into a GMSEC_BIN type, an error will be returned.</summary>
	/// <remarks>This function allocates memory that is attached to this object.
	/// the client does not need to clean up, but it must be copied to
	/// be stored anywhere. The scope is the lifetime of this Field object.
	/// </remarks>
	/// <param name="value">byte array value</param>
	/// <param name="size">size of byte array</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] array<Byte>^% value, [Runtime::InteropServices::Out] UInt32% size);
 	
	/// <summary>This function returns the value as GMSEC_I16 type.
	/// If the value can't be coerced into a GMSEC_I16 type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] Int16% value);
 	
	/// <summary>This function returns the value as GMSEC_U16 type.
	/// If the value can't be coerced into a GMSEC_U16 type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] UInt16% value);
 	
	/// <summary>This function returns the value as GMSEC_I32 type.
	/// If the value can't be coerced into a GMSEC_I32 type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] Int32% value);
 	
	/// <summary>This function returns the value as GMSEC_U32 type.
	/// If the value can't be coerced into a GMSEC_U32 type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] UInt32% value);
	
	/// <summary>This function returns the value as GMSEC_I64 type.
	/// If the value can't be coerced into a GMSEC_I64 type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] Int64% value);
 	
	/// <summary>This function returns the value as GMSEC_F32 type.
	/// If the value can't be coerced into a GMSEC_F32 type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] Single% value);
 	
	/// <summary>This function returns the value as GMSEC_F64 type.
	/// If the value can't be coerced into a GMSEC_F64 type, an error will be returned.</summary>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] Double% value);

	/// <summary>This function returns the value as GMSEC_STRING type
	/// If the value can't be coerced into a GMSEC_STRING type, an error will be returned.</summary>
	/// <remarks>
	/// This function allocates memory that is attached to this object.
	/// the client does not need to clean up, but it must be copied to
	/// be stored anywhere. The scope is the lifetime of this Field object.
	/// </remarks>
	/// <param name="value">out parameter, value of this field</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue([Runtime::InteropServices::Out] String ^ %value);
 	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_CHAR to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(SByte value);
 	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_BOOL to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(Boolean value);
 	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_I16 to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(Int16 value);
 	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_U16 to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(UInt16 value);
 	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_I32 to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(Int32 value);
 	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_U32 to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(UInt32 value);
	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_I64 to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(Int64 value);
	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type byte array</param>
	/// <param name="size">size to use from input array</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(array<Byte>^ value, UInt32 size);
 	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_F32 to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(Single value);
 	
	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_F64 to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(Double value);

	/// <summary>This function sets the value and type of this field.</summary>
	/// <param name="value">value of type GMSEC_STRING to set</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetValue(String ^ value);
	
	/// <summary>This function sets the value of type specified with value
	/// parsed from the string</summary>
	/// <param name="ftype">type</param>
	/// <param name="value">value as string</param>
	/// <returns>status - result of the operation</returns>
	/// <seealso cref="GMSECTypeDefs" />
	Nasa::Gmsec::Net::Status ^ SetValue(GMSECTypeDefs ftype, const String ^ value);

	/// <summary>This clears the message to "UNSET" state.</summary>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ UnSet();
	
	/// <summary>Overloaded equals operator.</summary>
	/// <remarks>
	/// field = null
	/// field-&gt;getUnmanImpl() = null
	/// in this case the operator will not equate them
	/// but field = null and field1 = null will equate, as will field-&gt;getUnmanImpl() = null and field2-&gt;getUnmanImpl() = null
	/// </remarks>
	/// <param name="a">Field 1</param>
	/// <param name="b">Field 2</param>
	/// <returns>if Field value is equal</returns>
	static bool operator==(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b);
	
	/// <summary>Overloaded not equals operator.</summary>
	/// <remarks>
	/// field = null
	/// field-&gt;getUnmanImpl() = null
	/// in this case the operator will think not equal and return true
	/// but field = null and field1 = null will be false, as will field-&gt;getUnmanImpl() = null and field2-&gt;getUnmanImpl() = null
	/// </remarks>
	/// <param name="a">Field 1</param>
	/// <param name="b">Field 2</param>
	/// <returns>if Field value is not equal</returns>
	static bool operator!=(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b);
	
	/// <summary>Overloaded greater than operator.</summary>
	/// <remarks>all null comparisons whether managed or impl will return false</remarks>
	/// <param name="a">Field 1</param>
	/// <param name="b">Field 2</param>
	/// <returns>if Field value is greater than</returns>
	static bool operator>(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b);
	
	/// <summary>Overloaded less than operator.</summary>
	/// <remarks>
	/// all null comparisons whether managed or impl will return false
	/// </remarks>
	/// <param name="a">Field 1</param>
	/// <param name="b">Field 2</param>
	/// <returns>if Field value is less than</returns>
	static bool operator<(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b);
	
	/// <summary>Overloaded greater than or equals operator.</summary>
	/// <remarks>
	/// field = null
	/// field-&gt;getUnmanImpl() = null
	/// in this case the operator will not be true
	/// but field = null and field1 = null will equate, as will field-&gt;getUnmanImpl() = null and field2-&gt;getUnmanImpl() = null
	/// all other null comparisons will yield false
	/// </remarks>
	/// <param name="a">Field 1</param>
	/// <param name="b">Field 2</param>
	/// <returns>if Field value is greater than or equals</returns>
	static bool operator>=(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b);

	/// <summary>Overloaded less than or equals operator.</summary>
	/// <remarks>
	/// field = null
	/// field-&gt;getUnmanImpl() = null
	/// in this case the operator will not be true
	/// but field = null and field1 = null will equate, as will field-&gt;getUnmanImpl() = null and field2-&gt;getUnmanImpl() = null
	/// all other null comparisons will yield false
	/// </remarks>
	/// <param name="a">Field 1</param>
	/// <param name="b">Field 2</param>
	/// <returns>if Field value is less than or equals</returns>
	static bool operator<=(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b);
	
	/// <summary>Lookup a standard field type identifier by string name. The valid strings
	/// correspond to the last part of the identifier names. ex. "BOOL" =&gt; BOOL</summary>
	/// <seealso cref="GMSECTypeDefs" />
	/// <param name="ftype">string that matches a known GMSEC field type</param>
	/// <returns>GMSEC field type identifier, or UNSET</returns>
	GMSECTypeDefs LookupType(const String ^ ftype);
	
	
	/// <summary>This function formats the field value to an XML tag. The buffer
	/// returned is attached to this Field object and has the same scope,
	/// or until the next ToXML() call.</summary>
	/// <param name="xml">out parameter, field XML</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ ToXML([Runtime::InteropServices::Out] const String ^ %xml);
};

}
}
}

#endif  // Field_Net

