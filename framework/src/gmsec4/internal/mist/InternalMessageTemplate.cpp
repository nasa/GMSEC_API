/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalMessageTemplate.cpp
 *
 *  @brief This file contains the InternalMessageTemplate class.
 *
**/

#include <gmsec4/internal/mist/InternalMessageTemplate.h>

#include <gmsec4/internal/mist/AbstractFieldDescriptor.h>
#include <gmsec4/internal/mist/Context.h>
#include <gmsec4/internal/mist/DescriptorList.h>
#include <gmsec4/internal/mist/InternalContext.h>
#include <gmsec4/internal/mist/SimpleFieldDescriptor.h>
#include <gmsec4/internal/mist/CompoundFieldDescriptor.h>

#include <gmsec4/Config.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/MessageFieldIterator.h>

#include <string>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalMessageTemplate::InternalMessageTemplate()
	: m_fields(0),
	  m_allowUserDefinedFields(false)
{
}


InternalMessageTemplate::~InternalMessageTemplate()
{
	delete m_fields;
	m_fields = 0;
}


// validate <message> against this template
Status InternalMessageTemplate::validate(Message& message, Context& context) const
{
	Status status;
	Status temp_status;

	if (m_fields->isEmpty())
	{
		status.set(MIST_ERROR, FIELD_NOT_SET, "MessageTemplate fields have not been set");
		return status;
	}

	m_fields->resetIterator();

	while (m_fields->hasNext())
	{
		AbstractFieldDescriptor& ab = m_fields->next();

		temp_status = ab.validate(message, context);

		if (temp_status.isError())
		{
			status = temp_status;
		}
	}

	if (!m_allowUserDefinedFields)
	{
		MessageFieldIterator& iter = message.getFieldIterator();

		while (iter.hasNext())
		{
			const Field& field = iter.next();

			const char* name = field.getName();

			if (!context.visited(name))
			{
				std::string desc("Unexpected Field not allowed, but found in Message");

				ContextStatus contextStatus;
				contextStatus.fieldName   = name;
				contextStatus.description = desc;
				contextStatus.errorLevel  = NON_ALLOWED_FIELD;

				context.ptr->addStatus(contextStatus);

				status.set(MIST_ERROR, NON_ALLOWED_FIELD, desc.c_str());
			}
		}
	}

	return status;
}


// apply (add fields from) this template blank <messge>
Status InternalMessageTemplate::instantiate(Message& message, Context& context) const
{
	Status status;

	if (m_fields->isEmpty())
	{
		status.set(MIST_ERROR, FIELD_NOT_SET, "MessageTemplate fields have not been set");
		return status;
	}

	message.clearFields();

	m_fields->resetIterator();

	while (m_fields->hasNext() && !status.isError())
	{
		AbstractFieldDescriptor& afd = m_fields->next();

		status = afd.addTo(message, context);
	}

	return status;
}


// apply (add fields from) this template to <message> with option 
// to validate existing fields
Status InternalMessageTemplate::apply(Message& message, Context& context) const
{
	Status status;

	if (m_fields->isEmpty())
	{
		status.set(MIST_ERROR, FIELD_NOT_SET, "MessageTemplate fields have not been set");
		return status;
	}

	bool validate = context.getValidate();

	m_fields->resetIterator();

	while (m_fields->hasNext() && !status.isError())
	{
		AbstractFieldDescriptor& afd = m_fields->next();

		status = afd.apply(message, context, (validate ? VALIDATE_ON_APPLY : NO_VALIDATE_ON_APPLY));
	}

	if (validate)
	{
		MessageFieldIterator& iter = message.getFieldIterator();

		while (iter.hasNext())
		{
			const Field& field = iter.next();

			const char* name = field.getName();

			if (!context.visited(name))
			{
				std::string desc("Unexpected Field not allowed, but found in Message.");

				ContextStatus contextStatus;
				contextStatus.fieldName   = name;
				contextStatus.description = desc;
				contextStatus.errorLevel  = NON_ALLOWED_FIELD;

				context.ptr->addStatus(contextStatus);

				status.set(MIST_ERROR, NON_ALLOWED_FIELD, desc.c_str());
			}
		}
	}

	return status;
}


void InternalMessageTemplate::setFields(std::vector<AbstractFieldDescriptor*>& inputFields)
{
	delete m_fields;

	m_fields = new DescriptorList();
	m_fields->setFields(inputFields);
}


void InternalMessageTemplate::setAllowUserDefinedFields(bool allow)
{
	m_allowUserDefinedFields = allow;
}
