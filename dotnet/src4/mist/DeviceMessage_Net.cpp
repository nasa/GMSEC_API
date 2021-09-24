/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// C# managed
#include <mist/DeviceMessage_Net.h>

#include <mist/Device_Net.h>
#include <mist/DeviceIterator_Net.h>

#include <Config_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


DeviceMessage::DeviceMessage(String^ subject, unsigned int version)
	: Message((gmsec::api::Message*) nullptr, false)
{
	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		m_impl  = new gmsec::api::mist::DeviceMessage(subjectStr, version);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


DeviceMessage::DeviceMessage(String^ subject, Config^ config, unsigned int version)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::DeviceMessage(subjectStr, *nativeConfig, version);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


DeviceMessage::DeviceMessage(String^ data)
	: Message((gmsec::api::Message*) nullptr, false)
{
	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::mist::DeviceMessage(dataStr);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process data string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(dataStr);
	}
}


DeviceMessage::DeviceMessage(DeviceMessage^ other)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "DeviceMessage is null");

	try
	{
		gmsec::api::mist::DeviceMessage* nativeMsg = ((DeviceMessage^) other)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::DeviceMessage(*nativeMsg);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


DeviceMessage::~DeviceMessage()
{
	this->!DeviceMessage();
}


long DeviceMessage::GetNumDevices()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceMessage is null");

	return (long) ((gmsec::api::mist::DeviceMessage*) m_impl)->getNumDevices();
}


Device^ DeviceMessage::GetDevice(long index)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceMessage is null");

	try
	{
		const gmsec::api::mist::Device& nativeDevice = ((gmsec::api::mist::DeviceMessage*) m_impl)->getDevice((size_t) index);

		return gcnew Device(const_cast<gmsec::api::mist::Device*>(&nativeDevice), false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void DeviceMessage::AddDevice(Device^ device)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceMessage is null");
	THROW_EXCEPTION_IF_NULLPTR(device, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		gmsec::api::mist::Device* nativeDevice = ((Device^) device)->GetUnmanagedImplementation();

		((gmsec::api::mist::DeviceMessage*) m_impl)->addDevice(*nativeDevice);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


DeviceIterator^ DeviceMessage::GetDeviceIterator()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceMessage is null");

	gmsec::api::mist::DeviceIterator& iter = ((gmsec::api::mist::DeviceMessage*) m_impl)->getDeviceIterator();

	return gcnew DeviceIterator(&iter);
}


DeviceMessage^ DeviceMessage::ConvertMessage(Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* nativeMsg = ((Message^) message)->GetUnmanagedImplementation();

		gmsec::api::mist::DeviceMessage nativeDevMsg = gmsec::api::mist::DeviceMessage::convertMessage(*nativeMsg);

		return gcnew DeviceMessage(new gmsec::api::mist::DeviceMessage(nativeDevMsg), true);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


DeviceMessage::!DeviceMessage()
{
	ResetUnmanagedImplementation();
}


DeviceMessage::DeviceMessage(gmsec::api::mist::DeviceMessage* deviceMsg, bool owned)
	: Message(deviceMsg, owned)
{
}


gmsec::api::mist::DeviceMessage* DeviceMessage::GetUnmanagedImplementation()
{
	return (gmsec::api::mist::DeviceMessage*) m_impl;
}


void DeviceMessage::ResetUnmanagedImplementation()
{
	Message::ResetUnmanagedImplementation();
}


void DeviceMessage::SetUnmanagedImplementation(gmsec::api::mist::DeviceMessage* deviceMsg, bool owned)
{
	ResetUnmanagedImplementation();

	Message::SetUnmanagedImplementation(deviceMsg, owned);
}
