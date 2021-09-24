/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <field/BinaryField_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


using namespace GMSEC::API;
using namespace System;


BinaryField::BinaryField(String^ name, array<Byte>^ data)
{
	char*  nameStr    = nullptr;
	Byte*  dataPtr    = nullptr;
	size_t dataLength = 0;

	if (data != nullptr)
	{
		pin_ptr<Byte> p = &data[0];

		dataPtr    = p;
		dataLength = (size_t) data->Length;
	}

	try
	{
		nameStr     = static_cast<char*>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name).ToPointer());
		m_impl      = new gmsec::api::BinaryField(nameStr, dataPtr, dataLength);
		m_implOwned = true;

		RegisterChild(m_impl);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::FIELD_ERROR, StatusCode::OUT_OF_MEMORY, "BinaryField constructor is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


BinaryField::~BinaryField()
{
	this->!BinaryField();
}


array<Byte>^ BinaryField::GetValue()
{
	unsigned char* values = m_impl->getValue();
	size_t         length = m_impl->getLength();

	array<Byte>^ data = gcnew array<Byte>((int) length);

	Runtime::InteropServices::Marshal::Copy((IntPtr) values, data, 0, (int) length);

	return data;
}


Int64 BinaryField::GetLength()
{
	return (Int64) m_impl->getLength();
}


BinaryField::!BinaryField()
{
	ResetUnmanagedImplementation();
}


BinaryField::BinaryField(gmsec::api::Field* field)
{
	m_impl      = dynamic_cast<gmsec::api::BinaryField*>(field);
	m_implOwned = false;

	RegisterChild(m_impl);
}


void BinaryField::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_implOwned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void BinaryField::SetUnmanagedImplementation(gmsec::api::BinaryField* field)
{
	ResetUnmanagedImplementation();

	m_impl = field;
}
