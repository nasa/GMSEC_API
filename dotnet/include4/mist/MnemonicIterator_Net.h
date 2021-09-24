/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MNEMONIC_ITERATOR_NET_H
#define GMSEC_API_MNEMONIC_ITERATOR_NET_H


// C++ API native
#include <gmsec4/mist/MnemonicIterator.h>


namespace GMSEC
{
namespace API
{
namespace MIST
{
// Forward declarations
ref class Mnemonic;


/// <summary>
/// The MnemonicIterator class can be used to iterate over the Mnemonic Object(s) contained within a MnemonicMessage.
///
/// <seealso cref="MnemonicMessage"/>
/// </summary>

public ref class MnemonicIterator
{
public:
	~MnemonicIterator();


	/// <summary>Provides indication as to whether there are additional Mnemonic objects that can be referenced using Next().</summary>
	///
	/// <seealso cref="next"/>
	/// <seealso cref="reset"/>
	bool HasNext();


	/// <summary>Returns a reference to the next available Mnemonic object.</summary>
	///
	/// <seealso cref="hasNext"/>
	/// <seealso cref="reset"/>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if the iterator has reached the end (i.e. there are no more Mnemonics.)</exception>
	Mnemonic^ Next();


	/// <summary>Resets the iterator to the beginning of the Mnemonic list that is maintained by the MnemonicMessage.</summary>
	void Reset();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!MnemonicIterator();


internal:
	/// <summary>Internal constructor</summary>
	MnemonicIterator(gmsec::api::mist::MnemonicIterator* iter);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


private:
	gmsec::api::mist::MnemonicIterator* m_impl;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
