/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/MnemonicSample_Net.h>
#include <field/Field_Net.h>
#include <field/FieldConverter_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/field/Field.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


MnemonicSample::MnemonicSample(System::String^ timestamp, Field^ rawValue)
{
	THROW_EXCEPTION_IF_NULLPTR(rawValue, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Raw Value Field is null");

	char* timeStr = nullptr;

	try	
	{
		timeStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(timestamp).ToPointer());

		gmsec::api::Field* nativeField = ((Field^) rawValue)->GetChild();

		m_impl  = new gmsec::api::mist::MnemonicSample(timeStr, *nativeField);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process timestamp string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(timeStr);
	}
}


MnemonicSample::MnemonicSample(MnemonicSample^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	gmsec::api::mist::MnemonicSample* nativeOther = ((MnemonicSample^) other)->GetUnmanagedImplementation();

	m_impl  = new gmsec::api::mist::MnemonicSample(*nativeOther);
	m_owned = true;
}


MnemonicSample::~MnemonicSample()
{
	this->!MnemonicSample();
}


String^ MnemonicSample::GetTimestamp()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	return gcnew String(m_impl->getTimestamp());
}


Field^ MnemonicSample::GetRawValue()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	const gmsec::api::Field& nativeRawValue = m_impl->getRawValue();

	return FieldConverter::CreateFieldReference(const_cast<gmsec::api::Field*>(&nativeRawValue));
}


void MnemonicSample::SetEUValue(Field^ euValue)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");
	THROW_EXCEPTION_IF_NULLPTR(euValue, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Field is null");

	gmsec::api::Field* nativeField = ((Field^) euValue)->GetChild();

	try
	{
		m_impl->setEUValue(*nativeField);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


bool MnemonicSample::HasEUValue()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	return m_impl->hasEUValue();
}


Field^ MnemonicSample::GetEUValue()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	try
	{
		const gmsec::api::Field& nativeField = m_impl->getEUValue();

		return FieldConverter::CreateFieldReference(const_cast<gmsec::api::Field*>(&nativeField));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MnemonicSample::SetTextValue(String^ textValue)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	char* textStr = nullptr;

	try	
	{
		textStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(textValue).ToPointer());

		m_impl->setTextValue(textStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process text value string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(textStr);
	}
}


bool MnemonicSample::HasTextValue()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	return m_impl->hasTextValue();
}


String^ MnemonicSample::GetTextValue()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	try
	{
		return gcnew String(m_impl->getTextValue());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MnemonicSample::SetFlags(Int32 flags)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	m_impl->setFlags(flags);
}


bool MnemonicSample::HasFlags()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	return m_impl->hasFlags();
}


Int32 MnemonicSample::GetFlags()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	try
	{
		return m_impl->getFlags();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MnemonicSample::SetLimitEnableDisable(bool value)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	m_impl->setLimitEnableDisable(value);
}


bool MnemonicSample::HasLimitEnableDisable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	return m_impl->hasLimitEnableDisable();
}


bool MnemonicSample::GetLimitEnableDisable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	try
	{
		return m_impl->getLimitEnableDisable();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MnemonicSample::SetLimit(LimitFlag limit)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	m_impl->setLimit(static_cast<gmsec::api::mist::MnemonicSample::LimitFlag>(limit));
}


bool MnemonicSample::HasLimit()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	return m_impl->hasLimit();
}


MnemonicSample::LimitFlag MnemonicSample::GetLimit()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	try
	{
		return static_cast<MnemonicSample::LimitFlag>(m_impl->getLimit());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MnemonicSample::SetStalenessStatus(bool staleness)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	m_impl->setStalenessStatus(staleness);
}


bool MnemonicSample::HasStalenessStatus()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	return m_impl->hasStalenessStatus();
}


bool MnemonicSample::GetStalenessStatus()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	try
	{
		return m_impl->getStalenessStatus();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MnemonicSample::SetQuality(bool quality)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	m_impl->setQuality(quality);
}


bool MnemonicSample::HasQuality()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	return m_impl->hasQuality();
}


bool MnemonicSample::GetQuality()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "MnemonicSample is null");

	try
	{
		return m_impl->getQuality();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MnemonicSample::!MnemonicSample()
{
	ResetUnmanagedImplementation();
}


MnemonicSample::MnemonicSample(gmsec::api::mist::MnemonicSample* sample, bool owned)
{
	SetUnmanagedImplementation(sample, owned);
}


gmsec::api::mist::MnemonicSample* MnemonicSample::GetUnmanagedImplementation()
{
	return m_impl;
}


void MnemonicSample::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void MnemonicSample::SetUnmanagedImplementation(gmsec::api::mist::MnemonicSample* sample, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = sample;
	m_owned = owned;
}
