
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseMessageTemplate.cpp
 *
 *  @brief This file contains the BaseMessageTemplate class.
 *
**/

#include <gmsec/internal/mist/BaseMessageTemplate.h>

#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/SimpleFieldDescriptor.h>
#include <gmsec/internal/mist/CompoundFieldDescriptor.h>
#include <gmsec/internal/mist/BaseSimpleFieldDescriptor.h>
#include <gmsec/internal/mist/BaseCompoundFieldDescriptor.h>

#include <gmsec/Field.h>
#include <gmsec/Message.h>
#include <gmsec/Config.h>

#include <gmsec/util/String.h>
#include <gmsec/util/List.h>

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;

BaseMessageTemplate::BaseMessageTemplate()
	: fields(0),
	allowUserDefinedFields(false)
{
}

BaseMessageTemplate::~BaseMessageTemplate()
{
	delete fields;
	fields = 0;
}

// validate <message> against this template
Status BaseMessageTemplate::Validate(Message *message, Context &context) const
{
	Status status;
	Status temp_status;

	if(!fields)
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_UNSET_FIELD, "MessageTemplate fields have not been set");
		return status;
	}
	for(int i = 0; i < fields->Size(); i++)
	{
		AbstractFieldDescriptor *ab;
		temp_status = fields->Get(i, ab);

		if(temp_status.isError())
		{
			status = temp_status;
		}

		temp_status = ab->Validate(message, context);
		if(temp_status.isError())
		{
			status = temp_status;
		}
	}

	if(!allowUserDefinedFields)
	{
		Field field;
		Status newStatus = Status();
		newStatus = message->GetFirstField(field);
		while(!(newStatus.IsError()))
		{
			const char * name;
			temp_status = field.GetName(name);
			if(temp_status.isError())
			{
				status = temp_status;
			}
			if(!(context.Visited(name)))
			{
				std::string code("Additional fields not allowed, but found field ");
				code += name;
				temp_status = context.ptr->AddStatus(name, GMSEC_NON_ALLOWED_FIELD, code);
				status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_NON_ALLOWED_FIELD, "Additional field found.");
			}
			newStatus = message->GetNextField(field);
			//delete name[]
		}
	}

	return status;
}

// apply (add fields from) this template blank <messge>
Status BaseMessageTemplate::Instantiate(Message *message, Context &context) const
{
	Status status;

	if(!fields)
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_UNSET_FIELD, "MessageTemplate fields have not been set");
		return status;
	}

	message->ClearFields();

	for(int i = 0; i < fields->Size(); i++)
	{
		AbstractFieldDescriptor *ab;
		status = fields->Get(i, ab);
		if(status.isError())
		{
			ab->AddTo(message, context);
		}
		else
		{
			status = ab->AddTo(message, context);
		}
	}

	return status;
}

// apply (add fields from) this template to <message> with option 
// to validate existing fields
Status BaseMessageTemplate::Apply(Message *message, Context &context) const
{
	Status status;
	bool validate = context.GetValidate();

	if(!fields)
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_UNSET_FIELD, "MessageTemplate fields have not been set");
		return status;
	}
	for(int i = 0; i < fields->Size(); i++)
	{
		AbstractFieldDescriptor *ab;
		status = fields->Get(i, ab);
		if(status.isError())
		{
			ab->Apply(message, context, (validate ? VALIDATE_ON_APPLY : NO_VALIDATE_ON_APPLY));
		}
		else
		{
			status = ab->Apply(message, context, (validate ? VALIDATE_ON_APPLY : NO_VALIDATE_ON_APPLY));
		}
	}
	if(validate)
	{
		Field field;
		Status newStatus = Status();

		newStatus = message->GetFirstField(field);
		while(!(newStatus.IsError()))
		{
			const char * name;
			field.GetName(name);

			if(!(context.Visited(name)))
			{
				std::string code("Additional fields not allowed, but found field ");
				code += name;
				status = context.ptr->AddStatus(name, GMSEC_NON_ALLOWED_FIELD, code);
				status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_NON_ALLOWED_FIELD, "Additional field found.");
			}
			newStatus = message->GetNextField(field);
			//delete name[];
		}
	}

	return status;
}

Status BaseMessageTemplate::SetFields(util::List<AbstractFieldDescriptor*> &inputFields)
{
	Status status;
	DescriptorList * fieldList = new DescriptorList();
	fieldList->SetFields(inputFields);

	if(fields)
	{
		delete fields;
	}

	fields = fieldList;

	return status;
}

Status BaseMessageTemplate::SetAllowUserDefinedFields(bool allow)
{
	Status status;
	allowUserDefinedFields = allow;
	return status;
}
