/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/DeviceParam_Net.h>

#include <field/Field_Net.h>
#include <field/FieldConverter_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


DeviceParam::DeviceParam(String^ name, String^ timestamp, Field^ value)
{
	THROW_EXCEPTION_IF_NULLPTR(value, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Value Field is null");

	char* nameStr = nullptr;
	char* timeStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		timeStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(timestamp).ToPointer());

		gmsec::api::Field* nativeField = ((Field^) value)->GetChild();

		m_impl  = new gmsec::api::mist::DeviceParam(nameStr, timeStr, *nativeField);
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
		FREE_HGLOBAL_IF_NOT_NULLPTR(timeStr);
	}
}


DeviceParam::DeviceParam(DeviceParam^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Other DeviceParam is null");

	try
	{
		gmsec::api::mist::DeviceParam* nativeOther = ((DeviceParam^) other)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::DeviceParam(*nativeOther);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


DeviceParam::~DeviceParam()
{
	this->!DeviceParam();
}


String^ DeviceParam::GetName()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceParam is null");

	return gcnew String(m_impl->getName());
}


String^ DeviceParam::GetTimestamp()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceParam is null");

	return gcnew String(m_impl->getTimestamp());
}


Field^ DeviceParam::GetValue()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceParam is null");

	const gmsec::api::Field& nativeField = m_impl->getValue();

	Field^ field = FieldConverter::CreateFieldReference(const_cast<gmsec::api::Field*>(&nativeField));

	return field;
}


DeviceParam::!DeviceParam()
{
	ResetUnmanagedImplementation();
}


DeviceParam::DeviceParam(gmsec::api::mist::DeviceParam* param, bool owned)
{
	SetUnmanagedImplementation(param, owned);
}


gmsec::api::mist::DeviceParam* DeviceParam::GetUnmanagedImplementation()
{
	return m_impl;
}


void DeviceParam::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void DeviceParam::SetUnmanagedImplementation(gmsec::api::mist::DeviceParam* param, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = param;
	m_owned = owned;
}
