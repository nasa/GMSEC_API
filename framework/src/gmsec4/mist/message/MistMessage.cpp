/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MistMessage.cpp
 *
 * @brief This file contains the implementation for MistMessage.
 */

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/internal/mist/message/InternalMistMessage.h>
#include <gmsec4/internal/InternalMessage.h>

#include <gmsec4/mist/Specification.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::mist::message::internal;
using namespace gmsec::api::util;


MistMessage::MistMessage(const char* subject, const char* schemaID, const Specification& spec)
	: Message(new InternalMistMessage(subject, InternalMistMessage::findKind(schemaID, spec.getVersion()), schemaID, spec))
{
}


MistMessage::MistMessage(const char* subject, const char* schemaID, const gmsec::api::Config& config, const Specification& spec)
	: Message(new InternalMistMessage(subject, InternalMistMessage::findKind(schemaID, spec.getVersion()), schemaID, config, spec))
{
}


MistMessage::MistMessage(const MistMessage& other)
	: Message(new InternalMistMessage(dynamic_cast<const InternalMistMessage&>(other.getInternal())))
{
}


MistMessage::MistMessage(const Message& msg)
	: Message(new InternalMistMessage(msg))
{
}


MistMessage::MistMessage(const Message& msg, const Config& specConfig)
	: Message(new InternalMistMessage(msg, specConfig))
{
}


MistMessage::MistMessage(const char* data)
	: Message(new InternalMistMessage(data))
{
}


MistMessage::MistMessage(const Specification& spec, const char* data)
	: Message(new InternalMistMessage(spec, data))
{
}


MistMessage::MistMessage(gmsec::api::mist::message::internal::InternalMistMessage* internalMsg)
	: Message(internalMsg)
{
}


MistMessage::~MistMessage()
{
}


void MistMessage::setStandardFields(const DataList<Field*>& standardFields)
{
	InternalMistMessage::setStandardFields(standardFields);
}


void MistMessage::clearStandardFields()
{
	InternalMistMessage::clearStandardFields();
}


const char* MistMessage::getSchemaID() const
{
	return dynamic_cast<InternalMistMessage&>(getInternal()).getSchemaID();
}


void MistMessage::setValue(const char* fieldName, const char* value)
{
	dynamic_cast<InternalMistMessage&>(getInternal()).setValue(fieldName, value);
}


void MistMessage::setValue(const char* fieldName, GMSEC_I64 value)
{
	dynamic_cast<InternalMistMessage&>(getInternal()).setValue(fieldName, value);
}


void MistMessage::setValue(const char* fieldName, GMSEC_F64 value)
{
	dynamic_cast<InternalMistMessage&>(getInternal()).setValue(fieldName, value);
}
