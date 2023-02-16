/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file Config.cpp
 *
 *  @brief Config class for configuring Connection and Message objects.
 */

#include <gmsec5/Config.h>
#include <gmsec5/internal/InternalConfig.h>


using namespace gmsec::api5;


Config::Config()
	: m_internal(new internal::InternalConfig())
{
}


Config::Config(int argc, char* argv[])
	: m_internal(new internal::InternalConfig(argc, argv))
{
}


Config::Config(const char* data, DataType type)
	: m_internal(new internal::InternalConfig(data, type))
{
}


Config::Config(const Config& other)
	: m_internal(new internal::InternalConfig(*other.m_internal))
{
}


Config::~Config()
{
	delete m_internal;
	m_internal = 0;
}


Config& Config::operator=(const Config& other)
{
	if (this != &other)
	{
		delete m_internal;
		m_internal = new internal::InternalConfig(*other.m_internal);
	}

	return *this;
}


void Config::addValue(const char* name, const char* value)
{
	return m_internal->addValue(name, value);
}


bool Config::clearValue(const char* name)
{
	return m_internal->clearValue(name);
}


const char* Config::getValue(const char* name) const
{
	return m_internal->getValue(name);
}


const char* Config::getValue(const char* name, const char* defaultValue) const
{
	return m_internal->getValue(name, defaultValue);
}


bool Config::getBooleanValue(const char* name) const
{
	return m_internal->getBooleanValue(name);
}


bool Config::getBooleanValue(const char* name, bool defaultValue) const
{
	return m_internal->getBooleanValue(name, defaultValue);
}


int Config::getIntegerValue(const char* name) const
{
	return m_internal->getIntegerValue(name);
}


int Config::getIntegerValue(const char* name, int defaultValue) const
{
	return m_internal->getIntegerValue(name, defaultValue);
}


double Config::getDoubleValue(const char* name) const
{
	return m_internal->getDoubleValue(name);
}


double Config::getDoubleValue(const char* name, double defaultValue) const
{
	return m_internal->getDoubleValue(name, defaultValue);
}


void Config::clear()
{
	return m_internal->clear();
}


bool Config::getFirst(const char*& name, const char*& value) const
{
	return m_internal->getFirst(name, value);
}


bool Config::getNext(const char*& name, const char*& value) const
{
	return m_internal->getNext(name, value);
}


void Config::merge(const Config& other, bool overwriteExisting)
{
	m_internal->merge(other, overwriteExisting);
}


const char* Config::toXML() const
{
	return m_internal->toXML();
}


void Config::fromXML(const char* xml)
{
	m_internal->fromXML(xml);
}


Config Config::getFromFile(const char* filename, const char* configName)
{
	return internal::InternalConfig::getFromFile(filename, configName);
}


const char* Config::toJSON() const
{
	return m_internal->toJSON();
}
