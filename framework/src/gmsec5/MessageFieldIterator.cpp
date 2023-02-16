/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file MessageFieldIterator.h
 */

#include <gmsec5/MessageFieldIterator.h>

#include <gmsec5/internal/InternalMessage.h>
#include <gmsec5/internal/InternalMessageFieldIterator.h>


using namespace gmsec::api5;


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
