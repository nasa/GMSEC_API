/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <field/U8Field_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


using namespace GMSEC::API;
using namespace System;


U8Field::U8Field(String^ name, Byte value)
{
	char* nameStr = nullptr;

	try
	{
		nameStr     = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name).ToPointer());
		m_impl      = new gmsec::api::U8Field(nameStr, value);
		m_implOwned = true;

		RegisterChild(m_impl);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::FIELD_ERROR, StatusCode::OUT_OF_MEMORY, "U8Field constructor is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


U8Field::~U8Field()
{
	this->!U8Field();
}


Byte U8Field::GetValue()
{
	return m_impl->getValue();
}


U8Field::!U8Field()
{
	ResetUnmanagedImplementation();
}


U8Field::U8Field(gmsec::api::Field* field)
{
    m_impl      = dynamic_cast<gmsec::api::U8Field*>(field);
	m_implOwned = false;

	RegisterChild(m_impl);
}


void U8Field::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_implOwned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void U8Field::SetUnmanagedImplementation(gmsec::api::U8Field* field)
{
	ResetUnmanagedImplementation();

	m_impl = field;
}
