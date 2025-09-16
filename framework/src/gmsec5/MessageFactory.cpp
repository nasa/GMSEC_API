/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFactory.cpp
 *
 * @brief This file contains the public interface for the MessageFactory.
 */

#include <gmsec5/MessageFactory.h>

#include <gmsec5/internal/InternalMessageFactory.h>

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageValidator.h>
#include <gmsec5/Specification.h>

#include <gmsec5/field/Field.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


MessageFactory::MessageFactory()
	: m_internal(new InternalMessageFactory())
{
}


MessageFactory::MessageFactory(const Config& config)
	: m_internal(new InternalMessageFactory(config))
{
}


MessageFactory::~MessageFactory()
{
	delete m_internal;
}


void MessageFactory::setStandardFields(const gmsec::api5::util::List<Field*>& standardFields)
{
	m_internal->setStandardFields(standardFields);
}


void MessageFactory::clearStandardFields()
{
	m_internal->clearStandardFields();
}


void MessageFactory::setMessageConfig(const Config& msgConfig)
{
	m_internal->setMessageConfig(msgConfig);
}


Message MessageFactory::createMessage() const
{
	return m_internal->createMessage();
}


Message MessageFactory::createMessage(const char* schemaID) const
{
	return m_internal->createMessage(schemaID);
}


Message MessageFactory::fromData(const char* data, DataType type)
{
	return m_internal->fromData(data, type);
}


const Specification& MessageFactory::getSpecification() const
{
	return m_internal->getSpecification();
}


void MessageFactory::registerMessageValidator(MessageValidator* validator)
{
	m_internal->registerMessageValidator(validator);
}


void MessageFactory::registerMessageValidator(GMSEC_MessageValidator* validator)
{
	m_internal->registerMessageValidator(validator);
}
