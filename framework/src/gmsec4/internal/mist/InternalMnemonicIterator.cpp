/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/mist/InternalMnemonicIterator.h>

#include <gmsec4/internal/mist/message/InternalMnemonicMessage.h>


using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


InternalMnemonicIterator::InternalMnemonicIterator(InternalMnemonicMessage& msg)
	: m_msg(msg),
	  m_ready(false),
	  m_nextMnemonic(0)
{
}


InternalMnemonicIterator::~InternalMnemonicIterator()
{
}


bool InternalMnemonicIterator::hasNext()
{
	return makeReady();
}


const Mnemonic& InternalMnemonicIterator::next()
{
	const Mnemonic* mnemonic = 0;

	if (makeReady())
	{
		mnemonic = m_nextMnemonic;
		m_nextMnemonic = 0;
	}

	if (!mnemonic)
	{
		throw Exception(ITERATOR_ERROR, ITER_INVALID_NEXT, "There are no more Mnemonics");
	}

	return *mnemonic;
}


void InternalMnemonicIterator::reset()
{
	m_msg.resetMnemonicIterator();
}


bool InternalMnemonicIterator::makeReady()
{
	bool ready = false;

	if (m_nextMnemonic)
	{
		ready = true;
	}
	else
	{
		while (m_msg.hasNextMnemonic() && !ready)
		{
			const Mnemonic& mnemonic = m_msg.nextMnemonic();

			m_nextMnemonic = &mnemonic;

			ready = true;
		}
	}

	return ready;
}
