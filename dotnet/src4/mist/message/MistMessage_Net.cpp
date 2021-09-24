/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/message/MistMessage_Net.h>

#include <mist/Specification_Net.h>

#include <Config_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace GMSEC::API::MIST::MESSAGE;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


MistMessage::MistMessage(String^ subject, String^ schemaID, Specification^ spec)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* subjectStr  = nullptr;
	char* schemaIDStr = nullptr;

	try
	{
		subjectStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		schemaIDStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(schemaID).ToPointer());

		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MistMessage(subjectStr, schemaIDStr, *specNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process parameter string value");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(schemaIDStr);
	}
}


MistMessage::MistMessage(System::String^ subject, System::String^ schemaID, Config^ config, Specification^ spec)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* subjectStr  = nullptr;
	char* schemaIDStr = nullptr;

	try
	{
		subjectStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		schemaIDStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(schemaID).ToPointer());

		gmsec::api::Config*              cfgNative  = config->GetUnmanagedImplementation();
		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MistMessage(subjectStr, schemaIDStr, *cfgNative, *specNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process parameter string value");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(schemaIDStr);
	}
}


MistMessage::MistMessage(Message^ msg)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(msg, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Message is null");

	try
	{
		gmsec::api::Message* msgNative = msg->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MistMessage(*msgNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MistMessage::MistMessage(Message^ msg, Config^ specConfig)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(msg, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Message is null");
	THROW_EXCEPTION_IF_NULLPTR(specConfig, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Config is null");

	try
	{
		gmsec::api::Message* msgNative = msg->GetUnmanagedImplementation();
		gmsec::api::Config*  cfgNative = specConfig->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MistMessage(*msgNative, *cfgNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MistMessage::MistMessage(MistMessage^ other)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "MistMessage is null");

	try
	{
		gmsec::api::mist::message::MistMessage* otherNative = other->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MistMessage(*otherNative);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


MistMessage::MistMessage(System::String^ data)
	: Message((gmsec::api::Message*) nullptr, false)
{
	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::mist::message::MistMessage(dataStr);
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


MistMessage::MistMessage(Specification^ spec, System::String^ data)
	: Message((gmsec::api::Message*) nullptr, false)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		gmsec::api::mist::Specification* specNative = spec->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::message::MistMessage(*specNative, dataStr);
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


MistMessage::~MistMessage()
{
	this->!MistMessage();
}


void MistMessage::SetStandardFields(List<Field^>^ standardFields)
{
	THROW_EXCEPTION_IF_NULLPTR(standardFields, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "List of Standard Fields is null");

	gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

	for (int i = 0; i < standardFields->Count; ++i)
	{
		Field^ field = standardFields[i];

		if (field != nullptr)
		{
			nativeFields.push_back(field->GetChild());
		}
	}

	gmsec::api::mist::message::MistMessage::setStandardFields(nativeFields);
}


void MistMessage::ClearStandardFields()
{
	gmsec::api::mist::message::MistMessage::clearStandardFields();
}


String^ MistMessage::GetSchemaID()
{
	return gcnew String(this->GetUnmanagedImplementation()->getSchemaID());
}


void MistMessage::SetValue(String^ fieldName, String^ value)
{
	char* fieldNameStr = nullptr;
	char* valueStr     = nullptr;

	try
	{
		fieldNameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());
		valueStr     = static_cast<char*>(Marshal::StringToHGlobalAnsi(value).ToPointer());

		this->GetUnmanagedImplementation()->setValue(fieldNameStr, valueStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process parameter string value");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(fieldNameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(valueStr);
	}
}


void MistMessage::SetValue(String^ fieldName, Int64 value)
{
	char* fieldNameStr = nullptr;

	try
	{
		fieldNameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());

		this->GetUnmanagedImplementation()->setValue(fieldNameStr, (GMSEC_I64) value);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process parameter string value");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(fieldNameStr);
	}
}

	
void MistMessage::SetValue(String^ fieldName, double value)
{
	char* fieldNameStr = nullptr;

	try
	{
		fieldNameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());

		this->GetUnmanagedImplementation()->setValue(fieldNameStr, (GMSEC_F64) value);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process parameter string value");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(fieldNameStr);
	}
}


MistMessage::!MistMessage()
{
	ResetUnmanagedImplementation();
}


MistMessage::MistMessage(gmsec::api::mist::message::MistMessage* mistMsg, bool owned)
	: Message(mistMsg, owned)
{
}


gmsec::api::mist::message::MistMessage* MistMessage::GetUnmanagedImplementation()
{
	return (gmsec::api::mist::message::MistMessage*) m_impl;
}


void MistMessage::ResetUnmanagedImplementation()
{
	Message::ResetUnmanagedImplementation();
}


void MistMessage::SetUnmanagedImplementation(gmsec::api::mist::message::MistMessage* mistMsg, bool owned)
{
	ResetUnmanagedImplementation();

	Message::SetUnmanagedImplementation(mistMsg, owned);
}
