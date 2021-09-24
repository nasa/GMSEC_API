/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <ConfigFileIterator_Net.h>

#include <GMSEC_Exception_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace System;
using namespace System::Runtime::InteropServices;


ConfigFileIterator::~ConfigFileIterator()
{
	this->!ConfigFileIterator();
}


bool ConfigFileIterator::HasNextConfig()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::ITERATOR_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFileIterator is null");

	return m_impl->hasNextConfig();
}


bool ConfigFileIterator::HasNextMessage()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::ITERATOR_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFileIterator is null");

	return m_impl->hasNextMessage();
}


bool ConfigFileIterator::HasNextSubscription()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::ITERATOR_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFileIterator is null");

	return m_impl->hasNextSubscription();
}


bool ConfigFileIterator::HasNextCustomElement()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::ITERATOR_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFileIterator is null");

	return m_impl->hasNextCustomElement();
}


ConfigFile::ConfigEntry^ ConfigFileIterator::NextConfig()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::ITERATOR_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFileIterator is null");

	try
	{
		const gmsec::api::ConfigFile::ConfigEntry& entry = m_impl->nextConfig();

		return gcnew ConfigFile::ConfigEntry((gmsec::api::ConfigFile::ConfigEntry*) &entry);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ConfigFile::MessageEntry^ ConfigFileIterator::NextMessage()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::ITERATOR_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFileIterator is null");

	try
	{
		const gmsec::api::ConfigFile::MessageEntry& entry = m_impl->nextMessage();

		return gcnew ConfigFile::MessageEntry((gmsec::api::ConfigFile::MessageEntry*) &entry);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ConfigFile::SubscriptionEntry^ ConfigFileIterator::NextSubscription()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::ITERATOR_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFileIterator is null");

	try
	{
		const gmsec::api::ConfigFile::SubscriptionEntry& entry = m_impl->nextSubscription();

		return gcnew ConfigFile::SubscriptionEntry((gmsec::api::ConfigFile::SubscriptionEntry*) &entry);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


System::String^ ConfigFileIterator::NextCustomElement()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::ITERATOR_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFileIterator is null");

	try
	{
		return gcnew String(m_impl->nextCustomElement());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ConfigFileIterator::ConfigFileIterator()
{
	// nothing to do
}


ConfigFileIterator::!ConfigFileIterator()
{
	ResetUnmanagedImplementation();
}


ConfigFileIterator::ConfigFileIterator(gmsec::api::ConfigFileIterator* iter)
{
	m_impl = iter;
}


gmsec::api::ConfigFileIterator* ConfigFileIterator::GetUnmanagedImplementation()
{
	return m_impl;
}


void ConfigFileIterator::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}


void ConfigFileIterator::SetUnmanagedImplementation(gmsec::api::ConfigFileIterator* iter)
{
	ResetUnmanagedImplementation();

	m_impl = iter;
}
