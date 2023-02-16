/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalConfigFileIterator.cpp
 *
 *  @This file contains a definition for an iterator for the ConfigFile.
 */

#include <gmsec5/internal/InternalConfigFileIterator.h>

#include <gmsec5/GmsecException.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


InternalConfigFileIterator::InternalConfigFileIterator(InternalConfigFile& cfgFile)
	: m_cfgFile(cfgFile),
	  m_nextConfigEntry(0),
	  m_nextMessageEntry(0),
	  m_nextSubscriptionEntry(0),
	  m_returnableConfigEntry(),
	  m_returnableMessageEntry(),
	  m_returnableSubscriptionEntry()
{
}


InternalConfigFileIterator::~InternalConfigFileIterator()
{
	delete m_nextConfigEntry;
	delete m_nextMessageEntry;
	delete m_nextSubscriptionEntry;
}


bool InternalConfigFileIterator::hasNextConfig()
{
	return makeConfigReady();
}


bool InternalConfigFileIterator::hasNextMessage()
{
	return makeMessageReady();
}


bool InternalConfigFileIterator::hasNextSubscription()
{
	return makeSubscriptionReady();
}


const ConfigFile::ConfigEntry& InternalConfigFileIterator::nextConfig()
{
	if (makeConfigReady())
	{
		m_returnableConfigEntry = *m_nextConfigEntry;

		delete m_nextConfigEntry;
		m_nextConfigEntry = 0;
	}
	else
	{
		throw GmsecException(ITERATOR_ERROR, ITER_INVALID_NEXT, "There are no more Config entries");
	}

	return m_returnableConfigEntry;
}


const ConfigFile::MessageEntry& InternalConfigFileIterator::nextMessage()
{
	if (makeMessageReady())
	{
		m_returnableMessageEntry = *m_nextMessageEntry;

		delete m_nextMessageEntry;
		m_nextMessageEntry = 0;
	}
	else
	{
		throw GmsecException(ITERATOR_ERROR, ITER_INVALID_NEXT, "There are no more Message entries");
	}

	return m_returnableMessageEntry;
}


const ConfigFile::SubscriptionEntry& InternalConfigFileIterator::nextSubscription()
{
	if (makeSubscriptionReady())
	{
		m_returnableSubscriptionEntry = *m_nextSubscriptionEntry;

		delete m_nextSubscriptionEntry;
		m_nextSubscriptionEntry = 0;
	}
	else
	{
		throw GmsecException(ITERATOR_ERROR, ITER_INVALID_NEXT, "There are no more Subscription entries");
	}

	return m_returnableSubscriptionEntry;
}


void InternalConfigFileIterator::reset()
{
	m_cfgFile.resetIterators();
}


bool InternalConfigFileIterator::makeConfigReady()
{
	bool ready = false;

	if (m_nextConfigEntry)
	{
		ready = true;
	}
	else
	{
		while (m_cfgFile.hasNextConfig() && !ready)
		{
			m_nextConfigEntry = new ConfigFile::ConfigEntry(m_cfgFile.nextConfig());

			ready = true;
		}
	}

	return ready;
}


bool InternalConfigFileIterator::makeMessageReady()
{
	bool ready = false;

	if (m_nextMessageEntry)
	{
		ready = true;
	}
	else
	{
		while (m_cfgFile.hasNextMessage() && !ready)
		{
			m_nextMessageEntry = new ConfigFile::MessageEntry(m_cfgFile.nextMessage());

			ready = true;
		}
	}

	return ready;
}


bool InternalConfigFileIterator::makeSubscriptionReady()
{
	bool ready = false;

	if (m_nextSubscriptionEntry)
	{
		ready = true;
	}
	else
	{
		while (m_cfgFile.hasNextSubscription() && !ready)
		{
			m_nextSubscriptionEntry = new ConfigFile::SubscriptionEntry(m_cfgFile.nextSubscription());

			ready = true;
		}
	}

	return ready;
}
