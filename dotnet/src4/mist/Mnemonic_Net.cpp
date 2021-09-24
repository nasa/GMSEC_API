/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/Mnemonic_Net.h>
#include <mist/MnemonicSample_Net.h>
#include <field/Field_Net.h>
#include <field/FieldConverter_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/field/Field.h>
#include <gmsec4/util/DataList.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

void Mnemonic::ConstructorImplementation(String^ name, List<MnemonicSample^>^ samples)
{
        THROW_EXCEPTION_IF_NULLPTR(samples, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Sample list is null");

        char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::util::DataList<gmsec::api::mist::MnemonicSample*> sampleList;

		for (int i = 0; i < samples->Count; ++i)
		{
			MnemonicSample^ sample = samples[i];

			sampleList.push_back(sample->GetUnmanagedImplementation());
		}

		m_impl  = new gmsec::api::mist::Mnemonic(nameStr, sampleList);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Mnemonic::Mnemonic(String^ name, List<MnemonicSample^>^ samples)
{
        ConstructorImplementation(name, samples);	
}


Mnemonic::Mnemonic(String^ name)
{
        ConstructorImplementation(name, gcnew List<MnemonicSample^>());
}


Mnemonic::Mnemonic(Mnemonic^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	gmsec::api::mist::Mnemonic* nativeOther = ((Mnemonic^) other)->GetUnmanagedImplementation();

	m_impl  = new gmsec::api::mist::Mnemonic(*nativeOther);
	m_owned = true;
}


Mnemonic::~Mnemonic()
{
	this->!Mnemonic();
}


String^ Mnemonic::GetName()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	return gcnew String(m_impl->getName());
}


void Mnemonic::SetStatus(Field^ status)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");
	THROW_EXCEPTION_IF_NULLPTR(status, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Status Field is null");

	try
	{
		gmsec::api::Field* nativeStatus = ((Field^) status)->GetChild();

		m_impl->setStatus(*nativeStatus);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


bool Mnemonic::StatusAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	return m_impl->statusAvailable();
}


Field^ Mnemonic::GetStatus()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	try
	{
		const gmsec::api::Field& nativeStatus = m_impl->getStatus();

		return FieldConverter::CreateFieldReference(const_cast<gmsec::api::Field*>(&nativeStatus));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Mnemonic::SetUnits(String^ units)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	char* unitStr = nullptr;

	try
	{
		unitStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(units).ToPointer());

		m_impl->setUnits(unitStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process units string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(unitStr);
	}
}


bool Mnemonic::UnitsAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	return m_impl->unitsAvailable();
}


String^ Mnemonic::GetUnits()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	try
	{
		return gcnew String(m_impl->getUnits());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Mnemonic::AddSample(MnemonicSample^ sample)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");
	THROW_EXCEPTION_IF_NULLPTR(sample, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MnemonicSample is null");

	gmsec::api::mist::MnemonicSample* nativeSample = ((MnemonicSample^) sample)->GetUnmanagedImplementation();

	m_impl->addSample(*nativeSample);
}


long Mnemonic::GetSampleCount()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	return (long) m_impl->getSampleCount();
}


MnemonicSample^ Mnemonic::GetSample(long index)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Mnemonic is null");

	try
	{
		const gmsec::api::mist::MnemonicSample& nativeSample = m_impl->getSample((size_t) index);

		return gcnew MnemonicSample(const_cast<gmsec::api::mist::MnemonicSample*>(&nativeSample), false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Mnemonic::!Mnemonic()
{
	ResetUnmanagedImplementation();
}


Mnemonic::Mnemonic(gmsec::api::mist::Mnemonic* mnemonic, bool owned)
{
	SetUnmanagedImplementation(mnemonic, owned);
}


gmsec::api::mist::Mnemonic* Mnemonic::GetUnmanagedImplementation()
{
	return m_impl;
}


void Mnemonic::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void Mnemonic::SetUnmanagedImplementation(gmsec::api::mist::Mnemonic* mnemonic, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = mnemonic;
	m_owned = owned;
}
