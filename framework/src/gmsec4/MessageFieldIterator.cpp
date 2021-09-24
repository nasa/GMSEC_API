/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file MessageFieldIterator.h
**/

#include <gmsec4/MessageFieldIterator.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/internal/InternalMessage.h>

#include <gmsec4/internal/InternalMessageFieldIterator.h>


namespace gmsec
{
namespace api
{

MessageFieldIterator::MessageFieldIterator(internal::InternalMessage& msg, Selector selector)
	: m_iter(new internal::InternalMessageFieldIterator(msg, selector))
{
}


MessageFieldIterator::~MessageFieldIterator()
{
	delete m_iter;
}


bool MessageFieldIterator::hasNext() const
{
	return m_iter->hasNext();
}


const Field& MessageFieldIterator::next() const
{
	return m_iter->next();
}


void MessageFieldIterator::reset()
{
	m_iter->reset();
}

} // namespace api
} // namespace gmsec
