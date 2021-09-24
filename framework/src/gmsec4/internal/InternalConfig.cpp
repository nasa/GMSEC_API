/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <gmsec4/internal/InternalConfig.h>

#include <gmsec4/Config.h>
#include <gmsec4/ConfigOptions.h>
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
	  m_xml()
{
	m_configIter = m_configs.begin();
}


InternalConfig::InternalConfig(int argc, char* argv[])
	: m_configs(),
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

	if (!dataParsed)
	{
		throw Exception(CONFIG_ERROR, PARSE_ERROR,
			"Unable to parse configuration data string; it must contain valid XML data, JSON data, or key=pair values");
	}

	m_configIter = m_configs.begin();
}


InternalConfig::InternalConfig(const InternalConfig& cfg)
	: m_configs(),
	  m_xml()
{
	m_configs.insert(cfg.m_configs.begin(), cfg.m_configs.end());

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
	std::string uc_name = StringUtil::stringToUpper(name);

	if (uc_name == LOG_LEVEL)
	{
		LogLevel level = Log::fromString(value);
		Log::setReportingLevel(level);
	}
	else if (uc_name == LOG_FILE)
	{
		std::ostream* out = 0;

		if (StringUtil::stringEqualsIgnoreCase(value, LOG_STDOUT))
		{
			out = &std::cout;
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, LOG_STDERR))
		{
			out = &std::cerr;
		}
		else
		{
			out = new std::ofstream(value, std::ios::app);

			// Evaluate that the file stream is valid (using the operator!() method of ostream).
			if (!*out)
			{
				delete out;
				out = &std::cerr;

				LogLevel level = Log::getReportingLevel();
				Log::setReportingLevel(logERROR);
				GMSEC_ERROR << "Failed to open log file: " << value;
				Log::setReportingLevel(level);
			}
		}

		InternalLog::setDefaultStream(out);
	}
	else if (uc_name == GMSEC_CONVERT_FIELD_NAMES)
	{
		if (StringUtil::stringEqualsIgnoreCase(value, "uppercase"))
		{
			StringConverter::instance().setMode(StringConverter::TO_UPPERCASE);
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, "lowercase"))
		{
			StringConverter::instance().setMode(StringConverter::TO_LOWERCASE);
		}
		else
		{
			StringConverter::instance().setMode(StringConverter::NO_CONVERSION);
		}
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
	return getValue(name, NULL);
}


const char* InternalConfig::getValue(const char* name, const char* defaultValue) const
{
	const char* value = defaultValue;

	if (name != NULL && *name != '\0')
	{
		ConfigMapIter it = m_configs.find(name);

		if (it != m_configs.end())
		{
			value = it->second.c_str();
		}
	}

	return value;
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

	const char* cfgValue = it->second.c_str();
	bool value;

	if (StringUtil::stringEqualsIgnoreCase(cfgValue, "true") || StringUtil::stringEquals(cfgValue, "1"))
	{
		value = true;
	}
	else if (StringUtil::stringEqualsIgnoreCase(cfgValue, "false") || StringUtil::stringEquals(cfgValue, "0"))
	{
		value = false;
	}
	else
	{
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not represent a boolean value";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
	}

	return value;
}


bool InternalConfig::getBooleanValue(const char* name, bool defaultValue) const
{
	bool value = defaultValue;

	if (name != NULL && *name != '\0')
	{
		ConfigMapIter it = m_configs.find(name);

		if (it != m_configs.end())
		{
			const char* cfgValue = it->second.c_str();

			if (StringUtil::stringEqualsIgnoreCase(cfgValue, "true") || StringUtil::stringEquals(cfgValue, "1"))
			{
				value = true;
			}
			else if (StringUtil::stringEqualsIgnoreCase(cfgValue, "false") || StringUtil::stringEquals(cfgValue, "0"))
			{
				value = false;
			}
		}
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

	int value;

	try
	{
		value = StringUtil::getValue<int>(it->second.c_str());
	}
	catch (...)
	{
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not represent an integer value";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
	}

	return value;
}


int InternalConfig::getIntegerValue(const char* name, int defaultValue) const
{
	int value = defaultValue;

	if (name != NULL && *name != '\0')
	{
		ConfigMapIter it = m_configs.find(name);

		if (it != m_configs.end())
		{
			try
			{
				value = StringUtil::getValue<int>(it->second.c_str());
			}
			catch (...)
			{
				GMSEC_WARNING << "Config entry '" << name << "' does not represent an integer value; returning default value";
			}
		}
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

	double value;

	try
	{
		value = StringUtil::getValue<double>(it->second.c_str());
	}
	catch (...)
	{
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not represent a double value";
		throw Exception(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
	}

	return value;
}


double InternalConfig::getDoubleValue(const char* name, double defaultValue) const
{
	double value = defaultValue;

	if (name != NULL && *name != '\0')
	{
		ConfigMapIter it = m_configs.find(name);

		if (it != m_configs.end())
		{
			try
			{
				value = StringUtil::getValue<double>(it->second.c_str());
			}
			catch (...)
			{
				GMSEC_WARNING << "Config entry '" << name << "' does not represent a double value; returning default value";
			}
		}
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
	if (!mename || !StringUtil::stringEqualsIgnoreCase(mename, "CONFIG"))
	{
		throw Exception(CONFIG_ERROR, XML_PARSE_ERROR,
			"Invalid XML -- CONFIG element not found");
	}

	clear();

	for (tinyxml2::XMLElement* attr = element->FirstChildElement(); attr ; attr = attr->NextSiblingElement())
	{
		//field nodes only
		const char* caname = attr->Name();

		if (caname && StringUtil::stringEqualsIgnoreCase(caname, "PARAMETER"))
		{
			const char* name  = attr->Attribute("NAME");
			const char* value = attr->GetText();

			if (name == NULL)
			{
				name  = attr->Attribute("name");
			}

			if (name && value)
			{
				addValue(name, value);
			}
		}
	}
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

	if (origRoot.isMember("CONFIG") || origRoot.isMember("config"))
	{
		root = (origRoot.isMember("CONFIG") ? origRoot["CONFIG"] : origRoot["config"]);
	}
	else
	{
		throw Exception(MSG_ERROR, JSON_PARSE_ERROR,
			"Invalid JSON Config format -- no CONFIG root element");
	}

	if (root.isMember("PARAMETER") || root.isMember("parameter"))
	{
		const Json::Value params = (root.isMember("PARAMETER") ? root["PARAMETER"] : root["parameter"]);

		for (unsigned int i = 0; i < params.size(); ++i)
		{
			const Json::Value& param = params[i];

			std::string name;
			std::string value;

			if (param.isMember("NAME") || param.isMember("name"))
			{
				try
				{
					name = (param.isMember("NAME") ? param["NAME"].asCString() : param["name"].asCString());
				}
				catch (...)
				{
					double tmpName = (param.isMember("NAME") ? param["NAME"].asDouble() : param["name"].asDouble());
					std::ostringstream oss;
					oss << tmpName;
					name = oss.str();
				}
			}

			if (param.isMember("VALUE") || param.isMember("value"))
			{
				try
				{
					value = (param.isMember("VALUE") ? param["VALUE"].asCString() : param["value"].asCString());
				}
				catch (...)
				{
					double tmpValue = (param.isMember("VALUE") ? param["VALUE"].asDouble() : param["value"].asDouble());
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
    if (!name || *name == '\0')
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
