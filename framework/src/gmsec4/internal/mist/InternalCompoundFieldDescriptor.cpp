/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalFieldDescriptor.cpp
 *
 *  @brief This file contains the CompoundFieldDescriptor class.
 *
**/

#include <gmsec4/internal/mist/InternalCompoundFieldDescriptor.h>

#include <gmsec4/internal/mist/CompoundFieldDescriptor.h>
#include <gmsec4/internal/mist/Context.h>
#include <gmsec4/internal/mist/DescriptorList.h>

#include <gmsec4/Message.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalCompoundFieldDescriptor::InternalCompoundFieldDescriptor()
	: AbstractBaseFieldDescriptor(),
	  m_children(new DescriptorList())
{
}


InternalCompoundFieldDescriptor::InternalCompoundFieldDescriptor(std::vector<AbstractFieldDescriptor*>& newChildren)
	: AbstractBaseFieldDescriptor(),
	  m_children(new DescriptorList(newChildren))
{
}


InternalCompoundFieldDescriptor::~InternalCompoundFieldDescriptor()
{
	delete m_children;
	m_children = 0;
}


Status InternalCompoundFieldDescriptor::addTo(Message& message, Context& context) const
{
	Status status;
	Status addStatus;

	context.ptr->incrementDepth(m_name);

	if (!m_children->isEmpty())
	{
		m_children->resetIterator();

		while (m_children->hasNext() && !addStatus.isError())
		{
			AbstractFieldDescriptor& afd = m_children->next();

			addStatus = afd.addTo(message, context);
		}
	}

	context.ptr->decrementDepth();

	if (addStatus.isError())
	{
		status = addStatus;
	}

	return status;
}


Status InternalCompoundFieldDescriptor::validate(Message& message, Context& context) const
{
	return findField(message, context, VALIDATE);
}


Status InternalCompoundFieldDescriptor::apply(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
{
	return findField(message, context, validate);
}


Status InternalCompoundFieldDescriptor::getChildren(DescriptorList*& newChildren) const
{
	Status status;
	newChildren = m_children;
	return status;
}


Status InternalCompoundFieldDescriptor::setChildren(std::vector<AbstractFieldDescriptor*>& fdlist)
{
	Status status;

	if (m_children)
	{
		delete m_children;
	}

	m_children = new DescriptorList();

	m_children->setFields(fdlist);

	return status;
}


Status InternalCompoundFieldDescriptor::addChild(AbstractFieldDescriptor& fd)
{
	Status status;
	m_children->addDescriptor(&fd);
	return status;
}


Status InternalCompoundFieldDescriptor::findField(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
{
	Status status;
	
	context.ptr->incrementDepth(m_name);

	if (!m_children->isEmpty())
	{
		m_children->resetIterator();

		while (m_children->hasNext())
		{
			AbstractFieldDescriptor& afd = m_children->next();

			status = afd.findField(message, context, validate);
		}
	}

	context.ptr->decrementDepth();

	return status;
}
