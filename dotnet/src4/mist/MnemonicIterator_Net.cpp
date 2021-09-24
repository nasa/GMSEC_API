/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <mist/MnemonicIterator_Net.h>

#include <mist/Mnemonic_Net.h>

#include <GMSEC_Exception_Net.h>

// C++ API native
#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API::MIST;


MnemonicIterator::MnemonicIterator(gmsec::api::mist::MnemonicIterator* iter)
{
	m_impl = iter;
}


MnemonicIterator::~MnemonicIterator()
{
	this->!MnemonicIterator();
}


bool MnemonicIterator::HasNext()
{
	return m_impl->hasNext();
}


Mnemonic^ MnemonicIterator::Next()
{
	try
	{
		gmsec::api::mist::Mnemonic& nativeMnemonic = const_cast<gmsec::api::mist::Mnemonic&>(m_impl->next());

		return gcnew Mnemonic(&nativeMnemonic, false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MnemonicIterator::Reset()
{
	m_impl->reset();
}


MnemonicIterator::!MnemonicIterator()
{
	ResetUnmanagedImplementation();
}


void MnemonicIterator::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}
