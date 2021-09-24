/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// C# managed
#include <mist/ProductFileMessage_Net.h>

#include <mist/ProductFile_Net.h>
#include <mist/ProductFileIterator_Net.h>

#include <Config_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


ProductFileMessage::ProductFileMessage(String^ subject, ResponseStatus status, String^ productType,
	                                   String^ productSubtype, unsigned int version)
	: Message((gmsec::api::Message*) nullptr, false)
{
	char* subjectStr = nullptr;
	char* typeStr    = nullptr;
	char* subtypeStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		typeStr    = static_cast<char*>(Marshal::StringToHGlobalAnsi(productType).ToPointer());
		subtypeStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(productSubtype).ToPointer());

		m_impl  = new gmsec::api::mist::ProductFileMessage(subjectStr,
		                                                   static_cast<gmsec::api::mist::ResponseStatus::Response>(status),
		                                                   typeStr, subtypeStr, version);
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
		FREE_HGLOBAL_IF_NOT_NULLPTR(typeStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(subtypeStr);
	}
}


ProductFileMessage::ProductFileMessage(String^ subject, Config^ config, ResponseStatus status,
	                                   String^ productType, String^ productSubtype, unsigned int version)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	char* subjectStr = nullptr;
	char* typeStr    = nullptr;
	char* subtypeStr = nullptr;

	try
	{
		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		typeStr    = static_cast<char*>(Marshal::StringToHGlobalAnsi(productType).ToPointer());
		subtypeStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(productSubtype).ToPointer());

		m_impl  = new gmsec::api::mist::ProductFileMessage(subjectStr, *nativeConfig,
		                                                   static_cast<gmsec::api::mist::ResponseStatus::Response>(status),
		                                                   typeStr, subtypeStr, version);
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
		FREE_HGLOBAL_IF_NOT_NULLPTR(typeStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(subtypeStr);
	}
}


ProductFileMessage::ProductFileMessage(String^ data)
	: Message((gmsec::api::Message*) nullptr, false)
{
	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::mist::ProductFileMessage(dataStr);
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


ProductFileMessage::ProductFileMessage(ProductFileMessage^ other)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "ProductFileMessage is null");

	try
	{
		gmsec::api::mist::ProductFileMessage* nativeMsg = ((ProductFileMessage^) other)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::ProductFileMessage(*nativeMsg);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ProductFileMessage::~ProductFileMessage()
{
	this->!ProductFileMessage();
}


ResponseStatus ProductFileMessage::GetResponseStatus()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFileMessage is null");

	return static_cast<ResponseStatus>(((gmsec::api::mist::ProductFileMessage*) m_impl)->getResponseStatus());
}


String^ ProductFileMessage::GetProductType()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFileMessage is null");

	return gcnew String(((gmsec::api::mist::ProductFileMessage*) m_impl)->getProductType());
}


String^ ProductFileMessage::GetProductSubtype()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFileMessage is null");

	return gcnew String(((gmsec::api::mist::ProductFileMessage*) m_impl)->getProductSubtype());
}


long ProductFileMessage::GetNumProductFiles()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFileMessage is null");

	return (long) ((gmsec::api::mist::ProductFileMessage*) m_impl)->getNumProductFiles();
}


ProductFile^ ProductFileMessage::GetProductFile(long index)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFileMessage is null");

	try
	{
		const gmsec::api::mist::ProductFile& nativeProductFile = ((gmsec::api::mist::ProductFileMessage*) m_impl)->getProductFile((size_t) index);

		return gcnew ProductFile(const_cast<gmsec::api::mist::ProductFile*>(&nativeProductFile), false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ProductFileMessage::AddProductFile(ProductFile^ productFile)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFileMessage is null");
	THROW_EXCEPTION_IF_NULLPTR(productFile, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	try
	{
		gmsec::api::mist::ProductFile* nativeProductFile = ((ProductFile^) productFile)->GetUnmanagedImplementation();

		((gmsec::api::mist::ProductFileMessage*) m_impl)->addProductFile(*nativeProductFile);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ProductFileIterator^ ProductFileMessage::GetProductFileIterator()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFileMessage is null");

	gmsec::api::mist::ProductFileIterator& iter = ((gmsec::api::mist::ProductFileMessage*) m_impl)->getProductFileIterator();

	return gcnew ProductFileIterator(&iter);
}


ProductFileMessage^ ProductFileMessage::ConvertMessage(Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* nativeMsg = ((Message^) message)->GetUnmanagedImplementation();

		gmsec::api::mist::ProductFileMessage nativeProdFileMsg = gmsec::api::mist::ProductFileMessage::convertMessage(*nativeMsg);

		return gcnew ProductFileMessage(new gmsec::api::mist::ProductFileMessage(nativeProdFileMsg), true);
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


ProductFileMessage::ProductFileMessage(gmsec::api::mist::ProductFileMessage* productFileMsg, bool owned)
	: Message(productFileMsg, owned)
{
}


gmsec::api::mist::ProductFileMessage* ProductFileMessage::GetUnmanagedImplementation()
{
	return (gmsec::api::mist::ProductFileMessage*) m_impl;
}


void ProductFileMessage::ResetUnmanagedImplementation()
{
	Message::ResetUnmanagedImplementation();
}


void ProductFileMessage::SetUnmanagedImplementation(gmsec::api::mist::ProductFileMessage* productFileMsg, bool owned)
{
	ResetUnmanagedImplementation();

	Message::SetUnmanagedImplementation(productFileMsg, owned);
}
