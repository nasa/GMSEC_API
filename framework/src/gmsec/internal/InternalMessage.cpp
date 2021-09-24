/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file InternalMessage.cpp
 *
 * This file forwards functions from the C++ Message interface to the
 * implementation.
 */

#include <gmsec/internal/InternalMessage.h>

#include <gmsec/Field.h>
#include <gmsec/FieldIterator.h>
#include <gmsec/Message.h>

#include <gmsec/internal/Log.h>
#include <gmsec/internal/SimpleFieldIterator.h>

#include <gmsec/internal/API4_Adapter.h>

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>
#include <gmsec4/Exception.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>


#define DEFAULT_SUBJECT "GMSEC.DEFAULT.DEFAULT1.TYPE.SUBTYPE"


using namespace gmsec;
using namespace gmsec::internal;


InternalMessage::InternalMessage()
	: m_adapter(new gmsec::api::Message(DEFAULT_SUBJECT, gmsec::api::Message::PUBLISH)),
	  m_fields(MessageFieldMap::BINARY_TREE_MAP, 100),
	  m_kindUnset(true)
{
}


InternalMessage::InternalMessage(const char* subject, GMSEC_MSG_KIND msgKind)
	: m_adapter(0),
	  m_fields(MessageFieldMap::BINARY_TREE_MAP, 100),
	  m_kindUnset(msgKind == GMSEC_MSG_UNSET)
{
	gmsec::api::Message::MessageKind kind = API4_Adapter::API3MsgKindToAPI4(msgKind);

	m_adapter = new gmsec::api::Message(subject, kind);
}


InternalMessage::InternalMessage(const char* subject, GMSEC_MSG_KIND msgKind, Config* config)
	: m_adapter(0),
	  m_fields(MessageFieldMap::BINARY_TREE_MAP, 100),
	  m_kindUnset(msgKind == GMSEC_MSG_UNSET)
{
	// Note: The subject will need to be validated elsewhere (i.e. BaseConnection)
	//
	gmsec::api::Message::MessageKind kind = API4_Adapter::API3MsgKindToAPI4(msgKind);
	gmsec::api::Config               cfg  = API4_Adapter::API3ConfigToAPI4(config);

	m_adapter = new gmsec::api::Message(subject, kind, cfg);

	// A little redundant, but we do this because it may alter m_kindUnset
	SetConfig(config);
}


InternalMessage::InternalMessage(gmsec::api::Message* msg4)
	: m_adapter(msg4),
	  m_fields(MessageFieldMap::BINARY_TREE_MAP, 100),
	  m_kindUnset(false)
{
}


InternalMessage::~InternalMessage()
{
	delete m_adapter;
	m_adapter = NULL;
}


// deprecated
bool InternalMessage::isValid()
{
	return IsValid();
}


bool InternalMessage::IsValid()
{
	return true;
}


const char* InternalMessage::GetLibraryRootName()
{
	return "GMSECAPI";
}


Status InternalMessage::SetConfig(Config* config)
{
	Status status;

	gmsec::api::Config cfg = API4_Adapter::API3ConfigToAPI4(config);

	m_adapter->addConfig(cfg);

	const char* key   = 0;
	const char* value = 0;

	bool hasPair = cfg.getFirst(key, value);

	while (hasPair)
	{
		if (gmsec::api::util::StringUtil::stringEqualsIgnoreCase(key, "SUBJECT"))
		{
			gmsec::api::internal::MessageBuddy::getInternal(*m_adapter).setSubject(value);
		}
		else if (gmsec::api::util::StringUtil::stringEqualsIgnoreCase(key, "KIND"))
		{
			if (gmsec::api::util::StringUtil::stringEqualsIgnoreCase(value, "PUBLISH") ||
			    gmsec::api::util::StringUtil::stringEqualsIgnoreCase(value, "GMSEC_MSG_PUBLISH"))
			{
				gmsec::api::internal::MessageBuddy::getInternal(*m_adapter).setKind(gmsec::api::Message::PUBLISH);
				m_kindUnset = false;
			}
			else if (gmsec::api::util::StringUtil::stringEqualsIgnoreCase(value, "REQUEST") ||
			         gmsec::api::util::StringUtil::stringEqualsIgnoreCase(value, "GMSEC_MSG_REQUEST"))
			{
				gmsec::api::internal::MessageBuddy::getInternal(*m_adapter).setKind(gmsec::api::Message::REQUEST);
				m_kindUnset = false;
			}
			else if (gmsec::api::util::StringUtil::stringEqualsIgnoreCase(value, "REPLY") ||
			         gmsec::api::util::StringUtil::stringEqualsIgnoreCase(value, "GMSEC_MSG_REPLY"))
			{
				gmsec::api::internal::MessageBuddy::getInternal(*m_adapter).setKind(gmsec::api::Message::REPLY);
				m_kindUnset = false;
			}
			else
			{
				// Will treat all other values (whether UNSET, GMSEC_MSG_UNSET, or bogus value) as GMSEC_MSG_UNSET.
				// Of course, the internal 4.x message will be set to PUBLISH.
				gmsec::api::internal::MessageBuddy::getInternal(*m_adapter).setKind(gmsec::api::Message::PUBLISH);
				m_kindUnset = true;
			}
		}

		hasPair = cfg.getNext(key, value);
	}

	return status;
}


Status InternalMessage::SetKind(GMSEC_MSG_KIND msgKind)
{
	Status status;

	m_kindUnset = (msgKind == GMSEC_MSG_UNSET);

	gmsec::api::Message::MessageKind kind = API4_Adapter::API3MsgKindToAPI4(msgKind);

	gmsec::api::internal::MessageBuddy::getInternal(*m_adapter).setKind(kind);

	return status;
}


Status InternalMessage::GetKind(GMSEC_MSG_KIND& kind)
{
	Status status;
	kind = (m_kindUnset ? GMSEC_MSG_UNSET : API4_Adapter::API4MsgKindToAPI3(m_adapter->getKind()));
	return status;
}


Status InternalMessage::SetSubject(const char* subject)
{
	Status status;

	try
	{
		gmsec::api::internal::MessageBuddy::getInternal(*m_adapter).setSubject(subject);
	}
	catch (gmsec::api::Exception& e)
	{
		status.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_INVALID_SUBJECT_NAME, e.getErrorMessage());
	}

	return status;
}


Status InternalMessage::GetSubject(const char*& subject)
{
	Status status;

	subject = m_adapter->getSubject();

	return status;
}


Status InternalMessage::ClearFields()
{
	Status status;

	m_adapter->clearFields();

	return status;
}


Status InternalMessage::AddField(Field& field)
{
	Status status;

	gmsec::api::Field* field4 = API4_Adapter::API3FieldToAPI4(field);

	if (field4)
	{
		m_adapter->addField(*field4);
		delete field4;
	}
	else
	{
		status.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_UNKNOWN_FIELD_TYPE, "Unknown field type");
	}

	return status;
}


Status InternalMessage::ClearField(const char* name)
{
	Status status;

	if (name == NULL || std::string(name).empty())
	{
		status.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_INVALID_FIELD_NAME, "Field name is invalid");
		return status;
	}

	(void) m_adapter->clearField(name);

	return status;
}


Status InternalMessage::GetField(const char* name, Field& field)
{
	Status status;

	if (name == NULL)
	{
		status.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_INVALID_FIELD_NAME, "Field name is NULL");
		return status;
	}

	const gmsec::api::Field* field4 = m_adapter->getField(name);

	if (field4 != NULL)
	{
		Field* field3 = API4_Adapter::API4FieldToAPI3(*field4);

		if (field3)
		{
			field = *field3;
			delete field3;
		}
		else
		{
			status.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_INVALID_FIELD_NAME, "Field not found");
		}
	}
	else
	{
		status.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_INVALID_FIELD_NAME, "Field not found");
	}

	return status;
}


Status InternalMessage::GetFieldCount(GMSEC_I32& count)
{
	Status status;

	count = (GMSEC_I32) m_adapter->getFieldCount();

	return status;
}


Status InternalMessage::GetFirstField(Field& field)
{
	Status status;

    m_fieldIterWarned = false;

    status = Reset(m_fieldIter);

	if (!status.isError())
	{
		status = GetNextField(field);
	}

	return status;
}


Status InternalMessage::GetNextField(Field& field)
{
	Status status;

	bool setEnd = false;

	if (m_fieldIter.HasNext())
	{
		status = m_fieldIter.Next(field);

		if (status.GetClass() == GMSEC_STATUS_ITERATOR_ERROR && status.GetCode() == GMSEC_INVALID_NEXT)
		{
			setEnd = true;
		}
	}
	else
	{
		setEnd = true;
	}

	if (setEnd)
	{
		status.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_FIELDS_END_REACHED, "No field available.");

		if (m_fieldIterWarned)
		{
			LOG_WARNING << "Invalid GetNextField: " << status.Get();
		}

		m_fieldIterWarned = true;
	}

	return status;
}


Status InternalMessage::CopyFields(InternalMessage* toMsg)
{
	Status status;

	m_adapter->copyFields(*toMsg->m_adapter);

	return status;
}


Status InternalMessage::ToXML(const char*& xml)
{
	Status status;

	xml = m_adapter->toXML();

	return status;
}


Status InternalMessage::FromXML(const char* xml)
{
	Status status;

	try
	{
		gmsec::api::Message* tmp = new gmsec::api::Message(xml);

		delete m_adapter;

		m_adapter = tmp;

		m_kindUnset = false;
	}
	catch (gmsec::api::Exception& e)
	{
		status.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_XML_PARSE_ERROR, e.getErrorMessage());
	}

	return status;
}


const char* InternalMessage::ToJSON()
{
	return (m_adapter != NULL ? m_adapter->toJSON() : NULL);
}


Status InternalMessage::FromJSON(const char* json)
{
	Status status;

	try
	{
		gmsec::api::Message* tmp = new gmsec::api::Message(json);

		delete m_adapter;

		m_adapter = tmp;

		m_kindUnset = false;
	}
	catch (gmsec::api::Exception& e)
	{
		status.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_JSON_PARSE_ERROR, e.getErrorMessage());
	}

	return status;
}


Status InternalMessage::GetMSGSize(GMSEC_U32& size)
{
	Status status;

	size = (GMSEC_U32) m_adapter->getSize();

	return status;
}


Status InternalMessage::Reset(FieldIterator& iter3)
{
	Status status;

	m_fields.ClearFields();

	const gmsec::api::MessageFieldIterator& iter4 = m_adapter->getFieldIterator();

	while (iter4.hasNext())
	{
		const gmsec::api::Field& field4 = iter4.next();

		Field* field3 = API4_Adapter::API4FieldToAPI3(field4);

		if (field3)
		{
			m_fields.AddField(field4.getName(), *field3);
			delete field3;
		}
	}

	SimpleFieldIterator* sfi = new SimpleFieldIterator(m_fields);
	sfi->update(iter3);

	return status;
}


gmsec::api::Message* InternalMessage::getAdapter()
{
	return m_adapter;
}
