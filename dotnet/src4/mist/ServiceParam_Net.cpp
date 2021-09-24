/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/ServiceParam_Net.h>

#include <field/Field_Net.h>
#include <field/FieldConverter_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


ServiceParam::ServiceParam(String^ name, Field^ value)
{
	THROW_EXCEPTION_IF_NULLPTR(value, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Value Field is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::Field* nativeField = ((Field^) value)->GetChild();

		m_impl  = new gmsec::api::mist::ServiceParam(nameStr, *nativeField);
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
	}
}


ServiceParam::ServiceParam(ServiceParam^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Other ServiceParam is null");

	try
	{
		gmsec::api::mist::ServiceParam* nativeOther = ((ServiceParam^) other)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::ServiceParam(*nativeOther);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ServiceParam::~ServiceParam()
{
	this->!ServiceParam();
}


String^ ServiceParam::GetName()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ServiceParam is null");

	return gcnew String(m_impl->getName());
}


Field^ ServiceParam::GetValue()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ServiceParam is null");

	const gmsec::api::Field& nativeField = m_impl->getValue();

	Field^ field = FieldConverter::CreateFieldReference(const_cast<gmsec::api::Field*>(&nativeField));

	return field;
}


ServiceParam::!ServiceParam()
{
	ResetUnmanagedImplementation();
}


ServiceParam::ServiceParam(gmsec::api::mist::ServiceParam* param, bool owned)
{
	SetUnmanagedImplementation(param, owned);
}


gmsec::api::mist::ServiceParam* ServiceParam::GetUnmanagedImplementation()
{
	return m_impl;
}


void ServiceParam::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void ServiceParam::SetUnmanagedImplementation(gmsec::api::mist::ServiceParam* param, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = param;
	m_owned = owned;
}
