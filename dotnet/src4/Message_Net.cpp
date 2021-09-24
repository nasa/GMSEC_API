/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <Message_Net.h>

#include <Config_Net.h>
#include <GMSEC_Net.h>
#include <Fields_Net.h>
#include <MessageFieldIterator_Net.h>

#include <field/FieldConverter_Net.h>


// C++ API native
#include <gmsec4/Exception.h>
#include <gmsec4/util/DataList.h>


using namespace GMSEC::API;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


Message::Message(String^ subject, MessageKind kind)
{
	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		m_impl  = new gmsec::api::Message(subjectStr, static_cast<gmsec::api::Message::MessageKind>(kind));
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


Message::Message(String^ subject, MessageKind kind, Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MSG_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::Message(subjectStr, static_cast<gmsec::api::Message::MessageKind>(kind), *nativeConfig);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


Message::Message(String^ data)
{
	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::Message(dataStr);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process data string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(dataStr);
	}
}


Message::Message(Message^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MSG_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	gmsec::api::Message* nativeOther = ((Message^) other)->GetUnmanagedImplementation();

	m_impl  = new gmsec::api::Message(*nativeOther);
	m_owned = true;
}


Message::~Message()
{
	this->!Message();
}


Message::MessageKind Message::GetKind()
{
	return static_cast<Message::MessageKind>(m_impl->getKind());
}


String^ Message::GetSubject()
{
	return gcnew String(m_impl->getSubject());
}


void Message::SetSubject(String^ subject)
{
	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		m_impl->setSubject(subjectStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


Config^ Message::GetConfig()
{
	gmsec::api::Config& cfg = const_cast<gmsec::api::Config&>(m_impl->getConfig());

	return gcnew Config(&cfg, false);
}


void Message::AddConfig(Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MSG_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

	m_impl->addConfig(*nativeConfig);
}


bool Message::AddField(Field^ field)
{
	THROW_EXCEPTION_IF_NULLPTR(field, StatusClass::MSG_ERROR, StatusCode::INVALID_FIELD, "Field is null");

	gmsec::api::Field* nativeField = ((Field^) field)->GetChild();

	return m_impl->addField(*nativeField);
}


bool Message::AddFields(List<Field^>^ fields)
{
	THROW_EXCEPTION_IF_NULLPTR(fields, StatusClass::MSG_ERROR, StatusCode::UNINITIALIZED_OBJECT, "List of Fields is null");

	bool fieldsReplaced = false;

	try
	{
		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		for (int i = 0; i < fields->Count; ++i)
		{
			Field^ field = fields[i];

			if (field != nullptr)
			{
				nativeFields.push_back(field->GetChild());
			}
		}

		fieldsReplaced = m_impl->addFields(nativeFields);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}

	return fieldsReplaced;
}


void Message::ClearFields()
{
	m_impl->clearFields();
}


bool Message::ClearField(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->clearField(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Int64 Message::GetIntegerValue(String^ fieldName)
{
	char* fieldNameStr = nullptr;

	try
	{
		fieldNameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());

		return m_impl->getIntegerValue(fieldNameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process field name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(fieldNameStr);
	}
}


UInt64 Message::GetUnsignedIntegerValue(String^ fieldName)
{
	char* fieldNameStr = nullptr;

	try
	{
		fieldNameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());

		return m_impl->getUnsignedIntegerValue(fieldNameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process field name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(fieldNameStr);
	}
}


double Message::GetDoubleValue(String^ fieldName)
{
	char* fieldNameStr = nullptr;

	try
	{
		fieldNameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());

		return m_impl->getDoubleValue(fieldNameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process field name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(fieldNameStr);
	}
}


String^ Message::GetStringValue(String^ fieldName)
{
	char* fieldNameStr = nullptr;

	try
	{
		fieldNameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());

		return gcnew String(m_impl->getStringValue(fieldNameStr));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process field name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(fieldNameStr);
	}
}


Field^ Message::GetField(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		const gmsec::api::Field* nativeField = m_impl->getField(nameStr);

		if (nativeField)
		{
			return FieldConverter::CreateFieldReference(const_cast<gmsec::api::Field*>(nativeField));
		}
		else
		{
			return nullptr;
		}
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Field::FieldType Message::GetFieldType(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return static_cast<Field::FieldType>(m_impl->getFieldType(nameStr));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


BinaryField^ Message::GetBinaryField(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::BinaryField& field = const_cast<gmsec::api::BinaryField&>(m_impl->getBinaryField(nameStr));

		return gcnew BinaryField(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


BooleanField^ Message::GetBooleanField(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::BooleanField& field = const_cast<gmsec::api::BooleanField&>(m_impl->getBooleanField(nameStr));

		return gcnew BooleanField(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


CharField^ Message::GetCharField(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::CharField& field = const_cast<gmsec::api::CharField&>(m_impl->getCharField(nameStr));

		return gcnew CharField(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


F32Field^ Message::GetF32Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::F32Field& field = const_cast<gmsec::api::F32Field&>(m_impl->getF32Field(nameStr));

		return gcnew F32Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


F64Field^ Message::GetF64Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::F64Field& field = const_cast<gmsec::api::F64Field&>(m_impl->getF64Field(nameStr));

		return gcnew F64Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


I8Field^ Message::GetI8Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::I8Field& field = const_cast<gmsec::api::I8Field&>(m_impl->getI8Field(nameStr));

		return gcnew I8Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


I16Field^ Message::GetI16Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::I16Field& field = const_cast<gmsec::api::I16Field&>(m_impl->getI16Field(nameStr));

		return gcnew I16Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


I32Field^ Message::GetI32Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::I32Field& field = const_cast<gmsec::api::I32Field&>(m_impl->getI32Field(nameStr));

		return gcnew I32Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


I64Field^ Message::GetI64Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::I64Field& field = const_cast<gmsec::api::I64Field&>(m_impl->getI64Field(nameStr));

		return gcnew I64Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


U8Field^ Message::GetU8Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::U8Field& field = const_cast<gmsec::api::U8Field&>(m_impl->getU8Field(nameStr));

		return gcnew U8Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


U16Field^ Message::GetU16Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::U16Field& field = const_cast<gmsec::api::U16Field&>(m_impl->getU16Field(nameStr));

		return gcnew U16Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


U32Field^ Message::GetU32Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::U32Field& field = const_cast<gmsec::api::U32Field&>(m_impl->getU32Field(nameStr));

		return gcnew U32Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


U64Field^ Message::GetU64Field(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::U64Field& field = const_cast<gmsec::api::U64Field&>(m_impl->getU64Field(nameStr));

		return gcnew U64Field(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


StringField^ Message::GetStringField(System::String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::StringField& field = const_cast<gmsec::api::StringField&>(m_impl->getStringField(nameStr));

		return gcnew StringField(&field);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MSG_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


int Message::GetFieldCount()
{
	return (int) m_impl->getFieldCount();
}


void Message::CopyFields(Message^ toMsg)
{
	THROW_EXCEPTION_IF_NULLPTR(toMsg, StatusClass::MSG_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	gmsec::api::Message* nativeMsg = toMsg->GetUnmanagedImplementation();

	m_impl->copyFields(*nativeMsg);
}


String^ Message::ToXML()
{
	return gcnew String(m_impl->toXML());
}


String^ Message::ToJSON()
{
	return gcnew String(m_impl->toJSON());
}


long Message::GetSize()
{
	return (long) m_impl->getSize();
}


MessageFieldIterator^ Message::GetFieldIterator()
{
	gmsec::api::MessageFieldIterator& iter = m_impl->getFieldIterator();

	return gcnew MessageFieldIterator(&iter);
}


MessageFieldIterator^ Message::GetFieldIterator(MessageFieldIterator::Selector selector)
{
	gmsec::api::MessageFieldIterator::Selector sel = static_cast<gmsec::api::MessageFieldIterator::Selector>(selector);
	gmsec::api::MessageFieldIterator& iter = m_impl->getFieldIterator(sel);

	return gcnew MessageFieldIterator(&iter);
}


Message::!Message()
{
	ResetUnmanagedImplementation();
}


Message::Message(gmsec::api::Message* msg, bool owned)
{
	SetUnmanagedImplementation(msg, owned);
}


gmsec::api::Message* Message::GetUnmanagedImplementation()
{
	return m_impl;
}


void Message::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl  = nullptr;
		m_owned = false;
	}
}


void Message::SetUnmanagedImplementation(gmsec::api::Message* msg, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = msg;
	m_owned = owned;
}
