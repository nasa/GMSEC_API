/*
 * Copyright 2007-2018 United States Government as represented by the
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
#include <json.h>

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


InternalConfig::InternalConfig(const char* data)
	: m_configs(),
	  m_configIter(),
	  m_xml()
{
	if (!data || std::string(data).empty())
	{
		throw Exception(CONFIG_ERROR, XML_PARSE_ERROR, "XML string cannot be NULL, nor an empty string");
	}

	bool dataParsed = false;

	if (!dataParsed)
	{
		// Determine if we have an XML data string

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError code = doc.Parse(data);

		if (code == tinyxml2::XML_NO_ERROR)
		{
			tinyxml2::XMLElement* element = doc.RootElement();

			if (!element)
			{
				throw Exception(CONFIG_ERROR, XML_PARSE_ERROR,
					"Invalid XML format -- no root element");
			}

			fromXML(element);

			dataParsed = true;
		}
	}

	if (!dataParsed)
	{
		// If here, determine if we have a JSON data string

		Json::Reader reader;
		Json::Value  root;
		bool         success = reader.parse(data, root);

		if (success)
		{
			if (!root)
			{
				throw Exception(MSG_ERROR, JSON_PARSE_ERROR,
					"Invalid JSON message format -- invalid JSON string data");
			}

			fromJSON(root);

			dataParsed = true;
		}
	}

	if (!dataParsed)
	{
		// If here, determine if we have raw string containing key=value pairs

		std::vector<std::string> pairs;

		getKeyValuePairs(data, pairs);

		for (std::vector<std::string>::iterator it = pairs.begin(); it != pairs.end(); ++it)
		{
			std::string pair  = *it;
			size_t      equal = pair.find("=");

			addValue(pair.substr(0, equal).c_str(), pair.substr(equal + 1).c_str());
		}

		dataParsed = (pairs.size() > 0);
	}

	if (dataParsed)
	{
		m_configIter = m_configs.begin();
	}
	else
	{
		throw Exception(CONFIG_ERROR, PARSE_ERROR,
			"Unable to parse configuration data string; it must contain valid XML data, JSON data, or key=pair values");
	}
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
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not exist";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_NAME, oss.str().c_str());
	}

	const char* value = it->second.c_str();

	if (!StringUtil::stringEqualsIgnoreCase(value, "true") && !StringUtil::stringEqualsIgnoreCase(value, "false"))
	{
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not represent a boolean value";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
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
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not exist";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_NAME, oss.str().c_str());
	}

	std::istringstream iss(it->second.c_str());

	int value;

	iss >> value;

	if (iss.fail())
	{
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not represent an integer value";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
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
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not exist";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_NAME, oss.str().c_str());
	}

	std::istringstream iss(it->second.c_str());

	double value;

	iss >> value;

	if (iss.fail())
	{
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not represent a double value";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
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
		oss << "\t" << "<PARAMETER NAME=\"" << name << "\">"
		    << StringUtil::toXML(value)
		    << "</PARAMETER>" << "\n";

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


const char* InternalConfig::toJSON(bool standalone) const
{
	std::ostringstream oss;

	const char* name  = NULL;
	const char* value = NULL;

	bool hasConfig = getFirst(name, value);

	if (hasConfig)
	{
		if (standalone)
		{
			oss << "{";
		}

		oss << "\"CONFIG\":{\"PARAMETER\":[";

		while (hasConfig)
		{
			oss << "{\"NAME\":\"" << name
			    << "\",\"VALUE\":"
			    << StringUtil::toJSON(value)
			    << "}";

			hasConfig = getNext(name, value);

			if (hasConfig)
			{
				oss << ",";
			}
		}

		oss << "]}";

		if (standalone)
		{
			oss << "}";
		}

	}

	m_json = oss.str();

	return m_json.c_str();
}


void InternalConfig::fromJSON(const Json::Value& origRoot)
{
	Json::Value root;

	if (origRoot.isMember("CONFIG"))
	{
		root = origRoot["CONFIG"];
	}
	else
	{
		throw Exception(MSG_ERROR, JSON_PARSE_ERROR,
			"Invalid JSON Config format -- no root element");
	}

	if (root.isMember("PARAMETER"))
	{
		const Json::Value params = root["PARAMETER"];

		for (unsigned int i = 0; i < params.size(); ++i)
		{
			const Json::Value& param = params[i];

			std::string name;
			std::string value;

			if (param.isMember("NAME"))
			{
				try
				{
					name = param["NAME"].asCString();
				}
				catch (...)
				{
					double tmpName = param["NAME"].asDouble();
					std::ostringstream oss;
					oss << tmpName;
					name = oss.str();
				}
			}

			if (param.isMember("VALUE"))
			{
				try
				{
					value = param["VALUE"].asCString();
				}
				catch (...)
				{
					double tmpValue = param["VALUE"].asDouble();
					std::ostringstream oss;
					oss << tmpValue;
					value = oss.str();
				}
			}

			if (!name.empty() && !value.empty())
			{
				addValue(name.c_str(), value.c_str());
			}
		}
	}
}


void InternalConfig::validateConfigName(const char* name) const
{
    if (!name || std::string(name).empty())
    {
        throw Exception(CONFIG_ERROR, INVALID_CONFIG_NAME,
                "Config name cannot be NULL, nor an empty string");
    }
}


void InternalConfig::getKeyValuePairs(const std::string& data, std::vector<std::string>& pairs)
{
	std::string token;
	int         foundQuote = 0;

	for (std::string::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		const char ch = *it;

		if (ch == '"')
		{
			foundQuote ^= 1;
		}
		else if (!foundQuote && isspace(ch))
		{
			// Found end of token

			if (!token.empty() && token.find("=") != std::string::npos)
			{
				pairs.push_back(token);
			}

			token.clear();
		}
		else
		{
			token += ch;
		}
	}

	// Capture last token, if any
	if (!token.empty() && token.find("=") != std::string::npos)
	{
		pairs.push_back(token);
	}
}
