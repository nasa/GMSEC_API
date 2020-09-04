/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MessageTemplate.cpp
 *
 *  @brief This file contains the MessageTemplate class.
 *
**/

#include <gmsec4/internal/mist/MessageTemplate.h>
#include <gmsec4/internal/mist/Context.h>

#include <gmsec4/internal/mist/InternalMessageTemplate.h>
#include <gmsec4/internal/mist/InternalContext.h>

#include <gmsec4/Message.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


MessageTemplate::MessageTemplate()
	: m_iMsgTemplate(new internal::InternalMessageTemplate())
{
}


MessageTemplate::~MessageTemplate()
{
	delete m_iMsgTemplate;
	m_iMsgTemplate= 0;
}


MessageTemplate::MessageTemplate(std::vector<AbstractFieldDescriptor*>& inputFields, bool allow)
	: m_iMsgTemplate(new InternalMessageTemplate())
{
	m_iMsgTemplate->setFields(inputFields);
	m_iMsgTemplate->setAllowUserDefinedFields(allow);
}


Status MessageTemplate::validate(Message& message, Context& context) const
{
	context.resetContext(new InternalContext);

	return m_iMsgTemplate->validate(message, context);
}


Status MessageTemplate::instantiate(Message& message, Context& context) const
{
	context.resetContext(new InternalContext);

	return m_iMsgTemplate->instantiate(message, context);
}


Status MessageTemplate::apply(Message& message, Context& context) const
{
	context.resetContext(new InternalContext);

	return m_iMsgTemplate->apply(message, context);
}
