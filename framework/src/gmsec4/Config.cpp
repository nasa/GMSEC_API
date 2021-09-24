/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 *  @file Config.cpp
 *
 *  Opaque Config class
 *
 *
*/

#include <gmsec4/Config.h>
#include <gmsec4/internal/InternalConfig.h>


namespace gmsec
{
namespace api
{


Config::Config()
	: m_iConfig(new internal::InternalConfig())
{
}


Config::Config(int argc, char* argv[])
	: m_iConfig(new internal::InternalConfig(argc, argv))
{
}


Config::Config(const char* data)
	: m_iConfig(new internal::InternalConfig(data))
{
}


Config::Config(const Config& other)
	: m_iConfig(new internal::InternalConfig(*other.m_iConfig))
{
}


Config::~Config()
{
	delete m_iConfig;
	m_iConfig = 0;
}


Config& Config::operator=(const Config& other)
{
	if (this != &other)
	{
		delete m_iConfig;
		m_iConfig = new internal::InternalConfig(*other.m_iConfig);
	}

	return *this;
}


void Config::addValue(const char* name, const char* value)
{
	return m_iConfig->addValue(name, value);
}


bool Config::clearValue(const char* name)
{
	return m_iConfig->clearValue(name);
}


const char* Config::getValue(const char* name) const
{
	return m_iConfig->getValue(name);
}

const char* Config::getValue(const char* name, const char* defaultValue) const
{
	return m_iConfig->getValue(name, defaultValue);
}

bool Config::getBooleanValue(const char* name) const
{
	return m_iConfig->getBooleanValue(name);
}


bool Config::getBooleanValue(const char* name, bool defaultValue) const
{
	return m_iConfig->getBooleanValue(name, defaultValue);
}


int Config::getIntegerValue(const char* name) const
{
	return m_iConfig->getIntegerValue(name);
}


int Config::getIntegerValue(const char* name, int defaultValue) const
{
	return m_iConfig->getIntegerValue(name, defaultValue);
}


double Config::getDoubleValue(const char* name) const
{
	return m_iConfig->getDoubleValue(name);
}


double Config::getDoubleValue(const char* name, double defaultValue) const
{
	return m_iConfig->getDoubleValue(name, defaultValue);
}


void Config::clear()
{
	return m_iConfig->clear();
}


bool Config::getFirst(const char*& name, const char*& value) const
{
	return m_iConfig->getFirst(name, value);
}


bool Config::getNext(const char*& name, const char*& value) const
{
	return m_iConfig->getNext(name, value);
}


void Config::merge(const Config& other, bool overwriteExisting)
{
	m_iConfig->merge(other, overwriteExisting);
}


const char* Config::toXML() const
{
	return m_iConfig->toXML();
}


void Config::fromXML(const char* xml)
{
	m_iConfig->fromXML(xml);
}


const char* Config::toJSON() const
{
	return m_iConfig->toJSON();
}

} // namespace api
} // namespace gmsec
