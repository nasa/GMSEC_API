/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalMessageFieldIterator.cpp
 */

#include <gmsec5/internal/InternalMessageFieldIterator.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Fields.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


InternalMessageFieldIterator::InternalMessageFieldIterator(InternalMessage& msg, MessageFieldIterator::Selector selector)
	: m_msg(msg),
	  m_selector(selectAllFields),
	  m_ready(false),
	  m_nextField(0)
{
	setSelector(selector);

	reset();
}


InternalMessageFieldIterator::~InternalMessageFieldIterator()
{
}


bool InternalMessageFieldIterator::hasNext()
{
	return makeReady();
}


const Field& InternalMessageFieldIterator::next()
{
	const Field* field = 0;

	if (makeReady())
	{
		field = m_nextField;
		m_nextField = 0;
	}

	if (!field)
	{
		throw GmsecException(ITERATOR_ERROR, ITER_INVALID_NEXT, "There are no more fields");
	}

	return *field;
}


void InternalMessageFieldIterator::reset()
{
	m_msg.resetFieldIterator();
}


void InternalMessageFieldIterator::setSelector(MessageFieldIterator::Selector selector)
{
	switch (selector)
	{
	case MessageFieldIterator::Selector::ALL_FIELDS:
		m_selector = selectAllFields;
		break;
	case MessageFieldIterator::Selector::HEADER_FIELDS:
		m_selector = selectHeaderFields;
		break;
	case MessageFieldIterator::Selector::NON_HEADER_FIELDS:
		m_selector = selectNonHeaderFields;
		break;
	case MessageFieldIterator::Selector::TRACKING_FIELDS:
		m_selector = selectTrackingFields;
		break;
	}
}


bool InternalMessageFieldIterator::makeReady()
{
	bool ready = false;

	if (m_nextField)
	{
		ready = true;
	}
	else
	{
		while (m_msg.hasNextField())
		{
			const Field& field = m_msg.nextField();

			ready = (*m_selector)(field);

			if (ready)
			{
				m_nextField = &field;
				break;
			}
		}
	}

	return ready;
}


bool InternalMessageFieldIterator::selectAllFields(const Field& field)
{
	return true;
}


bool InternalMessageFieldIterator::selectHeaderFields(const Field& field)
{
	return field.isHeader();
}


bool InternalMessageFieldIterator::selectNonHeaderFields(const Field& field)
{
	return !field.isHeader();
}


bool InternalMessageFieldIterator::selectTrackingFields(const Field& field)
{
	return field.isTracking();
}
