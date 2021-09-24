/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/message/MnemonicMessage_Net.h>

#include <mist/Mnemonic_Net.h>
#include <mist/MnemonicIterator_Net.h>
#include <mist/Specification_Net.h>

#include <Config_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace GMSEC::API::MIST::MESSAGE;
using namespace System;
using namespace System::Runtime::InteropServices;


MnemonicMessage::MnemonicMessage(String^ subject, String^ schemaID, Specification^ spec)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* subjectStr  = nullptr;
	char* schemaIDStr = nullptr;

	try
	{
		subjectStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		schemaIDStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(schemaID).ToPointer());

		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MnemonicMessage(subjectStr, schemaIDStr, *specNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process parameter strings");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(schemaIDStr);
	}
}


MnemonicMessage::MnemonicMessage(String^ subject, String^ schemaID, Config^ config, Specification^ spec)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* subjectStr  = nullptr;
	char* schemaIDStr = nullptr;

	try
	{
		subjectStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		schemaIDStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(schemaID).ToPointer());

		gmsec::api::Config*              cfgNative  = config->GetUnmanagedImplementation();
		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MnemonicMessage(subjectStr, schemaIDStr, *cfgNative, *specNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process parameter strings");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(schemaIDStr);
	}
}


MnemonicMessage::MnemonicMessage(MnemonicMessage^ other)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MnemonicMessage is null");

	try
	{
		gmsec::api::mist::message::MnemonicMessage* otherNative = other->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MnemonicMessage(*otherNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MnemonicMessage::MnemonicMessage(String^ data)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::mist::message::MnemonicMessage(dataStr);
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


MnemonicMessage::MnemonicMessage(Specification^ spec, System::String^ data)
    : MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MnemonicMessage(*specNative, dataStr);
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


MnemonicMessage::~MnemonicMessage()
{
	this->!MnemonicMessage();
}


void MnemonicMessage::AddMnemonic(Mnemonic^ mnemonic)
{
	THROW_EXCEPTION_IF_NULLPTR(mnemonic, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	try
	{
		gmsec::api::mist::Mnemonic* mnemonicNative = mnemonic->GetUnmanagedImplementation();

		this->GetUnmanagedImplementation()->addMnemonic(*mnemonicNative);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


long MnemonicMessage::GetNumMnemonics()
{
	return (long) this->GetUnmanagedImplementation()->getNumMnemonics();
}


Mnemonic^ MnemonicMessage::GetMnemonic(long index)
{
	try
	{
		const gmsec::api::mist::Mnemonic& mnemonicNative = this->GetUnmanagedImplementation()->getMnemonic(index);

		return gcnew Mnemonic(const_cast<gmsec::api::mist::Mnemonic*>(&mnemonicNative), false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MnemonicIterator^ MnemonicMessage::GetMnemonicIterator()
{
	gmsec::api::mist::MnemonicIterator& mnemIterNative = this->GetUnmanagedImplementation()->getMnemonicIterator();

	return gcnew MnemonicIterator(const_cast<gmsec::api::mist::MnemonicIterator*>(&mnemIterNative));
}


MnemonicMessage^ MnemonicMessage::ConvertMessage(Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* msgNative = message->GetUnmanagedImplementation();

		gmsec::api::mist::message::MnemonicMessage mnemMsgNative = gmsec::api::mist::message::MnemonicMessage::convertMessage(*msgNative);

		return gcnew MnemonicMessage(new gmsec::api::mist::message::MnemonicMessage(mnemMsgNative), true);
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


MnemonicMessage::MnemonicMessage(gmsec::api::mist::message::MnemonicMessage* mnemonicMsg, bool owned)
	: MistMessage(mnemonicMsg, owned)
{
}


gmsec::api::mist::message::MnemonicMessage* MnemonicMessage::GetUnmanagedImplementation()
{
	return (gmsec::api::mist::message::MnemonicMessage*) m_impl;
}


void MnemonicMessage::ResetUnmanagedImplementation()
{
	 MistMessage::ResetUnmanagedImplementation();
}


void MnemonicMessage::SetUnmanagedImplementation(gmsec::api::mist::message::MnemonicMessage* mnemonicMsg, bool owned)
{
	ResetUnmanagedImplementation();

	MistMessage::SetUnmanagedImplementation(mnemonicMsg, owned);
}
