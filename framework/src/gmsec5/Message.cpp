/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Message.cpp
 *
 * @brief This file contains the public interface to messages.
 */

#include <gmsec5/Message.h>

#include <gmsec5/internal/InternalMessage.h>

#include <gmsec5/Config.h>
#include <gmsec5/Specification.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


Message::Message()
	: m_internal(new InternalMessage(*this))
{
}


Message::Message(const Message& other)
	: m_internal(new InternalMessage(*this, *other.m_internal))
{
}


Message::~Message()
{
	delete m_internal;
}


Message& Message::operator=(const Message& other)
{
	if (this != &other)
	{
		delete m_internal;

		m_internal = new InternalMessage(*this, *other.m_internal);
	}

	return *this;
}


void Message::acknowledge() const
{
	m_internal->acknowledge();
}


void Message::destroy(Message*& msg)
{
	InternalMessage::destroy(msg);
}


const char* Message::getSchemaID() const
{
	return m_internal->getSchemaID();
}


unsigned int Message::getVersion() const
{
	return m_internal->getVersion();
}


Specification::SchemaLevel Message::getSchemaLevel() const
{
	return m_internal->getSchemaLevel();
}


Status Message::isCompliant() const
{
	return m_internal->isCompliant();
}


void Message::registerMessageValidator(MessageValidator* validator)
{
	m_internal->registerMessageValidator(validator);
}


void Message::registerMessageValidator(GMSEC_MessageValidator* validator)
{
	m_internal->registerMessageValidator(validator);
}


bool Message::setFieldValue(const char* fieldName, const char* value)
{
	return m_internal->setFieldValue(fieldName, value);
}


bool Message::setFieldValue(const char* fieldName, GMSEC_I16 value)
{
	return m_internal->setFieldValue(fieldName, value);
}


bool Message::setFieldValue(const char* fieldName, GMSEC_U16 value)
{
	return m_internal->setFieldValue(fieldName, value);
}


bool Message::setFieldValue(const char* fieldName, GMSEC_I32 value)
{
	return m_internal->setFieldValue(fieldName, value);
}


bool Message::setFieldValue(const char* fieldName, GMSEC_U32 value)
{
	return m_internal->setFieldValue(fieldName, value);
}


bool Message::setFieldValue(const char* fieldName, GMSEC_I64 value)
{
	return m_internal->setFieldValue(fieldName, value);
}


bool Message::setFieldValue(const char* fieldName, GMSEC_U64 value)
{
	return m_internal->setFieldValue(fieldName, value);
}


bool Message::setFieldValue(const char* fieldName, GMSEC_F64 value)
{
	return m_internal->setFieldValue(fieldName, value);
}
	 

void Message::setConfig(const Config& config)
{
	m_internal->setConfig(config);
}


Config& Message::getConfig()
{
	return m_internal->getConfig();
}


const Config& Message::getConfig() const
{
	return m_internal->getConfig();
}


void Message::setSubject(const char* subject)
{
	m_internal->setSubject(subject);
}


const char* Message::getSubject() const
{
	return m_internal->getSubject();
}


void Message::setSubjectElement(const char* name, const char* value)
{
	m_internal->setSubjectElement(name, value);
}


void Message::setKind(Message::Kind kind)
{
	m_internal->setKind(kind);
}


Message::Kind Message::getKind() const
{
	return m_internal->getKind();
}


bool Message::addField(const Field& field)
{
	return m_internal->addField(field);
}


bool Message::addField(const char* name, const GMSEC_U8* blob, size_t len)
{
	return m_internal->addField(name, blob, len);
}


bool Message::addField(const char* name, bool value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_CHAR value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_I8 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_F64 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_F32 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_I32 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_I64 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_I16 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, const char* value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_U8 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_U16 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_U32 value)
{
	return m_internal->addField(name, value);
}


bool Message::addField(const char* name, GMSEC_U64 value)
{
	return m_internal->addField(name, value);
}


bool Message::addFields(const gmsec::api5::util::List<Field*>& fields)
{
	return m_internal->addFields(fields);
}


void Message::clearFields()
{
	m_internal->clearFields();
}


bool Message::clearField(const char* name)
{
	return m_internal->clearField(name);
}


bool Message::hasField(const char* name) const
{
	return m_internal->hasField(name);
}


const char* Message::getStringValue(const char* fieldName) const
{
	return m_internal->getStringValue(fieldName);
}


bool Message::getBooleanValue(const char* fieldName) const
{
	return m_internal->getBooleanValue(fieldName);
}


GMSEC_I16 Message::getI16Value(const char* fieldName) const
{
	return m_internal->getI16Value(fieldName);
}


GMSEC_I32 Message::getI32Value(const char* fieldName) const
{
	return m_internal->getI32Value(fieldName);
}


GMSEC_I64 Message::getI64Value(const char* fieldName) const
{
	return m_internal->getI64Value(fieldName);
}


GMSEC_U16 Message::getU16Value(const char* fieldName) const
{
	return m_internal->getU16Value(fieldName);
}


GMSEC_U32 Message::getU32Value(const char* fieldName) const
{
	return m_internal->getU32Value(fieldName);
}


GMSEC_U64 Message::getU64Value(const char* fieldName) const
{
	return m_internal->getU64Value(fieldName);
}


GMSEC_F64 Message::getF64Value(const char* fieldName) const
{
	return m_internal->getF64Value(fieldName);
}


const Field* Message::getField(const char* name) const
{
	return m_internal->getField(name);
}


Field::Type Message::getFieldType(const char* name) const
{
	return m_internal->getFieldType(name);
}


const BinaryField& Message::getBinaryField(const char* name) const
{
	return m_internal->getBinaryField(name);
}


const BooleanField& Message::getBooleanField(const char* name) const
{
	return m_internal->getBooleanField(name);
}


const CharField& Message::getCharField(const char* name) const
{
	return m_internal->getCharField(name);
}


const F32Field& Message::getF32Field(const char* name) const
{
	return m_internal->getF32Field(name);
}


const F64Field& Message::getF64Field(const char* name) const
{
	return m_internal->getF64Field(name);
}


const I8Field& Message::getI8Field(const char* name) const
{
	return m_internal->getI8Field(name);
}


const I16Field& Message::getI16Field(const char* name) const
{
	return m_internal->getI16Field(name);
}


const I32Field& Message::getI32Field(const char* name) const
{
	return m_internal->getI32Field(name);
}


const I64Field& Message::getI64Field(const char* name) const
{
	return m_internal->getI64Field(name);
}


const U8Field& Message::getU8Field(const char* name) const
{
	return m_internal->getU8Field(name);
}


const U16Field& Message::getU16Field(const char* name) const
{
	return m_internal->getU16Field(name);
}


const U32Field& Message::getU32Field(const char* name) const
{
	return m_internal->getU32Field(name);
}


const U64Field& Message::getU64Field(const char* name) const
{
	return m_internal->getU64Field(name);
}


const StringField& Message::getStringField(const char* name) const
{
	return m_internal->getStringField(name);
}


size_t Message::getFieldCount() const
{
	return m_internal->getFieldCount();
}


void Message::copyFields(Message& toMsg) const
{
	m_internal->copyFields(*toMsg.m_internal);
}


const char* Message::toXML() const
{
	return m_internal->toXML();
}


const char* Message::toJSON() const
{
	return m_internal->toJSON();
}


size_t Message::getSize() const
{
	return m_internal->getSize(*this);
}


MessageFieldIterator& Message::getFieldIterator(MessageFieldIterator::Selector selector) const
{
	return m_internal->getFieldIterator(selector);
}
