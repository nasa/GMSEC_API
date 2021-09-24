/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MESSAGE_NET_H
#define GMSEC_API_MESSAGE_NET_H


// Managed
#include <MessageFieldIterator_Net.h>
#include <field/Field_Net.h>


// C++ API native
#include <gmsec4/Message.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Config;
ref class BinaryField;
ref class BooleanField;
ref class CharField;
ref class F32Field;
ref class F64Field;
ref class I8Field;
ref class I16Field;
ref class I32Field;
ref class I64Field;
ref class StringField;
ref class U8Field;
ref class U16Field;
ref class U32Field;
ref class U64Field;


/// <summary>
/// The Message object is a container for GMSEC Messages.  The methods of this class allow for the
//  construction and manipulation of the data in a message.
/// </summary>


public ref class Message
{
public:
	enum class MessageKind
	{
		PUBLISH = 1,
		REQUEST = 2,
		REPLY   = 3
	};


	/// <summary>Basic message Constructor</summary>
	///
	/// <param name="subject">The subject/topic to associate with the message</param>
	/// <param name="kind">The message kind to associate with the message</param>
	///
	/// <exception cref="GMSEC_Exception">If the subject is null or contains an empty-string</exception>
	Message(System::String^ subject, MessageKind kind);


	/// <summary>%Message constructor that accepts a configuration</summary>
	///
	/// <note>Only Message-related configuration options are held by the Message object.
	/// These configuration options are NOT included with the message when it is sent
	/// across the GMSEC Bus.</note>
	///
	/// <param name="subject">The subject/topic to associate with the message</param>
	/// <param name="kind">The message kind to associate with the message</param>
	/// <param name="config">A configuration object that contains message-specific options</param>
	///
	/// <exception cref="GMSEC_Exception">If the subject is null or contains an empty-string</exception>
	Message(System::String^ subject, MessageKind kind, Config^ config);


	/// <summary>Specialized constructor that initializes the message instance using either
	/// XML or JSON text</summary>
	///
	/// <param name="data">An XML or JSON formatted string</param>
	///
	/// <exception cref="GMSEC_Exception">If the subject is null or contains an empty-string</exception>
	Message(System::String^ data);


	/// <summary>Basic (and safe) deep copy constructor</summary>
	///
	/// <param name="other">Message object to copy</param>
	Message(Message^ other);


	/// <summary>Destructor</summary>
	~Message();


	/// <summary>Returns message kind</summary>
	MessageKind GetKind();


	/// <summary>Returns message subject</summary>
	System::String^ GetSubject();


	/// <summary>Allows for the setting/changing of the message subject</summary>
	///
	/// <param name="subject">The subject/topic to associate with the message</param>
	///
	/// <exception cref="GMSEC_Exception">If the subject is null or contains an empty-string</exception>
	void SetSubject(System::String^ subject);


	/// <summary>Returns configuration associcated with the message</summary>
	Config^ GetConfig();


	/// <summary>Associatates a configuration object with the message</summary>
	///
	/// <note>Only Message-related configuration options are held by the Message object.
	/// These configuration options are NOT included with the message when it is sent
	/// across the GMSEC Bus.</note>
	///
	/// <param name="config">The configuration object</param>
	/// <exception cref="GMSEC_Exception">If the Config object is null</exception>
	void AddConfig(Config^ config);


	/// <summary>Adds a copy of the field object to the message</summary>
	/// <param name="field">The field object</param>
	/// <returns>True if a new field was added; false if an existing field was replaced.</returns>
	/// <exception cref="GMSEC_Exception">If the Field object is null</exception>
	bool AddField(Field^ field);


	/// <summary>Adds each Field in the given list to the Message.
	/// <param name="fields">The list of field objects</param>
	/// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
	/// <exception cref="GMSEC_Exception">If the Field list is null</exception>
	bool AddFields(System::Collections::Generic::List<Field^>^ fields);


	/// <summary>Removes all fields associated with the message</summary>
	void ClearFields();


	/// <summary>Removes the fields, identified by name, that is associated with the message</summary>
	/// <param name="name">The name of the field to remove</param>
	/// <returns>True if the operation was a success; false otherwise.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null or contains an empty-string.</exception>
	bool ClearField(System::String^ name);


	/// <summary>Attempts to convert the field value into a signed 64-bit integer number representation.</summary>
	/// <param name="fieldName">The name of the field to access</param>
	/// <returns>Returns the field value as a signed integer.</returns>
	/// <exception cref="GMSEC_Exception">An Exception is thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
	System::Int64 GetIntegerValue(System::String^ fieldName);


	/// <summary>Attempts to convert the field value into an unsigned 64-bit integer number representation.</summary>
	/// <param name="fieldName">The name of the field to access</param>
	/// <returns>Returns the field value as an unsigned integer.</returns>
	/// <exception cref="GMSEC_Exception">An Exception is thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
	System::UInt64 GetUnsignedIntegerValue(System::String^ fieldName);


	/// <summary>Attempts to convert the field value into a 64-bit floating point number representation.</summary>
	/// <param name="fieldName">The name of the field to access</param>
	/// <returns>Returns the field value as a floating point number.</returns>
	/// <exception cref="GMSEC_Exception">An Exception is thrown if the field cannot be found, or if it cannot successfully be converted to a double.</exception>
	double GetDoubleValue(System::String^ fieldName);


	/// <summary>Attempts to convert the field value into string representation.</summary>
	/// <param name="fieldName">The name of the field to access</param>
	/// <returns>Returns the field value as a string.</returns>
	/// <exception cref="GMSEC_Exception">An Exception is thrown if the field cannot be found, or if it cannot successfully be converted to a string.</exception>
	System::String^ GetStringValue(System::String^ fieldName);


	/// <summary>Returns the field, identified by name, that is associated with the message</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>Returns a handle to a field on success, or null on lookup failure.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null or contains an empty-string.</exception>
	Field^ GetField(System::String^ name);


	/// <summary>Returns the type of the field, identified by name, that is associated with the message</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>Returns the field type.</returns>
	/// <seealso Field::FieldType/>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string or if the field cannot be referenced.</exception>
	Field::FieldType GetFieldType(System::String^ name);


	/// <summary>Returns a handle to the BinaryField, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the BinaryField.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a BinaryField.</exception>
	BinaryField^ GetBinaryField(System::String^ name);


	/// <summary>Returns a handle to the BooleanField, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the BooleanField.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a BooleanField.</exception>
	BooleanField^ GetBooleanField(System::String^ name);


	/// <summary>Returns a handle to the CharField, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the CharField.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a CharField.</exception>
	CharField^ GetCharField(System::String^ name);


	/// <summary>Returns a handle to the F32Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the F32Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a F32Field.</exception>
	F32Field^ GetF32Field(System::String^ name);


	/// <summary>Returns a handle to the F64Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the F64Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a F64Field.</exception>
	F64Field^ GetF64Field(System::String^ name);


	/// <summary>Returns a handle to the I8Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the I8Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a I8Field.</exception>
	I8Field^ GetI8Field(System::String^ name);


	/// <summary>Returns a handle to the I16Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the I16Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a I16Field.</exception>
	I16Field^ GetI16Field(System::String^ name);


	/// <summary>Returns a handle to the I32Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the I32Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a I32Field.</exception>
	I32Field^ GetI32Field(System::String^ name);


	/// <summary>Returns a handle to the I64Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the I64Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a I64Field.</exception>
	I64Field^ GetI64Field(System::String^ name);


	/// <summary>Returns a handle to the U8Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the U8Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a U8Field.</exception>
	U8Field^ GetU8Field(System::String^ name);


	/// <summary>Returns a handle to the U16Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the U16Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a U16Field.</exception>
	U16Field^ GetU16Field(System::String^ name);


	/// <summary>Returns a handle to the U32Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the U32Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a U32Field.</exception>
	U32Field^ GetU32Field(System::String^ name);


	/// <summary>Returns a handle to the U64Field, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the U64Field.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a U64Field.</exception>
	U64Field^ GetU64Field(System::String^ name);


	/// <summary>Returns a handle to the StringField, as identified by the given name.</summary>
	/// <param name="name">The name of the field to fetch</param>
	/// <returns>On success, returns a handle to the StringField.</returns>
	/// <exception cref="GMSEC_Exception">If the subject is null, contains an empty-string if the field is non-existent,
	/// or cannot be interpreted as a StringField.</exception>
	StringField^ GetStringField(System::String^ name);


	/// <summary>Returns the number of fields associated with the message.</summary>
	int GetFieldCount();


	/// <summary>Copies the fields associated with the message to another message.</summary>
	/// <param name="toMsg">The destination message where to copy the fields.</param>
	void CopyFields(Message^ toMsg);


	/// <summary>Returns the XML string representation of the message.</summary>
	System::String^ ToXML();


	/// <summary>Returns the JSON string representation of the message.</summary>
	System::String^ ToJSON();


	/// <summary>Returns the size of the message, in bytes.</summary>
	long GetSize();


	/// <summary>
	/// Method that allows the callee to get the MessageFieldIterator associated with the %Message.
	/// This iterator will allow for applications to iterate over the Field objects stored within the %Message.
	/// The iterator is reset each time GetFieldIterator() is called.  The iterator itself is destroyed when the
	/// %Message object is destroyed.
	///</summary>
	///
	/// <note>
	/// Only one MessageFieldIterator object is associated with a %Message object; multiple calls to
	/// GetFieldIterator() will return a reference to the same MessageFieldIterator object.  Each call will reset the iterator.
	/// </note>
	///
	/// <returns>A reference to a MessageFieldIterator object.</returns>
	MessageFieldIterator^ GetFieldIterator();


	/// <summary>Returns a handle to the specialized MessageFieldIterator, which can then be used to iterate over the Fields contained within the message object.</summary>
	/// <param name="selector">The specialized type of MessageFieldIterator to return.</param>
	/// <seealso cref="MessageFieldIterator::Selector"/>
	MessageFieldIterator^ GetFieldIterator(MessageFieldIterator::Selector selector);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Message();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="msg">unmanaged implementation to initialize with</param>
	Message(gmsec::api::Message* msg, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::Message* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::Message* msg, bool owned);


protected:
	gmsec::api::Message* m_impl;
	bool                 m_owned;
};

} // end namespace API
} // end namespace GMSEC

#endif
