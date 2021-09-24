
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseFieldDescriptor.cpp
 *
 *  @brief This file contains the CompoundFieldDescriptor class.
 *
**/

#include <gmsec/internal/mist/BaseCompoundFieldDescriptor.h>

#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/CompoundFieldDescriptor.h>
#include <gmsec/internal/mist/DescriptorList.h>

#include <gmsec/Message.h>

#include <gmsec/internal/Value.h>

#include <gmsec/util/String.h>

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;


/*******************************************************/

BaseCompoundFieldDescriptor::BaseCompoundFieldDescriptor()
	:AbstractBaseFieldDescriptor(),
	children(new DescriptorList())
{
}

BaseCompoundFieldDescriptor::BaseCompoundFieldDescriptor(util::List<AbstractFieldDescriptor *> &newChildren)
	:AbstractBaseFieldDescriptor(),
	children(new DescriptorList(newChildren))
{
}

BaseCompoundFieldDescriptor::~BaseCompoundFieldDescriptor()
{
	delete children;
	children = 0;
}

Status BaseCompoundFieldDescriptor::AddTo(Message *message, Context &context) const
{
	Status status;

	status = context.ptr->IncrementDepth(fname);
	if(!(children->IsEmpty()))
	{
		for(int i = 0; i < children->Size(); i++)
		{
			AbstractFieldDescriptor *ab;
			children->Get(i, ab);
			if(status.isError())
			{
				ab->AddTo(message, context);
			}
			else
			{
				status = ab->AddTo(message, context);
			}
		}
	}
	if(status.isError())
	{
		context.ptr->DecrementDepth();
	}
	else
	{
		status = context.ptr->DecrementDepth();
	}

	return status;
}

Status BaseCompoundFieldDescriptor::Validate(Message *message, Context &context) const
{
	return FindField(message,context,VALIDATE);
}

Status BaseCompoundFieldDescriptor::Apply(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
{
	return FindField(message, context, validate);
}

//const DescriptorList * BaseFieldD...
Status BaseCompoundFieldDescriptor::GetChildren(DescriptorList* &newChildren) const
{
	Status status;
	newChildren = children;
	return status;
}

Status BaseCompoundFieldDescriptor::SetChildren(util::List<AbstractFieldDescriptor*> &fdlist)
{
	Status status;

	if (children)
	{
		delete children;
	}
	DescriptorList * flist = new DescriptorList();
	status = flist->SetFields(fdlist);
	children = flist;

	return status;
}

Status BaseCompoundFieldDescriptor::AddChild(AbstractFieldDescriptor &fd)
{
	Status status;
	status = children->AddDescriptor(fd);
	return status;
}

//validate: 0, validateOnApply: 1, noValidateOnApply: 2
Status BaseCompoundFieldDescriptor::FindField(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
{
	Status status;
	
	status = context.ptr->IncrementDepth(fname);
	if(!(children->IsEmpty()))
	{
		for(int i = 0; i < children->Size(); i++)
		{
			AbstractFieldDescriptor *ab;
			children->Get(i, ab);
			if(status.isError())
			{
				ab->FindField(message, context, validate);
			}
			else
			{
				status = ab->FindField(message, context, validate);
			}
		}
	}
	if(status.isError())
	{
		context.ptr->DecrementDepth();
	}
	else
	{
		status = context.ptr->DecrementDepth();
	}

	return status;
}
