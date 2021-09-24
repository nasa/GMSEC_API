/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/message/ProductFileMessage_Net.h>

#include <mist/ProductFile_Net.h>
#include <mist/ProductFileIterator_Net.h>
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


ProductFileMessage::ProductFileMessage(String^        subject,
                                       ResponseStatus status,
                                       MessageKind    kind,
                                       String^        productType,
                                       String^        productSubtype,
                                       Specification^ spec)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* subjectStr     = nullptr;
	char* prodTypeStr    = nullptr;
	char* prodSubtypeStr = nullptr;

	try
	{
		subjectStr     = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		prodTypeStr    = static_cast<char*>(Marshal::StringToHGlobalAnsi(productType).ToPointer());
		prodSubtypeStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(productSubtype).ToPointer());

		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl = new gmsec::api::mist::message::ProductFileMessage(subjectStr,
		                                                           static_cast<gmsec::api::mist::ResponseStatus::Response>(status),
		                                                           static_cast<gmsec::api::Message::MessageKind>(kind),
		                                                           prodTypeStr,
		                                                           prodSubtypeStr,
		                                                           *specNative);
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
		FREE_HGLOBAL_IF_NOT_NULLPTR(prodTypeStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(prodSubtypeStr);
	}
}


ProductFileMessage::ProductFileMessage(String^        subject,
                                       ResponseStatus status,
                                       MessageKind    kind,
                                       String^        productType,
                                       String^        productSubtype,
                                       Config^        config,
                                       Specification^ spec)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* subjectStr     = nullptr;
	char* prodTypeStr    = nullptr;
	char* prodSubtypeStr = nullptr;

	try
	{
		subjectStr     = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		prodTypeStr    = static_cast<char*>(Marshal::StringToHGlobalAnsi(productType).ToPointer());
		prodSubtypeStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(productSubtype).ToPointer());

		gmsec::api::Config*              cfgNative  = config->GetUnmanagedImplementation();
		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl = new gmsec::api::mist::message::ProductFileMessage(subjectStr,
		                                                           static_cast<gmsec::api::mist::ResponseStatus::Response>(status),
		                                                           static_cast<gmsec::api::Message::MessageKind>(kind),
		                                                           prodTypeStr,
		                                                           prodSubtypeStr,
		                                                           *cfgNative,
		                                                           *specNative);
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
		FREE_HGLOBAL_IF_NOT_NULLPTR(prodTypeStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(prodSubtypeStr);
	}
}


ProductFileMessage::ProductFileMessage(String^        subject,
                                       ResponseStatus status,
                                       String^        schemaID,
                                       Specification^ spec)
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

		m_impl = new gmsec::api::mist::message::ProductFileMessage(subjectStr,
		                                                           static_cast<gmsec::api::mist::ResponseStatus::Response>(status),
		                                                           schemaIDStr,
		                                                           *specNative);
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


ProductFileMessage::ProductFileMessage(String^        subject,
                                       ResponseStatus status,
                                       String^        schemaID,
                                       Config^        config,
                                       Specification^ spec)
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

		m_impl = new gmsec::api::mist::message::ProductFileMessage(subjectStr,
		                                                           static_cast<gmsec::api::mist::ResponseStatus::Response>(status),
		                                                           schemaIDStr,
		                                                           *cfgNative,
		                                                           *specNative);
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


ProductFileMessage::ProductFileMessage(ProductFileMessage^ other)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFileMessage is null");

	try
	{
		gmsec::api::mist::message::ProductFileMessage* otherNative = other->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::ProductFileMessage(*otherNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ProductFileMessage::ProductFileMessage(System::String^ data)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	char* dataStr = nullptr;

	try
	{
		dataStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::mist::message::ProductFileMessage(dataStr);
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


ProductFileMessage::ProductFileMessage(Specification^ spec, System::String^ data)
    : MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::ProductFileMessage(*specNative, dataStr);
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


ProductFileMessage::~ProductFileMessage()
{
	this->!ProductFileMessage();
}


void ProductFileMessage::AddProductFile(ProductFile^ productFile)
{
	THROW_EXCEPTION_IF_NULLPTR(productFile, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	try
	{
		gmsec::api::mist::ProductFile* productFileNative = productFile->GetUnmanagedImplementation();

		this->GetUnmanagedImplementation()->addProductFile(*productFileNative);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


long ProductFileMessage::GetNumProductFiles()
{
	return (long) this->GetUnmanagedImplementation()->getNumProductFiles();
}


ProductFile^ ProductFileMessage::GetProductFile(long index)
{
	try
	{
		const gmsec::api::mist::ProductFile& productFileNative = this->GetUnmanagedImplementation()->getProductFile(index);

		return gcnew ProductFile(const_cast<gmsec::api::mist::ProductFile*>(&productFileNative), false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ResponseStatus ProductFileMessage::GetResponseStatus()
{
	return static_cast<ResponseStatus>(this->GetUnmanagedImplementation()->getResponseStatus());
}


String^ ProductFileMessage::GetProductType()
{
	return gcnew String(this->GetUnmanagedImplementation()->getProductType());
}


String^ ProductFileMessage::GetProductSubtype()
{
	return gcnew String(this->GetUnmanagedImplementation()->getProductSubtype());
}


ProductFileIterator^ ProductFileMessage::GetProductFileIterator()
{
	gmsec::api::mist::ProductFileIterator& productFileIterNative = this->GetUnmanagedImplementation()->getProductFileIterator();

	return gcnew ProductFileIterator(const_cast<gmsec::api::mist::ProductFileIterator*>(&productFileIterNative));
}


ProductFileMessage^ ProductFileMessage::ConvertMessage(Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* msgNative = message->GetUnmanagedImplementation();

		gmsec::api::mist::message::ProductFileMessage productFileMsgNative =
			gmsec::api::mist::message::ProductFileMessage::convertMessage(*msgNative);

		return gcnew ProductFileMessage(new gmsec::api::mist::message::ProductFileMessage(productFileMsgNative), true);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ProductFileMessage::!ProductFileMessage()
{
	ResetUnmanagedImplementation();
}


ProductFileMessage::ProductFileMessage(gmsec::api::mist::message::ProductFileMessage* productFileMsg, bool owned)
	: MistMessage(productFileMsg, owned)
{
}


gmsec::api::mist::message::ProductFileMessage* ProductFileMessage::GetUnmanagedImplementation()
{
	return (gmsec::api::mist::message::ProductFileMessage*) m_impl;
}


void ProductFileMessage::ResetUnmanagedImplementation()
{
	MistMessage::ResetUnmanagedImplementation();
}


void ProductFileMessage::SetUnmanagedImplementation(gmsec::api::mist::message::ProductFileMessage* productFileMsg, bool owned)
{
	MistMessage::SetUnmanagedImplementation(productFileMsg, owned);
}
