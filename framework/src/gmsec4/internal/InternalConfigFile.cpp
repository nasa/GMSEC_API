/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalConfigFile.cpp
 *
 *  @brief This file contains a class for managing configuration files.
 *
 */

// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
//   Found in the Connection(Server s, const Options &o) constructor
#ifdef WIN32
#pragma warning(disable: 4355)
#endif


#include <gmsec4/internal/InternalConfigFile.h>

#include <gmsec4/internal/InternalConfigFileIterator.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Config.h>
#include <gmsec4/ConfigFileIterator.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/StdUniquePtr.h>

#include <tinyxml2.h>

#include <algorithm>
#include <cctype>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


InternalConfigFile::InternalConfigFile()
	: m_filePath(),
	  m_xmlDoc(0),
	  m_docLoaded(false),
	  m_xmlOutput(),
	  m_configNodes(),
	  m_msgNodes(),
	  m_subscriptionNodes(),
	  m_customNodes(),
	  m_aliasMap(),
	  m_iter(*this)
{
	// m_aliasMap is a map of some allowable alternatives to expected
	// XML keywords in a config file, used in private method
	// nameOrAliasMatches()).
	//
	m_aliasMap["subject"]        = "PATTERN";
	m_aliasMap["network"]        = "DEFINITIONS";
	m_aliasMap["gmsectransport"] = "DEFINITIONS";
	m_aliasMap["transport"]      = "DEFINITIONS";
}


InternalConfigFile::~InternalConfigFile()
{
	delete m_xmlDoc;
}


void InternalConfigFile::load(const char* filePath)
{
	if (m_xmlDoc || m_docLoaded)
	{
		clearDoc();

		delete m_xmlDoc;

		m_xmlDoc = NULL;
		m_docLoaded = false;
		m_filePath = "";
	}

	if (!filePath || *filePath == '\0')
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Invalid or missing config file path.");
	}

	m_filePath = std::string(filePath);

	// open & parse file
	m_xmlDoc = new tinyxml2::XMLDocument();
	if (!m_xmlDoc)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Failed to allocate XMLDocument");
	}

	tinyxml2::XMLError code = m_xmlDoc->LoadFile(m_filePath.c_str());

	if (code == tinyxml2::XML_NO_ERROR)
	{
		try {
			parseDoc();
		}
		catch (Exception& e) {
			delete m_xmlDoc;
			m_xmlDoc = NULL;
			throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, e.what());
		}

		m_docLoaded = true;
	}
	else
	{
		std::string error = "Error: ";

		switch (code)
		{
			case tinyxml2::XML_NO_ATTRIBUTE:                    error += "No attribute"; break;
			case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:            error += "Wrong attribute type"; break;
			case tinyxml2::XML_ERROR_FILE_NOT_FOUND:            error += "File not found"; break;
			case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:  error += "File could not be opened"; break;
			case tinyxml2::XML_ERROR_FILE_READ_ERROR:           error += "File could not be read"; break;
			case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:          error += "Element mismatch"; break;
			case tinyxml2::XML_ERROR_PARSING_ELEMENT:           error += "Cannot parse element"; break;
			case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:         error += "Cannot parse attribute"; break;
			case tinyxml2::XML_ERROR_IDENTIFYING_TAG:           error += "Cannot identify tag"; break;
			case tinyxml2::XML_ERROR_PARSING_TEXT:              error += "Cannot parse text"; break;
			case tinyxml2::XML_ERROR_PARSING_CDATA:             error += "Cannot parse CDATA"; break;
			case tinyxml2::XML_ERROR_PARSING_COMMENT:           error += "Cannot parse comment"; break;
			case tinyxml2::XML_ERROR_PARSING_DECLARATION:       error += "Cannot parse declaration"; break;
			case tinyxml2::XML_ERROR_PARSING_UNKNOWN:           error += "Unknown parsing issue"; break;
			case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:            error += "Empty document"; break;
			case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:        error += "Mismatched element"; break;
			case tinyxml2::XML_ERROR_PARSING:                   error += "Cannot parse file"; break;
			case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:            error += "Cannot convert text"; break;
			case tinyxml2::XML_NO_TEXT_NODE:                    error += "No text node"; break;
			default:                                            error += "Unknown cause"; break;
		}

		delete m_xmlDoc;
		m_xmlDoc = NULL;
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, code, error.c_str());
	}
}


void InternalConfigFile::save(const char* filePath, bool compact)
{
	if ((!filePath && m_filePath.empty()) || (filePath && std::string(filePath).empty()))
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Invalid config file path.");
	}

	// Initialize a new XML document, if one is not currently loaded
	initNewXMLDocument();

	const char* savePath = (filePath ? filePath : m_filePath.c_str());

	if (m_xmlDoc->SaveFile(savePath, compact) != tinyxml2::XML_NO_ERROR)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Failed to save XML file");
	}
}


void InternalConfigFile::fromXML(const char* xml)
{
	if (m_xmlDoc || m_docLoaded)
	{
		clearDoc();

		delete m_xmlDoc;

		m_xmlDoc = NULL;
		m_docLoaded = false;
		m_filePath = "";
	}

	if (!xml || *xml == '\0')
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Invalid XML string");
	}

	m_xmlDoc = new tinyxml2::XMLDocument();

	if (!m_xmlDoc)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Unable to allocate XML document");
	}

	tinyxml2::XMLError code = m_xmlDoc->Parse(xml);

	if (code == tinyxml2::XML_NO_ERROR)
	{
		parseDoc();
		m_docLoaded = true;
	}
	else
	{
		handleDocErrors(m_xmlDoc, "InternalConfigFile::fromXML");

		delete m_xmlDoc;
		m_xmlDoc = NULL;
		m_docLoaded = false;

		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Unable to parse XML string");
	}
}


const char* InternalConfigFile::toXML() const
{
	if (!m_xmlDoc)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "No ConfigFile loaded");
	}

	m_xmlOutput = elementToString(m_xmlDoc->RootElement());

	return m_xmlOutput.c_str();
}


bool InternalConfigFile::isLoaded() const
{
	return m_docLoaded;
}


Config InternalConfigFile::lookupConfig(const char* name) const
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Configuration name is NULL, or is an empty string");
	}

	NodeMap::const_iterator node = m_configNodes.find(name);

	if (node == m_configNodes.end())
	{
		std::string err_msg = "Configuration does not exist for the name ";

		err_msg += name;

		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE, err_msg.c_str());
	}

	Config cfg;
	cfg.fromXML(elementToString(node->second).c_str());

	return cfg;  // returns a copy
}


Message InternalConfigFile::lookupMessage(const char* name) const
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Message name cannot be NULL, nor an empty string");
	}

	NodeMap::const_iterator node = m_msgNodes.find(name);

	if (node == m_msgNodes.end())
	{
		std::string err_msg = "Message does not exist for the name ";

		err_msg += name;

		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE, err_msg.c_str());
	}

	return Message(elementToString(node->second).c_str());
}


const char* InternalConfigFile::lookupSubscription(const char* name) const
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Subscription name cannot be NULL, nor an empty string");
	}

	NodeMap::const_iterator node = m_subscriptionNodes.find(name);

	if (node == m_subscriptionNodes.end())
	{
		std::string err_msg = "Subscription does not exist for the name ";

		err_msg += name;

		GMSEC_WARNING << err_msg.c_str();

		return NULL;

	}

	return getStrAttr(node->second, "PATTERN");
}


const ConfigFile::SubscriptionEntry& InternalConfigFile::lookupSubscriptionEntry(const char* name)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Subscription name cannot be NULL, nor an empty string");
	}

	NodeMap::const_iterator node = m_subscriptionNodes.find(name);

	if (node == m_subscriptionNodes.end())
	{
		std::string err_msg = "Subscription does not exist for the name ";

		err_msg += name;

		GMSEC_WARNING << err_msg.c_str();

		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE, err_msg.c_str());
	}

	m_subEntry.reset(new ConfigFile::SubscriptionEntry());

	m_subEntry->setName(node->first.c_str());
	m_subEntry->setSubject(getStrAttr(node->second, "PATTERN"));
 
	for(tinyxml2::XMLElement* element = node->second->FirstChildElement("EXCLUDE");
	    element;
	    element = element->NextSiblingElement())
	{
		const char* exclude = getStrAttr(element, "PATTERN");
		if (exclude)
		{
			m_subEntry->addExcludedPattern(exclude);
		}
		else
		{
			GMSEC_WARNING << "InternalConfigFile::SubscriptionEntry:  A pattern element has no excluded subject attribute defined.";
		}
	}
 
	return *(m_subEntry.get());
}


void InternalConfigFile::addSubscription(const char* name, const char* subscription)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Subscription name cannot be NULL, nor an empty string");
	}

	if (!subscription || std::string(subscription).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			   "Subscription topic cannot be NULL, nor an empty string");
	}

	std::string pattern = StringUtil::stringToUpper(subscription);
	pattern = StringUtil::trim(pattern);

	// Check if we were given an XML statement (e.g. <SUBSCRIPTION PATTERN="GMSEC.FOO.BAR"/>)
	if (pattern[0] == '<')
	{
		StdUniquePtr<tinyxml2::XMLDocument> tmpDoc(new tinyxml2::XMLDocument());

		if (tmpDoc.get() == NULL)
		{
			throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR,
				"Failed to allocate an XML Document");
		}

		tinyxml2::XMLError code = tmpDoc->Parse(pattern.c_str()); 

		if (code != tinyxml2::XML_NO_ERROR)
		{
			throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR,
				"Subscription has invalid XML format - parse failed");
		}

		const char* value = getStrAttr(tmpDoc->RootElement(), "PATTERN");
		if (value)
		{
			pattern = value;
			pattern = StringUtil::trim(pattern);
		}
		else
		{
			value = getStrAttr(tmpDoc->RootElement(), "SUBJECT");
			if (value)
			{
				pattern = value;
				pattern = StringUtil::trim(pattern);
			}
			else
			{
				throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR,
					"Subscription is missing PATTERN or SUBJECT attribute - parse failed");
			}
		}

		if (pattern.empty())
		{
			throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
				"Subscription topic cannot be an empty string");
		}
	}

	// Initialize a new XML document, if one is not currently loaded
	initNewXMLDocument();

	tinyxml2::XMLElement* subElement = createSubscriptionElement(name, pattern.c_str());

	// Remove the old subscription (If it exists)
	if (m_subscriptionNodes.find(name) != m_subscriptionNodes.end())
	{
		removeSubscription(name);
	}

	// Replace the XML element in the array
	m_subscriptionNodes[name] = subElement;

	appendToDocument(subElement, XML_SUBSCRIPTION);
}


void InternalConfigFile::addConfig(const char* name, const Config& config)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE, "Configuration name cannot be NULL, nor an empty string");
	}

	// Initialize a new XML document, if one is not currently loaded
	initNewXMLDocument();

	tinyxml2::XMLElement* cfgElement = createConfigElement(name, config);

	// Remove the old config (If it exists)
	if (m_configNodes.find(name) != m_configNodes.end())
	{
		removeConfig(name);
	}

	// Replace the XML element in the array
	m_configNodes[name] = cfgElement;

	appendToDocument(cfgElement, XML_CONFIG);
}


void InternalConfigFile::addMessage(const char* name, const Message& message)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Message name cannot be NULL, nor an empty string");
	}

	StdUniquePtr<tinyxml2::XMLDocument> tmpDoc(new tinyxml2::XMLDocument());

	if (tmpDoc.get() == NULL)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Failed to allocate an XML Document");
	}

	tinyxml2::XMLError code = tmpDoc->Parse(message.toXML()); 

	if (code == tinyxml2::XML_NO_ERROR)
	{
		// Initialize a new XML document, if one is not currently loaded
		initNewXMLDocument();

		tinyxml2::XMLElement* msgElement = createMessageElement(tmpDoc.get(), name, message);

		// Remove the old message (If it exists)
		if (m_msgNodes.find(name) != m_msgNodes.end())
		{
			removeMessage(name);
		}

		// Replace the XML element in the array
		m_msgNodes[name] = msgElement;

		appendToDocument(msgElement, XML_MESSAGE);
	}
	else
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Message has invalid XML format - parse failed");
	}
}


void InternalConfigFile::addCustomXML(const char* xml)
{
	if (!xml || std::string(xml).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE, "XML string cannot be NULL, nor an empty string");
	}

	std::string xmlStr = StringUtil::stripWhiteSpace(xml);

	if (xmlStr[0] != '<' || xmlStr[xmlStr.length() - 1] != '>' || xmlStr[xmlStr.length() - 2] == '\\')
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR,
			"Invalid XML format - string does not start with < and end with an un-escaped >");
	}

	if (customXMLExists(xmlStr.c_str()))
	{
		// if it already exists, then there is no reason to insert it again.
		return;
	}

	StdUniquePtr<tinyxml2::XMLDocument> tmpDoc(new tinyxml2::XMLDocument());

	if (tmpDoc.get() == NULL)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Failed to allocate an XML Document");
	}

	// Parse the message's ToXML string to ensure that there are no errors
	tinyxml2::XMLError code = tmpDoc->Parse(xmlStr.c_str()); 

	if (code == tinyxml2::XML_NO_ERROR)
	{
		// Initialize a new XML document, if one is not currently loaded
		initNewXMLDocument();

		tinyxml2::XMLElement* customElement = createCustomElement(tmpDoc.get());

		const std::string elementStr = elementToString(customElement);

		// Remove the old custom element (If it exists)
		if (m_customNodes.find(elementStr) != m_customNodes.end())
		{
			removeCustomXML(elementStr.c_str());
		}

		m_customNodes[elementStr] = customElement;

		appendToDocument(customElement, XML_CUSTOM);
	}
	else
	{
		handleDocErrors(tmpDoc.get(), "InternalConfigFile::addCustomXML");

		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Custom XML has invalid - parse failed");
	}
}


bool InternalConfigFile::removeSubscription(const char* name)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Subscription name cannot be NULL, nor an empty string");
	}

	NodeMap::iterator node = m_subscriptionNodes.find(name);

	if (node == m_subscriptionNodes.end())
	{
		return false;
	}

	// Remove the element in fDoc
	tinyxml2::XMLElement* rootElement = m_xmlDoc->RootElement();

	if (rootElement)
	{
		rootElement->DeleteChild(node->second);
	}

	// Remove the element from the map
	m_subscriptionNodes.erase(name);

	return true;
}


bool InternalConfigFile::removeConfig(const char* name)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Configuration name cannot be NULL, nor an empty string");
	}

	NodeMap::iterator node = m_configNodes.find(name);

	if (node == m_configNodes.end())
	{
		return false;
	}

	// Remove the element in fDoc
	tinyxml2::XMLElement* rootElement = m_xmlDoc->RootElement();

	if (rootElement)
	{
		rootElement->DeleteChild(node->second);
	}

	// Remove the element from the map
	m_configNodes.erase(name);

	return true;
}


bool InternalConfigFile::removeMessage(const char* name)
{
	if (!name || std::string(name).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Message name cannot be NULL, nor an empty string");
	}

	NodeMap::iterator node = m_msgNodes.find(name);

	if (node == m_msgNodes.end())
	{
		return false;
	}

	// Remove the element in fDoc
	tinyxml2::XMLElement* rootElement = m_xmlDoc->RootElement();

	if (rootElement)
	{
		rootElement->DeleteChild(node->second);
	}

	// Remove the element from the map
	m_msgNodes.erase(name);

	return true;
}


bool InternalConfigFile::removeCustomXML(const char* xml)
{
	if (!xml || std::string(xml).empty())
	{
		throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE,
			"Custom XML string cannot be NULL, nor an empty string");
	}

	tinyxml2::XMLElement* customElement = getCustomXMLElement(xml);

	if (customElement)
	{
		const std::string elementStr = elementToString(customElement);

		// Remove the element from the XML File
		tinyxml2::XMLElement* rootElement = m_xmlDoc->RootElement();

		if (rootElement)
		{
			rootElement->DeleteChild(customElement);
		}

		m_customNodes.erase(elementStr.c_str());

		return true;
	}

	return false;
}


ConfigFileIterator& InternalConfigFile::getIterator()
{
	m_iter.m_iter->reset();
	return m_iter;
}


void InternalConfigFile::resetIterators()
{
	m_configNodesIter       = m_configNodes.begin();
	m_msgNodesIter          = m_msgNodes.begin();
	m_subscriptionNodesIter = m_subscriptionNodes.begin();
	m_customNodesIter       = m_customNodes.begin();
}


bool InternalConfigFile::hasNextConfig()
{
	return m_configNodesIter != m_configNodes.end();
}


bool InternalConfigFile::hasNextMessage()
{
	return m_msgNodesIter != m_msgNodes.end();
}


bool InternalConfigFile::hasNextSubscription()
{
	return m_subscriptionNodesIter != m_subscriptionNodes.end();
}


bool InternalConfigFile::hasNextCustomElement()
{
	return m_customNodesIter != m_customNodes.end();
}


ConfigFile::ConfigEntry InternalConfigFile::nextConfig()
{
	if (!hasNextConfig())
	{
		throw Exception(CONFIGFILE_ERROR, ITER_INVALID_NEXT, "No more Config entries");
	}

	ConfigFile::ConfigEntry entry;

	entry.setName(m_configNodesIter->first.c_str());
	entry.setConfig(Config(elementToString(m_configNodesIter->second).c_str()));

	++m_configNodesIter;

	return entry;
}


ConfigFile::MessageEntry InternalConfigFile::nextMessage()
{
	if (!hasNextMessage())
	{
		throw Exception(CONFIGFILE_ERROR, ITER_INVALID_NEXT, "No more Message entries");
	}

	ConfigFile::MessageEntry entry;

	entry.setName(m_msgNodesIter->first.c_str());
	entry.setMessage(Message(elementToString(m_msgNodesIter->second).c_str()));

	++m_msgNodesIter;

	return entry;
}


const ConfigFile::SubscriptionEntry& InternalConfigFile::nextSubscription()
{
	if (!hasNextSubscription())
	{
		throw Exception(CONFIGFILE_ERROR, ITER_INVALID_NEXT, "No more Subscription entries");
	}

	m_subEntry.reset(new ConfigFile::SubscriptionEntry());

	std::string tmpSubject = getStrAttr(m_subscriptionNodesIter->second, "PATTERN");

	m_subEntry->setName(m_subscriptionNodesIter->first.c_str());
	m_subEntry->setSubject(tmpSubject.c_str());

	for(tinyxml2::XMLElement* element = m_subscriptionNodesIter->second->FirstChildElement("EXCLUDE");
		element;
		element = element->NextSiblingElement())
	{
		const char* exclude = getStrAttr(element, "PATTERN");

		if (exclude)
		{
			m_subEntry->addExcludedPattern(exclude);
		}
		else
		{
			GMSEC_WARNING << "InternalConfigFile::SubscriptionEntry:  A pattern element has no excluded subject attribute defined.";
		}
	}

	++m_subscriptionNodesIter;

	return *(m_subEntry.get());
}


const char* InternalConfigFile::nextCustomElement()
{
	if (!hasNextCustomElement())
	{
		throw Exception(CONFIGFILE_ERROR, ITER_INVALID_NEXT, "No more Custom Element entries");
	}

	const char* entry = m_customNodesIter->first.c_str();

	++m_customNodesIter;

	return entry;
}


bool InternalConfigFile::nameOrAliasMatches(const char* searchName, const char* name) const
{
	// Private method nameOrAliasMatches() identifies whether
	// or not parameter "searchName" can be treated as the
	// equivalent of the token specified by parameter "name"
	// in an XML config file... e.g. "SUBJECT" can be used
	// as an equivalent to "PATTERN" in a SUBSCRIPTION element.
	//
	// It makes use of the private data member m_aliasMap.
	//
	// Note:  In the current implementation, there is no effort
	//        to break down where certain searchNames should
	//        occur within a an XML parsing tree, such as
	//        specifically in attributes, element names, etc...
	//
	if (!StringUtil::stringEqualsIgnoreCase(searchName, name))
	{
		std::string lowerSearchName = searchName;
		std::transform(lowerSearchName.begin(), lowerSearchName.end(), lowerSearchName.begin(), ::tolower);

		AliasMap::const_iterator it = m_aliasMap.find(lowerSearchName);

		if (it == m_aliasMap.end()) // no match found
		{
			return false;
		}

		// Match found for key; need to check if the found value matches as well.
		if (it->second != name)
		{
			return false;
		}
	}

	return true;
}


void InternalConfigFile::parseDoc()
{
	tinyxml2::XMLElement* root = m_xmlDoc->RootElement();

	if (!root)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Invalid XML document - no root element");
	}

	/* verify that our root node is named "DEFINITIONS" */
	bool missingDefs = false;
	const char* rename = root->Name();
	if (!rename)
	{
		missingDefs = true;
	}
	else if (!nameOrAliasMatches(rename, "DEFINITIONS"))
	{
		missingDefs = true;
	}

	if (missingDefs)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR,
		           "Invalid XML document - missing DEFINITIONS");
	}

	// read configs & messages
	// fields (child nodes)
	for (tinyxml2::XMLElement* element = root->FirstChildElement();
			element;
			element = element->NextSiblingElement())
	{
		//CONFIG
		const char* cname = element->Name();
		if (cname && StringUtil::stringEqualsIgnoreCase(cname, "CONFIG"))
		{
			const char* name = getNameAttr(element);
			if (name)
			{
				m_configNodes[name] = element;
			}
			else
			{
				GMSEC_WARNING << "InternalConfigFile::parseDoc :  A config element has no name attribute defined.";
			}
		}

		// MESSAGE
		else if (cname && StringUtil::stringEqualsIgnoreCase(cname, "MESSAGE"))
		{
			const char* name = getNameAttr(element);
			if (name)
			{
				m_msgNodes[name] = element;
			}
			else
			{
				GMSEC_WARNING << "InternalConfigFile::parseDoc :  A message element has no name attribute defined.";
			}
		}

		// SUBSCRIPTION
		else if (cname && StringUtil::stringEqualsIgnoreCase(cname, "SUBSCRIPTION"))
		{
			const char* name = getNameAttr(element);
			if (name)
			{
				const char* pattern = getStrAttr(element, "PATTERN");
				if (pattern)
				{
					m_subscriptionNodes[name] = element;
				}
				else
				{
					std::string err_msg = "InternalConfigFile::parseDoc :  Subscription element ";

					err_msg += name;
					err_msg += " has no PATTERN (or SUBJECT) defined.";

					GMSEC_WARNING << err_msg.c_str();
				}
			}
			else
			{
				GMSEC_WARNING << "InternalConfigFile::parseDoc :  A subscription element has no name attribute defined.";

			}
		}

		// CUSTOM
		else
		{
			m_customNodes[elementToString(element)] = element;
		}
	}
}


void InternalConfigFile::clearDoc()
{
	m_configNodes.clear();
	m_msgNodes.clear();
	m_subscriptionNodes.clear();
	m_customNodes.clear();
}


void InternalConfigFile::appendToDocument(tinyxml2::XMLElement* element, XmlType type)
{
	bool success = false;

	tinyxml2::XMLElement* rootElement = m_xmlDoc->RootElement();

	if (!rootElement)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Invalid XML document - no root element");
	}

	// Hierarchy of XML insertion (Imagine that you are reading the XML document from the bottom up
	// as you read this code from the top to bottom)
	switch (type)
	{
		case XML_CUSTOM:
		case XML_MESSAGE:
			if (!success)
			{
				// Add the new custom XML definition after the last MESSAGE element
				tinyxml2::XMLElement* lastMsgElement = rootElement->LastChildElement("MESSAGE");
				if (lastMsgElement != NULL)
				{
					rootElement->InsertAfterChild(lastMsgElement, element);
					success = true;
				}
			}
			// If MESSAGE elements exist, place it after the last CONFIG element

		case XML_CONFIG:
			if (!success)
			{
				tinyxml2::XMLElement* lastCfgElement = rootElement->LastChildElement("CONFIG");
				if (lastCfgElement != NULL)
				{
					rootElement->InsertAfterChild(lastCfgElement, element);
					success = true;
				}
			}
			// If no CONFIG elements exist, place it after the last SUBSCRIPTION element

		case XML_SUBSCRIPTION:
			if (!success)
			{
				tinyxml2::XMLElement* lastSubElement = rootElement->LastChildElement("SUBSCRIPTION");
				if (lastSubElement != NULL)
				{
					rootElement->InsertAfterChild(lastSubElement, element);
				}
				// If no SUBSCRIPTION elements exist, place it at the top of the children
				else
				{
					rootElement->InsertFirstChild(element);
				}
			}
	}
}


std::string InternalConfigFile::elementToString(tinyxml2::XMLElement* element) const
{
	if (!element)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "XML element is NULL");
	}

	tinyxml2::XMLPrinter printer;

	element->Accept(&printer);

	// If the last character in the printer string is a newline character, additional
	// subtraction is necessary.  The buffer may or may not have a newline character,
	// depending on the current tinyxml2 document settings (ie. compact == true).
	std::string elementStr(printer.CStr(), printer.CStrSize() - 1);

	// If the last character in the printer string is a newline character, additional
	// subtraction is necessary.  The buffer may or may not have a newline character,
	// depending on the current tinyxml2 document settings (ie. compact == true).
	const size_t actualSize = elementStr.size() - (elementStr[elementStr.size() - 1] == '\n' ? 1 : 0);

	return elementStr.substr(0, actualSize);
}


void InternalConfigFile::handleDocErrors(tinyxml2::XMLDocument* doc, const char* methodName) const
{
	const char* err1 = doc->GetErrorStr1();
	const char* err2 = doc->GetErrorStr2();

	if (!err1) err1 = "unknown";
	if (!err2) err2 = "unknown";

	GMSEC_WARNING << methodName << " : tiny2xml2::XMLDocument Error #1: " << err1;
	GMSEC_WARNING << methodName << " : tiny2xml2::XMLDocument Error #2: " << err2;
}


tinyxml2::XMLElement* InternalConfigFile::getCustomXMLElement(const char* xml) const
{
	if (!xml)
	{
		return NULL;
	}

	std::string xmlStr = StringUtil::stripWhiteSpace(xml);

	// Ensure that the input is a proper, full XML element
	if (xmlStr[0] != '<' || xmlStr[xmlStr.length() - 1] != '>' || xmlStr[xmlStr.length() - 2] == '\\')
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR,
			"Invalid XML format - string does not start with < and end with an un-escaped >");
	}

	tinyxml2::XMLDocument* tmpDoc = new tinyxml2::XMLDocument();

	if (tmpDoc->Parse(xmlStr.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		delete tmpDoc;

		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR,
			"Invalid XML format - unable to parse");
	}

	std::string tmpXML = elementToString(tmpDoc->RootElement());

	tinyxml2::XMLElement* customXMLElement = NULL;

	for (NodeMap::const_iterator it = m_customNodes.begin(); it != m_customNodes.end(); ++it)
	{
		std::string elementStr = elementToString(it->second);

		if (elementStr == tmpXML)
		{
			customXMLElement = it->second;
			break;
		}
	}

	delete tmpDoc;

	return customXMLElement;
}


bool InternalConfigFile::customXMLExists(const char* xml) const
{
	return getCustomXMLElement(xml) != NULL;
}


const char* InternalConfigFile::getNameAttr(tinyxml2::XMLElement* node) const
{
	return getStrAttr(node, "NAME");
}


const char* InternalConfigFile::getStrAttr(tinyxml2::XMLElement* node, const char* field) const
{
	const tinyxml2::XMLAttribute* attr = NULL;

	for (attr = node->FirstAttribute(); attr; attr = attr->Next())
	{
		const char* caname = attr->Name();

		if (caname && nameOrAliasMatches(caname, field))
		{
			return attr->Value();
		}
	}

	return NULL;
}


void InternalConfigFile::initNewXMLDocument()
{
	// Initialize the XML document if it has not already been loaded
	if (!m_xmlDoc)
	{
		m_xmlDoc = new tinyxml2::XMLDocument();

		if (!m_xmlDoc)
		{
			throw Exception(CONFIGFILE_ERROR, OUT_OF_MEMORY,
				"Failed to allocate new XML Document");
		}

		// Create and insert a new XML Declaration
		m_xmlDoc->InsertEndChild(m_xmlDoc->NewDeclaration(NULL));

		// Create a root element for the document
		m_xmlDoc->InsertEndChild(m_xmlDoc->NewElement("DEFINITIONS"));
	}
}


tinyxml2::XMLElement* InternalConfigFile::createSubscriptionElement(const char* name, const char* pattern)
{
	tinyxml2::XMLElement* subElement = m_xmlDoc->NewElement("SUBSCRIPTION");

	subElement->SetAttribute("NAME", name);
	subElement->SetAttribute("PATTERN", pattern);

	return subElement;
}


tinyxml2::XMLElement* InternalConfigFile::createConfigElement(const char* name, const Config& cfg)
{
	tinyxml2::XMLElement* cfgElement = m_xmlDoc->NewElement("CONFIG");

	if (name)
	{
		cfgElement->SetAttribute("NAME", name);
	}

	const char* cfgName  = 0;
	const char* cfgValue = 0;
	bool        havePair = cfg.getFirst(cfgName, cfgValue);

	while (havePair)
	{
		tinyxml2::XMLElement* paramElement = m_xmlDoc->NewElement("PARAMETER");
		tinyxml2::XMLText*    paramText    = m_xmlDoc->NewText(cfgValue);

		paramElement->SetAttribute("NAME", cfgName);
		paramElement->InsertEndChild(paramText);

		cfgElement->InsertEndChild(paramElement);

		havePair = cfg.getNext(cfgName, cfgValue);
	}

	return cfgElement;
}


tinyxml2::XMLElement* InternalConfigFile::createMessageElement(tinyxml2::XMLDocument* doc, const char* name, const Message& msg)
{
	if (!doc)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "XML document is NULL");
	}

	tinyxml2::XMLElement* rootElement = doc->RootElement();
	if (!rootElement)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Invalid XML document - no root element");
	}


	tinyxml2::XMLElement* msgElement = m_xmlDoc->NewElement("MESSAGE");

	// Add NAME, SUBJECT and TYPE attributes
	msgElement->SetAttribute("NAME", name);
	msgElement->SetAttribute("SUBJECT", msg.getSubject());

	const char* kind = NULL;
	switch (msg.getKind())
	{
		case Message::PUBLISH: kind = "PUBLISH"; break;
		case Message::REQUEST: kind = "REQUEST"; break;
		case Message::REPLY:   kind = "REPLY";   break;
	}
	msgElement->SetAttribute("TYPE", kind);


	// Check if we have a config
	tinyxml2::XMLElement* tmpElement = rootElement->FirstChildElement("CONFIG");
	if (tmpElement)
	{
		tinyxml2::XMLElement* cfgElement = createConfigElement(tmpElement->Name(), msg.getConfig());
		msgElement->InsertEndChild(cfgElement);
	}


	// Check if we have fields
	for (tmpElement = rootElement->FirstChildElement("FIELD"); tmpElement; tmpElement = tmpElement->NextSiblingElement())
	{
		const char* fldType  = tmpElement->Attribute("TYPE");
		const char* fldName  = tmpElement->Attribute("NAME");
		const char* fldBits  = tmpElement->Attribute("BITS");
		const char* fldHead  = tmpElement->Attribute("HEAD");
		const char* fldValue = tmpElement->GetText();

		tinyxml2::XMLElement* fldElement = m_xmlDoc->NewElement("FIELD");

		fldElement->SetAttribute("TYPE", fldType);
		fldElement->SetAttribute("NAME", fldName);

		if (fldBits)
		{
			fldElement->SetAttribute("BITS", fldBits);
		}

		if (fldHead)
		{
			fldElement->SetAttribute("HEAD", fldHead);
		}

		tinyxml2::XMLText* fldText = m_xmlDoc->NewText(fldValue);
		fldElement->InsertEndChild(fldText);

		msgElement->InsertEndChild(fldElement);
	}

	return msgElement;
}


tinyxml2::XMLElement* InternalConfigFile::createCustomElement(tinyxml2::XMLDocument* doc)
{
	// Grab the root element from the temporary document; use it to define our custom element
	tinyxml2::XMLElement* rootElement = doc->RootElement();

	if (!rootElement)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Invalid XML document - no root element");
	}

	tinyxml2::XMLElement* customElement = rootElement->ShallowClone(m_xmlDoc)->ToElement();

	if (!customElement)
	{
		throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Unable to create shallow clone of XML Document element");
	}

	for (tinyxml2::XMLNode* child = rootElement->FirstChild(); child != NULL; child = child->NextSibling())
	{
		tinyxml2::XMLNode* childClone = child->ShallowClone(m_xmlDoc);

		if (!childClone)
		{
			throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Unable to create Shallow Clone of XML Document");
		}

		tinyxml2::XMLElement* childElement = child->ToElement();

		if (!childElement)
		{
			throw Exception(CONFIGFILE_ERROR, XML_PARSE_ERROR, "Unable to reference XML Element from XML Document Child");
		}

		const char* text = childElement->GetText();

		if (text)
		{
			tinyxml2::XMLText* childText = m_xmlDoc->NewText(text);

			childClone->InsertEndChild(childText);
		}

		customElement->InsertEndChild(childClone);
	}

	return customElement;
}


// InternalConfigEntry
//
InternalConfigEntry::InternalConfigEntry()
	: m_name(),
	  m_config()
{
}


InternalConfigEntry::InternalConfigEntry(const InternalConfigEntry& other)
	: m_name(other.m_name),
	  m_config(other.m_config)
{
}


InternalConfigEntry::~InternalConfigEntry()
{
}


InternalConfigEntry& InternalConfigEntry::operator=(const InternalConfigEntry& other)
{
	if (this != &other)
	{
		m_name   = other.m_name;
		m_config = other.m_config;
	}

	return *this;
}


const char* InternalConfigEntry::getName() const
{
	return m_name.c_str();
}


const Config& InternalConfigEntry::getConfig() const
{
	return m_config;
}


void InternalConfigEntry::setName(const char* name)
{
	m_name = name;
}


void InternalConfigEntry::setConfig(const Config& cfg)
{
	m_config = cfg;
}


// InternalMessageEntry
//
InternalMessageEntry::InternalMessageEntry()
	: m_name(),
	  m_message("BOGUS", Message::PUBLISH)
{
}


InternalMessageEntry::InternalMessageEntry(const InternalMessageEntry& other)
	: m_name(other.m_name),
	  m_message(other.m_message)
{
}


InternalMessageEntry::~InternalMessageEntry()
{
}


InternalMessageEntry& InternalMessageEntry::operator=(const InternalMessageEntry& other)
{
	if (this != &other)
	{
		m_name    = other.m_name;
		m_message = other.m_message;
	}

	return *this;
}


const char* InternalMessageEntry::getName() const
{
	return m_name.c_str();
}


const Message& InternalMessageEntry::getMessage() const
{
	return m_message;
}


void InternalMessageEntry::setName(const char* name)
{
	m_name = name;
}


void InternalMessageEntry::setMessage(const Message& msg)
{
	m_message = msg;
}


// InternalSubscriptionEntry
//
InternalSubscriptionEntry::InternalSubscriptionEntry()
	: m_name(),
	  m_pattern(),
	  m_excludedPatterns(),
	  m_excludedPatternsIter(m_excludedPatterns.begin())
{
}


InternalSubscriptionEntry::InternalSubscriptionEntry(const InternalSubscriptionEntry& other)
	: m_name(other.m_name),
	  m_pattern(other.m_pattern),
	  m_excludedPatterns(other.m_excludedPatterns),
	  m_excludedPatternsIter(m_excludedPatterns.begin())
{
}


InternalSubscriptionEntry::~InternalSubscriptionEntry()
{
}


InternalSubscriptionEntry& InternalSubscriptionEntry::operator=(const InternalSubscriptionEntry& other)
{
	if (this != &other)
	{
		m_name                 = other.m_name;
		m_pattern              = other.m_pattern;
		m_excludedPatterns     = other.m_excludedPatterns;
		m_excludedPatternsIter = m_excludedPatterns.begin();
	}

	return *this;
}


const char* InternalSubscriptionEntry::getName() const
{
	return m_name.c_str();
}


const char* InternalSubscriptionEntry::getPattern() const
{
	return m_pattern.c_str();
}


void InternalSubscriptionEntry::setName(const char* name)
{
	m_name = name;
}


void InternalSubscriptionEntry::setPattern(const char* pattern)
{
	m_pattern = pattern;
}


bool InternalSubscriptionEntry::hasNextExcludedPattern() const
{
	// We need to ensure that non-const iterators are used when performing the
	// check of whether we have another excluded pattern or not.
	// This nonsense is needed to workaround a compiler limitation on Solaris.

	InternalSubscriptionEntry& safe = const_cast<InternalSubscriptionEntry&>(*this);

	return safe.m_excludedPatternsIter != safe.m_excludedPatterns.end();
}


const char* InternalSubscriptionEntry::nextExcludedPattern() const
{
	if (!hasNextExcludedPattern())
	{
		throw Exception(CONFIGFILE_ERROR, ITER_INVALID_NEXT, "No more Excluded Subject entries");
	}

	const char* pattern = m_excludedPatternsIter->c_str();
        
	++m_excludedPatternsIter;

	return pattern;
}


void InternalSubscriptionEntry::addExcludedPattern(const char* pattern)
{
	m_excludedPatterns.push_back(pattern);	

	if (m_excludedPatterns.size() == 1)
	{
		m_excludedPatternsIter = m_excludedPatterns.begin();
	}
}
