
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file ConfigFile.h
 *
 *  @brief This file contains a class for managing configuration files.
 *
**/

#ifndef gmsec_ConfigFile_h
#define gmsec_ConfigFile_h


#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/ConfigFileIterator.h>
#include <gmsec/Status.h>
#include <gmsec/util/List.h>
#include <gmsec/util/String.h>

#include <string>


namespace gmsec
{

// forward declare so we don't end up with recursive includes
class Config;
class Message;
class Connection;


namespace internal
{
class BaseConfigFile;
}


/** @class ConfigFile
 * @brief This class is for the managment of standard configuration files.
 *
 *  Example configuration file format:
 *  @code
 * <?xml version="1.0" encoding="UTF-8"?>
 * <DEFINITIONS>
 *	<SUBSCRIPTION NAME="events" PATTERN="GMSEC.MISSION.CONST.SAT.EVT.MSG.>"/>
 *	<SUBSCRIPTION NAME="custom1" PATTERN="CUSTOM.MESSAGE.SUBJECTS.*"/>
 * 	<CONFIG NAME="config1">
 * 		<PARAMETER NAME="connectiontype">gmsec_icsswb</PARAMETER>
 * 		<PARAMETER NAME="hostname">localhost</PARAMETER>
 * 		<PARAMETER NAME="port">10005</PARAMETER>
 * 	</CONFIG>
 * 	<CONFIG NAME="config2">
 * 		<PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
 * 		<PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
 * 		<PARAMETER NAME="tracking">true</PARAMETER>
 * 		<PARAMETER NAME="isthreaded">true</PARAMETER>
 * 		<PARAMETER NAME="compress">true</PARAMETER>
 * 	</CONFIG>
 *	<MESSAGE NAME="msg1" SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
 * 		<CONFIG NAME="msg_config">
 * 			<PARAMETER NAME="KIND">GMSEC_MSG_PUBLISH</PARAMETER>
 * 		</CONFIG>
 * 		<FIELD TYPE="CHAR" NAME="char_field">c</FIELD>
 * 		<FIELD TYPE="BOOL" NAME="bool_field">TRUE</FIELD>
 * 		<FIELD TYPE="SHORT" NAME="short_field">123</FIELD>
 * 		<FIELD TYPE="USHORT" NAME="ushort_field">123</FIELD>
 * 		<FIELD TYPE="LONG" NAME="long_field">123</FIELD>
 * 		<FIELD TYPE="ULONG" NAME="ulong_field">123</FIELD>
 * 		<FIELD TYPE="STRING" NAME="string_field">This is a test</FIELD>
 * 		<FIELD TYPE="FLOAT" NAME="float_field">123</FIELD>
 * 		<FIELD TYPE="DOUBLE" NAME="double_field">123</FIELD>
 * 		<FIELD TYPE="BIN" NAME="bin_field">4a4c4d4e4f5051</FIELD>
 * 	</MESSAGE>
 * </DEFINITIONS>
 *  @endcode
 *
 * A ConfigFile object has a one-to-many mapping to connection configurations.
 * If a user wishes to maintain only one connection per object, the use of a Config
 * object is suggested.
 *
**/
class GMSEC_API ConfigFile
{
public:
	/** @fn ConfigFile()
	 * @description Once the ConfigFile object has been created, use
	 * Load(const char* filepath) to load a configuration file, or if
	 * the plan is to add elements to the empty ConfigFile object, then
	 * use Save(const char* filepath, bool compact) to save any changes.
	 *
	 * @brief Basic constructor that creates an empty configuration file object.
	 */
	GMSEC_DEPRECATED ConfigFile();

	/** @fn ConfigFile(const char *filepath)
	 * @note This constructor has been deprecated.  Use ConfigFile() instead.
	 * @brief This constructor creates the association of this object
	 * with a configuration file.
	 *
	 * @param filepath - full or relative path to configuration file
	 */
	GMSEC_DEPRECATED ConfigFile(const char *filepath);

	/** @fn ~ConfigFile()
	 * @brief Destructor.
	 */
	~ConfigFile();

	/** @fn Load()
	 * @note This function has been deprecated. Use Load(const char* filepath) instead.
	 * @brief This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @return status of whether the load and parse was successful
	 */
	Status Load();

	/** @fn Load(const char* filepath)
	 * @brief This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param const char * filePath - The location to which the file will be saved
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
	bool IsLoaded();

	/** @fn LookupConfig( const char *name, gmsec::Config *cfg )
	 * @brief This function will attempt to find a named connection in the
	 * config file and set the passed in config object with
	 * the loaded values.
	 *
	 *  Example config file load and get config:
	 *  @code
	  * // create configfile object and attach to file
	  * ConfigFile cf("path/to/file.xml");
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
	 * ConfigFile cf("path/to/file.xml");
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
	 * Please note that passing in an existing config object will not clear the object prior to setting the new
	 * values read in from XML format. Any values that are passed in will either overwrite the values
	 * corresponding to the configuration names, or be appended to the configuration. It is suggested that users
	 * use Config::Clear() prior to calling this function. 
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
	Status Reset(ConfigFileIterator &iter);
	
	/*
	Note: The following methods may have unsafe behaviour.
	*/

	/** @fn GetSubscriptionNames (gmsec::util::List<std::string> &names)
	 * @brief Gets the list of SubscriptionNames.
	 * @note This function has been deprecated; use ConfigFileIterator::<Has>NextSubscriptionName() instead.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<string>
	 * @return status - whether successful or not
	 */
	Status GetSubscriptionNames (gmsec::util::List<std::string> &names);

	/** @fn GetSubscriptions (gmsec::util::List<std::string> &patterns)
	 * @brief Gets the list of Subscriptions.
	 * @note This function has been deprecated; use ConfigFileIterator::<Has>NextSubscription() instead.
	 * @note Function may cause unsafe behavior
	 *
	 * @param patterns - gmsec List<string>
	 * @return status - whether successful or not
	 */
	Status GetSubscriptions (gmsec::util::List<std::string> &patterns);

	/** @fn GetConfigNames (gmsec::util::List<std::string> &names)
	 * @brief Gets the list of ConfigNames.
	 * @note This function has been deprecated; use ConfigFileIterator::<Has>NextConfigName() instead.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<string>
	 * @return status - whether successful or not
	 */
	Status GetConfigNames (gmsec::util::List<std::string> &names);

	/** @fn GetConfigs (gmsec::util::List<Config *> &configList)
	 * @brief Gets the list of Configs.
	 * @note This function has been deprecated; use ConfigFileIterator::<Has>NextConfig() instead.
	 * @note Function may cause unsafe behavior
	 *
	 * @param configList - gmsec List<Config>
	 * @return status - whether successful or not
	 */
	Status GetConfigs (gmsec::util::List<Config *> &configList);

	/** @fn GetMessageNames (gmsec::util::List<std::string> &names)
	 * @brief Gets the list of MessageNames.
	 * @note This function has been deprecated; use ConfigFileIterator::<Has>NextMessageName() instead.
	 * @note Function may cause unsafe behavior
	 *
	 * @param names - gmsec List<string>
	 * @return status - whether successful or not
	 */
	Status GetMessageNames (gmsec::util::List<std::string> &names);

	/** @fn GetMessages (gmsec::Connection * connection, gmsec::util::List<gmsec::Message *> &message)
	 * @brief Gets the list of Messages.
	 * @note This function has been deprecated; use ConfigFileIterator::<Has>NextMessage() instead.
	 * @note Function may cause unsafe behavior
	 *
	 * @param message - gmsec List<message>
	 * @param connection
	 * @return status - whether successful or not
	 */
	Status GetMessages (gmsec::Connection * connection, gmsec::util::List<gmsec::Message *> &message);

	/** @fn AddSubscription (const char* name, const char* subscription)
	 * @brief Adds a Subscription definition to the list of subscriptions.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for
	 * determining the placement of elements within the document, if saved.
	 * When adding a subscription element, it will initially be inserted after
	 * the last <SUBSCRIPTION> element in the document, or as the very first
	 * child of the root element if there are no other subscription definitions.
	 *
	 * @note Adding a subscription with a name that currently exists will overwrite the current subscription.
	 * @note Adding a subscription definition to a newly instantiated ConfigFile object that has not loaded
	 * a document will create a new XML document.
	 *
	 * @param name - name to be associated with the subscription
	 * @param subscription - XML string representation of a gmsec Subscription.
	 * @return status - whether successful or not
	 */
	Status AddSubscription (const char* name, const char* subscription);

	/** @fn AddConfig (const char* name, gmsec::Config& config)
	 * @brief Adds a config definition to the list of configurations.
	 * The Config object, though passed by reference, will not be modified.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for
	 * determining the placement of elements within the document, if saved.
	 * When adding a config element, it will initially be inserted after
	 * the last <CONFIG> element in the document, if no other config definitions are
	 * present in the document, the new config definition will be added after the
	 * last <SUBSCRIPTION> element in the document, if there are no subscription
	 * definitions then it will be added as the very first child of the root element.
	 *
	 * @note Adding a config with a name that currently exists will overwrite the current configuration definition.
	 * @note Adding a config to a newly instantiated ConfigFile object that has not loaded a document
	 * will create a new XML document.
	 *
	 * @param name - name to be associated with the configuration
	 * @param config - gmsec config
	 * @return status - whether successful or not
	 */
	Status AddConfig (const char* name, gmsec::Config& config);

	/** @fn AddMessage (const char* name, gmsec::Message& message)
	 * @brief Adds a message definition to the list of messages.
	 * The Message object, though passed by reference, will not be modified.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for
	 * determining the placement of elements within the document, if saved.
	 * When adding a message element, it will initially be inserted after
	 * the last <MESSAGE> element in the document, if no other message definitions
	 * are present in the document, it will be added after the last <CONFIG> element
	 * in the document, if no other definitions are present in the document, the new
	 * config definition will be added after the last <SUBSCRIPTION> element in the
	 * document, if there are no subscription definitions then it will be added as
	 * the very first child of the root element.
	 *
	 * @note Adding a message with a name that currently exists will overwrite the current message definition.
	 * @note Adding a message definition to a newly instantiated ConfigFile object that has not loaded a document
	 * will create a new XML document.
	 *
	 * @param name - name to be associated with the message definition
	 * @param message - gmsec message
	 * @return status - whether successful or not
	 */
	Status AddMessage (const char* name, gmsec::Message& message);

	/** @fn AddCustomXML (const char * xml)
	 * @brief Adds a custom XML string to the ConfigFile.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for
	 * determining the placement of elements within the document, if saved.
	 * When adding a custom element, it will initially be inserted after
	 * the last <MESSAGE> element in the document, if no message definitions
	 * are present in the document, it will be added after the last <CONFIG> element
	 * in the document, if no other definitions are present in the document, the new
	 * config definition will be added after the last <SUBSCRIPTION> element in the
	 * document, if there are no subscription definitions then it will be added as
	 * the very first child of the root element.
	 *
	 * @note Adding a custom XML to a newly instantiated ConfigFile object that has not loaded a document
	 * will create a new XML document.
	 *
	 * @param xml - a custom, user-created XML string
	 * @return status - whether successful or not
	 */
	Status AddCustomXML (const char * xml);

	/** @fn RemoveSubscription (const char * name)
	 * @brief Removes the Subscription definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the subscription
	 * @return status - whether successful or not
	 */
	Status RemoveSubscription (const char * name);

	/** @fn RemoveConfig (const char * name)
	 * @brief Removes the Config definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the configuration definition
	 * @return status - whether successful or not
	 */
	Status RemoveConfig (const char * name);

	/** @fn RemoveMessage (const char * name)
	 * @brief Removes the Message definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the message definition
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

private:
	// not implemented
	ConfigFile &operator= (const ConfigFile &);
	ConfigFile(const ConfigFile &);

	friend class internal::BaseConfigFile;
	internal::BaseConfigFile * ptr;
};

} // namespace gmsec

#endif /* gmsec_ConfigFile_h */

