
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file MessageTemplate.cpp
 *
 *  @brief This file contains the MessageTemplate class.
 *
**/

#include <gmsec/internal/mist/MessageTemplate.h>
#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/BaseMessageTemplate.h>
#include <gmsec/internal/mist/BaseContext.h>

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;

MessageTemplate::MessageTemplate(): ptr(new internal::BaseMessageTemplate())
{}

MessageTemplate::~MessageTemplate()
{
	delete ptr;
	ptr = 0;
}

MessageTemplate::MessageTemplate(util::List<AbstractFieldDescriptor*> &inputFields,bool allow)
: ptr(new internal::BaseMessageTemplate())
{
	ptr->SetFields(inputFields);
	ptr->SetAllowUserDefinedFields(allow);

}

Status MessageTemplate::Validate(Message *message, Context &context) const
{
	Status status;
	BaseContext * bc = new BaseContext();
	status = context.ResetContext(bc);
	if(status.IsError())
		return status;
	status = ptr->Validate(message, context);
	return status;
}

Status MessageTemplate::Instantiate(Message *message, Context &context) const
{
	Status status;
	BaseContext * bc = new BaseContext();
	status = context.ResetContext(bc);
	if(status.isError())
		return status;
	status = ptr->Instantiate(message, context);
	return status;
}

Status MessageTemplate::Apply(Message *message, Context &context) const
{
	Status status;
	BaseContext * bc = new BaseContext();
	status = context.ResetContext(bc);
	if(status.IsError())
		return status;
	status = ptr->Apply(message, context);
	return status;
}
