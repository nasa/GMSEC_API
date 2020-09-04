/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file DescriptorList.cpp
 *
 *  @brief This file contains the DescriptorList class.
 *
**/

#include <gmsec4/internal/mist/DescriptorList.h>
#include <gmsec4/internal/mist/AbstractFieldDescriptor.h>

#include <gmsec4/Exception.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


DescriptorList::DescriptorList()
	: m_fields(),
	  m_iter(m_fields.begin())
{
}


DescriptorList::DescriptorList(FieldDescriptorList& newFields)
	: m_fields(newFields),
	  m_iter(m_fields.begin())
{
}


DescriptorList::~DescriptorList()
{
	for (FieldDescriptorList::iterator it = m_fields.begin(); it != m_fields.end(); ++it)
	{
		AbstractFieldDescriptor* ab = *it;
		delete ab;
	}

	m_fields.clear();
	m_iter = m_fields.begin();
}


void DescriptorList::setFields(FieldDescriptorList& newFields)
{
	for (FieldDescriptorList::iterator it = m_fields.begin(); it != m_fields.end(); ++it)
	{
		AbstractFieldDescriptor* ab = *it;
		delete ab;
	}

	m_fields.clear();

	m_fields = newFields;
	m_iter   = m_fields.begin();
}


void DescriptorList::addDescriptor(AbstractFieldDescriptor* fd)
{
	m_fields.push_back(fd);
}


bool DescriptorList::isEmpty() const
{
	return m_fields.size() == 0;
}


void DescriptorList::resetIterator()
{
	m_iter = m_fields.begin();
}


bool DescriptorList::hasNext() const
{
	return m_iter != m_fields.end();
}


AbstractFieldDescriptor& DescriptorList::next()
{
	if (m_iter == m_fields.end())
	{
		throw Exception(MIST_ERROR, ITER_INVALID_NEXT, "At end of DescriptorList!");
	}

	AbstractFieldDescriptor* ad = *m_iter;

	++m_iter;

	return *ad;
}


size_t DescriptorList::size() const
{
	return m_fields.size();
}


AbstractFieldDescriptor& DescriptorList::getDescriptor(size_t index) const
{
	if (index >= m_fields.size())
	{
		throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE,
			"Index is beyond the size of the descriptor list.");
	}

	return *m_fields[index];
}
