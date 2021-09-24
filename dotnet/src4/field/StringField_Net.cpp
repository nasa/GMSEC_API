/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <field/StringField_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


using namespace GMSEC::API;
using namespace System;


StringField::StringField(String^ name, String^ value)
{
	char* nameStr  = nullptr;
	char* valueStr = nullptr;

	try
	{
		nameStr     = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name).ToPointer());
		valueStr    = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(value).ToPointer());
		m_impl      = new gmsec::api::StringField(nameStr, valueStr);
		m_implOwned = true;

		RegisterChild(m_impl);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::FIELD_ERROR, StatusCode::OUT_OF_MEMORY, "StringField constructor is unable to process input String(s)");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(valueStr);
	}
}


StringField::~StringField()
{
	this->!StringField();
}


String^ StringField::GetValue()
{
	return gcnew String(m_impl->getValue());
}


StringField::!StringField()
{
	ResetUnmanagedImplementation();
}


StringField::StringField(gmsec::api::Field* field)
{
    m_impl      = dynamic_cast<gmsec::api::StringField*>(field);
	m_implOwned = false;

	RegisterChild(m_impl);
}


void StringField::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_implOwned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void StringField::SetUnmanagedImplementation(gmsec::api::StringField* field)
{
	ResetUnmanagedImplementation();

	m_impl = field;
}
