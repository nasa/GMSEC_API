/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file ConfigFile.cpp
 */

#include <gmsec/internal/BaseConfigFile.h>
#include <gmsec/internal/BaseConfig.h>
#include <gmsec/internal/InternalMessage.h>
#include <gmsec/internal/MessageBuddy.h>
#include <gmsec/internal/SimpleConfigFileIterator.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/internal/Log.h>
#include <tinyxml2.h>
#include <stdexcept>

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::util;
using std::string;
using gmsec::Status;


BaseConfigFile::BaseConfigFile()
	:
	fFilePath(),
	fDoc(NULL),
	fDocLoaded(false)
{
	//
	// m_aliasMap is a map of some allowable alternatives to expected
	// XML keywords in a config file, used in private method
	// nameOrAliasMatches()).
	//
	m_aliasMap["subject"]        = "PATTERN";
	m_aliasMap["network"]        = "DEFINITIONS";
	m_aliasMap["gmsectransport"] = "DEFINITIONS";
	m_aliasMap["transport"]      = "DEFINITIONS";
}


// deprecated!
BaseConfigFile::BaseConfigFile(const char *filepath)
	:
	fFilePath(),
	fDoc(NULL),
	fDocLoaded(false)
{
	if (filepath)
	{
		fFilePath = std::string(filepath);
	}

	//
	// m_aliasMap is a map of some allowable alternatives to expected
	// XML keywords in a config file, used in private method
	// nameOrAliasMatches()).
	//
	m_aliasMap["subject"]        = "PATTERN";
	m_aliasMap["network"]        = "DEFINITIONS";
	m_aliasMap["gmsectransport"] = "DEFINITIONS";
	m_aliasMap["transport"]      = "DEFINITIONS";
}


BaseConfigFile::~BaseConfigFile()
{
	if (NULL != fDoc)
	{
		delete fDoc;
		fDoc = NULL;
	}
}


const char *BaseConfigFile::getNameAttr(tinyxml2::XMLElement *element)
{
	return getStrAttr(element, "NAME");
}


const char *BaseConfigFile::getStrAttr(tinyxml2::XMLElement *element, const char * field)
{
	const tinyxml2::XMLAttribute *attr = NULL;
	for (attr = element->FirstAttribute(); attr; attr = attr->Next())
	{
		const char *caname = attr->Name();
		if (caname != NULL)
		{
			if (nameOrAliasMatches(caname, field))
			{
				return attr->Value();
			}
		}
	}
	return NULL;
}


// deprecated!
Status BaseConfigFile::Load()
{
	Status result;

	if (fDocLoaded)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Config file already loaded");
		LOG_WARNING << "BaseConfigFile::Load : " << result.Get(); 
		return result;
	}

	if (fFilePath.empty())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid or missing config file path.");
		LOG_WARNING << "BaseConfigFile::Load : " << result.Get();
		return result;
	}

	return Load(fFilePath.c_str());
}


Status BaseConfigFile::Load(const char* filepath)
{
	Status result;

	if (filepath == NULL || std::string(filepath).length() == 0)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid or missing config file path.");
		LOG_WARNING << "BaseConfigFile::Load : " << result.Get();
		return result;
	}

	if (fDoc || fDocLoaded)
	{
		ClearDoc();

		delete fDoc;

		fDoc       = NULL;
		fDocLoaded = false;
		fFilePath  = "";
	}

	fFilePath = std::string(filepath);

	// open & parse file
	fDoc = new tinyxml2::XMLDocument();
	if (fDoc == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Failed to allocate XMLDocument");
		LOG_WARNING << "BaseConfigFile::Load : " << result.Get();
		return result;
	}

	tinyxml2::XMLError code = fDoc->LoadFile(fFilePath.c_str());
	if (code == tinyxml2::XML_NO_ERROR)
	{
		result = ParseDoc();
	}
	else
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Failed to parse XML file");
		LOG_WARNING << "BaseConfigFile::Load : " << result.Get() << ' ' << fDoc->GetErrorStr1();
	}

	if (code != tinyxml2::XML_NO_ERROR || result.IsError())
	{
		delete fDoc;
		fDoc = NULL;
		fDocLoaded = false;
	}
	else
	{
		fDocLoaded = true;
	}

	return result;
}


Status BaseConfigFile::Save(const char * filePath, bool compact)
{
	Status result;

	if ((filePath == NULL && fFilePath.empty()) || (filePath != NULL && filePath[0] == '\0'))
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid config file path.");
		LOG_WARNING << "BaseConfigFile::Save : " << result.Get();
		return result;
	}

	// Initialize a new XML document, if one is not currently loaded
	result = InitNewXMLDocument();

	if (!result.IsError())
	{
		const char* savePath = (filePath ? filePath : fFilePath.c_str());

		if (fDoc->SaveFile(savePath, compact) != tinyxml2::XML_NO_ERROR)
		{
			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_XML_PARSE_ERROR,
			           "Failed to save XML file");
			LOG_WARNING << "BaseConfigFile::Save : " << result.Get() << ' ' << fDoc->GetErrorStr1();
		}
	}

	return result;
}


Status BaseConfigFile::ParseDoc()
{
	Status result;

	tinyxml2::XMLDocument *doc = fDoc;

	/* get root node */
	tinyxml2::XMLElement *root_element = doc->RootElement();
	if (root_element == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml format - no root element");
		LOG_WARNING << "BaseConfigFile::ParseDoc : " << result.Get();
		return result;
	}

	/* verify that our root node is named "DEFINITIONS" */
	bool        missing_definitions = false;
	const char *rename = root_element->Name();
	if (rename == NULL)
	{
		missing_definitions = true;
	}
	else if (!nameOrAliasMatches(rename, "DEFINITIONS"))
	{
		missing_definitions = true;
	}

	if (missing_definitions)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml format - missing DEFINITIONS");
		LOG_WARNING << "BaseConfigFile::ParseDoc : " << result.Get();
		return result;
	}

	// read configs & messages
	// fields (child nodes)
	for (tinyxml2::XMLElement *element = root_element->FirstChildElement();
			element;
			element = element->NextSiblingElement())
	{
		//CONFIG
		const char *cname = element->Name();
		if (cname != NULL && stringEquals(cname, "CONFIG"))
		{
			const char *name = getNameAttr(element);
			if (name)
			{
				fConfigNodes[name] = element;
			}
			else
			{
				LOG_WARNING << "BaseConfigFile::ParseDoc :  A config element has no name attribute defined, and thus has not been added to the list of configurations.";
			}
		}
		// MESSAGE
		else if (cname != NULL && stringEquals(cname, "MESSAGE"))
		{
			const char *name = getNameAttr(element);
			if (name)
			{
				fMsgNodes[name] = element;
			}
			else
			{
				LOG_WARNING << "BaseConfigFile::ParseDoc :  A message element has no name attribute defined, and thus has not been added to the list of messages.";
			}
		}
		// SUBSCRIPTION
		else if (cname != NULL && stringEquals(cname, "SUBSCRIPTION"))
		{
			const char *name = getNameAttr(element);
			if (name)
			{
				const char *pattern = getStrAttr(element, "PATTERN");
				if (pattern)
				{
					fSubscriptionNodes[name] = element;
				}
				else
				{
					LOG_WARNING << "BaseConfigFile::ParseDoc :  Subscription element " << name << " has no PATTERN (or SUBJECT) defined, and thus has not been added to the list of subscriptions.";
				}
			}
			else
			{
				LOG_WARNING << "BaseConfigFile::ParseDoc :  A subscription element has no name attribute defined, and thus has not been added to the list of subscriptions.";
			}
		}
		// CUSTOM
		else
		{
			fCustomNodes[ElementToString(element)] = element;
		}
	}

	return result;
}


void BaseConfigFile::ClearDoc()
{
	fConfigNodes.clear();
	fMsgNodes.clear();
	fSubscriptionNodes.clear();
	fCustomNodes.clear();
}


Status BaseConfigFile::FromXML(const char *xml)
{
	Status result;

	if (fDocLoaded)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Config file already loaded");
		LOG_WARNING << "BaseConfigFile::FromXML : " << result.Get();
		return result;
	}

	if (xml == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml config file format");
		LOG_WARNING << "BaseConfigFile::FromXML : " << result.Get();
		return result;
	}

	fDoc = new tinyxml2::XMLDocument();
	if (fDoc == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml message format");
		LOG_WARNING << "BaseConfigFile::FromXML : " << result.Get();
		return result;
	}

	tinyxml2::XMLError code = fDoc->Parse(xml);
	if (code == tinyxml2::XML_NO_ERROR)
	{
		result = ParseDoc();
	}
	else
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Invalid xml format - parse failed");
		HandleDocErrors(fDoc, "BaseConfigFile::FromXML");
		LOG_WARNING << "BaseConfigFile::FromXML : " << result.Get();
		return result;
	}

	if (result.IsError())
	{
		//make sure we free the dom tree
		delete fDoc;
		fDoc = NULL;
		fDocLoaded = false;
	}
	else
	{
		fDocLoaded = true;
	}

	return result;
}


Status BaseConfigFile::ToXML(const char *&xml)
{
	Status result;

	if (fDoc == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "XML not available");
		LOG_WARNING << "BaseConfigFile::ToXML : " << result.Get();
		return result;
	}

	tinyxml2::XMLPrinter printer;
	fDoc->Print(&printer);
	fXmlOutput = string(printer.CStr(), printer.CStrSize() - 1);

	xml = fXmlOutput.c_str();

	return result;
}


Status BaseConfigFile::LookupConfig(const char *name, Config *cfg)
{
	Status result;

	if (cfg == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_INVALID_CONFIG,
		           "Config is null");
		LOG_WARNING << "BaseConfigFile::LookupConfig : " << result.Get();
		return result;
	}

	if (name == NULL || string(name).empty())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_INVALID_CONFIG,
		           "Can't find config by a NULL or empty name.");
		LOG_WARNING << "BaseConfigFile::LookupConfig : " << result.Get();
		return result;
	}

	NodeItr node = fConfigNodes.find(name);

	if (node == fConfigNodes.end())
	{
		string err_msg = "Can't find config by name ";

		err_msg += name;
		err_msg += " in the file.";

		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_INVALID_CONFIG,
		           err_msg.c_str());
		LOG_WARNING << "BaseConfigFile::LookupConfig : " << result.Get();
	}
	else
	{
		result = BaseConfig::getInternal(cfg)->FromXML(node->second);
	}

	return result;
}

Status BaseConfigFile::LookupMessage(const char *name, gmsec::Message *msg)
{
	Status result;

	if (msg == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message is null");
		LOG_WARNING << "BaseConfigFile::LookupMessage : " << result.Get();
		return result;
	}

	if (name == NULL || string(name).empty())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Name is invalid because it is NULL or empty");
		LOG_WARNING << "BaseConfigFile::LookupMessage : " << result.Get();
		return result;
	}

	NodeItr node = fMsgNodes.find(name);

	if (node == fMsgNodes.end())
	{
		string err_msg = "Message by name ";

		err_msg += name;
		err_msg += " does not exist.";

		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           err_msg.c_str());
		LOG_WARNING << "BaseConfigFile::LookupMessage : " << result.Get();
	}
	else
	{
		// TODO: currently the FromXML() implementation in the Message
		//		 will load/set a configuration block listed within a message
		//	     definition, but it would be nice to reference an existing
		//       configuration by name, in the XML, so that many messages can
		//	     reference the same config without cut&paste.
		//
		//       In order to do this, we'd probably have to look for a config
		//       reference in the message node and replace it with one listed
		//       in the global config list. (node insertion)?

		// set message from XML
		result = MessageBuddy::getInternal(msg)->FromXML(ElementToString(node->second).c_str());
	}

	return result;
}


Status BaseConfigFile::LookupSubscription(const char *name, const char *&pattern)
{
	Status result;

	if (name == NULL || string(name).empty())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Name is invalid because it is NULL or empty");
		LOG_WARNING << "BaseConfigFile::LookupSubscription : " << result.Get();
		return result;
	}

	NodeItr node = fSubscriptionNodes.find(name);

	if (node == fSubscriptionNodes.end())
	{
		string err_msg = "Subscription by name ";

		err_msg += name;
		err_msg += " does not exist.";

		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_OTHER_ERROR,
		           err_msg.c_str());
		LOG_WARNING << "BaseConfigFile::LookupSubscription : " << result.Get();
	}
	else
	{
		// reset pattern for output
		pattern = getStrAttr(node->second, "PATTERN");
		if (pattern == NULL)
		{
			string err_msg = "Subscription by name ";

			err_msg += name;
			err_msg += " does not have a defined pattern.";

			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_OTHER_ERROR,
			           err_msg.c_str());
			LOG_WARNING << "BaseConfigFile::LookupSubscription : " << result.Get();
		}
	}

	return result;
}


Status BaseConfigFile::Reset(gmsec::ConfigFileIterator &iter)
{
	gmsec::util::List<NodeItr> nodeIters;
	NodeItr itCfg_bgn = fConfigNodes.begin();
	nodeIters.add(itCfg_bgn);
	NodeItr itCfg_end = fConfigNodes.end();
	nodeIters.add(itCfg_end);
	NodeItr itMsg_bgn = fMsgNodes.begin();
	nodeIters.add(itMsg_bgn);
	NodeItr itMsg_end = fMsgNodes.end();
	nodeIters.add(itMsg_end);
	NodeItr itSub_bgn = fSubscriptionNodes.begin();
	nodeIters.add(itSub_bgn);
	NodeItr itSub_end = fSubscriptionNodes.end();
	nodeIters.add(itSub_end);
	NodeItr itCustomXml_bgn = fCustomNodes.begin();
	nodeIters.add(itCustomXml_bgn);
	NodeItr itCustomXml_end = fCustomNodes.end();
	nodeIters.add(itCustomXml_end);
	
	return iter.ptr->Initialize(nodeIters);
}


Status BaseConfigFile::GetConfigNames(gmsec::util::List<string> &names)
{
	Status result;
	names.clear();
	for(NodeItr it = fConfigNodes.begin(); it != fConfigNodes.end(); ++it)
	{
		string name;
		name = it->first;
		names.add(name);
	}

	return result;
}


Status BaseConfigFile::GetConfigNames(gmsec::util::List<gmsec::util::String> &names)
{
	Status result;
	names.clear();
	for(NodeItr it = fConfigNodes.begin(); it != fConfigNodes.end(); ++it)
	{
		gmsec::util::String gus_name;
		string name;

		name = it->first;
		gus_name = gmsec::util::String(name.c_str());
		names.add(gus_name);
	}
	return result;
}


Status BaseConfigFile::GetConfigs(gmsec::util::List<gmsec::Config *> &configList)
{
	Status result;
	configList.clear();
	for(NodeItr it = fConfigNodes.begin(); it != fConfigNodes.end(); ++it)
	{
		gmsec::Config * cfg = new gmsec::Config();
		BaseConfig::getInternal(cfg)->FromXML(it->second);
		configList.add(cfg);
	}
	return result;
}


Status BaseConfigFile::GetSubscriptionNames(gmsec::util::List<string> &names)
{
	Status result;
	names.clear();
	for(NodeItr it = fSubscriptionNodes.begin(); it != fSubscriptionNodes.end(); ++it)
	{
		string name;
		name = it->first;
		names.add(name);
	}
	return result;
}


Status BaseConfigFile::GetSubscriptionNames(gmsec::util::List<gmsec::util::String> &names)
{
	Status result;
	names.clear();
	for(NodeItr it = fSubscriptionNodes.begin(); it != fSubscriptionNodes.end(); ++it)
	{
		gmsec::util::String gus_name;
		string name;

		name = it->first;
		gus_name = gmsec::util::String(name.c_str());
		names.add(gus_name);
	}
	return result;
}


Status BaseConfigFile::GetSubscriptions(gmsec::util::List<string> &patterns)
{
	Status result;
	patterns.clear();
	for(NodeItr it = fSubscriptionNodes.begin(); it != fSubscriptionNodes.end(); ++it)
	{
		string subscription;
		const char *pattern = getStrAttr(it->second, "PATTERN");
		subscription = string(pattern);
		patterns.add(subscription);
	}
	return result;
}


Status BaseConfigFile::GetSubscriptions(gmsec::util::List<gmsec::util::String> &patterns)
{
	Status result;
	patterns.clear();
	for(NodeItr it = fSubscriptionNodes.begin(); it != fSubscriptionNodes.end(); ++it)
	{
		gmsec::util::String gus_subscription;

		const char *pattern = getStrAttr(it->second, "PATTERN");
		gus_subscription = gmsec::util::String(pattern);
		patterns.add(gus_subscription);
	}
	return result;
}


Status BaseConfigFile::GetMessageNames(gmsec::util::List<string> &names)
{
	Status result;
	names.clear();
	for(NodeItr it = fMsgNodes.begin(); it != fMsgNodes.end(); ++it)
	{
		string name;
		name = it->first;
		names.add(name);
	}
	return result;
}


Status BaseConfigFile::GetMessageNames(gmsec::util::List<gmsec::util::String> &names)
{
	Status result;
	names.clear();
	for(NodeItr it = fMsgNodes.begin(); it != fMsgNodes.end(); ++it)
	{
		gmsec::util::String gus_name;
		string name;

		name = it->first;
		gus_name = gmsec::util::String(name.c_str());
		names.add(gus_name);
	}
	return result;
}


Status BaseConfigFile::GetMessages(gmsec::Connection *connection, gmsec::util::List<gmsec::Message*> &message)
{
	Status result;
	message.clear();
	for(NodeItr it = fMsgNodes.begin(); it != fMsgNodes.end(); ++it)
	{
		gmsec::Message * msg = NULL;
		connection->CreateMessage("TEMP", GMSEC_MSG_PUBLISH, msg);
		MessageBuddy::getInternal(msg)->FromXML(ElementToString(it->second).c_str());
		message.add(msg);
	}
	return result;
}


Status BaseConfigFile::InitNewXMLDocument()
{
	Status result;

	// Initialize the XML document if it has not already been loaded
	if (fDoc == NULL)
	{
		fDoc = new tinyxml2::XMLDocument();
		if (fDoc == NULL)
		{
			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
				GMSEC_XML_PARSE_ERROR,
				"Failed to allocate XMLDocument");
			LOG_WARNING << "BaseConfigFile::Load : " << result.Get();
			return result;
		}

		LOG_INFO << "Initializing a new XMLDocument - Ensure to specify a file location when calling the Save function.";

		// Create and insert a new XML Declaration
		tinyxml2::XMLDeclaration* pDecl = fDoc->NewDeclaration(NULL);
		fDoc->InsertEndChild(pDecl);

		// Create a root element for the document
		tinyxml2::XMLElement* new_root = fDoc->NewElement("DEFINITIONS");
		fDoc->InsertEndChild(new_root);
	}

	return result;
}


Status BaseConfigFile::AddSubscription(const char *name, const char* subscription)
{
	Status result;
	string xmlStr;
	const char* xml;

	if (subscription == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			   GMSEC_XML_PARSE_ERROR,
			   "XML string provided is null");
		LOG_WARNING << "BaseConfigFile::AddSubscription : " << result.Get();
		return result;
	}

	if (name == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"Invalid xml format - subscription requires a name");
		LOG_WARNING << "BaseConfigFile::AddSubscription : " << result.Get();
		return result;
	}

	tinyxml2::XMLDocument *tmpDoc = new tinyxml2::XMLDocument();
	if (tmpDoc == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Failed to allocate XMLDocument");
		LOG_WARNING << "BaseConfigFile::AddSubscription : " << result.Get();
		return result;
	}

	if (subscription[0] == '<' && subscription[stringLength(subscription) - 1] == '>')
	{
		xml = subscription;
	}
	else
	{
		xmlStr = "<SUBSCRIPTION PATTERN=\"" + string(subscription) + "\" />";
		xml = xmlStr.c_str();
	}

	// Parse the provided string to ensure that there are no errors
	tinyxml2::XMLError code = tmpDoc->Parse(xml);
	if (code == tinyxml2::XML_NO_ERROR)
	{
		// Grab the root element from the temporary document and add the NAME attribute
		tinyxml2::XMLElement *tmpRootElement = tmpDoc->RootElement();

		if (tmpRootElement == NULL)
		{
			delete tmpDoc;

			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Root Element of XML Document is NULL");
			LOG_WARNING << "BaseConfigFile::AddSubscription: " << result.Get();
			return result;
		}

		tmpRootElement->SetAttribute("NAME", name);

		// Remove the old subscription (If it exists)
		if (fSubscriptionNodes.find(name) != fSubscriptionNodes.end())
		{
			RemoveSubscription(name);
		}

		// Replace the XML element in the array
		fSubscriptionNodes[name] = tmpRootElement;

		// Initialize a new XML document, if one is not currently loaded
		InitNewXMLDocument();

		AppendToDocument(tmpRootElement, XML_SUBSCRIPTION);
	}
	else
	{
		delete tmpDoc;

		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"Invalid xml format - parse failed");
		HandleDocErrors(tmpDoc, "BaseConfigFile::AddSubscription");
		LOG_WARNING << "BaseConfigFile::AddSubscription : " << result.Get();
	}

	return result;
}


Status BaseConfigFile::AddConfig(const char *name, gmsec::Config& config)
{
	Status result;

	if (name == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"Invalid xml format - configuration definition requires a name");
		LOG_WARNING << "BaseConfigFile::AddConfig : " << result.Get();
		return result;
	}

	tinyxml2::XMLDocument *tmpDoc = new tinyxml2::XMLDocument();
	if (tmpDoc == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Failed to allocate XMLDocument");
		LOG_WARNING << "BaseConfigFile::AddConfig : " << result.Get();
		return result;
	}

	// Parse the config's ToXML string to ensure that there are no errors
	const char *configXML;
	config.ToXML(configXML);
	tinyxml2::XMLError code = tmpDoc->Parse(configXML); 
	if (code == tinyxml2::XML_NO_ERROR)
	{
		// Grab the root element from the temporary document and add the NAME attribute
		tinyxml2::XMLElement *tmpRootElement = tmpDoc->RootElement();

		if (tmpRootElement == NULL)
		{
			delete tmpDoc;

			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Root Element of XML Document is NULL");
			LOG_WARNING << "BaseConfigFile::AddConfig: " << result.Get();
			return result;
		}

		tmpRootElement->SetAttribute("NAME", name);

		// Remove the old config (If it exists)
		if (fConfigNodes.find(name) != fConfigNodes.end())
		{
			RemoveConfig(name);
		}

		// Replace the XML element in the array
		fConfigNodes[name] = tmpRootElement;

		// Initialize a new XML document, if one is not currently loaded
		InitNewXMLDocument();

		AppendToDocument(tmpRootElement, XML_CONFIG);
	}
	else
	{
		delete tmpDoc;

		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"Invalid xml format - parse failed");
		HandleDocErrors(tmpDoc, "BaseConfigFile::AddConfig");
		LOG_WARNING << "BaseConfigFile::AddConfig : " << result.Get();
	}

	return result;
}


Status BaseConfigFile::AddMessage(const char *name, gmsec::Message& message)
{
	Status result;

	if (name == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"Invalid xml format - message definition requires a name");
		LOG_WARNING << "BaseConfigFile::AddMessage : " << result.Get();
		return result;
	}

	tinyxml2::XMLDocument *tmpDoc = new tinyxml2::XMLDocument();
	if (tmpDoc == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Failed to allocate XMLDocument");
		LOG_WARNING << "BaseConfigFile::AddMessage : " << result.Get();
		return result;
	}

	// Parse the message's ToXML string to ensure that there are no errors
	const char *msgXML;
	message.ToXML(msgXML);
	tinyxml2::XMLError code = tmpDoc->Parse(msgXML); 
	if (code == tinyxml2::XML_NO_ERROR)
	{
		// Grab the root element from the temporary document and add the NAME attribute
		tinyxml2::XMLElement *tmpRootElement = tmpDoc->RootElement();

		if (tmpRootElement == NULL)
		{
			delete tmpDoc;

			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Root Element of XML Document is NULL");
			LOG_WARNING << "BaseConfigFile::AddMessage: " << result.Get();
			return result;
		}

		tmpRootElement->SetAttribute("NAME", name);

		// Remove the old message (If it exists)
		if (fMsgNodes.find(name) != fMsgNodes.end())
		{
			RemoveMessage(name);
		}

		// Replace the XML element in the array
		fMsgNodes[name] = tmpRootElement;

		// Initialize a new XML document, if one is not currently loaded
		InitNewXMLDocument();

		AppendToDocument(tmpRootElement, XML_MESSAGE);
	}
	else
	{
		delete tmpDoc;

		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"Invalid xml format - parse failed");
		HandleDocErrors(tmpDoc, "BaseConfigFile::AddMessage");
		LOG_WARNING << "BaseConfigFile::AddMessage : " << result.Get();
	}

	return result;
}


Status BaseConfigFile::AddCustomXML(const char* xml)
{
	Status result;

	if (xml == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"XML string provided is null");
		LOG_WARNING << "BaseConfigFile::AddCustomXML : " << result.Get();
		return result;
	}

	std::string xmlStr = gmsec::util::stripWhiteSpace(xml);

	if (xmlStr[0] != '<' || xmlStr[xmlStr.length() - 1] != '>' || xmlStr[xmlStr.length() - 2] == '\\')
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"Invalid xml format - string does not start with < and end with an un-escaped >");
		LOG_WARNING << "BaseConfigFile::AddCustomXML : " << result.Get();
		return result;
	}


	if (CustomXMLExists(xmlStr.c_str()))
	{
		// Since we are inserting a bonafide string,
		// if it already exists, then there is no reason to insert it again.
		return result;
	}

	tinyxml2::XMLDocument *tmpDoc = new tinyxml2::XMLDocument();
	if (tmpDoc == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_XML_PARSE_ERROR,
		           "Failed to allocate XMLDocument");
		LOG_WARNING << "BaseConfigFile::AddCustomXML : " << result.Get();
		return result;
	}

	// Parse the message's ToXML string to ensure that there are no errors
	tinyxml2::XMLError code = tmpDoc->Parse(xmlStr.c_str()); 
	if (code == tinyxml2::XML_NO_ERROR)
	{
		// Grab the root element from the temporary document and add the NAME attribute
		tinyxml2::XMLElement *tmpRootElement = tmpDoc->RootElement();

		const std::string elementStr = ElementToString(tmpRootElement);

		// Remove the old custom element (If it exists)
		if (fCustomNodes.find(elementStr) != fCustomNodes.end())
		{
			RemoveCustomXML(elementStr.c_str());
		}

		fCustomNodes[elementStr] = tmpRootElement;

		// Initialize a new XML document, if one is not currently loaded
		InitNewXMLDocument();

		AppendToDocument(tmpRootElement, XML_CUSTOM);
	}
	else
	{
		delete tmpDoc;

		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			GMSEC_XML_PARSE_ERROR,
			"Invalid xml format - parse failed");
		HandleDocErrors(tmpDoc, "BaseConfigFile::AddMessage");
		LOG_WARNING << "BaseConfigFile::AddMessage : " << result.Get();
	}

	return result;
}


Status BaseConfigFile::RemoveSubscription(const char * name)
{
	Status result;

	if (name == NULL || string(name).empty())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Name is invalid");
		LOG_WARNING << "BaseConfigFile::RemoveSubscription : " << result.Get();
		return result;
	}

	NodeItr node = fSubscriptionNodes.find(name);

	if (node == fSubscriptionNodes.end())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Subscription by that name does not exist.");
		LOG_WARNING << "BaseConfigFile::RemoveSubscription : " << result.Get();
	}
	else
	{
		// Remove the element in fDoc
		tinyxml2::XMLElement *fDocRootElement = fDoc->RootElement();

		if (fDocRootElement == NULL)
		{
			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Root Element of XML Document is NULL");
			LOG_WARNING << "BaseConfigFile::RemoveSubscription: " << result.Get();
			return result;
		}

		fDocRootElement->DeleteChild(node->second);

		// Remove the element from the map
		fSubscriptionNodes.erase(name);
	}

	return result;
}


Status BaseConfigFile::RemoveConfig(const char * name)
{
	Status result;

	if (name == NULL || string(name).empty())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Name is invalid");
		LOG_WARNING << "BaseConfigFile::RemoveConfig : " << result.Get();
		return result;
	}

	NodeItr node = fConfigNodes.find(name);

	if (node == fConfigNodes.end())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Config by that name does not exist.");
		LOG_WARNING << "BaseConfigFile::RemoveConfig : " << result.Get();
	}
	else
	{
		// Remove the element from the XML File
		tinyxml2::XMLElement *fDocRootElement = fDoc->RootElement();

		if (fDocRootElement == NULL)
		{
			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Root Element of XML Document is NULL");
			LOG_WARNING << "BaseConfigFile::RemoveConfig: " << result.Get();
			return result;
		}

		fDocRootElement->DeleteChild(node->second);

		// Remove the element from the map
		fConfigNodes.erase(name);
	}

	return result;
}


Status BaseConfigFile::RemoveMessage(const char * name)
{
	Status result;

	if (name == NULL || string(name).empty())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Name is invalid");
		LOG_WARNING << "BaseConfigFile::RemoveMessage : " << result.Get();
		return result;
	}

	NodeItr node = fMsgNodes.find(name);

	if (node == fMsgNodes.end())
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_INVALID_MESSAGE,
		           "Message by that name does not exist.");
		LOG_WARNING << "BaseConfigFile::RemoveMessage : " << result.Get();
	}
	else
	{
		// Remove the element from the XML File
		tinyxml2::XMLElement *fDocRootElement = fDoc->RootElement();

		if (fDocRootElement == NULL)
		{
			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Root Element of XML Document is NULL");
			LOG_WARNING << "BaseConfigFile::RemoveMessage: " << result.Get();
			return result;
		}

		fDocRootElement->DeleteChild(node->second);

		// Remove the element from the map
		fMsgNodes.erase(name);
	}

	return result;
}


Status BaseConfigFile::RemoveCustomXML(const char * xml)
{
	Status result;

	if (xml == NULL)
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Name is invalid");
		LOG_WARNING << "BaseConfigFile::RemoveCustomXML : " << result.Get();
		return result;
	}

	tinyxml2::XMLElement* customElement = GetCustomXMLElement(xml);
	if (customElement != NULL)
	{
		const std::string elementStr = ElementToString(customElement);

		// Remove the element from the XML File
		tinyxml2::XMLElement *fDocRootElement = fDoc->RootElement();

		if (fDocRootElement == NULL)
		{
			result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Root Element of XML Document is NULL");
			LOG_WARNING << "BaseConfigFile::RemoveCustomXML : " << result.Get();
			return result;
		}

		fDocRootElement->DeleteChild(customElement);

		fCustomNodes.erase(elementStr.c_str());
	}
	// Only propagate an error if the user is not forcing completion of this function
	else
	{
		result.Set(GMSEC_STATUS_CONFIGFILE_ERROR,
		           GMSEC_INVALID_MESSAGE,
	                   "Custom XML element does not exist.");
		LOG_WARNING << "BaseConfigFile::RemoveCustomXML : " << result.Get();
	}

	return result;
}


void BaseConfigFile::AppendToDocument(tinyxml2::XMLElement* element, const XmlType type)
{
	if (type != XML_SUBSCRIPTION && type != XML_CONFIG
		&& type != XML_MESSAGE && type != XML_CUSTOM)
	{
		LOG_WARNING << "BaseConfigFile::AppendToDocument : Unknown XmlType enum";
		return;
	}

	bool success = false;
	tinyxml2::XMLElement *fDocRootElement = fDoc->RootElement();

	if (fDocRootElement == NULL)
	{
		LOG_WARNING << "BaseConfigFile::AppendToDocument : Root Element of XML Document is NULL";
		return;
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
				tinyxml2::XMLElement *lastMsgElement = fDocRootElement->LastChildElement("MESSAGE");
				if (lastMsgElement != NULL)
				{
					fDocRootElement->InsertAfterChild(lastMsgElement, element);
					success = true;
				}
			}
			// If MESSAGE elements exist, place it after the last CONFIG element
		case XML_CONFIG:
			if (!success)
			{
				tinyxml2::XMLElement *lastCfgElement = fDocRootElement->LastChildElement("CONFIG");
				if (lastCfgElement != NULL)
				{
					fDocRootElement->InsertAfterChild(lastCfgElement, element);
					success = true;
				}
			}
			// If no CONFIG elements exist, place it after the last SUBSCRIPTION element
		case XML_SUBSCRIPTION:
			if (!success)
			{
				tinyxml2::XMLElement *lastSubElement = fDocRootElement->LastChildElement("SUBSCRIPTION");
				if (lastSubElement != NULL)
				{
					fDocRootElement->InsertAfterChild(lastSubElement, element);
				}
				// If no SUBSCRIPTION elements exist, place it at the top of the children
				else
				{
					fDocRootElement->InsertFirstChild(element);
				}
			}
	}
}


string BaseConfigFile::ElementToString(tinyxml2::XMLElement* element)
{
	tinyxml2::XMLPrinter printer;
	element->Accept(&printer);

	// If the last character in the printer string is a newline character, additional
	// subtraction is necessary.  The buffer may or may not have a newline character,
	// depending on the current tinyxml2 document settings (ie. compact == true).
	string elementStr = string(printer.CStr(), printer.CStrSize() - 1);

	// If the last character in the printer string is a newline character, additional
	// subtraction is necessary.  The buffer may or may not have a newline character,
	// depending on the current tinyxml2 document settings (ie. compact == true).
	const size_t actualSize = elementStr.size() - (elementStr[elementStr.size() - 1] == '\n' ? 1 : 0);

	return elementStr.substr(0, actualSize);
}


tinyxml2::XMLElement* BaseConfigFile::GetCustomXMLElement(const char* xml)
{
	if (xml == NULL)
	{
		return NULL;
	}

	string xmlStr = gmsec::util::stripWhiteSpace(xml);

	// Ensure that the input is a proper, full XML element
	if (xmlStr[0] != '<' || xmlStr[xmlStr.length() - 1] != '>' || xmlStr[xmlStr.length() - 2] == '\\')
	{
		const char* errorString =
			"Invalid xml format - string does not start with < and end with an un-escaped >";
		LOG_WARNING << "BaseConfigFile::GetCustomXMLElement : " << errorString;
		return NULL;
	}

	tinyxml2::XMLDocument* tmpDoc = new tinyxml2::XMLDocument();

	if (tmpDoc == NULL)
	{
		const char* errorString = "Failed to allocate XMLDocument";
		LOG_WARNING << "BaseConfigFile::GetCustomXMLElement : " << errorString;
		return NULL;
	}

	if (tmpDoc->Parse(xmlStr.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		delete tmpDoc;
		const char* errorString = "Unparseable XML string given";
		LOG_WARNING << "BaseConfigFile::GetCustomXMLElement : " << errorString;
		return NULL;
	}

	tinyxml2::XMLElement* tmpXMLElement = tmpDoc->RootElement();
	string tmpXML = ElementToString(tmpXMLElement);
	tinyxml2::XMLElement* customXMLElement = NULL;

	for(NodeItr it = fCustomNodes.begin(); it != fCustomNodes.end(); ++it)
	{
		string elementStr = ElementToString(it->second);

		if (elementStr.compare(tmpXML) == 0)
		{
			customXMLElement = it->second;
			break;
		}
	}

	delete tmpDoc;

	return customXMLElement;
}


bool BaseConfigFile::CustomXMLExists(const char* xmlStr)
{
	bool result = (GetCustomXMLElement(xmlStr) != NULL);

	return result;
}


void  BaseConfigFile::HandleDocErrors(tinyxml2::XMLDocument* doc, const char* functionId)
{
	const char *err1 = doc->GetErrorStr1();
	if (!err1) err1 = "unknown";
	LOG_WARNING << functionId << " : tiny2xml2::XMLDocument Error #1: " << err1;
	const char *err2 = doc->GetErrorStr2();
	if (!err2) err2 = "unknown";
	LOG_WARNING << functionId << " : tiny2xml2::XMLDocument Error #2: " << err2;
}


bool BaseConfigFile::nameOrAliasMatches(const char* searchName,
                                        const char* name)
{
	//
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
	bool        ret_val;
	string lowercase_search_name;

	if (!stringEquals(searchName,name))
	{
		ret_val = true;

		lowercase_search_name = "";
		for (unsigned int i = 0; i < strlen(searchName); i++)
		{
			lowercase_search_name += tolower(searchName[i]);
		}

		std::map<string,string>::iterator it =
		  m_aliasMap.find(lowercase_search_name);

		if (it == m_aliasMap.end()) // no match found
		{
			ret_val = false;
		}
		else
		{
			//
			// Match found for key; need to check if the 
			// found value matches as well.
			//
			string name_str = it->second;

			if (!stringEquals(name_str.c_str(), name))
			{
				ret_val = false;
			}
		}
	}
	else
	{
		ret_val = true;
	}

	return ret_val;

}


