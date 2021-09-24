/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/message/DeviceMessage_Net.h>

#include <mist/Device_Net.h>
#include <mist/DeviceIterator_Net.h>
#include <mist/Specification_Net.h>

#include <Config_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace GMSEC::API::MIST::MESSAGE;
using namespace System;
using namespace System::Runtime::InteropServices;


DeviceMessage::DeviceMessage(String^ subject, Specification^ spec)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* subjectStr  = nullptr;

	try
	{
		subjectStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::DeviceMessage(subjectStr, *specNative);
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


DeviceMessage::DeviceMessage(String^ subject, Config^ config, Specification^ spec)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* subjectStr  = nullptr;

	try
	{
		subjectStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Config*              cfgNative  = config->GetUnmanagedImplementation();
		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::DeviceMessage(subjectStr, *cfgNative, *specNative);
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


DeviceMessage::DeviceMessage(DeviceMessage^ other)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceMessage is null");

	try
	{
		gmsec::api::mist::message::DeviceMessage* otherNative = other->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::DeviceMessage(*otherNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


DeviceMessage::DeviceMessage(String^ data)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	char* dataStr  = nullptr;

	try
	{
		dataStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::mist::message::DeviceMessage(dataStr);
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


DeviceMessage::DeviceMessage(Specification^ spec, System::String^ data)
	: MistMessage((gmsec::api::mist::message::MistMessage*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::DeviceMessage(*specNative, dataStr);
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


DeviceMessage::~DeviceMessage()
{
	this->!DeviceMessage();
}


void DeviceMessage::AddDevice(Device^ device)
{
	THROW_EXCEPTION_IF_NULLPTR(device, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		gmsec::api::mist::Device* devNative = device->GetUnmanagedImplementation();

		this->GetUnmanagedImplementation()->addDevice(*devNative);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


long DeviceMessage::GetNumDevices()
{
	return (long) this->GetUnmanagedImplementation()->getNumDevices();
}


Device^ DeviceMessage::GetDevice(long index)
{
	try
	{
		const gmsec::api::mist::Device& devNative = this->GetUnmanagedImplementation()->getDevice(index);

		return gcnew Device(const_cast<gmsec::api::mist::Device*>(&devNative), false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


DeviceIterator^ DeviceMessage::GetDeviceIterator()
{
	gmsec::api::mist::DeviceIterator& devIterNative = this->GetUnmanagedImplementation()->getDeviceIterator();

	return gcnew DeviceIterator(const_cast<gmsec::api::mist::DeviceIterator*>(&devIterNative));
}


DeviceMessage^ DeviceMessage::ConvertMessage(Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* msgNative = message->GetUnmanagedImplementation();

		gmsec::api::mist::message::DeviceMessage devMsgNative = gmsec::api::mist::message::DeviceMessage::convertMessage(*msgNative);

		return gcnew DeviceMessage(new gmsec::api::mist::message::DeviceMessage(devMsgNative), true);
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


DeviceMessage::DeviceMessage(gmsec::api::mist::message::DeviceMessage* devMsg, bool owned)
	: MistMessage(devMsg, owned)
{
}


gmsec::api::mist::message::DeviceMessage* DeviceMessage::GetUnmanagedImplementation()
{
	return (gmsec::api::mist::message::DeviceMessage*) m_impl;
}


void DeviceMessage::ResetUnmanagedImplementation()
{
	MistMessage::ResetUnmanagedImplementation();
}


void DeviceMessage::SetUnmanagedImplementation(gmsec::api::mist::message::DeviceMessage* devMsg, bool owned)
{
	ResetUnmanagedImplementation();

	MistMessage::SetUnmanagedImplementation(devMsg, owned);
}
