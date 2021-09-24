/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <field/U64Field_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


using namespace GMSEC::API;
using namespace System;


U64Field::U64Field(String^ name, UInt64 value)
{
	char* nameStr = nullptr;

	try
	{
		nameStr     = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name).ToPointer());
		m_impl      = new gmsec::api::U64Field(nameStr, value);
		m_implOwned = true;

		RegisterChild(m_impl);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::FIELD_ERROR, StatusCode::OUT_OF_MEMORY, "U64Field constructor is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


U64Field::~U64Field()
{
	this->!U64Field();
}


UInt64 U64Field::GetValue()
{
	return m_impl->getValue();
}


U64Field::!U64Field()
{
	ResetUnmanagedImplementation();
}


U64Field::U64Field(gmsec::api::Field* field)
{
    m_impl      = dynamic_cast<gmsec::api::U64Field*>(field);
	m_implOwned = false;

	RegisterChild(m_impl);
}


void U64Field::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_implOwned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void U64Field::SetUnmanagedImplementation(gmsec::api::U64Field* field)
{
	ResetUnmanagedImplementation();

	m_impl = field;
}
