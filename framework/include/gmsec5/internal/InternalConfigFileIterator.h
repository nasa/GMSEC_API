/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalConfigFileIterator.h
 *
 *  @This file contains a definition for an iterator for the ConfigFile.
 */

#ifndef GMSEC_API5_INTERNAL_CONFIG_FILE_ITERATOR_H
#define GMSEC_API5_INTERNAL_CONFIG_FILE_ITERATOR_H

#include <gmsec5/Config.h>
#include <gmsec5/ConfigFileIterator.h>
#include <gmsec5/Message.h>

#include <gmsec5/internal/InternalConfigFile.h>

#include <gmsec5/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api5
{
namespace internal
{


class GMSEC_API InternalConfigFileIterator
{
public:
	InternalConfigFileIterator(InternalConfigFile& cfgFile);


	~InternalConfigFileIterator();


	bool CALL_TYPE hasNextConfig();


	bool CALL_TYPE hasNextMessage();


	bool CALL_TYPE hasNextSubscription();


	const ConfigFile::ConfigEntry& CALL_TYPE nextConfig();


	const ConfigFile::MessageEntry& CALL_TYPE nextMessage();


	const ConfigFile::SubscriptionEntry& CALL_TYPE nextSubscription();


	void CALL_TYPE reset();


private:
	// Declared, but not implemented.
	InternalConfigFileIterator(const InternalConfigFileIterator&);
	InternalConfigFileIterator& operator=(const InternalConfigFileIterator&);

	bool makeConfigReady();
	bool makeMessageReady();
	bool makeSubscriptionReady();

	InternalConfigFile&            m_cfgFile;
	ConfigFile::ConfigEntry*       m_nextConfigEntry;
	ConfigFile::MessageEntry*      m_nextMessageEntry;
	ConfigFile::SubscriptionEntry* m_nextSubscriptionEntry;

	ConfigFile::ConfigEntry        m_returnableConfigEntry;
	ConfigFile::MessageEntry       m_returnableMessageEntry;
	ConfigFile::SubscriptionEntry  m_returnableSubscriptionEntry;
};


}  //namespace internal
}  //namespace api5
}  //namespace gmsec

#endif
