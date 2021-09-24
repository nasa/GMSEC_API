/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/ProductFile_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


ProductFile::ProductFile(String^ name, String^ description, String^ version,
	                     String^ format, String^ uri)
{
	char* nameStr    = nullptr;
	char* descStr    = nullptr;
	char* versionStr = nullptr;
	char* formatStr  = nullptr;
	char* uriStr     = nullptr;

	try
	{
		nameStr    = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());
		descStr    = static_cast<char*>(Marshal::StringToHGlobalAnsi(description).ToPointer());
		versionStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(version).ToPointer());
		formatStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(format).ToPointer());
		uriStr     = static_cast<char*>(Marshal::StringToHGlobalAnsi(uri).ToPointer());

		m_impl  = new gmsec::api::mist::ProductFile(nameStr, descStr, versionStr, formatStr, uriStr);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process string Parameter(s)");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(descStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(versionStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(formatStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(uriStr);
	}
}


ProductFile::ProductFile(String^ name, String^ description, String^ version,
	                     String^ format, array<Byte>^ data)
{
	char*  nameStr    = nullptr;
	char*  descStr    = nullptr;
	char*  versionStr = nullptr;
	char*  formatStr  = nullptr;
	Byte*  dataPtr    = nullptr;
	size_t dataLength = 0;

	if (data != nullptr)
	{
		pin_ptr<Byte> p = &data[0];

		dataPtr    = p;
		dataLength = (size_t) data->Length;
	}

	try
	{
		nameStr    = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());
		descStr    = static_cast<char*>(Marshal::StringToHGlobalAnsi(description).ToPointer());
		versionStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(version).ToPointer());
		formatStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(format).ToPointer());

		m_impl  = new gmsec::api::mist::ProductFile(nameStr, descStr, versionStr, formatStr, dataPtr, dataLength);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process string Parameter(s)");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(descStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(versionStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(formatStr);
	}
}


ProductFile::ProductFile(ProductFile^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Other ProductFile is null");

	try
	{
		gmsec::api::mist::ProductFile* nativeOther = ((ProductFile^) other)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::ProductFile(*nativeOther);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ProductFile::~ProductFile()
{
	this->!ProductFile();
}


String^ ProductFile::GetName()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	return gcnew String(m_impl->getName());
}


String^ ProductFile::GetDescription()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	return gcnew String(m_impl->getDescription());
}


String^ ProductFile::GetVersion()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	return gcnew String(m_impl->getVersion());
}


String^ ProductFile::GetFormat()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	return gcnew String(m_impl->getFormat());
}


bool ProductFile::UriAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	return m_impl->uriAvailable();
}


String^ ProductFile::GetURI()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	return gcnew String(m_impl->getURI());
}


bool ProductFile::ContentsAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	return m_impl->contentsAvailable();
}


array<Byte>^ ProductFile::GetContents()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ProductFile is null");

	unsigned char* data   = nullptr;
	size_t         length = m_impl->getContents(data);

	array<Byte>^ contents = gcnew array<Byte>((int) length);

	Marshal::Copy((IntPtr) data, contents, 0, (int) length);

	return contents;
}


ProductFile::!ProductFile()
{
	ResetUnmanagedImplementation();
}


ProductFile::ProductFile(gmsec::api::mist::ProductFile* param, bool owned)
{
	SetUnmanagedImplementation(param, owned);
}


gmsec::api::mist::ProductFile* ProductFile::GetUnmanagedImplementation()
{
	return m_impl;
}


void ProductFile::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void ProductFile::SetUnmanagedImplementation(gmsec::api::mist::ProductFile* param, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = param;
	m_owned = owned;
}
