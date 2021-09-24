/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec4/internal/mist/message/InternalProductFileMessage.h>

#include <gmsec4/internal/mist/InternalProductFileIterator.h>


using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


InternalProductFileIterator::InternalProductFileIterator(InternalProductFileMessage& msg)
	: m_msg(msg),
	  m_ready(false),
	  m_nextProductFile(0)
{
}


InternalProductFileIterator::~InternalProductFileIterator()
{
}


bool InternalProductFileIterator::hasNext()
{
	return makeReady();
}


const ProductFile& InternalProductFileIterator::next()
{
	const ProductFile* productFile = 0;

	if (makeReady())
	{
		productFile = m_nextProductFile;
		m_nextProductFile = 0;
	}

	if (!productFile)
	{
		throw Exception(ITERATOR_ERROR, ITER_INVALID_NEXT, "There are no more ProductFiles");
	}

	return *productFile;
}


void InternalProductFileIterator::reset()
{
	m_msg.resetProductFileIterator();
}


bool InternalProductFileIterator::makeReady()
{
	bool ready = false;

	if (m_nextProductFile)
	{
		ready = true;
	}
	else
	{
		while (m_msg.hasNextProductFile() && !ready)
		{
			const ProductFile& productFile = m_msg.nextProductFile();

			m_nextProductFile = &productFile;

			ready = true;
		}
	}

	return ready;
}
