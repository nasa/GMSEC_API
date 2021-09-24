/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/Message.h>

#include <gmsec4/Config.h>

#include <gmsec4/internal/InternalMessage.h>

namespace gmsec
{
namespace api
{

Message::Message(const char* subject, MessageKind kind)
	: m_iMessage(new internal::InternalMessage(subject, kind))
{
}


Message::Message(const char* subject, MessageKind kind, const Config& config)
	: m_iMessage(new internal::InternalMessage(subject, kind, config))
{
}


Message::Message(const char* fromXML)
	: m_iMessage(new internal::InternalMessage(fromXML))
{
}


Message::Message(const Message& other)
	: m_iMessage(new internal::InternalMessage(*other.m_iMessage))
{
}


Message::Message(internal::InternalMessage* internal)
	: m_iMessage(internal)
{
}


Message::~Message()
{
	delete m_iMessage;
	m_iMessage = NULL;
}


Message& Message::operator=(const Message& other)
{
	if (this != &other)
	{
		delete m_iMessage;
		m_iMessage = new internal::InternalMessage(*other.m_iMessage);
	}

	return *this;
}
	 

void Message::addConfig(const Config& config)
{
	m_iMessage->addConfig(config);
}


const Config& Message::getConfig() const
{
	return m_iMessage->getConfig();
}


const char* Message::getSubject() const
{
	return m_iMessage->getSubject();
}


void Message::setSubject(const char* subject)
{
	m_iMessage->setSubject(subject);
}


Message::MessageKind Message::getKind() const
{
	return m_iMessage->getKind();
}


bool Message::addField(const Field& field)
{
	return m_iMessage->addField(field);
}


bool Message::addField(const char* name, GMSEC_BIN bin, size_t len)
{
	return m_iMessage->addField(name, bin, len);
}


bool Message::addField(const char* name, bool value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_CHAR value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_I8 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_F64 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_F32 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_I32 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_I64 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_I16 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, const char* value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_U8 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_U16 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_U32 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_U64 value)
{
	return m_iMessage->addField(name, value);
}


bool Message::addFields(const gmsec::api::util::DataList<Field*>& fields)
{
	return m_iMessage->addFields(fields);
}


void Message::clearFields()
{
	m_iMessage->clearFields();
}


bool Message::clearField(const char* name)
{
	return m_iMessage->clearField(name);
}


GMSEC_I64 Message::getIntegerValue(const char* fieldName) const
{
	return m_iMessage->getIntegerValue(fieldName);
}


GMSEC_U64 Message::getUnsignedIntegerValue(const char* fieldName) const
{
	return m_iMessage->getUnsignedIntegerValue(fieldName);
}


GMSEC_F64 Message::getDoubleValue(const char* fieldName) const
{
	return m_iMessage->getDoubleValue(fieldName);
}


const char* Message::getStringValue(const char* fieldName) const
{
	return m_iMessage->getStringValue(fieldName);
}


bool Message::hasField(const char* name) const
{
	return m_iMessage->hasField(name);
}


const Field* Message::getField(const char* name) const
{
	return m_iMessage->getField(name);
}


Field::FieldType Message::getFieldType(const char* name) const
{
	return m_iMessage->getFieldType(name);
}


const BinaryField& Message::getBinaryField(const char* name) const
{
	return m_iMessage->getBinaryField(name);
}


const BooleanField& Message::getBooleanField(const char* name) const
{
	return m_iMessage->getBooleanField(name);
}


const CharField& Message::getCharField(const char* name) const
{
	return m_iMessage->getCharField(name);
}


const F32Field& Message::getF32Field(const char* name) const
{
	return m_iMessage->getF32Field(name);
}


const F64Field& Message::getF64Field(const char* name) const
{
	return m_iMessage->getF64Field(name);
}


const I8Field& Message::getI8Field(const char* name) const
{
	return m_iMessage->getI8Field(name);
}


const I16Field& Message::getI16Field(const char* name) const
{
	return m_iMessage->getI16Field(name);
}


const I32Field& Message::getI32Field(const char* name) const
{
	return m_iMessage->getI32Field(name);
}


const I64Field& Message::getI64Field(const char* name) const
{
	return m_iMessage->getI64Field(name);
}


const U8Field& Message::getU8Field(const char* name) const
{
	return m_iMessage->getU8Field(name);
}


const U16Field& Message::getU16Field(const char* name) const
{
	return m_iMessage->getU16Field(name);
}


const U32Field& Message::getU32Field(const char* name) const
{
	return m_iMessage->getU32Field(name);
}


const U64Field& Message::getU64Field(const char* name) const
{
	return m_iMessage->getU64Field(name);
}


const StringField& Message::getStringField(const char* name) const
{
	return m_iMessage->getStringField(name);
}


size_t Message::getFieldCount() const
{
	return m_iMessage->getFieldCount();
}


void Message::copyFields(Message& toMsg) const
{
	m_iMessage->copyFields(*toMsg.m_iMessage);
}


const char* Message::toXML() const
{
	return m_iMessage->toXML();
}


const char* Message::toJSON() const
{
	return m_iMessage->toJSON();
}


size_t Message::getSize() const
{
	return m_iMessage->getSize(*this);
}


MessageFieldIterator& Message::getFieldIterator(MessageFieldIterator::Selector selector) const
{
	return m_iMessage->getFieldIterator(selector);
}


void Message::registerInternal(gmsec::api::internal::InternalMessage* internal)
{
	delete m_iMessage;
	m_iMessage = internal;
}

gmsec::api::internal::InternalMessage& Message::getInternal() const
{
	return *m_iMessage;
}

} //namespace api
} //namespace gmsec
