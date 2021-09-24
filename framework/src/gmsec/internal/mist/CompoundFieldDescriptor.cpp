
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file CompoundFieldDescriptor.cpp
 *
 *  @brief This file contains the CompoundFieldDescriptor class.
 *
**/

#include <gmsec/internal/mist/CompoundFieldDescriptor.h>
#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/BaseContext.h>
#include <gmsec/internal/mist/BaseSimpleFieldDescriptor.h>
#include <gmsec/internal/mist/BaseCompoundFieldDescriptor.h>


using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;


CompoundFieldDescriptor::CompoundFieldDescriptor()
	:ptr(new BaseCompoundFieldDescriptor())
{
}

CompoundFieldDescriptor::CompoundFieldDescriptor(util::List<AbstractFieldDescriptor *> &children)
	:ptr(new BaseCompoundFieldDescriptor(children))
{
}

CompoundFieldDescriptor::~CompoundFieldDescriptor()
{
	delete ptr;
	ptr = 0;
}

Status CompoundFieldDescriptor::AddTo(Message *message, Context &context) const
{
	return ptr->AddTo(message, context);
}

Status CompoundFieldDescriptor::Validate(Message *message, Context &context) const
{
	return ptr->Validate(message, context);
}

Status CompoundFieldDescriptor::Apply(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
{
	return ptr->Apply(message, context, validate);
}

Status CompoundFieldDescriptor::FindField(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
{
	return ptr->FindField(message, context, validate);
}

Status CompoundFieldDescriptor::SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description)
{
	return ptr->SetFieldVariables(name, mode, clss, description);
}

