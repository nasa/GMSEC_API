
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
#ifndef Message_Net
#define Message_Net

// managed
#include "Config_Net.h"
#include "Field_Net.h"
#include "FieldIterator_Net.h"
#include "GMSEC_Net.h"
#include "GMSECDefs_Net.h"
#include "GMSECErrors_Net.h"

// native
#include "gmsec/Message.h"

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

// forward references
ref class ConfigFile;
ref class Connection;

/// <summary>
/// The Message object is a container for GMSEC Messages.  The methods of this class
/// allow the construction and manipulation of the data in a message.
/// </summary>
/// <example>
/// <code lang="C++">
/// //Example creation and use:
/// Status^ result;
/// Message^ msg = nullptr;
/// result = conn->CreateMessage("GMSEC.TEST.PUBLISH", GMSECMsgKindDefs::PUBLISH, msg);
/// if( result->IsError() ) { /* handle error */ }
/// // send message
/// result = conn->Publish(msg);
/// if( result->IsError() ) { /* handle error */ }
/// result = conn->DestroyMessage(msg);
/// if( result->IsError() ) { /* handle error */ }
/// </code>
/// <code lang="C#">
/// //Example creation and use:
/// Status result;
/// Message msg = null;
/// result = conn.CreateMessage("GMSEC.TEST.PUBLISH", GMSECMsgKindDefs.PUBLISH, out msg);
/// if (result.IsError()) { /* handle error */ }
/// // send message
/// result = conn.Publish(msg);
/// if (result.IsError()) { /* handle error */ }
/// result = conn.DestroyMessage(msg);
/// if (result.IsError()) { /* handle error */ }
/// </code>
/// <code lang="VB.NET">
/// //Example creation and use:
/// Dim result As Status
/// Dim msg As Message
/// msg = Nothing
/// result = conn.CreateMessage("GMSEC.TEST.PUBLISH", CShort(GMSECMsgKindDefs.PUBLISH), msg)
/// If (result.IsError()) Then
///     'handle error
/// End If
/// ' send message
/// result = conn.Publish(msg)
/// If (result.IsError()) Then
///     'handle error
/// End If
/// result = conn.DestroyMessage(msg)
/// If (result.IsError()) Then
///     'handle error
/// End If
/// </code>
/// <code lang="f#">
/// //Example creation and use:
/// let (result, msg) = conn.CreateMessage("GMSEC.TEST.PUBLISH", GMSECMsgKindDefs.PUBLISH);
/// if( result.IsError() ) then () // Handle Error
/// // send message
/// let result2 = conn.Publish(msg);
/// if( result2.IsError() ) then () // Handle Error
/// let result3 = conn.DestroyMessage(msg);
/// if( result3.IsError() ) then () // Handle Error
/// </code>
/// </example>
/// <seealso cref="Connection" />
/// <seealso cref="Field" />
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class Message
{
private:
	gmsec::Message * mImpl;

	/// <summary>Default constructor</summary>
	Message();

	/// <summary>Destructor</summary>
	~Message();

internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	/// <param name="msg">unmanaged implementation to initialize with</param>
	Message(gmsec::Message * msg);

	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::Message * GetUnmanImpl();
	
	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanImpl(gmsec::Message* message);

protected:
	/// <summary>Finalizer, set objects to null</summary>
	!Message();

public:
	/// <summary>This function will check if the message is Valid.</summary>
	/// <returns>true if the message is valid</returns>
	bool IsValid();
	
	/// <summary>This function identifies the root library name and therefore the
	/// connection type that this message is associated with. This information
	/// must match the library root name of the connection in order to be sent
	/// on that connection.</summary>
	/// <returns>middleware implementation library root name</returns>
	String ^ GetLibraryRootName();

	/// <summary>This function sets the Configuration of this message.  This can be used to
	/// pass middleware specific items to the Message object.
	/// </summary>
	/// <remarks>
	/// <para>All messages implmentations understand the following config values:</para>
	/// <para>"subject"	-	sets the message subject</para>
	/// <para>"kind"		-	sets the message kind</para>
	/// <para>Configuration values from the passed in config will be copied or set within the message. Therefore
	/// the pased in config WILL NOT BE STORED and is the responsibility of the client program to clean up.</para>
    /// </remarks>
	/// <param name="config">configuration object that contains values to us</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetConfig(Nasa::Gmsec::Net::Config ^ config);
	
	/// <summary>This function set the 'kind' of this message.</summary>
	/// <param name="kind">valid message kind</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetKind(GMSECMsgKindDefs kind);
	
	/// <summary>This function get the 'kind' of this message.</summary>
	/// <param name="kind">out parameter, current message kind</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetKind( 
		[Runtime::InteropServices::Out] GMSECMsgKindDefs %kind);
	
	/// <summary>This function set the subject to publish/request this message upon.</summary>
	/// <param name="subject">valid message subject name</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ SetSubject(const String ^ subject);
	
	/// <summary>This function get the subject this message will be/was published upon.</summary>
	/// <param name="subject">out parameter, subject of the message</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetSubject([Runtime::InteropServices::Out] const String ^ %subject);
	
	/// <summary>This function will remove all fields form this message.</summary>
	/// <remarks>
	/// <b>This may not be available in all middleware implementations.</b>
	/// </remarks>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ ClearFields();

	/// <summary>This function will add a new field to this message. A new field will be added to the message
	/// that matches the field passed in. Therefore the field is owned by the client program and can be reused.
	/// The client program must also clean up that field when no longer needed.</summary>
	/// <example>
	/// <code lang="C++">
	/// Status^ result;
	/// Field^ fd = gcnew Field();
	/// //set field name and value
	/// result = fd->SetName("char_field");
	/// if( result->IsError() ) { /* handle error */ }
	/// result = fd->SetValue((sbyte) 0x63);
	/// if( result->IsError() ) { /* handle error */ }
	/// //add to message (will copy)
	/// result = msg->AddField(fd);
	/// if( result->IsError() ) { /* handle error */ }
	/// </code>
	/// <code lang="C#">
	/// Status result;
	/// Field fd = new Field();
	/// //set field name and value
	/// result = fd.SetName("char_field");
	/// if (result.IsError()) { /* handle error */ }
	/// result = fd.SetValue((sbyte) 0x63);
	/// if (result.IsError()) { /* handle error */ }
	/// //add to message (will copy)
	/// result = msg.AddField(fd);
	/// if (result.IsError()) { /* handle error */ }
	/// </code>
	/// <code lang="VB.NET">
	/// Dim result As Status
	/// Dim fd As New Field()
	/// 'set field name and value
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
	///     'handle 
	/// </code>
	/// <code lang="f#">
	/// let fd = new Field();
	/// //set field name and value
	/// let result = fd.SetName("char_field");
	/// if( result.IsError() ) then () // Handle Error
	/// let result2 = fd.SetValue(0x63);
	/// if( result2.IsError() ) then () // Handle Error
	/// //add to message (will copy)
	/// let result3 = msg.AddField(fd);
	/// if( result3.IsError() ) then () // Handle Error
	/// </code>
	/// </example>
	/// <param name="field">field object to copy info from</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ AddField(Nasa::Gmsec::Net::Field ^ field);
	
	/// <summary>This function will remove a field from this message by name.</summary>
	/// <remarks>
	/// <b>This may not be available in all middleware implementations.</b>
	/// </remarks>
	/// <param name="name"> name of field to remove</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ ClearField(const String ^ name);
	
	/// <summary>This function will copy a field within a message into a field created by the client program, by name.</summary>
	/// <example>
	/// <code lang="C++">
	/// String^ value = nullptr;
	/// Field^ fd;
	/// Status^ result = msg->GetField("char_field",fd);
	/// if (result->IsError())
	/// {
	/// 	//handle error
	/// }
	/// result = fd->GetValue(value);
	/// if (result->IsError())
	/// {
	/// 	//handle error
	/// }
	/// Console::WriteLine("value = " + value);
	/// </code>
	/// <code lang="C#">
	/// String value = null;
	/// Field fd;
	/// Status result = msg.GetField("char_field", out fd);
	/// if (result.IsError())
	/// {
	/// 	//handle error
	/// }
	/// result = fd.GetValue(out value);
	/// if (result.IsError())
	/// {
	/// 	//handle error
	/// }
	///Console.WriteLine("value = " + value);
	/// </code>
	/// <code lang="VB.NET">
	/// Dim value As String
	/// value = Nothing
	/// Dim fd As Field
	/// fd = Nothing
	/// Dim result As Status
	/// result = msg.GetField("char_field", fd)
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// result = fd.GetValue(value)
	/// If (result.IsError()) Then
	///     'handle error
	/// End If	/// Console.WriteLine("value = " + value)
	/// </code>
	/// <code lang="f#">
	/// let (result, fd) = msg.GetField("char_field");
	/// if( result.IsError() ) then () // Handle Error
	/// let (result2, value : sbyte) = fd.GetValue();
	/// if( result2.IsError() ) then () // Handle Error
	/// Console.WriteLine("value = " + value.ToString());
	/// </code>
	/// </example>
	/// <param name="name">name of field to get</param>
	/// <param name="field">out parameter, field object to copy info into</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetField(const String ^ name, 
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Field ^% field);
	
	/// <summary>This function gets the number of fields contained in this message.</summary>
	/// <param name="count">out parameter, number of fields in this message</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetFieldCount([Runtime::InteropServices::Out] Int32 %count);
	
	/// <summary>This function and GetNextField() can be used to iterate through the fields of a message.</summary>
	/// <example>
	/// <code lang="C++">
	/// C++ CLI
	///  Field^ fd;
	///  // get first field
	///  Status^ result = msg->GetFirstField(fd);
	///  while (!result->IsError())
	///  {
	/// 	// do something with the field here
	/// 
	/// 	// get next field
	/// 	result = msg->GetNextField(fd);
	///  }
	/// </code>
	/// <code lang="C#">
	/// C#
	/// Field fd;
	/// // get first field
	/// Status result = msg.GetFirstField(out fd);
	/// while (!result.IsError())
	/// {
	///     // do something with the field here
	/// 
	///     // get next field
	///     result = msg.GetNextField(out fd);
	/// }
	/// </code>
	/// <code lang="VB.NET">
	/// 'VB
	/// Dim fd As Field
	/// fd = Nothing
	/// ' get first field
	/// Dim result As Status
	/// result = msg.GetFirstField(fd)
	/// While (Not result.IsError())
	///     ' do something with the field here
	/// 
	///     ' get next field
	///     result = msg.GetNextField(fd)
	/// End While
	/// </code>
	/// <code lang="f#">
	/// // get first field
	/// let (result, fd) = msg.GetFirstField()
	/// let mutable IsError = result.IsError()
	/// let mutable fdTop = fd
	/// let mutable resultTop = result;
	/// while not IsError do
	///     // do something with the field here
	///     // get next field
	///     let (result2, fd2) = msg.GetNextField()
	///     fdTop &lt;- fd2
	///     resultTop &lt;- result2
	///     IsError &lt;- resultTop.IsError()
	/// </code>
	/// </example>
	/// <seealso cref="GetNextField()" />
	/// <param name="field">field object to copy field into</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetFirstField([Runtime::InteropServices::Out] Nasa::Gmsec::Net::Field ^% field);

	/// <summary>This function and GetFirstField() are used to iterate through the fields of a message.</summary>
	/// <seealso cref="GetFirstField()" />
	/// <param name="field">field object to copy field into</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetNextField([Runtime::InteropServices::Out] Nasa::Gmsec::Net::Field ^% field);
	
	/// <summary>This function will copy all fields from this message to a new message in the fastest way possible</summary>
	/// <param name="toMsg">the message to copy fields into</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ CopyFields(Nasa::Gmsec::Net::Message^ toMsg);
	
	/// <summary>This function will copy all fields from this message to a new message in the fastest way possible
	/// the format used by FromXML() and ConfigFile::GetMessage().</summary>
	/// <code lang="xml" xml:space="preserve">
	/// <MESSAGE SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
	/// 	<FIELD TYPE="CHAR" NAME="char_field">c</FIELD>
	/// 	<FIELD TYPE="BOOL" NAME="bool_field">TRUE</FIELD>
	/// 	<FIELD TYPE="I16" NAME="S">-123</FIELD>
	/// 	<FIELD TYPE="U16" NAME="T">123</FIELD>
	/// 	<FIELD TYPE="I32" NAME="I">-12345678</FIELD>
	/// 	<FIELD TYPE="U32" NAME="J">123456789</FIELD>
	/// 	<FIELD TYPE="STRING" NAME="S">This is a test</FIELD>
	/// 	<FIELD TYPE="F32" NAME="F">123.45</FIELD>
	/// 	<FIELD TYPE="F64" NAME="G">1.234e56</FIELD>
	/// 	<FIELD TYPE="BIN" NAME="X">4a4c4d4e4f5051</FIELD>
	/// </MESSAGE>
	/// </code>
	/// <param name="xml">out parameter, xml string</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ ToXML([Runtime::InteropServices::Out] const String ^ %xml);
	
	/// <summary>This function will set subject, kind, and fields into a message as specified by the passed in
	/// xml string. The format is compatible with what is used by ToXML() and ConfigFile::GetMessage();</summary>
	/// <param name="xml">xml string to parse</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ FromXML(const String ^ xml);
	
	/// <summary>This function will return the physical storage size of the message in bytes.</summary>
	/// <remarks>
	/// <b>This may not be available in all middleware implementations.</b>
	/// </remarks>
	/// <param name="size">out parameter, size of message in bytes</param>
	/// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetMSGSize([Runtime::InteropServices::Out] UInt32 %size);
	
	/// <summary>Lookup a standard message kind identifier by string name. The valid strings
	/// correspond to the last part of the identifier names. ex. "PUBLISH"</summary>
	/// <seealso cref="GMSECMsgKindDefs" />
	/// <param name="kind">string that matches a known GMSEC message type</param>
	/// <returns>GMSEC message kind identifier, or UNSET</returns>
	GMSECMsgKindDefs LookupKind(const String ^ kind);
	
	/// <summary>Resets a field iterator object to start from the beginning of a message's fields.</summary>
	/// <seealso cref="GMSECMsgKindDefs" />
	/// <param name="iter">The FieldIterator object</param>
	/// <returns>result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ Reset(FieldIterator^ iter);
};

}
}
}

#endif  // Message_Net

