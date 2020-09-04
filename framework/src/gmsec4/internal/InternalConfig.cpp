/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <gmsec4/internal/InternalConfig.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/util/Log.h>

#include <gmsec4/internal/util/InternalLog.h>

#include <gmsec4/internal/StringUtil.h>

#include <tinyxml2.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


InternalConfig::InternalConfig()
	: m_configs(),
	  m_configIter(),
	  m_xml()
{
}


InternalConfig::InternalConfig(int argc, char* argv[])
	: m_configs(),
	  m_configIter(),
	  m_xml()
{
	for (int i = 0; i < argc; ++i)
	{
		std::string tmp(argv[i]);

		// Determine if we have a key=value pair
		//
		size_t t = tmp.find("=");
		if (t != std::string::npos)
		{
			std::string key   = tmp.substr(0,t);
			std::string value = tmp.substr(t+1);

			if (key.length() > 0 && value.length() > 0)
			{
				addValue(key.c_str(), value.c_str());
			}
		}
	}

	m_configIter = m_configs.begin();
}


InternalConfig::InternalConfig(const char* xml)
	: m_configs(),
	  m_configIter(),
	  m_xml()
{
	if (!xml || std::string(xml).empty())
	{
		throw Exception(CONFIG_ERROR, XML_PARSE_ERROR, "XML string cannot be NULL, nor an empty string");
	}

	fromXML(xml);
	m_configIter = m_configs.begin();
}


InternalConfig::InternalConfig(const InternalConfig& cfg)
	: m_configs(),
	  m_configIter(),
	  m_xml()
{
	const char* name  = 0;
	const char* value = 0;

	bool hasNext = cfg.getFirst(name, value);

	while (hasNext)
	{
		addValue(name, value);

		hasNext = cfg.getNext(name, value);
	}

	m_configIter = m_configs.begin();
}


InternalConfig::~InternalConfig()
{
}


void InternalConfig::clear()
{
	m_configs.clear();
}


void InternalConfig::addValue(const char* name, const char* value)
{
	validateConfigName(name);

	if (!value)
	{
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, "Config value is NULL");
	}

	m_configs[name] = value;

	// special handling of log parameters
	std::string lowername = StringUtil::stringToLower(name);

	if (lowername == "loglevel")
	{
		LogLevel level = Log::fromString(value);
		Log::setReportingLevel(level);
	}
	else if (lowername == "logfile")
	{
		std::ostream* out = 0;

		if (StringUtil::stringEqualsIgnoreCase(value, "STDOUT"))
		{
			out = &std::cout;
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, "STDERR"))
		{
			out = &std::cerr;
		}
		else
		{
			out = new std::ofstream(value, std::ios::app);

			// Evaluate that the file stream is valid (using the operator!() method of ostream).
			if (!*out)
			{
				GMSEC_ERROR << "Failed to open log file: " << value;
				delete out;
				out = &std::cerr;
			}
		}

		InternalLog::setDefaultStream(out);
	}
}


bool InternalConfig::clearValue(const char* name)
{
	validateConfigName(name);

	ConfigMap::iterator it = m_configs.find(name);

	if (it == m_configs.end())
	{
		return false;
	}

	m_configs.erase(name);
	return true;
}


const char* InternalConfig::getValue(const char* name) const
{
	validateConfigName(name);

	ConfigMapIter it = m_configs.find(name);

	if (it == m_configs.end())
	{
		return NULL;
	}

	return it->second.c_str();
}


const char* InternalConfig::getValue(const char* name, const char* defaultValue) const
{
	validateConfigName(name);

	ConfigMapIter it = m_configs.find(name);

	if (it == m_configs.end())
	{
		return defaultValue;
	}

	return it->second.c_str();
}


bool InternalConfig::getBooleanValue(const char* name) const
{
	validateConfigName(name);

	ConfigMapIter it = m_configs.find(name);

	if (it == m_configs.end())
	{
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_NAME, "Config entry does not exist");
	}

	const char* value = it->second.c_str();

	if (!StringUtil::stringEqualsIgnoreCase(value, "true") && !StringUtil::stringEqualsIgnoreCase(value, "false"))
	{
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, "Config entry does not represent a boolean value");
	}

	return StringUtil::stringEqualsIgnoreCase(value, "true");
}


bool InternalConfig::getBooleanValue(const char* name, bool defaultValue) const
{
	validateConfigName(name);

	bool value = defaultValue;

	try {
		value = getBooleanValue(name);
	}
	catch (Exception& e) {
		GMSEC_VERBOSE << e.what() << "; returning given default value [" << (value ? "true" : "false") << "]";
	}

	return value;
}


int InternalConfig::getIntegerValue(const char* name) const
{
	validateConfigName(name);

	ConfigMapIter it = m_configs.find(name);

	if (it == m_configs.end())
	{
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_NAME, "Config entry does not exist");
	}

	std::istringstream iss(it->second.c_str());

	int value;

	iss >> value;

	if (iss.fail())
	{
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, "Config entry does not represent an integer value");
	}

	return value;
}


int InternalConfig::getIntegerValue(const char* name, int defaultValue) const
{
	validateConfigName(name);

	int value = defaultValue;

	try {
		value = getIntegerValue(name);
	}
	catch (Exception& e) {
		GMSEC_VERBOSE << e.what() << "; returning given default value [" << value << "]";
	}

	return value;
}


double InternalConfig::getDoubleValue(const char* name) const
{
	validateConfigName(name);

	ConfigMapIter it = m_configs.find(name);

	if (it == m_configs.end())
	{
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_NAME, "Config entry does not exist");
	}

	std::istringstream iss(it->second.c_str());

	double value;

	iss >> value;

	if (iss.fail())
	{
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, "Config entry does not represent a double value");
	}

	return value;
}


double InternalConfig::getDoubleValue(const char* name, double defaultValue) const
{
	validateConfigName(name);

	double value = defaultValue;

	try {
		value = getDoubleValue(name);
	}
	catch (Exception& e) {
		GMSEC_VERBOSE << e.what() << "; returning given default value [" << value << "]";
	}

	return value;
}


bool InternalConfig::getFirst(const char*& name, const char*& value) const
{
	m_configIter = m_configs.begin();

	return getNext(name, value);
}


bool InternalConfig::getNext(const char*& name, const char*& value) const
{
	name  = NULL;
	value = NULL;

	if (m_configIter == m_configs.end())
	{
		return false;
	}

	name  = m_configIter->first.c_str();
	value = m_configIter->second.c_str();

	++m_configIter;

	return true;
}


void InternalConfig::merge(const Config& other, bool overwriteExisting)
{
	const char* name  = 0;
	const char* value = 0;

	bool hasPair = other.getFirst(name, value);

	while (hasPair)
	{
		const char* hasValue = this->getValue(name);

		if ((!hasValue) || (hasValue && overwriteExisting))
		{
			this->addValue(name, value);
		}
		else
		{
			GMSEC_DEBUG << "Existing [" << name << ", " << value << "] is not being merged with Config";
		}

		hasPair = other.getNext(name, value);
	}
}


const char* InternalConfig::toXML() const
{
	std::ostringstream oss;

	oss << "<CONFIG>" << "\n";

	const char* name = 0;
	const char* value = 0;

	bool havePair = getFirst(name, value);

	while (havePair)
	{
		oss << "\t" << "<PARAMETER NAME=\"" << name << "\">" << value << "</PARAMETER>" << "\n";

		havePair = getNext(name, value);
	}

	oss << "</CONFIG>";

	m_xml = oss.str();

	return m_xml.c_str();
}


void InternalConfig::fromXML(const char* xml)
{
	if (!xml || std::string(xml).empty())
	{
		throw Exception(CONFIG_ERROR, XML_PARSE_ERROR,
			"XML string cannot be NULL, nor empty");
	}

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError code = doc.Parse(xml);

	if (code != tinyxml2::XML_NO_ERROR)
	{
		throw Exception(CONFIG_ERROR, XML_PARSE_ERROR, code,
			"Invalid XML format -- parse error");
	}

	tinyxml2::XMLElement* element = doc.RootElement();

	if (!element)
	{
		throw Exception(CONFIG_ERROR, XML_PARSE_ERROR,
			"Invalid XML format -- no root element");
	}

	fromXML(element);
}


void InternalConfig::fromXML(tinyxml2::XMLElement* element)
{
	if (!element)
	{
		throw Exception(CONFIG_ERROR, XML_PARSE_ERROR,
			"Invalid XML -- element is NULL");
	}

	const char* mename = element->Name();
	if (!mename || std::string(mename) != "CONFIG")
	{
		throw Exception(CONFIG_ERROR, XML_PARSE_ERROR,
			"Invalid XML -- CONFIG element not found");
	}

	clear();

	for (tinyxml2::XMLElement* attr = element->FirstChildElement(); attr ; attr = attr->NextSiblingElement())
	{
		//field nodes only
		const char* caname = attr->Name();

		if (caname && std::string(caname) == "PARAMETER")
		{
			const char* name  = attr->Attribute("NAME");
			const char* value = attr->GetText();

			if (name && value)
			{
				addValue(name, value);
			}
		}
	}

	m_configIter = m_configs.begin();
}


void InternalConfig::validateConfigName(const char* name) const
{
    if (!name || std::string(name).empty())
    {
        throw Exception(CONFIG_ERROR, INVALID_CONFIG_NAME,
                "Config name cannot be NULL, nor an empty string");
    }
}
