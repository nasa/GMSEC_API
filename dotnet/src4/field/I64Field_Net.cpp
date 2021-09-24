/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <field/I64Field_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


using namespace GMSEC::API;
using namespace System;


I64Field::I64Field(String^ name, Int64 value)
{
	char* nameStr = nullptr;

	try
	{
		nameStr     = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name).ToPointer());
		m_impl      = new gmsec::api::I64Field(nameStr, value);
		m_implOwned = true;

		RegisterChild(m_impl);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::FIELD_ERROR, StatusCode::OUT_OF_MEMORY, "I64Field constructor is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


I64Field::~I64Field()
{
	this->!I64Field();
}


Int64 I64Field::GetValue()
{
	return m_impl->getValue();
}


I64Field::!I64Field()
{
	ResetUnmanagedImplementation();
}


I64Field::I64Field(gmsec::api::Field* field)
{
    m_impl      = dynamic_cast<gmsec::api::I64Field*>(field);
	m_implOwned = false;

	RegisterChild(m_impl);
}


void I64Field::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_implOwned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void I64Field::SetUnmanagedImplementation(gmsec::api::I64Field* field)
{
	ResetUnmanagedImplementation();

	m_impl = field;
}
