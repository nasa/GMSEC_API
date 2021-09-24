/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MNEMONIC_NET_H
#define GMSEC_API_MNEMONIC_NET_H


// C++ API native
#include <gmsec4/mist/Mnemonic.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Field;

namespace MIST
{

// Forward declarations
ref class MnemonicSample;


/// <summary>
/// This class is a lightweight container for holding information on a Mnemonic,
/// and used to generate GMSEC Mnemonic messages by the ConnectionManager class.
/// </summary>


public ref class Mnemonic
{
public:
	/// <summary>Constructor</summary>
	///
	/// <param name="name">Mnemonic name</param>
	/// <param name="samples">List of MnemonicSample objects</param>
	Mnemonic(System::String^ name, System::Collections::Generic::List<MnemonicSample^>^ samples);


	/// <summary>Constructor</summary>
	///
	/// <param name="name">Mnemonic name</param>
	Mnemonic(System::String^ name);


	/// <summary>Copy-constructor for making deep-copy of other Mnemonic object.</summary>
	///
	/// <param name="other">The other Mnemonic object to copy</param>
	Mnemonic(Mnemonic^ other);


	/// <summary>Destructor</summary>
	~Mnemonic();


	/// <summary>Returns the name associated with the mnemonic.</summary>
	System::String^ GetName();


	/// <summary>Sets the status Field within the mnemonic.</summary>
	///
	/// <param name="status">The Field object containing a status value</param>
	void SetStatus(Field^ status);


	/// <summary>Returns whether there is a status Field associated with the mnemonic.</summary>
	bool StatusAvailable();


	/// <summary>Returns a reference to the status Field</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a status Field has not been set within the mnemonic</exception>
	///
	/// <seealso cref="Mnemonic::setStatus"/>
	/// <seealso cref="Mnemonic::statusAvailable"/>
	Field^ GetStatus();


	/// <summary>Sets the unit string withing the mnemonic.</summary>
	///
	/// <param name="units">String containing units description</param>
	void SetUnits(System::String^ units);


	/// <summary>Returns whether there is a unit string associated with the mnemonic.</summary>
	bool UnitsAvailable();


	/// <summary>Returns a reference to the units string</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a units string has not been set within the mnemonic</exception>
	///
	/// <seealso cref="Mnemonic::setUnits"/>
	/// <seealso cref="Mnemonic::unitsAvailable"/>
	System::String^ GetUnits();


	/// <summary>Adds the given MnemonicSample to the Mnemonic</summary>
	///
	/// <param name="sample">The MnemonicSample object</param>
	void AddSample(MnemonicSample^ sample);


	/// <summary>Returns the number of MnemonicSample objects stored within the Mnemonic.</summary>
	long GetSampleCount();


	/// <summary>Returns a reference to the MnemonicSample object stored within the Mnemonic.</summary>
	///
	/// <param name="index">The zero-based index of the MnemonicSample to access</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if the index is out of bounds</exception>
	///
	/// <seealso cref="Mnemonic::getSampleCount"/>
	MnemonicSample^ GetSample(long index);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Mnemonic();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="mnemonic">unmanaged implementation to initialize with</param>
	Mnemonic(gmsec::api::mist::Mnemonic* mnemonic, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::Mnemonic* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::Mnemonic* mnemonic, bool owned);


private:
        void ConstructorImplementation(System::String^ name, 
                                       System::Collections::Generic::List<MnemonicSample^>^ samples);

	gmsec::api::mist::Mnemonic* m_impl;
	bool                        m_owned;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
