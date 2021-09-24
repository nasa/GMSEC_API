
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseConfigFile.h
 *  @brief This file contains a class for managing configuration files.
 *
 *
**/

#ifndef gmsec_internal_BaseConfigFile_h
#define gmsec_internal_BaseConfigFile_h


#include <gmsec/Connection.h>
#include <gmsec/internal/ci_less.h>
#include <gmsec/internal/tinyxml2.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/util/String.h>

#include <list>

namespace gmsec
{

// forward declare so we don't end up with recursive includes
class Config;
class Message;

namespace internal
{

/** @class BaseConfigFile
 *	@brief This class is for the managment of standard configuration files.
 * In the future, there may be alternative implementations, but for now
 * it just uses the pre-3.0 implementation.
 * @sa gmsec::ConfigFile
 *
**/
class GMSEC_API BaseConfigFile
{
private:
	// Declared, but not implemented.
	BaseConfigFile(const BaseConfigFile &);
	BaseConfigFile &operator=(const BaseConfigFile &);

	std::string fXmlOutput;

	std::string fFilePath;
	tinyxml2::XMLDocument *fDoc;
	bool fDocLoaded;

	typedef std::map<std::string, tinyxml2::XMLElement *, ci_less> NodeMap;
	typedef NodeMap::iterator NodeItr;

	NodeMap fConfigNodes;
	NodeMap fMsgNodes;
	NodeMap fSubscriptionNodes;
	NodeMap fCustomNodes;

	std::map<std::string,std::string> m_aliasMap;

	// helper function to parse XML
	Status ParseDoc();

	// helper function to clear NodeMap objects
	void ClearDoc();

	// helper function to append new XML to the document's hierarchy
	enum XmlType { XML_SUBSCRIPTION, XML_CONFIG, XML_MESSAGE, XML_CUSTOM };
	void AppendToDocument(tinyxml2::XMLElement* element, const XmlType type);

	// helper function to retrieve and display xml document errors
	void HandleDocErrors(tinyxml2::XMLDocument *doc, const char *functionId);

	// helper functions to retrieve CustomXML strings from the document
	tinyxml2::XMLElement* GetCustomXMLElement(const char* xmlStr);
	bool CustomXMLExists(const char* xmlStr);

protected:

/*
// Old, obsolete Lists
	gmsec::util::List<std::string> subscriptionNames;
	gmsec::util::List<std::string> subscriptions;
	gmsec::util::List<std::string> configNames;
	gmsec::util::List<tinyxml2::XMLElement *> configs;
	gmsec::util::List<std::string> messageNames;
	gmsec::util::List<tinyxml2::XMLElement *> messageXmls;
*/

	// helper to get name="" attribute from node
	const char *getNameAttr(tinyxml2::XMLElement *node);

	const char *getStrAttr(tinyxml2::XMLElement *node, const char * field);

	// Helper to create a new, fresh XML document
	Status InitNewXMLDocument();

public:

	/** @fn BaseConfigFile()
	 * @brief Basic constructor to create empty configuration file.
	 *
	 */
	BaseConfigFile();

	/** @fn BaseConfigFile( const char *filepath )
	 * @note This constructor has been deprecated; use BaseConfigFile() instead.
	 * @brief This constructor creates the association of this object
	 * with a configuration file (if file path is given).
	 *
	 * @param filepath - full or relative path to configuration file
	 */
	BaseConfigFile(const char *filepath);

	/** @fn ~BaseConfigFile()
	 * @brief Destructor.
	 */
	virtual ~BaseConfigFile();

	/** @fn Load()
	 * @note This function has been deprecated; use Load(const char* filepath) instead.
	 * @brief This function opens the configuration file and parses the XML.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @return status of whether the load and parse was successful
	 */
	Status Load();

	/** @fn Load(const char* filepath)
	 * @brief This function opens the configuration file and parses the XML.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @return status of whether the load and parse was successful
	 */
	Status Load(const char* filepath);

	/** @fn Save(const char * filePath, bool compact = false)
	 * @brief This function saves the currently open configuration file to
	 * the location specified.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param const char * filePath - The location to which the file will be saved;
	 * if no path is specified or is specified as NULL, then the path, if any, that
	 * was used with Load(const char* filepath) will be used.
	 * @param compact - determines whether the XML output will be compacted or not
	 * @return status of whether the file save operation was successful
	 */
	Status Save(const char * filePath = NULL, bool compact = false);

	/** @fn FromXML( const char *xml)
	 * @brief This function parses the XML in the passed in string.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @return status of whether the parse and parse was successful
	 */
	Status FromXML(const char *xml);

	/** @fn ToXML( const char *&xml )
	 * @brief This function produces an XML config file containing all
	 *	known config file tags.
	 *
	 * @return status of whether the parse and parse was successful
	 */
	Status ToXML(const char *&xml);

	/** @fn IsLoaded()
	 * @brief This function will return true is the config file was successfully loaded
	 *
	 * @return true if file loaded
	 */
	bool IsLoaded()
	{
		return fDocLoaded;
	}

	/** @fn LookupConfig( const char *name, gmsec::Config *cfg )
	 * @brief This function will attempt to find a named connection in the
	 * config file and set the passed in config object with
	 * the loaded values.
	 *
	 *  Example config file load and get config:
	 *  @code
	  * // create configfile object and attach to file
	  * BaseConfigFile cf("path/to/file.xml");
	  *
	 * // load & parse
	  * result = cf.Load();
	  * if( result.isError() )
	 *	//handle error
	  *
	  * // get 2 configs from file
	  * Config c1,c2;
	  * cf.LookupConfig("config1",&c1);
	  * cf.LookupConfig("config2",&c2);
	  *  @endcode
	 *
	 * @param name - value in the name="" attribute of the \<CONFIG/\> block to seed the gmsec::Config object
	 * @param cfg - Config object to set
	 * @return status of whether named CONFIG block was found
	 */
	Status LookupConfig(const char *name, gmsec::Config *cfg);

	/** @fn LookupMessage( const char *name, gmsec::Message *msg )
	 * @brief This function will attempt to find a named message in the
	 * config file and create a new message from that xml
	 * the loaded fields and values.
	 *
	 *  Example config file load and get message:
	 *  @code
	  * // create configfile object and attach to file
	  * BaseConfigFile cf("path/to/file.xml");
	  *
	 * // load & parse
	  * result = cf.Load();
	  * if( result.isError() )
	 *	//handle error
	  *
	  * // create message from config file entry
	  * Message *msg = NULL;
	  * result = conn->CreateMessage(msg);
	  * if( result.isError() )
	 *	//handle error
	  *
	  * // load message subject, type, and fields from config file
	  * result = cf.LookupMessage("msg1",msg);
	  * if( result.isError() )
	 *	//handle error
	  *  @endcode
	 *
	 * @param name - value in the name="" attribute of the \<MESSAGE/\> block to seed the gmsec::Message object
	 * @param msg - Message object to set
	 * @return status of whether named MESSAGE block was found
	 */
	Status LookupMessage(const char *name, gmsec::Message *msg);

	/** @fn LookupSubscription( const char *name, const char *&pattern )
	 * @brief This function will look up a subscription pattern defined in
	 * the config file. This is usefull to allow easy modification of subject
	 * names without code changes.
	 *
	 * Example
	 * @code
	 * char *pattern = NULL;
	 *
	 * // lookup subject pattern by name
	 * result = cf.LookupSubscription( "events", pattern );
	  * if( result.isError() )
	 *	//handle error
	 *
	 * cout << "Subscribe to: " << pattern << endl;
	 *
	 * // subscribe on the connection
	 * result = conn->Subscribe(pattern);
	  * if( result.isError() )
	 *	//handle error
	 * @endcode
	 *
	 * @param name - value in the name="" attribute of the \<SUBSCRIPTION/\> block containing the subscription pattern
	 * @param pattern - out parameter, the pattern defined in the config file
	 * @return status of whether named SUBSCRIPTION block was found
	 */
	Status LookupSubscription(const char *name, const char *&pattern);

	/** @fn Status Reset(ConfigFileIterator &iter)
	 * @brief This function resets the ConfigFileIterator to this configFile
	 * 
	 * @param iter - ConfigFileIterator to be set.
	 * @return status of whether ConfigFileIterator was reset 
	 */
	Status Reset(gmsec::ConfigFileIterator &iter);

	/*
	Note: The following methods may have unsafe behaviour.
	*/
	/** @fn GetSubscriptionNames (gmsec::util::List<std::string> &names)
	 * @brief Gets the list of SubscriptionNames.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<string>
	 * @return status - whether successful or not
	 */
	Status GetSubscriptionNames (gmsec::util::List<std::string> &names);

	/** @fn GetSubscriptionNames (gmsec::util::List<gmsec::util::String> &names)
	 * @brief Gets the list of SubscriptionNames.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<gmsec string>
	 * @return status - whether successful or not
	 */
	Status GetSubscriptionNames (gmsec::util::List<gmsec::util::String> &names);

	/** @fn GetSubscriptions (gmsec::util::List<std::string> &patterns)
	 * @brief Gets the list of Subscriptions.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param patterns - gmsec List<string>
	 * @return status - whether successful or not
	 */
	Status GetSubscriptions (gmsec::util::List<std::string> &patterns);

	/** @fn GetSubscriptions (gmsec::util::List<gmsec::util::String> &patterns)
	 * @brief Gets the list of Subscriptions.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param patterns - gmsec List<gmsec string>
	 * @return status - whether successful or not
	 */
	Status GetSubscriptions (gmsec::util::List<gmsec::util::String> &patterns);

	/** @fn GetConfigNames (gmsec::util::List<std::string> &names)
	 * @brief Gets the list of ConfigNames.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<string>
	 * @return status - whether successful or not
	 */
	Status GetConfigNames (gmsec::util::List<std::string> &names);

	/** @fn GetConfigNames (gmsec::util::List<gmsec::util::String> &names)
	 * @brief Gets the list of ConfigNames.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<gmsec string>
	 * @return status - whether successful or not
	 */
	Status GetConfigNames (gmsec::util::List<gmsec::util::String> &names);

	/** @fn GetConfigs (gmsec::util::List<Config *> &configList)
	 * @brief Gets the list of Configs.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param configList - gmsec List<Config>
	 * @return status - whether successful or not
	 */
	Status GetConfigs (gmsec::util::List<Config *> &configList);

	/** @fn GetMessageNames (gmsec::util::List<std::string> &names)
	 * @brief Gets the list of MessageNames.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<string>
	 * @return status - whether successful or not
	 */
	Status GetMessageNames (gmsec::util::List<std::string> &names);

	/** @fn GetMessageNames (gmsec::util::List<gmsec::util::String> &names)
	 * @brief Gets the list of MessageNames.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<gmsec string>
	 * @return status - whether successful or not
	 */
	Status GetMessageNames (gmsec::util::List<gmsec::util::String> &names);

	/** @fn GetMessages (gmsec::Connection * connection, gmsec::util::List<gmsec::Message *> &message)
	 * @brief Gets the list of Messages.
	 * @note This function has been deprecated; remove for API 4.0.
	 * @note Function may cause unsafe behavior
	 *
	 * @param message - gmsec List<message>
	 * @param connection
	 * @return status - whether successful or not
	 */
	Status GetMessages (gmsec::Connection * connection, gmsec::util::List<gmsec::Message *> &message);

	/** @fn AddSubscription (const char * subscription, const char * name)
	 * @brief Adds a Subscription definition to the list of subscriptions.
	 * @note Adding a subscription with a name that currently exists will overwrite the current subscription.
	 * @note Adding a subscription to a newly instantiated ConfigFile object that has not loaded a document
	 * will create a new XML document.
	 * @note Function may cause unsafe behavior
	 *
	 * @param name - name to be associated with the subscription
	 * @param subscription - either the XML string representation of a gmsec Subscription, or a pattern for
	 * a subscription which will automatically be translated into the XML string representation thereof.
	 * @return status - whether successful or not
	 */
	Status AddSubscription (const char* name, const char* subscription);

	/** @fn AddConfig (gmsec::Config& config, const char * name)
	 * @brief Adds a config definition to the list of configurations.
	 * The Config object, though passed by reference, will not be modified.
	 * @note Adding a config with a name that currently exists will overwrite the current configuration definition.
	 * @note Adding a config to a newly instantiated ConfigFile object that has not loaded a document
	 * will create a new XML document.
	 * @note Function may cause unsafe behavior
	 *
	 * @param name - name to be associated with the configuration
	 * @param config - gmsec config
	 * @return status - whether successful or not
	 */
	Status AddConfig (const char* name, gmsec::Config& config);

	/** @fn AddMessage (gmsec::Message& message, const char * name)
	 * @brief Adds a message definition to the list of messages.
	 * The Message object, though passed by reference, will not be modified.
	 * @note Adding a message with a name that currently exists will overwrite the current message definition.
	 * @note Adding a message definition to a newly instantiated ConfigFile object that has not loaded a document
	 * will create a new XML document.
	 * @note Function may cause unsafe behavior
	 *
	 * @param name - name to be associated with the message definition
	 * @param message - gmsec message
	 * @return status - whether successful or not
	 */
	Status AddMessage (const char* name, gmsec::Message& message);

	/** @fn AddCustomXML (const char * xml)
	 * @brief Adds a custom XML strings to the document.
	 * @note Adding a custom XML to a newly instantiated ConfigFile object that has not loaded a document
	 * will create a new XML document.
	 * @note Function may cause unsafe behavior
	 *
	 * @param xml - a custom user-created XML string
	 * @return status - whether successful or not
	 */
	Status AddCustomXML (const char * xml);

	/** @fn RemoveSubscription (const char * name)
	 * @brief Removes the subscription that is mapped to the provided name.
	 *
	 * @param name - name to be associated with the subscription
	 * @return status - whether successful or not
	 */
	Status RemoveSubscription (const char * name);

	/** @fn RemoveConfig (const char * name)
	 * @brief Removes the config that is mapped to the provided name.
	 *
	 * @param name - name to be associated with the configuration definition
	 * @return status - whether successful or not
	 */
	Status RemoveConfig (const char * name);

	/** @fn RemoveMessage (const char * name)
	 * @brief Removes the message that is mapped to the provided name.
	 *
	 * @param name - name to be associated with the message definition
	 * @return status - whether successful or not
	 */
	Status RemoveMessage (const char * name);

	/** @fn RemoveCustomXML (const char * xml)
	 * @brief Removes the provided xml element.
	 *
	 * @param xml - xml element string matching the custom xml definition
	 * @return status - whether successful or not
	 */
	Status RemoveCustomXML (const char * xml);

	/** @fn nameOrAliasMatches (const char * searchName, const char* name)
	 * @brief Returns whether or not an incoming string matches an
	 *        valid attribute name or one of its aliases.
	 *
	 * @param searchName - incoming string to check equivalence against
	 * @param name       - attribute name or alias where the check is
	 *                     made that searchName matches.
	 *
	 * @return boolean   - whether or not the searchName is equivalent
	 *                     to the name or an appropriate alias.
	 *
	 */
	bool nameOrAliasMatches(const char* searchName,
	                        const char* name);

	// helper function to retrieve the string representation of an XMLElement
	static std::string ElementToString(tinyxml2::XMLElement* element);

};

} // namespace internal
} // namespace gmsec

#endif /* gmsec_internal_BaseConfigFile_h */

