/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFileIterator.cpp
 *
 *  @This file contains a definition for an iterator for the ConfigFile.
 */

#include <gmsec5/ConfigFileIterator.h>

#include <gmsec5/internal/InternalConfigFileIterator.h>

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


ConfigFileIterator::ConfigFileIterator(internal::InternalConfigFile& cfgFile)
	: m_iter(new InternalConfigFileIterator(cfgFile))
{
}


ConfigFileIterator::~ConfigFileIterator()
{
	delete m_iter;
}


bool ConfigFileIterator::hasNextConfig() const
{
	return m_iter->hasNextConfig();
}


bool ConfigFileIterator::hasNextMessage() const
{
	return m_iter->hasNextMessage();
}


bool ConfigFileIterator::hasNextSubscription() const
{
	return m_iter->hasNextSubscription();
}


const ConfigFile::ConfigEntry& ConfigFileIterator::nextConfig() const
{
	return m_iter->nextConfig();
}


const ConfigFile::MessageEntry& ConfigFileIterator::nextMessage() const
{
	return m_iter->nextMessage();
}


const ConfigFile::SubscriptionEntry& ConfigFileIterator::nextSubscription() const
{
	return m_iter->nextSubscription();
}


void ConfigFileIterator::reset()
{
	m_iter->reset();
}
