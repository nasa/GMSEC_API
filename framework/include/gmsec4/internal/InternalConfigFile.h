/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalConfigFile.h
 *
 *  @brief This file contains a class for managing configuration files.
 *
 */

#ifndef GMSEC_API_INTERNAL_CONFIGFILE_H
#define GMSEC_API_INTERNAL_CONFIGFILE_H

#include <gmsec4/internal/ci_less.h>
#include <gmsec4/internal/tinyxml2.h>

#include <gmsec4/Config.h>
#include <gmsec4/ConfigFile.h>
#include <gmsec4/ConfigFileIterator.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/wdllexp.h>

#include <map>
#include <string>
#include <list>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalConfigFile
{
public:
	InternalConfigFile();


	~InternalConfigFile();


	void CALL_TYPE load(const char* filepath);


	void CALL_TYPE save(const char* filePath = NULL, bool compact = false);


	void CALL_TYPE fromXML(const char* xml);


	const char* CALL_TYPE toXML() const;


	bool CALL_TYPE isLoaded() const;


	Config CALL_TYPE lookupConfig(const char* name) const;


	Message CALL_TYPE lookupMessage(const char* name) const;


	const char* CALL_TYPE lookupSubscription(const char* name) const;


	const ConfigFile::SubscriptionEntry& lookupSubscriptionEntry(const char* name);


	void CALL_TYPE addSubscription(const char* name, const char* subscription);


	void CALL_TYPE addConfig(const char* name, const Config& config);


	void CALL_TYPE addMessage(const char* name, const Message& message);


	void CALL_TYPE addCustomXML(const char* xml);


	bool CALL_TYPE removeSubscription(const char* name);


	bool CALL_TYPE removeConfig(const char* name);


	bool CALL_TYPE removeMessage(const char* name);


	bool CALL_TYPE removeCustomXML(const char* xml);


	ConfigFileIterator& CALL_TYPE getIterator();


	void resetIterators();


	bool hasNextConfig();


	bool hasNextMessage();


	bool hasNextSubscription();


	bool hasNextCustomElement();


	ConfigFile::ConfigEntry nextConfig();


	ConfigFile::MessageEntry nextMessage();


	const ConfigFile::SubscriptionEntry& nextSubscription();


	const char* nextCustomElement();


private:
	bool CALL_TYPE nameOrAliasMatches(const char* searchName, const char* name) const;

	// defined, but not implemented
	InternalConfigFile(const InternalConfigFile&);
	InternalConfigFile& operator=(const InternalConfigFile&);

	enum XmlType { XML_SUBSCRIPTION, XML_CONFIG, XML_MESSAGE, XML_CUSTOM };

	void CALL_TYPE parseDoc();

	void CALL_TYPE clearDoc();

	void CALL_TYPE appendToDocument(tinyxml2::XMLElement* element, XmlType type);

	std::string CALL_TYPE elementToString(tinyxml2::XMLElement* element) const;

	void CALL_TYPE handleDocErrors(tinyxml2::XMLDocument* doc, const char* methodName) const;

	tinyxml2::XMLElement* CALL_TYPE getCustomXMLElement(const char* xml) const;

	bool CALL_TYPE customXMLExists(const char* xml) const;

	const char* CALL_TYPE getNameAttr(tinyxml2::XMLElement* node) const;

	const char* CALL_TYPE getStrAttr(tinyxml2::XMLElement* node, const char* field) const;

	void CALL_TYPE initNewXMLDocument();

	tinyxml2::XMLElement* CALL_TYPE createSubscriptionElement(const char* name, const char* pattern);
	tinyxml2::XMLElement* CALL_TYPE createConfigElement(const char* name, const Config& cfg);
	tinyxml2::XMLElement* CALL_TYPE createMessageElement(tinyxml2::XMLDocument* doc, const char* name, const Message& msg);
	tinyxml2::XMLElement* CALL_TYPE createCustomElement(tinyxml2::XMLDocument* doc);


	// private data members
	typedef std::map<std::string, tinyxml2::XMLElement*, ci_less> NodeMap;
	typedef std::map<std::string, std::string>                    AliasMap;
	typedef util::StdSharedPtr<ConfigFile::SubscriptionEntry>     SubEntry;

	std::string            m_filePath;
	tinyxml2::XMLDocument* m_xmlDoc;
	bool                   m_docLoaded;
	mutable std::string    m_xmlOutput;

	NodeMap                m_configNodes;
	NodeMap                m_msgNodes;
	NodeMap                m_subscriptionNodes;
	NodeMap                m_customNodes;

	NodeMap::iterator      m_configNodesIter;
	NodeMap::iterator      m_msgNodesIter;
	NodeMap::iterator      m_subscriptionNodesIter;
	NodeMap::iterator      m_customNodesIter;

	AliasMap               m_aliasMap;

	ConfigFileIterator     m_iter;

	SubEntry               m_subEntry;
};


class InternalConfigEntry
{
public:
	InternalConfigEntry();
	InternalConfigEntry(const InternalConfigEntry& other);
	~InternalConfigEntry();
	InternalConfigEntry& operator=(const InternalConfigEntry& other);

	const char* getName() const;
	const Config& getConfig() const;

	void setName(const char* name);
	void setConfig(const Config& cfg);

private:
	std::string m_name;
	Config      m_config;
};


class InternalMessageEntry
{
public:
	InternalMessageEntry();
	InternalMessageEntry(const InternalMessageEntry& other);
	~InternalMessageEntry();
	InternalMessageEntry& operator=(const InternalMessageEntry& other);

	const char* getName() const;
	const Message& getMessage() const;

	void setName(const char* name);
	void setMessage(const Message& msg);

private:
	std::string m_name;
	Message     m_message;
};


class InternalSubscriptionEntry
{
public:
	InternalSubscriptionEntry();
	InternalSubscriptionEntry(const InternalSubscriptionEntry& other);
	~InternalSubscriptionEntry();
	InternalSubscriptionEntry& operator=(const InternalSubscriptionEntry& other);

	const char* getName() const;
	const char* getPattern() const;

	void setName(const char* name);
	void setPattern(const char* subject);

	bool hasNextExcludedPattern() const;
	const char* nextExcludedPattern() const; 	
	void addExcludedPattern(const char* pattern);

private:
	typedef std::list<std::string> ExcludedPatterns;

	std::string                        m_name;
	std::string                        m_pattern;
	ExcludedPatterns                   m_excludedPatterns;
	mutable ExcludedPatterns::iterator m_excludedPatternsIter;
};


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
