/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MIST_MNEMONIC_MESSAGE_NET_H
#define GMSEC_API_MIST_MNEMONIC_MESSAGE_NET_H


#include <mist/message/MistMessage_Net.h>


// C++ API native
#include <gmsec4/mist/message/MnemonicMessage.h>


namespace GMSEC
{
namespace API
{
	// Forward declaration(s)
	ref class Config;

namespace MIST
{
	// Forward declaration(s)
	ref class Mnemonic;
	ref class MnemonicIterator;
	ref class Specification;

namespace MESSAGE
{


/// <summary>
/// A MistMessage object that is capable of storing Mnemonic objects
/// The following message schema IDs and their templates are supported:
/// 2014.00.GMSEC.MSG.MVAL @n
/// 2014.00.GMSEC.REQ.MVAL @n
/// 2014.00.GMSEC.RESP.MVAL @n
/// 2016.00.GMSEC.MSG.MVAL @n
/// 2016.00.GMSEC.REQ.MVAL @n
/// 2016.00.GMSEC.RESP.MVAL
/// </summary>
///
/// <seealso cref="MistMessage"/>
/// <seealso cref="Config"/>
/// <seealso cref="Mnemonic"/>
/// <seealso cref="MnemonicIterator"/>
/// <seealso cref="Specification"/>

public ref class MnemonicMessage : public MistMessage
{
public:
	/// <summary>
	/// Initializes the message instance and automatically builds the appropriate
	/// schemaID based on the version of Specification used
	/// </summary>
	///
	/// <param name="subject">The subject string for the message.</param>
	/// <param name="schemaID">The string used to identify the message schema in the GMSEC ISD.</param>
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if either the subject or the schema ID is null or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification is null or if a failure occurs when loading the template directory.
	/// </exception>
	MnemonicMessage(System::String^ subject, System::String^ schemaID, Specification^ spec);


	/// <summary>
	/// Initializes the message instance and automatically builds the appropriate
	/// schemaID based on the version of Specification used
	/// </summary>
	///
	/// <param name="subject">The subject string for the message.</param>
	/// <param name="schemaID">The string used to identify the message schema in the GMSEC ISD.</param>
	/// <param name="config">A configuration to associate with the message.</param>
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if either the subject or the schema ID is null or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification is null or if a failure occurs when loading the template directory.
	/// </exception>
	MnemonicMessage(System::String^ subject, System::String^ schemaID, Config^ config, Specification^ spec);


	/// <summary>Copy constructor - initializes the message instance using the other given MnemonicMessage.<summary>
	///
	/// <param name="other">The MnemonicMessage to be copied.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given MnemonicMessage is null.
	/// </exception>
	MnemonicMessage(MnemonicMessage^ other);


	/// <summary>Initializes a MnemonicMessage using an XML or JSON string.</summary>
	///
	/// <param name="data">XML or JSON string used to initialize the message.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
	/// </exception>
	///
	/// <remarks>This method has been deprecated; use MnemonicMessage(Specification^, System::String^) instead.
	MnemonicMessage(System::String^ data);


	/// <summary>Initializes a MnemonicMessage using an XML or JSON string, and ensures the message adheres to the provided specification.</summary>
	///
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	/// <param name="data">XML or JSON string used to initialize the message.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
	/// </exception>
	MnemonicMessage(Specification^ spec, System::String^ data);


	/// <summary>Destructor</summary>
	~MnemonicMessage();


	/// <summary>Add a Mnemonic to the message</summary>
	///
	/// <param name="mnemonic">The Mnemonic to add</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given Mnemonic is null.
	/// </exception>
    void AddMnemonic(Mnemonic^ mnemonic);


	/// <summary>Returns the number of Mnemonic objects stored in this message.</summary>
    long GetNumMnemonics();


	/// <summary>Get the Mnemonic at the given index.</summary>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if index specified is not in the range of Mnemonics in the message.
	/// </exception>
    Mnemonic^ GetMnemonic(long index);


	/// <summary>
	/// Method that allows the callee to get the MnemonicIterator associated with the MnemonicMessage.
	/// This iterator will allow for applications to iterate over the Mnemonic objects stored within the %MnemonicMessage.
	/// The iterator is reset each time getMnemonicIterator() is called.  The iterator itself is destroyed when the
	/// MnemonicMessage object is destroyed.
	///
	/// Note: Only one MnemonicIterator object is associated with a %MnemonicMessage object; multiple calls to
	/// getMnemonicIterator() will return a reference to the same MnemonicIterator object.  Each call will reset the iterator.
	///
    MnemonicIterator^ GetMnemonicIterator();


	/// <summary>Constructs and returns a MnemonicMessage from an ordinary message.</summary>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given Message is null.
	/// </exception>
    static MnemonicMessage^ ConvertMessage(Message^ message);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!MnemonicMessage();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="mistMsg">unmanaged implementation to initialize with</param>
	/// <param name="owned">indicates whether the unmanaged implemention is owned by this object</param>
	MnemonicMessage(gmsec::api::mist::message::MnemonicMessage* mnemonicMsg, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::message::MnemonicMessage* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::message::MnemonicMessage* mnemonicMsg, bool owned);
};

} // end namespace MESSAGE
} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
