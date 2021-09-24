/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MNEMONIC_MESSAGE_NET_H
#define GMSEC_API_MNEMONIC_MESSAGE_NET_H


// C# managed
#include <Message_Net.h>

#include <mist/MIST_Defs_Net.h>


// C++ API native
#include <gmsec4/mist/MnemonicMessage.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Config;

namespace MIST
{

// Forward declarations
ref class Mnemonic;
ref class MnemonicIterator;


/// <summary>
/// The class defines a GMSEC MSG MVAL (Mnemonic value) message.
/// </summary>
///
/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.MnemonicMessage instead.


public ref class MnemonicMessage : public Message
{
public:
	/// <summary>Basic constructor</summary>
	///
	/// <param name="subject">Message subject</param>
	/// <param name="version">The GMSEC Interface Specification Document (ISD) version to be used</param>
	///
	/// <seealso cref="MIST_Defs_Net.h"/>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.MnemonicMessage instead.
	MnemonicMessage(System::String^ subject, unsigned int version);


	/// <summary>Constructor that initializes the message with a specific configuration</summary>
	///
	/// <param name="subject">Message subject</param>
	/// <param name="config">A configuration to associate with the message</param>
	/// <param name="version">The GMSEC Interface Specification Document (ISD) version to be used</param>
	///
	/// <seealso cref="MIST_Defs_Net.h"/>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.MnemonicMessage instead.
	MnemonicMessage(System::String^ subject, Config^ config, unsigned int version);


	/// <summary>Constructor that initializes the message using an XML or JSON string</summary>
	///
	/// <param name="data">XML or JSON string used to initialize the contents of the message</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if the data cannot be parsed</exception>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.MnemonicMessage instead.
	MnemonicMessage(System::String^ data);


	/// <summary>Copy-constructor</summary>
	///
	/// <param name="other">The MnemonicMessage object to copy</param>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.MnemonicMessage instead.
	MnemonicMessage(MnemonicMessage^ other);


	/// <summary>Destructor</summary>
	~MnemonicMessage();


	/// <summary>Associate a Mnemonic object with the message.</summary>
	///
	/// <param name="mnemonic">The Mnemonic object</param>
	void AddMnemonic(Mnemonic^ mnemonic);


	/// <summary>Returns the number of Mnemonic objects stored within the message.</summary>
	long GetNumMnemonics();


	/// <summary>Returns a reference to the Mnemonic object referenced by the given index.</summary>
	///
	/// <seealso cref="MnemonicMessage::getNumMnemonics"/>
	Mnemonic^ GetMnemonic(long index);


    /// <summary>
    /// Method that allows the callee to get the MnemonicIterator associated with the %MnemonicMessage.
    /// This iterator will allow for applications to iterate over the Mnemonic objects stored within the %MnemonicMessage.
    /// The iterator is reset each time GetMnemonicIterator() is called.  The iterator itself is destroyed when the
    /// %MnemonicMessage object is destroyed.
    /// </summary>
    ///
    /// <note>
    /// Only one MnemonicIterator object is associated with a %MnemonicMessage object; multiple calls to
    /// GetMnemonicIterator() will return a reference to the same MnemonicIterator object.  Each call will reset the iterator.
    /// </note>
    ///
    /// <returns>A reference to a MnemonicIterator object.</return>
	MnemonicIterator^ GetMnemonicIterator();


	/// <summary>Constructs and returns a MnemonicMessage from an an ordinary message.</summary>
	static MnemonicMessage^ ConvertMessage(Message^ message);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!MnemonicMessage();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="mnemonicMessage">unmanaged implementation to initialize with</param>
	MnemonicMessage(gmsec::api::mist::MnemonicMessage* mnemonicMsg, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::MnemonicMessage* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::MnemonicMessage* mnemonicMsg, bool owned);
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
