/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalConfigFile.h
 *
 *  @brief This file contains a class for managing configuration files.
 */

#ifndef GMSEC_API5_INTERNAL_CONFIGFILE_H
#define GMSEC_API5_INTERNAL_CONFIGFILE_H

#include <gmsec5/internal/ci_less.h>
#include <gmsec5/internal/tinyxml2.h>

#include <gmsec5/Config.h>
#include <gmsec5/ConfigFile.h>
#include <gmsec5/ConfigFileIterator.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageFactory.h>

#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/wdllexp.h>

#include <map>
#include <string>
#include <list>


namespace gmsec
{
namespace api5
{
namespace internal
{

class GMSEC_API InternalConfigFile
{
public:
	InternalConfigFile();


	InternalConfigFile(const Config& config);


	~InternalConfigFile();


	void CALL_TYPE load(const char* filepath);


	void CALL_TYPE save(const char* filePath = NULL, bool compact = false);


	void CALL_TYPE fromXML(const char* xml);


	const char* CALL_TYPE toXML() const;


	bool CALL_TYPE isLoaded() const;


	void CALL_TYPE addConfig(const char* name, const Config& config);


	Config CALL_TYPE lookupConfig(const char* name) const;


	bool CALL_TYPE removeConfig(const char* name);


	void CALL_TYPE addMessage(const char* name, const Message& message);


	Message CALL_TYPE lookupMessage(const char* name) const;


	bool CALL_TYPE removeMessage(const char* name);


	void CALL_TYPE addSubscriptionEntry(const ConfigFile::SubscriptionEntry& entry);


	const ConfigFile::SubscriptionEntry& CALL_TYPE lookupSubscriptionEntry(const char* name);


	bool CALL_TYPE removeSubscriptionEntry(const char* name);


	ConfigFileIterator& CALL_TYPE getIterator();


	void CALL_TYPE resetIterators();


	bool CALL_TYPE hasNextConfig();


	bool CALL_TYPE hasNextMessage();


	bool CALL_TYPE hasNextSubscription();


	ConfigFile::ConfigEntry CALL_TYPE nextConfig();


	ConfigFile::MessageEntry CALL_TYPE nextMessage();


	const ConfigFile::SubscriptionEntry& CALL_TYPE nextSubscription();


private:
	bool nameOrAliasMatches(const char* searchName, const char* name) const;

	// defined, but not implemented
	InternalConfigFile(const InternalConfigFile&);
	InternalConfigFile& operator=(const InternalConfigFile&);

	enum XmlType { XML_SUBSCRIPTION, XML_CONFIG, XML_MESSAGE };

	void parseDoc();

	void clearDoc();

	void appendToDocument(tinyxml2::XMLElement* element, XmlType type);

	std::string elementToString(tinyxml2::XMLElement* element) const;

	void handleDocErrors(tinyxml2::XMLDocument* doc, const char* methodName) const;

	const char* getNameAttr(tinyxml2::XMLElement* node) const;

	const char* getStrAttr(tinyxml2::XMLElement* node, const char* field) const;

	void initNewXMLDocument();

	tinyxml2::XMLElement* createConfigElement(const char* name, const Config& cfg);
	tinyxml2::XMLElement* createMessageElement(tinyxml2::XMLDocument* doc, const char* name, const Message& msg);
	tinyxml2::XMLElement* createSubscriptionElement(const ConfigFile::SubscriptionEntry& entry);


	// private data members
	typedef std::map<std::string, tinyxml2::XMLElement*, ci_less> NodeMap;
	typedef std::map<std::string, std::string>                    AliasMap;
	typedef util::StdSharedPtr<ConfigFile::SubscriptionEntry>     SubEntry;

	std::string                        m_filePath;
	tinyxml2::XMLDocument*             m_xmlDoc;
	bool                               m_docLoaded;
	mutable std::string                m_xmlOutput;
	util::StdUniquePtr<MessageFactory> m_msgFactory;

	NodeMap                            m_configNodes;
	NodeMap                            m_msgNodes;
	NodeMap                            m_subscriptionNodes;

	NodeMap::iterator                  m_configNodesIter;
	NodeMap::iterator                  m_msgNodesIter;
	NodeMap::iterator                  m_subscriptionNodesIter;

	AliasMap                           m_aliasMap;
	ConfigFileIterator                 m_iter;
	SubEntry                           m_subEntry;
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
	Message*    m_message;
};


class InternalSubscriptionEntry
{
public:
	InternalSubscriptionEntry();
	InternalSubscriptionEntry(const char* name, const char* pattern);
	InternalSubscriptionEntry(const InternalSubscriptionEntry& other);
	~InternalSubscriptionEntry();
	InternalSubscriptionEntry& operator=(const InternalSubscriptionEntry& other);

	const char* getName() const;

	const char* getPattern() const;

	void addExcludedPattern(const char* pattern);
	bool hasNextExcludedPattern() const;
	const char* nextExcludedPattern() const; 	

private:
	typedef std::list<std::string> ExcludedPatterns;

	std::string                        m_name;
	std::string                        m_pattern;
	ExcludedPatterns                   m_excludedPatterns;
	mutable ExcludedPatterns::iterator m_excludedPatternsIter;
};


} // end namespace internal
} // end namespace api5
} // end namespace gmsec

#endif
