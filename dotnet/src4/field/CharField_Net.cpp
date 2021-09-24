/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <field/CharField_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


using namespace GMSEC::API;
using namespace System;


CharField::CharField(String^ name, char value)
{
	char* nameStr = nullptr;

	try
	{
		nameStr     = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name).ToPointer());
		m_impl      = new gmsec::api::CharField(nameStr, value);
		m_implOwned = true;

		RegisterChild(m_impl);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::FIELD_ERROR, StatusCode::OUT_OF_MEMORY, "CharField constructor is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


CharField::~CharField()
{
	this->!CharField();
}


char CharField::GetValue()
{
	return m_impl->getValue();
}


CharField::!CharField()
{
	ResetUnmanagedImplementation();
}


CharField::CharField(gmsec::api::Field* field)
{
    m_impl      = dynamic_cast<gmsec::api::CharField*>(field);
	m_implOwned = false;

	RegisterChild(m_impl);
}


void CharField::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_implOwned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void CharField::SetUnmanagedImplementation(gmsec::api::CharField* field)
{
	ResetUnmanagedImplementation();

	m_impl = field;
}
