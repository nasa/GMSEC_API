/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <GMSEC_Exception_Net.h>
#include <Status_Net.h>

#include <GMSEC_Net.h>

// C++ API native
#include <gmsec4/Status.h>


using namespace GMSEC::API;
using namespace System;


GMSEC_Exception::GMSEC_Exception(StatusClass errorClass, StatusCode errorCode, String^ errorMsg)
{
	char* errorMsgStr = nullptr;
	try
	{
		// convert input param from managed to unmanaged
		errorMsgStr = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(errorMsg).ToPointer());

		// initialize the unmanaged implementation pointer and object
		m_impl = new gmsec::api::Exception(static_cast<gmsec::api::StatusClass>(errorClass),
		                                   static_cast<gmsec::api::StatusCode>(errorCode),
		                                   errorMsgStr);
	}
	catch (...)
	{
		ResetUnmanagedImplementation();
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(errorMsgStr);
	}
}


GMSEC_Exception::GMSEC_Exception(StatusClass errorClass, StatusCode errorCode, Int32 customCode, String^ errorMsg)
{
	char* errorMsgStr = nullptr;
	try
	{
		// convert input param from managed to unmanaged
		errorMsgStr = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(errorMsg).ToPointer());

		// initialize the unmanaged implementation pointer and object
		m_impl = new gmsec::api::Exception(static_cast<gmsec::api::StatusClass>(errorClass),
		                                   static_cast<gmsec::api::StatusCode>(errorCode),
		                                   customCode,
		                                   errorMsgStr);
	}
	catch (...)
	{
		ResetUnmanagedImplementation();
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(errorMsgStr);
	}
}


GMSEC_Exception::GMSEC_Exception(GMSEC_Exception^ %other)
{
	gmsec::api::Exception* nativeEx = other->GetUnmanagedImplementation();

	m_impl = new gmsec::api::Exception(*nativeEx);
}


GMSEC_Exception::GMSEC_Exception(Status^ %status)
{
	gmsec::api::Status* nativeStatus = status->GetUnmanagedImplementation();

	m_impl = new gmsec::api::Exception(*nativeStatus);
}


GMSEC_Exception::~GMSEC_Exception()
{
	this->!GMSEC_Exception();
}


StatusClass GMSEC_Exception::GetErrorClass()
{
	RETURN_VALUE_IF_NULLPTR_CHECK(m_impl, StatusClass::OTHER_ERROR);

	return static_cast<StatusClass>(m_impl->getErrorClass());
}


StatusCode GMSEC_Exception::GetErrorCode()
{
	RETURN_VALUE_IF_NULLPTR_CHECK(m_impl, StatusCode::OTHER_ERROR_CODE);

	return static_cast<StatusCode>(m_impl->getErrorCode());
}


Int32 GMSEC_Exception::GetCustomCode()
{
	RETURN_VALUE_IF_NULLPTR_CHECK(m_impl, 0);

	return m_impl->getCustomCode();
}


String^ GMSEC_Exception::GetErrorMessage()
{
	RETURN_VALUE_IF_NULLPTR_CHECK(m_impl, nullptr);

	return gcnew String(m_impl->getErrorMessage());
}


String^ GMSEC_Exception::ToString()
{
	START_MANAGED_CATCH
	{
		RETURN_VALUE_IF_NULLPTR_CHECK(m_impl, nullptr);

		return gcnew String(m_impl->what());
	}
	END_MANAGED_CATCH_RETURN_VALUE(nullptr)
}


GMSEC_Exception::!GMSEC_Exception()
{
	ResetUnmanagedImplementation();
}


gmsec::api::Exception* GMSEC_Exception::GetUnmanagedImplementation()
{
	return m_impl;
}


void GMSEC_Exception::SetUnmanagedImplementation(gmsec::api::Exception* exception)
{
	ResetUnmanagedImplementation();

	m_impl = exception;
}


void GMSEC_Exception::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


GMSEC_Exception::GMSEC_Exception(gmsec::api::Exception& e)
{
	m_impl = new gmsec::api::Exception(e);
}
