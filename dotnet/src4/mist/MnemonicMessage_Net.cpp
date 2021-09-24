/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// C# managed
#include <mist/MnemonicMessage_Net.h>

#include <mist/Mnemonic_Net.h>
#include <mist/MnemonicIterator_Net.h>

#include <Config_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


MnemonicMessage::MnemonicMessage(String^ subject, unsigned int version)
	: Message((gmsec::api::Message*) nullptr, false)
{
	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		m_impl  = new gmsec::api::mist::MnemonicMessage(subjectStr, version);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


MnemonicMessage::MnemonicMessage(String^ subject, Config^ config, unsigned int version)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::MnemonicMessage(subjectStr, *nativeConfig, version);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


MnemonicMessage::MnemonicMessage(String^ data)
	: Message((gmsec::api::Message*) nullptr, false)
{
	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::mist::MnemonicMessage(dataStr);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process data string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(dataStr);
	}
}


MnemonicMessage::MnemonicMessage(MnemonicMessage^ other)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "MnemonicMessage is null");

	try
	{
		gmsec::api::mist::MnemonicMessage* nativeMsg = ((MnemonicMessage^) other)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::MnemonicMessage(*nativeMsg);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MnemonicMessage::~MnemonicMessage()
{
	this->!MnemonicMessage();
}


long MnemonicMessage::GetNumMnemonics()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MnemonicMessage is null");

	return (long) ((gmsec::api::mist::MnemonicMessage*) m_impl)->getNumMnemonics();
}


Mnemonic^ MnemonicMessage::GetMnemonic(long index)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MnemonicMessage is null");

	try
	{
		const gmsec::api::mist::Mnemonic& nativeMnemonic = ((gmsec::api::mist::MnemonicMessage*) m_impl)->getMnemonic((size_t) index);

		return gcnew Mnemonic(const_cast<gmsec::api::mist::Mnemonic*>(&nativeMnemonic), false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MnemonicMessage::AddMnemonic(Mnemonic^ mnemonic)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MnemonicMessage is null");
	THROW_EXCEPTION_IF_NULLPTR(mnemonic, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	try
	{
		gmsec::api::mist::Mnemonic* nativeMnemonic = ((Mnemonic^) mnemonic)->GetUnmanagedImplementation();

		((gmsec::api::mist::MnemonicMessage*) m_impl)->addMnemonic(*nativeMnemonic);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MnemonicIterator^ MnemonicMessage::GetMnemonicIterator()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MnemonicMessage is null");

	gmsec::api::mist::MnemonicIterator& iter = ((gmsec::api::mist::MnemonicMessage*) m_impl)->getMnemonicIterator();

	return gcnew MnemonicIterator(&iter);
}


MnemonicMessage^ MnemonicMessage::ConvertMessage(Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* nativeMsg = ((Message^) message)->GetUnmanagedImplementation();

		gmsec::api::mist::MnemonicMessage nativeDevMsg = gmsec::api::mist::MnemonicMessage::convertMessage(*nativeMsg);

		return gcnew MnemonicMessage(new gmsec::api::mist::MnemonicMessage(nativeDevMsg), true);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MnemonicMessage::!MnemonicMessage()
{
	ResetUnmanagedImplementation();
}


MnemonicMessage::MnemonicMessage(gmsec::api::mist::MnemonicMessage* mnemonicMsg, bool owned)
	: Message(mnemonicMsg, owned)
{
}


gmsec::api::mist::MnemonicMessage* MnemonicMessage::GetUnmanagedImplementation()
{
	return (gmsec::api::mist::MnemonicMessage*) m_impl;
}


void MnemonicMessage::ResetUnmanagedImplementation()
{
	Message::ResetUnmanagedImplementation();
}


void MnemonicMessage::SetUnmanagedImplementation(gmsec::api::mist::MnemonicMessage* mnemonicMsg, bool owned)
{
	ResetUnmanagedImplementation();

	Message::SetUnmanagedImplementation(mnemonicMsg, owned);
}
