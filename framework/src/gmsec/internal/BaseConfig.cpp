
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file BaseConfig.cpp
 *
 *  General string collection class for configuration item support.
 *
*/

#include <gmsec/internal/BaseConfig.h>
#include <gmsec/Config.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/internal/Log.h>
#include <tinyxml2.h>

#include <iostream>
#include <fstream>

using std::string;
using std::ostringstream;
using std::endl;

using gmsec::Status;
using namespace gmsec::internal;
using namespace gmsec::util;


BaseConfig::BaseConfig()
{
}


BaseConfig::BaseConfig(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		string tmp(argv[i]);
		/* determine if this is a key=value pair */
		size_t t = tmp.find("=");
		if (t != string::npos)
		{
			string key   = tmp.substr(0,t);
			string value = tmp.substr(t+1);

			if (key.length() > 0 && value.length() > 0)
			{
				AddValue(key.c_str(),value.c_str());
			}
		}
	}

	fCfgItr = fConfigs.begin();
}


BaseConfig::BaseConfig(const char *xml)
{
	// parse xml
	FromXML(xml);
	// ensure our iterator is initialized
	fCfgItr = fConfigs.begin();
}


BaseConfig::BaseConfig(const BaseConfig &cfg)
{
	Status result;
	const char *name = 0, *value = 0;

	result = cfg.GetFirst(name,value);
	while (GMSEC_STATUS_NO_ERROR == result.GetClass())
	{
		AddValue(name,value);
		result = cfg.GetNext(name,value);
	}

	fCfgItr = fConfigs.end();
}


BaseConfig::~BaseConfig()
{
	Clear();
}



BaseConfig *BaseConfig::getInternal(gmsec::Config *external)
{
	return external->ptr;
}


Status BaseConfig::Clear()
{
	Status result;

	fConfigs.clear();

	return result;
}


Status BaseConfig::AddValue(const char *name, const char *value)
{
	Status result;

	if (NULL == name)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_INVALID_CONFIG_NAME,
		           "Config name is NULL");
		LOG_WARNING << "BaseConfig::AddValue : " << result.Get();
		return result;
	}

	if (NULL == value)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_INVALID_CONFIG_NAME,
		           "Config value is NULL");
		LOG_WARNING << "BaseConfig::AddValue : " << result.Get();
		return result;
	}

	fConfigs[name] = string(value);

	// special handling of log parameters
	string lower = stringToLower(name);
	if (lower == "loglevel")
	{
		LogLevel level = Log::FromString(value);
		Log::SetReportingLevel(level);
	}
	else if (lower == "logfile")
	{
		std::ostream * out = 0;
		if (stringEqualsIgnoreCase(value, "STDOUT"))
			out = &std::cout;
		else if (stringEqualsIgnoreCase(value, "STDERR"))
			out = &std::cerr;
		else
			out = new std::ofstream(value, std::ios::app);
		Log::SetDefaultStream(out);
	}

	return result;
}

Status BaseConfig::ClearValue(const char *name)
{
	Status result;

	if (NULL == name)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_INVALID_CONFIG_NAME,
		           "Config name is NULL");
		LOG_WARNING << "BaseConfig::ClearValue : " << result.Get();
		return result;
	}

	fConfigs.erase(name);

	return result;
}


Status BaseConfig::GetValue(const char *name, const char *&value) const
{
	Status result;

	value = NULL;

	if (NULL == name)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_INVALID_CONFIG_NAME,
		           "Config name is NULL");
		LOG_WARNING << "BaseConfig::GetValue : " << result.Get();
		return result;
	}

	std::map<std::string, std::string, ci_less>::const_iterator tmpItr = fConfigs.find(name);
	if (tmpItr == fConfigs.end())
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_INVALID_CONFIG_VALUE,
		           "Config value not set");
		// This function is a query, so it is not appropriate
		// to generate a warning if a config value does not exist.
		LOG_DEBUG << "BaseConfig::GetValue(" << name << ") : " << result.Get();
	}
	else
	{
		value = tmpItr->second.c_str();
		if (value == NULL || stringLength(value) == 0)
		{
			result.Set(GMSEC_STATUS_CONFIG_ERROR,
			           GMSEC_INVALID_CONFIG_VALUE,
			           "Config value exists but empty");
			LOG_WARNING << "BaseConfig::GetValue(" << name << ") : " << result.Get();
		}
		else
		{
			// It is unacceptable to log all values since some may be secret.
			// A utility function
			//	GetValueDebug(Config &config, const char *name, const char *&value))
			// can serve this purpose (or a ConfigHelper class which also deals
			// with prefixes and memory).
			LOG_DEBUG << "BaseConfig::GetValue(" << name << ") : specified";
		}
	}

	return result;
}

bool BaseConfig::CheckBoolValue(const char *name, bool defaultValue) const
{
	if (name != NULL)
	{
		const char *tmp = NULL;
		this->GetValue(name,tmp);
		if (tmp != NULL)
		{
			if (stringEqualsIgnoreCase(tmp, "false"))
				return false;
			else
				return true;
		}
	}
	return defaultValue;

}


Status BaseConfig::GetFirst(const char *&name, const char *&value) const
{
	Status result;

	// These are const methods, but we need to set the value of CfgItr
	conBaseConfigMap *tmpBaseConfigs = &(((BaseConfig *)this)->fConfigs);
	configItr *tmpCfgItr = &(((BaseConfig *)this)->fCfgItr);

	if (!(*tmpBaseConfigs).empty())
	{
		(*tmpCfgItr) = (*tmpBaseConfigs).begin();
	}
	else
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_CONFIG_END_REACHED,
		           "Config is Empty");
		LOG_DEBUG << "BaseConfig::GetFirst : " << result.Get();
		return result;
	}

	return GetNext(name,value);
}

Status BaseConfig::GetNext(const char *&name, const char *&value) const
{

	Status result;

	// These are const methods, but we need to set the value of CfgItr
	conBaseConfigMap *tmpBaseConfigs = &(((BaseConfig *)this)->fConfigs);
	configItr *tmpCfgItr = &(((BaseConfig *)this)->fCfgItr);

	// Grab the values for name and value
	if ((*tmpCfgItr) == (*tmpBaseConfigs).end())
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_CONFIG_END_REACHED,
		           "End of config items reached");
	}
	else
	{
		// Grab the values for name and value
		name = (*(*tmpCfgItr)).first.c_str();
		value = (*(*tmpCfgItr)).second.c_str();
		(*tmpCfgItr)++;
	}

	return result;
}

Status BaseConfig::ToXML(const char *&xml)
{
	Status result;

	ostringstream strm;

	strm << "<CONFIG>" << endl;

	const char *name, *value;
	Status res = GetFirst(name,value);
	while (!res.isError())
	{
		strm << "\t" << "<PARAMETER NAME=\"" << name << "\">" << value << "</PARAMETER>" << endl;
		res = GetNext(name,value);
	}

	strm << "</CONFIG>" << endl;

	fXmlOutput = strm.rdbuf()->str();
	xml = fXmlOutput.c_str();

	return result;
}


Status BaseConfig::FromXML(tinyxml2::XMLElement *element)
{
	Status result;

	if (element == NULL)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml config format");
		LOG_WARNING << "BaseConfig::FromXML : " << result.Get();
		return result;
	}

	/* verify that our root node is "CONFIG" */
	const char *mename = element->Name();
	if (mename == NULL || !stringEquals(mename, "CONFIG"))
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml config format");
		LOG_WARNING << "BaseConfig::FromXML : " << result.Get();
		return result;
	}

	// fields (child nodes)
	tinyxml2::XMLElement *cur_node = NULL;
	for (cur_node = element->FirstChildElement(); cur_node; cur_node = cur_node->NextSiblingElement())
	{
		//field nodes only
		const char *cnname = cur_node->Name();
		if (cnname != NULL && stringEquals(cnname, "PARAMETER"))
		{
			const char *name = cur_node->Attribute("NAME");
			// consider cur_node TiXMLText(cur_node)
			const char *value = cur_node->GetText();

			if (name != NULL && value != NULL)
			{
				AddValue(name,value);
			}
		}
	}

	return result;
}


Status BaseConfig::FromXML(const char *xml)
{
	Status result;

	tinyxml2::XMLDocument doc;

	if (xml == NULL)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml config format - NULL pointer");
		LOG_WARNING << "BaseConfig::FromXML : " << result.Get();
		return result;
	}

	int code = doc.Parse(xml);
	if (code != 0)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml config format");
		result.SetCustomCode(code);
		LOG_WARNING << "BaseConfig::FromXML : " << result.Get();
		return result;
	}

	/* get root message node */
	tinyxml2::XMLElement *element = doc.RootElement();
	if (element == NULL)
	{
		result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml config format - no root element");
		LOG_WARNING << "BaseConfig::FromXML : " << result.Get();
		return result;
	}

	result = FromXML(element);

	return result;
}

