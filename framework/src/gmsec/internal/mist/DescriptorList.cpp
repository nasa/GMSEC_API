
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file DescriptorList.cpp
 *
 *  @brief This file contains the DescriptorList class.
 *
**/

#include <gmsec/internal/mist/DescriptorList.h>

#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/SimpleFieldDescriptor.h>
#include <gmsec/internal/mist/CompoundFieldDescriptor.h>

#include <gmsec/internal/Value.h>


using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;

DescriptorList::DescriptorList(util::List<AbstractFieldDescriptor*> &newFields)
{
	for(int i = 0; i < newFields.size(); i++)
	{
		AbstractFieldDescriptor * ab = 0;
		newFields.get(i, ab);
		if(ab != 0)
			fields.add(ab);
	}
}

DescriptorList::DescriptorList()
{}

DescriptorList::~DescriptorList()
{
	for(int i = 0; i < fields.size(); i++)
	{
		AbstractFieldDescriptor * ab = 0;
		bool found = fields.get(i, ab);
		if(found)
		{
			delete ab;
		}
	}
	fields.clear();
}

Status DescriptorList::SetFields(util::List<AbstractFieldDescriptor*> &newFields)
{
	Status status;

	for(int i = 0; i < fields.size(); i++)
	{
		AbstractFieldDescriptor * ab = 0;
		bool found = fields.get(i, ab);
		if(found)
			delete ab;
	}
	fields.clear();
	fields = newFields;
	return status;
}

Status DescriptorList::AddDescriptor(AbstractFieldDescriptor &fd)
{
	Status status;
	fields.add(&fd);
	return status;
}

bool DescriptorList::IsEmpty()
{
	return (fields.size() == 0);
}

Status DescriptorList::Get(int i, AbstractFieldDescriptor* &fd)
{
	Status status;
	if(!(fields.get(i, fd)))
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_MISSING_REQUIRED_FIELD, "FieldDescriptor not found");
	return status;
}

int DescriptorList::Size()
{
	return fields.size();
}
