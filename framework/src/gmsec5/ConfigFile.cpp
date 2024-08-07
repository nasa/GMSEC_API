/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file ConfigFile.cpp
 *
 *  @brief This file contains a class for managing configuration files.
 */

#include <gmsec5/ConfigFile.h>
#include <gmsec5/ConfigFileIterator.h>

#include <gmsec5/internal/InternalConfigFile.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


ConfigFile::ConfigFile()
	: m_internal(new InternalConfigFile())
{
}


ConfigFile::ConfigFile(const Config& config)
	: m_internal(new InternalConfigFile(config))
{
}


ConfigFile::~ConfigFile()
{
	delete m_internal;
}


void ConfigFile::load(const char* filepath)
{
	m_internal->load(filepath);
}


void ConfigFile::save(const char* filePath, bool compact) const
{
	m_internal->save(filePath, compact);
}


void ConfigFile::fromXML(const char* xml)
{
	m_internal->fromXML(xml);
}


const char* ConfigFile::toXML() const
{
	return m_internal->toXML();
}


bool ConfigFile::isLoaded() const
{
	return m_internal->isLoaded();
}


void ConfigFile::addConfig(const char* name, const Config& config)
{
	m_internal->addConfig(name, config);
}


Config ConfigFile::lookupConfig(const char* name) const
{
	return m_internal->lookupConfig(name);
}


bool ConfigFile::removeConfig(const char* name)
{
	return m_internal->removeConfig(name);
}


void ConfigFile::addMessage(const char* name, const Message& message)
{
	m_internal->addMessage(name, message);
}


Message ConfigFile::lookupMessage(const char* name) const
{
	return m_internal->lookupMessage(name);
}


bool ConfigFile::removeMessage(const char* name)
{
	return m_internal->removeMessage(name);
}


void ConfigFile::addSubscriptionEntry(const SubscriptionEntry& entry)
{
	m_internal->addSubscriptionEntry(entry);
}


const ConfigFile::SubscriptionEntry& ConfigFile::lookupSubscriptionEntry(const char* name)
{
	return m_internal->lookupSubscriptionEntry(name);
}


bool ConfigFile::removeSubscriptionEntry(const char* name)
{
	return m_internal->removeSubscriptionEntry(name);
}


ConfigFileIterator& ConfigFile::getIterator() const
{
	return m_internal->getIterator();
}


// ConfigEntry
//
ConfigFile::ConfigEntry::ConfigEntry()
	: m_internal(new InternalConfigEntry())
{
}


ConfigFile::ConfigEntry::ConfigEntry(const ConfigFile::ConfigEntry& other)
	: m_internal(new InternalConfigEntry(*other.m_internal))
{
}


ConfigFile::ConfigEntry::~ConfigEntry()
{
	delete m_internal;
}


ConfigFile::ConfigEntry& ConfigFile::ConfigEntry::operator=(const ConfigFile::ConfigEntry& other)
{
	if (this != &other)
	{
		delete m_internal;

		m_internal = new InternalConfigEntry(*other.m_internal);
	}

	return *this;
}


const char* ConfigFile::ConfigEntry::getName() const
{
	return m_internal->getName();
}


const Config& ConfigFile::ConfigEntry::getConfig() const
{
	return m_internal->getConfig();
}


void ConfigFile::ConfigEntry::setName(const char* name)
{
	m_internal->setName(name);
}


void ConfigFile::ConfigEntry::setConfig(const Config& cfg)
{
	m_internal->setConfig(cfg);
}


// MessageEntry
//
ConfigFile::MessageEntry::MessageEntry()
	: m_internal(new InternalMessageEntry())
{
}


ConfigFile::MessageEntry::MessageEntry(const ConfigFile::MessageEntry& other)
	: m_internal(new InternalMessageEntry(*other.m_internal))
{
}


ConfigFile::MessageEntry::~MessageEntry()
{
	delete m_internal;
}


ConfigFile::MessageEntry& ConfigFile::MessageEntry::operator=(const ConfigFile::MessageEntry& other)
{
	if (this != &other)
	{
		delete m_internal;

		m_internal = new InternalMessageEntry(*other.m_internal);
	}

	return *this;
}


const char* ConfigFile::MessageEntry::getName() const
{
	return m_internal->getName();
}


const Message& ConfigFile::MessageEntry::getMessage() const
{
	return m_internal->getMessage();
}


void ConfigFile::MessageEntry::setName(const char* name)
{
	m_internal->setName(name);
}


void ConfigFile::MessageEntry::setMessage(const Message& msg)
{
	m_internal->setMessage(msg);
}


// SubscriptionEntry
//
ConfigFile::SubscriptionEntry::SubscriptionEntry()
	: m_internal(new InternalSubscriptionEntry())
{
}


ConfigFile::SubscriptionEntry::SubscriptionEntry(const char* name, const char* pattern)
	: m_internal(new InternalSubscriptionEntry(name, pattern))
{
}


ConfigFile::SubscriptionEntry::SubscriptionEntry(const ConfigFile::SubscriptionEntry& other)
	: m_internal(new InternalSubscriptionEntry(*other.m_internal))
{
}


ConfigFile::SubscriptionEntry::~SubscriptionEntry()
{
	delete m_internal;
}


ConfigFile::SubscriptionEntry& ConfigFile::SubscriptionEntry::operator=(const ConfigFile::SubscriptionEntry& other)
{
	if (this != &other)
	{
		delete m_internal;

		m_internal = new InternalSubscriptionEntry(*other.m_internal);
	}

	return *this;
}


const char* ConfigFile::SubscriptionEntry::getName() const
{
	return m_internal->getName();
}


const char* ConfigFile::SubscriptionEntry::getPattern() const
{
        return m_internal->getPattern();
}


void ConfigFile::SubscriptionEntry::addExcludedPattern(const char* pattern)
{
	m_internal->addExcludedPattern(pattern);
}


bool ConfigFile::SubscriptionEntry::hasNextExcludedPattern() const
{
	return m_internal->hasNextExcludedPattern();
}


const char* ConfigFile::SubscriptionEntry::nextExcludedPattern() const
{
	return m_internal->nextExcludedPattern();
}
