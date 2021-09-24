/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/mist/ProductFileIterator.h>

#include <gmsec4/mist/ProductFile.h>

#include <gmsec4/internal/mist/message/InternalProductFileMessage.h>

#include <gmsec4/internal/mist/InternalProductFileIterator.h>

using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


ProductFileIterator::ProductFileIterator(InternalProductFileMessage& msg)
	: m_iter(new InternalProductFileIterator(msg))
{
}


ProductFileIterator::~ProductFileIterator()
{
	delete m_iter;
}


bool ProductFileIterator::hasNext() const
{
	return m_iter->hasNext();
}


const ProductFile& ProductFileIterator::next() const
{
	return m_iter->next();
}


void ProductFileIterator::reset()
{
	m_iter->reset();
}
