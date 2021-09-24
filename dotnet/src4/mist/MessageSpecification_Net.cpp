/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// C# managed
#include <mist/MessageSpecification_Net.h>
#include <mist/FieldSpecification_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;



MessageSpecification::MessageSpecification(String^ schemaID, List<FieldSpecification^>^ fieldSpecs)
{
	m_schemaID   = schemaID;
	m_fieldSpecs = fieldSpecs;
}


MessageSpecification::~MessageSpecification()
{
	this->!MessageSpecification();
}


System::String^ MessageSpecification::GetSchemaID()
{
	//DMW THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MessageSpecification is null");

	//DMW return gcnew String(m_impl->getSchemaID());

	return m_schemaID;
}


List<FieldSpecification^>^ MessageSpecification::GetFieldSpecifications()
{
	//DMW THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MessageSpecification is null");

	return m_fieldSpecs;
}


MessageSpecification::!MessageSpecification()
{
	//DMW ResetUnmanagedImplementation();
}


/*DMW
MessageSpecification::MessageSpecification(gmsec::api::mist::MessageSpecification* msgSpec, bool owned)
{
	SetUnmanagedImplementation(msgSpec, owned);
}


gmsec::api::mist::MessageSpecification* MessageSpecification::GetUnmanagedImplementation()
{
	return m_impl;
}


void MessageSpecification::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void MessageSpecification::SetUnmanagedImplementation(gmsec::api::mist::MessageSpecification* msgSpec, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = msgSpec;
	m_owned = owned;
}
*/
