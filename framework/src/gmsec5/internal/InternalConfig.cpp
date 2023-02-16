/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/InternalConfig.h>

#include <gmsec5/ConfigOptions.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/util/Log.h>

#include <gmsec5/internal/field/InternalField.h>
#include <gmsec5/internal/util/InternalLog.h>

#include <gmsec5/internal/StringUtil.h>

#include <tinyxml2.h>
#include <json.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


InternalConfig::InternalConfig()
	: m_configs(),
	  m_configName(),
	  m_configIter(),
	  m_xml()
{
}


InternalConfig::InternalConfig(int argc, char* argv[])
	: m_configs(),
	  m_configName(),
	  m_configIter(),
	  m_xml()
{
	for (int i = 0; i < argc; ++i)
	{
		std::string tmp(argv[i]);

		// Determine if we have a key=value pair
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


InternalConfig::InternalConfig(const char* data, DataType type)
	: m_configs(),
	  m_configName(),
	  m_configIter(),
	  m_xml()
{
	if (!data || std::string(data).empty())
	{
		throw GmsecException(CONFIG_ERROR, XML_PARSE_ERROR, "Config data string cannot be NULL, nor an empty string");
	}

	// Use type to discern which algorithm to use to parse data; if a failure occurs,
	// report the error immediately. Do NOT try to parse data using incorrect algorithm.

	switch (type)
	{
	case DataType::XML_DATA:
		{
			// Determine if we have an XML data string
			tinyxml2::XMLDocument doc;
			tinyxml2::XMLError code = doc.Parse(data);

			if (code != tinyxml2::XML_NO_ERROR)
			{
				throw GmsecException(CONFIG_ERROR, XML_PARSE_ERROR, StringUtil::xmlErrorToString(code));
			}

			tinyxml2::XMLElement* element = doc.RootElement();

			if (!element)
			{
				throw GmsecException(CONFIG_ERROR, XML_PARSE_ERROR, "Invalid XML format -- no root element");
			}

			fromXML(element);
		}
		break;

	case DataType::JSON_DATA:
		{
			// If here, determine if we have a JSON data string
			Json::Reader reader;
			Json::Value  root;
			bool         success = reader.parse(data, root);

			if (!success || !root)
			{
				throw GmsecException(MSG_ERROR, JSON_PARSE_ERROR, "Invalid JSON message format -- invalid JSON string data");
			}

			try
			{
				fromJSON(root);
			}
			catch (...)
			{
				throw GmsecException(MSG_ERROR, JSON_PARSE_ERROR, "Invalid JSON message format -- invalid JSON string data");
			}
		}
		break;

	case DataType::KEY_VALUE_DATA:
		{
			// If here, determine if we have a string containing key=value pairs
			std::vector<std::string> pairs;
			getKeyValuePairs(data, pairs);

			for (std::vector<std::string>::iterator it = pairs.begin(); it != pairs.end(); ++it)
			{
				std::string pair  = *it;
				size_t      equal = pair.find("=");

				addValue(pair.substr(0, equal).c_str(), pair.substr(equal + 1).c_str());
			}

			if (pairs.size() == 0)
			{
				throw GmsecException(CONFIG_ERROR, PARSE_ERROR, "Configuration data string does not contain key=value pair(s)");
			}
		}
		break;
	}

	m_configIter = m_configs.begin();
}


InternalConfig::InternalConfig(const InternalConfig& cfg)
	: m_configs(),
	  m_configName(cfg.m_configName),
	  m_configIter(),
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

	m_configIter = m_configs.begin();

	m_configName = "";
}


void InternalConfig::addValue(const char* name, const char* value)
{
	validateConfigName(name);

	if (!value)
	{
		throw GmsecException(CONFIG_ERROR, INVALID_CONFIG_VALUE, "Config value is NULL");
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
				Log::setReportingLevel(LogLevel::logERROR);
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
	else if (uc_name == GMSEC_IDENTIFY_HEADER_FIELDS)
	{
		if (StringUtil::stringEqualsIgnoreCase(value, "true"))
		{
			InternalField::identifyHeaderField(true);
		}
		else if (StringUtil::stringEqualsIgnoreCase(value, "false"))
		{
			InternalField::identifyHeaderField(false);
		}
		else
		{
			std::stringstream ss;
			ss << GMSEC_IDENTIFY_HEADER_FIELDS << " value must be either be true or false.";
			throw GmsecException(MSG_ERROR, UNUSED_CONFIG_ITEM, ss.str().c_str());
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

	m_configIter = m_configs.begin();

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
		throw GmsecException(CONFIG_ERROR, INVALID_CONFIG_NAME, oss.str().c_str());
	}

	const char* value = it->second.c_str();

	if (!StringUtil::stringEqualsIgnoreCase(value, "true") && !StringUtil::stringEqualsIgnoreCase(value, "false"))
	{
		std::ostringstream oss;
		oss << "Config entry '" << name << "' does not represent a boolean value";
		throw GmsecException(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
	}

	return StringUtil::stringEqualsIgnoreCase(value, "true");
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
			else
			{
				GMSEC_WARNING << "Config entry '" << name << "' does not represent a boolean value; returning default value";
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
		throw GmsecException(CONFIG_ERROR, INVALID_CONFIG_NAME, oss.str().c_str());
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
		throw GmsecException(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
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
		throw GmsecException(CONFIG_ERROR, INVALID_CONFIG_NAME, oss.str().c_str());
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
		throw GmsecException(CONFIG_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());
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

	if (m_configName.empty())
	{
		oss << "<CONFIG>" << "\n";
	}
	else
	{
		oss << "<CONFIG NAME=\"" << m_configName << "\">" << "\n";
	}

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
		throw GmsecException(CONFIG_ERROR, XML_PARSE_ERROR, "XML string cannot be NULL, nor empty");
	}
 
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError code = doc.Parse(xml);
 
	if (code != tinyxml2::XML_NO_ERROR)
	{
		throw GmsecException(CONFIG_ERROR, XML_PARSE_ERROR, code, "Invalid XML format -- parse error");
	}

	tinyxml2::XMLElement* element = doc.RootElement();

	if (!element)
	{
		throw GmsecException(CONFIG_ERROR, XML_PARSE_ERROR, "Invalid XML format -- no root element");
	}

	fromXML(element);
}


void InternalConfig::fromXML(tinyxml2::XMLElement* element)
{
	if (!element)
	{
		throw GmsecException(CONFIG_ERROR, XML_PARSE_ERROR, "Invalid XML -- element is NULL");
	}

	const char* mename = element->Name();
	if (!mename || !StringUtil::stringEqualsIgnoreCase(mename, "CONFIG"))
	{
		throw GmsecException(CONFIG_ERROR, XML_PARSE_ERROR, "Invalid XML -- CONFIG element not found");
	}

	clear();

	// check if configuration has a name
	const char* cfgName = element->Attribute("NAME");

	if (cfgName == NULL)
	{
		cfgName = element->Attribute("name");
	}
	if (cfgName != NULL)
	{
		m_configName = cfgName;
	}

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
				name = attr->Attribute("name");
			}

			if (name && value)
			{
				addValue(name, value);
			}
		}
	}
}


Config InternalConfig::getFromFile(const char* filename, const char* configName)
{
	if (!filename || *filename == '\0')
	{
		throw GmsecException(CONFIG_ERROR, UNINITIALIZED_OBJECT, "Configuration file name cannot be NULL, nor be an empty string");
	}

	std::ifstream ifs(filename, std::ios::in);

	if (ifs)
	{
		std::string data;
		std::string line;
		bool        keepLine = (configName == NULL || *configName == '\0');

		while (std::getline(ifs, line))
		{
			if (configName != NULL && !std::string(configName).empty())
			{
				if (line.find("NAME=\"" + std::string(configName) + "\"") != std::string::npos)
				{
					keepLine = true;
				}
			}

			if (keepLine)
			{
				data += line + "\n";
			}

			if (keepLine && line.find("</CONFIG>") != std::string::npos)
			{
				break;
			}
		}

		if (configName != NULL && !std::string(configName).empty() && data.empty())
		{
			throw GmsecException(CONFIG_ERROR, OTHER_ERROR_CODE, "Named configuration could not be found");
		}
		
		Config config;
		config.fromXML(data.c_str());
		return config;
	}

	throw GmsecException(CONFIG_ERROR, OTHER_ERROR_CODE, "Configuration file could not be found or opened");
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

		if (m_configName.empty())
		{
			oss << "\"CONFIG\":{\"PARAMETER\":[";
		}
		else
		{
			oss << "\"CONFIG\":{\"NAME\":\"" << m_configName << "\",\"PARAMETER\":[";
		}

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
		throw GmsecException(MSG_ERROR, JSON_PARSE_ERROR, "Invalid JSON Config format -- no CONFIG root element");
	}

	clear();

	// check if configuration has a name
	if (root.isMember("NAME") || root.isMember("name"))
	{
		m_configName = (root.isMember("NAME") ? root["NAME"].asCString() : root["name"].asCString());
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
        throw GmsecException(CONFIG_ERROR, INVALID_CONFIG_NAME, "Config name cannot be NULL, nor an empty string");
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
