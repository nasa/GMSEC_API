/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <field/U32Field_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


using namespace GMSEC::API;
using namespace System;


U32Field::U32Field(String^ name, UInt32 value)
{
	char* nameStr = nullptr;

	try
	{
		nameStr     = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name).ToPointer());
		m_impl      = new gmsec::api::U32Field(nameStr, value);
		m_implOwned = true;

		RegisterChild(m_impl);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::FIELD_ERROR, StatusCode::OUT_OF_MEMORY, "U32Field constructor is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


U32Field::~U32Field()
{
	this->!U32Field();
}


UInt32 U32Field::GetValue()
{
	return m_impl->getValue();
}


U32Field::!U32Field()
{
	ResetUnmanagedImplementation();
}


U32Field::U32Field(gmsec::api::Field* field)
{
    m_impl      = dynamic_cast<gmsec::api::U32Field*>(field);
	m_implOwned = false;

	RegisterChild(m_impl);
}


void U32Field::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_implOwned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void U32Field::SetUnmanagedImplementation(gmsec::api::U32Field* field)
{
	ResetUnmanagedImplementation();

	m_impl = field;
}
