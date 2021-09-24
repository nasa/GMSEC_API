/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MIST_MESSAGE_NET_H
#define GMSEC_API_MIST_MESSAGE_NET_H


#include <Message_Net.h>


// C++ API native
#include <gmsec4/mist/message/MistMessage.h>


namespace GMSEC
{
namespace API
{
	// Forward declaration(s)
	ref class Config;
	ref class Field;
	ref class Message;

namespace MIST
{
	// Forward declaration(s)
	ref class Specification;

namespace MESSAGE
{


/// <summary>
/// This Message object shares all of the functionality of standard messages,
/// but can also be auto-populated based on how the message schema identifies itself within
/// the given specification.
/// </summary>
///
/// <seealso cref="Message"/>
/// <seealso cref="Config"/>
/// <seealso cref="Specification"/>

public ref class MistMessage : public Message
{
public:
	/// <summary>Initializes the message instance with a template determined by ID and spec.</summary>
	///
	/// <param name="subject">The subject string for the message.</param>
	///
	/// <param name="schemaID">
	/// The string used to identify the message schema in C2MS or other message specification.
	/// (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
	/// </param>
	///
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	/// </exception>
	MistMessage(System::String^ subject, System::String^ schemaID, Specification^ spec);


	/// <summary>Initializes the message instance with a template determined by ID and spec.</summary>
	///
	/// <param name="subject">The subject string for the message.</param>
	///
	/// <param name="schemaID">
	/// The string used to identify the message schema in C2MS or other message specification.
	/// (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
	/// </param>
	///
	/// <param name="config">A configuration to associate with the message.</param>
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	/// </exception>
	MistMessage(System::String^ subject, System::String^ schemaID, Config^ config, Specification^ spec);


	/// <summary>Copy constructor - initializes the message instance using the other given MistMessage</summary>
	///
	/// <param name="other">The message to be copied.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given MistMessage is null.
	/// </exception>
	MistMessage(MistMessage^ other);


	/// <summary>Special constructor that constructs a MistMessage using a simple Message</summary>
	///
	/// <param name="msg">The simple Message to reference while building the MistMessage</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given Message object is null, or if the given Message does not house sufficient information to construct a MistMessage.
	/// </exception>
	MistMessage(Message^ msg);


	/// <summary>Special constructor that constructs a MistMessage using a simple Message</summary>
	///
	/// <param name="msg">The simple Message to reference while building the MistMessage</param>
	///
	/// <param name="specConfig">The specification configuration to apply when constructing the MistMessage</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if either the given Message or Config object is null, or if the given Message does not house sufficient information to construct a MistMessage.
	/// </exception>
	MistMessage(Message^ msg, Config^ specConfig);


	/// <summary>Initializes a MistMessage using an XML or JSON string.</summary>
	///
	/// <param name="data">XML or JSON string used to initialize the message.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
	/// </exception>
	///
	/// <remarks>This method has been deprecated; use MistMessage(Specification^, System::String^) instead.
	MistMessage(System::String^ data);


	/// <summary>Initializes a MistMessage using an XML or JSON string, and ensures the message adheres to the provided specification.</summary>
	///
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	/// <param name="data">XML or JSON string used to initialize the message.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
	/// </exception>
	MistMessage(Specification^ spec, System::String^ data);


	/// <summary>Destructor</summary>
	~MistMessage();


	/// <summary>
	/// Sets the internal list of fields which are to be automatically placed in all
	/// MistMessage objects that are created.  Internal copies of the Fields are made, and
	/// thus ownership of the fields that are provided are not retained by MistMessage.
	/// </summary>
	///
	/// <param name="standardFields">The list of fields to set as standard fields</param>
	static void SetStandardFields(System::Collections::Generic::List<Field^>^ standardFields);


	/// <summary>
	/// Destroys the lists of standard fields that are included with MistMessage objects.
	/// </summary>
	static void ClearStandardFields();


	/// <summary>Returns a string that identifies the schema that the Message is based off of.</summary>
	System::String^ GetSchemaID();


	/// <summary>
	/// Sets the value for a field indicated by user.  The value's type is automatically
	/// determined by the function depending on the message schema being used.  If the field 
	/// does not have a required type, a new StringField will be created with the specified name 
	/// and value.
	/// </summary>
	///
	/// <param name="fieldName">Name of the field to be modified/created</param>
	///
	/// <param name="value">
	/// The value of the field.  The value's type is automatically determined based on the message schema.
	/// </param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the field name is NULL or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the input value's type cannot be converted into the type required
	/// by the field, or if the string is too big when converting to char.
	/// </exception>
	void SetValue(System::String^ fieldName, System::String^ value);


	/// <summary>
	/// Sets the value for a field indicated by user.  The value's type is automatically
	/// determined by the function depending on the message schema being used.  If the field 
	/// does not have a required type, a new I64Field will be created with the specified name 
	/// and value.
	/// </summary>
	///
	/// <param name="fieldName">Name of the field to be modified/created</param>
	///
	/// <param name="value">
	/// The value of the field.  The value's type is automatically determined based on the message schema.
	/// </param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the field name is NULL or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the input value's type cannot be converted into the type required
	/// by the field.
	/// </exception>
	void SetValue(System::String^ fieldName, System::Int64 value);

	
	/// <summary>
	/// Sets the value for a field indicated by user.  The value's type is automatically
	/// determined by the function depending on the message schema being used.  If the field 
	/// does not have a required type, a new F64Field will be created with the specified name 
	/// and value.
	/// </summary>
	///
	/// <param name="fieldName">Name of the field to be modified/created</param>
	///
	/// <param name="value">
	/// The value of the field.  The value's type is automatically determined based on the message schema.
	/// </param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the field name is NULL or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the input value's type cannot be converted into the type required
	/// by the field.
	/// </exception>
	void SetValue(System::String^ fieldName, double value);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!MistMessage();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="mistMsg">unmanaged implementation to initialize with</param>
	/// <param name="owned">indicates whether the unmanaged implemention is owned by this object</param>
	MistMessage(gmsec::api::mist::message::MistMessage* mistMsg, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::message::MistMessage* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::message::MistMessage* mistMsg, bool owned);
};

} // end namespace MESSAGE
} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
