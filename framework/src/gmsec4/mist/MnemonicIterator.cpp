/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/mist/MnemonicIterator.h>

#include <gmsec4/mist/Mnemonic.h>

#include <gmsec4/internal/mist/message/InternalMnemonicMessage.h>

#include <gmsec4/internal/mist/InternalMnemonicIterator.h>


using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


MnemonicIterator::MnemonicIterator(InternalMnemonicMessage& msg)
	: m_iter(new gmsec::api::mist::internal::InternalMnemonicIterator(msg))
{
}


MnemonicIterator::~MnemonicIterator()
{
	delete m_iter;
}


bool MnemonicIterator::hasNext() const
{
	return m_iter->hasNext();
}


const Mnemonic& MnemonicIterator::next() const
{
	return m_iter->next();
}


void MnemonicIterator::reset()
{
	m_iter->reset();
}
