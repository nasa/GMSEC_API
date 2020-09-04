/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file CompoundFieldDescriptor.cpp
 *
 *  @brief This file contains the CompoundFieldDescriptor class.
 *
**/

#include <gmsec4/internal/mist/CompoundFieldDescriptor.h>
#include <gmsec4/internal/mist/Context.h>
#include <gmsec4/internal/mist/InternalContext.h>
#include <gmsec4/internal/mist/InternalCompoundFieldDescriptor.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


CompoundFieldDescriptor::CompoundFieldDescriptor()
	:ptr(new InternalCompoundFieldDescriptor())
{
}


CompoundFieldDescriptor::CompoundFieldDescriptor(std::vector<AbstractFieldDescriptor*>& children)
	:ptr(new InternalCompoundFieldDescriptor(children))
{
}


CompoundFieldDescriptor::~CompoundFieldDescriptor()
{
	delete ptr;
	ptr = 0;
}


Status CompoundFieldDescriptor::addTo(Message& message, Context& context) const
{
	return ptr->addTo(message, context);
}


Status CompoundFieldDescriptor::validate(Message& message, Context& context) const
{
	return ptr->validate(message, context);
}


Status CompoundFieldDescriptor::apply(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
{
	return ptr->apply(message, context, validate);
}


Status CompoundFieldDescriptor::findField(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
{
	return ptr->findField(message, context, validate);
}


void CompoundFieldDescriptor::setFieldVariables(const char* name, FieldMode mode, FieldClass clss, const char* description)
{
	ptr->setFieldVariables(name, mode, clss, description);
}
