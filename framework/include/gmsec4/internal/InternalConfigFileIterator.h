/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalConfigFileIterator.h
 *
 *  @This file contains a definition for an iterator for the ConfigFile.
 */

#ifndef GMSEC_API_INTERNAL_CONFIG_FILE_ITERATOR_H
#define GMSEC_API_INTERNAL_CONFIG_FILE_ITERATOR_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/Config.h>
#include <gmsec4/ConfigFileIterator.h>
#include <gmsec4/Message.h>

#include <gmsec4/internal/InternalConfigFile.h>

#include <string>


namespace gmsec
{
namespace api
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


	bool CALL_TYPE hasNextCustomElement();

	
	const ConfigFile::ConfigEntry& CALL_TYPE nextConfig();


	const ConfigFile::MessageEntry& CALL_TYPE nextMessage();


	const ConfigFile::SubscriptionEntry& CALL_TYPE nextSubscription();


	const char* CALL_TYPE nextCustomElement();


	void CALL_TYPE reset();


private:
	// Declared, but not implemented.
	InternalConfigFileIterator(const InternalConfigFileIterator&);
	InternalConfigFileIterator& operator=(const InternalConfigFileIterator&);

	bool CALL_TYPE makeConfigReady();
	bool CALL_TYPE makeMessageReady();
	bool CALL_TYPE makeSubscriptionReady();
	bool CALL_TYPE makeCustomElementReady();

	InternalConfigFile&            m_cfgFile;
	ConfigFile::ConfigEntry*       m_nextConfigEntry;
	ConfigFile::MessageEntry*      m_nextMessageEntry;
	ConfigFile::SubscriptionEntry* m_nextSubscriptionEntry;
	const char*                    m_nextCustomElementEntry;

	ConfigFile::ConfigEntry        m_returnableConfigEntry;
	ConfigFile::MessageEntry       m_returnableMessageEntry;
	ConfigFile::SubscriptionEntry  m_returnableSubscriptionEntry;
};


}  //namespace internal
}  //namespace api
}  //namespace gmsec

#endif
