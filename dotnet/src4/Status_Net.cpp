/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <Status_Net.h>

#include <GMSEC_Net.h>

using namespace GMSEC::API;
using namespace System;
using namespace System::Runtime::InteropServices;


Status::Status()
{
	m_impl = new gmsec::api::Status();
}


Status::Status(StatusClass statusClass, StatusCode statusCode, String^ reason)
{
	char* reasonStr = nullptr;

	try
	{
		reasonStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(reason).ToPointer());

		m_impl = new gmsec::api::Status(static_cast<gmsec::api::StatusClass>(statusClass),
			                            static_cast<gmsec::api::StatusCode>(statusCode),
			                            reasonStr);
	}
	catch (...)
	{
		ResetUnmanagedImplementation();
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(reasonStr);
	}
}


Status::Status(Status^ %other)
{
	gmsec::api::Status* native = other->GetUnmanagedImplementation();

	m_impl = new gmsec::api::Status(*native);
}


Status::~Status()
{
	this->!Status();
}


Status^ Status::operator=(Status^ %other)
{
	ResetUnmanagedImplementation();

	gmsec::api::Status* native = other->GetUnmanagedImplementation();

	m_impl = new gmsec::api::Status(*native);

	return this;
}


bool Status::IsError()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	return Convert::ToBoolean(m_impl->isError());
}


String^ Status::Get()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	return gcnew String(m_impl->get());
}


void Status::Set(StatusClass statusClass, StatusCode statusCode, String^ reason)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	char* reasonStr = nullptr;

	try
	{
		reasonStr = static_cast<char *>(Marshal::StringToHGlobalAnsi(reason).ToPointer());

		m_impl->set(static_cast<gmsec::api::StatusClass>(statusClass), static_cast<gmsec::api::StatusCode>(statusCode), reasonStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::OTHER_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process Status reason string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(reasonStr);
	}
}


StatusClass Status::GetClass()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	Int32 statusClass = m_impl->getClass();

	// If enum has definition, return it
	if (Enum::IsDefined(StatusClass::typeid, statusClass))
	{
		return static_cast<StatusClass>(statusClass);
	}

	return StatusClass::OTHER_ERROR;
}


void Status::SetClass(StatusClass statusClass)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	m_impl->setClass(static_cast<gmsec::api::StatusClass>(statusClass));
}


StatusCode Status::GetCode()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	Int32 statusCode = m_impl->getCode();

	// If enum has definition, return it
	if (Enum::IsDefined(StatusCode::typeid, statusCode))
	{
		return static_cast<StatusCode>(statusCode);
	}

	return StatusCode::OTHER_ERROR_CODE;
}


void Status::SetCode(StatusCode statusCode)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	m_impl->setCode(static_cast<gmsec::api::StatusCode>(statusCode));
}


String^ Status::GetReason()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	return gcnew String(m_impl->getReason());
}


void Status::SetReason(String^ reason)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	char* reasonStr = nullptr;

	try
	{
		reasonStr = static_cast<char *>(Marshal::StringToHGlobalAnsi(reason).ToPointer());

		m_impl->setReason(reasonStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::OTHER_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process Status reason string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(reasonStr);
	}
}


Int32 Status::GetCustomCode()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	return m_impl->getCustomCode();
}


void Status::SetCustomCode(Int32 code)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	m_impl->setCustomCode(code);
}


void Status::Reset()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::OTHER_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status is null");

	m_impl->reset();
}


Status::!Status()
{
	ResetUnmanagedImplementation();
}


gmsec::api::Status* Status::GetUnmanagedImplementation()
{
	return m_impl;
}


void Status::SetUnmanagedImplementation(gmsec::api::Status* status)
{
	ResetUnmanagedImplementation();

	m_impl = status;
}


void Status::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


Status::Status(gmsec::api::Status* status)
{
	m_impl = new gmsec::api::Status(*status);
}
