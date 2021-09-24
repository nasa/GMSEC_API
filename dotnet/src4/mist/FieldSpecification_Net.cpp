/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// C# managed
#include <mist/FieldSpecification_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


FieldSpecification::FieldSpecification(String^ name, String^ type, String^ mode, String^ clazz, String^ value, String^ desc)
{
	m_name  = name;
	m_type  = type;
	m_mode  = mode;
	m_class = clazz;
	m_value = value;
	m_desc  = desc;
}


FieldSpecification::~FieldSpecification()
{
	this->!FieldSpecification();
}


System::String^ FieldSpecification::GetName()
{
	//DMW THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "FieldSpecification is null");

	return m_name;
}


System::String^ FieldSpecification::GetFieldType()
{
	//DMW THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "FieldSpecification is null");

	return m_type;
}


System::String^ FieldSpecification::GetMode()
{
	//DMW THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "FieldSpecification is null");

	return m_mode;
}


System::String^ FieldSpecification::GetClassification()
{
	//DMW THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "FieldSpecification is null");

	return m_class;
}


System::String^ FieldSpecification::GetValue()
{
	//DMW THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "FieldSpecification is null");

	return m_value;
}


System::String^ FieldSpecification::GetDescription()
{
	//DMW THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "FieldSpecification is null");

	return m_desc;
}


FieldSpecification::!FieldSpecification()
{
	//DMW ResetUnmanagedImplementation();
}


/*DMW
FieldSpecification::FieldSpecification(gmsec::api::mist::FieldSpecification* fldSpec, bool owned)
{
	SetUnmanagedImplementation(fldSpec, owned);
}


gmsec::api::mist::FieldSpecification* FieldSpecification::GetUnmanagedImplementation()
{
	return m_impl;
}


void FieldSpecification::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void FieldSpecification::SetUnmanagedImplementation(gmsec::api::mist::FieldSpecification* fldSpec, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = fldSpec;
	m_owned = owned;
}
*/
