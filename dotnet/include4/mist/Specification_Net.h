/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#ifndef GMSEC_API_MIST_SPECIFICATION_NET_H
#define GMSEC_API_MIST_SPECIFICATION_NET_H

// C++ API native
#include <gmsec4/mist/Specification.h>


namespace GMSEC
{
namespace API
{
	// Forward declaration(s)
	ref class Config;
	ref class Message;

namespace MIST
{
	// Forward declaration(s)
	ref class SchemaIDIterator;
	ref class MessageSpecification;


/// <summary>
/// The Specification class loads a list of templates from a designated directory
/// that are used to create and validate message schemas. Each template has an associated ID
/// that is used to identify the kind of message schema the template will be defining.
/// </summary>
///
/// <seealso cref="Config"/>
/// <seealso cref="ShemaIDIterator"/>
///
public ref class Specification
{
public:
	/// <summary>
	/// Initializes the Specification instance with a configuration.
	/// </summary>
	///
	/// <param name="config">Configuration object</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// Thrown if expected values from the configuration are missing.
	/// </exception>
	Specification(Config^ config);


	/// <summary>
	/// Copy constructor - initializes the Specification instance based on another instance
	/// </summary>
	///
	/// <param name="other">The Specification object to be copied.</param>
	Specification(Specification^ other);


	/// <summary>Destructor</summary>
	virtual ~Specification();


	/// <summary>
	/// Looks up the message subject in the message registry to grab the
	/// appropriate template (based on its assigned schema ID).  The contents of the 
	/// message are then compared to the template to ensure the message complies with 
	/// the ISD. This function will also register the message with an appropriate schema 
	/// ID if it is not found in the registry.
	/// </summary>
	///
	/// <param name="msg">The message to be validated.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the message fails to pass validation.  
	/// A list of errors will be given of any issues found with the message.
	/// </exception>
	virtual void ValidateMessage(Message^ msg);


	/// <summary>
	/// Returns a SchemaIDIterator, which can be used to iterated over the IDs
	/// of the loaded templates.
	/// </summary>
	///
	/// <note>
	/// Only one SchemaIDIterator object is associated with a %Specification object; multiple calls to 
	/// getSchemaIDIterator() will return a reference to the same SchemaIDIterator object.  Each call
	/// will reset the iterator.
	/// </note>
	///
	/// <returns>A reference to a SchemaIDIterator object</returns>
	SchemaIDIterator^ GetSchemaIDIterator();


	/// <summary>
	/// Returns the version of the ISD being used as an unsigned integer.
	/// </summary>
	unsigned int GetVersion();


	/// <summary>
	/// Accessor for acquiring the list of MessageSpecification objects associated with the Specification.
	/// </summary>
	System::Collections::Generic::List<MessageSpecification^>^ GetMessageSpecifications();


	/// <summary>
	/// Returns a message schema from the template with the matching schemaID.
	/// This message schema can be used as input for a Message data constructor
	/// </summary>
	///
	/// <param name="subject">message subject</param>
	/// <param name="schemaID">ID used to look up the appropriate template</param>
	///
	/// <returns>XML string representation of the message schema.</returns>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the provided schemaID is not available in the list of loaded schema.
	/// </exception>
	System::String^ GetTemplateXML(System::String^ subject, System::String^ schemaID);


protected:
	/// <summary>Finalize, free memory and set objects to null</summary>
	!Specification();


	/// <summary>Pass through to Specification managed class</summary>
	///
	/// <param name="msg">the message</param>
	void ValidateMessageNative(const gmsec::api::Message& msg);


internal:
	/// <summary>
	/// Constructor that creates the association of this object with an existing unmanaged object
	/// </summary>
	Specification(gmsec::api::mist::Specification* spec, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	gmsec::api::mist::Specification* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::Specification* spec, bool owned);


private:
	gmsec::api::mist::Specification* m_impl;
	bool m_owned;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
