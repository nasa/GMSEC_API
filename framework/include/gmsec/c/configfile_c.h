
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file configfile_c.h
 *
 *  @brief This file contains function for handling generic configuration files.
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
 *	</CONFIG>
 *	<MESSAGE NAME="msg1" SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
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
 *
**/

#ifndef gmsec_c_configfile_c_h
#define gmsec_c_configfile_c_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn CreateConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, const char *filepath, GMSEC_STATUS_HANDLE status)
	 * @brief Create a ConfigFile and associate a file.  The created status MUST be cleaned up with DestroyConfigFile()
	 * when no longer needed.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] cfgfile - ConfigFile handle to be created
	 * @param[in]  filepath - full path to file to load & parse
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa DestroyConfigFile()
	 */
	GMSEC_DEPRECATED GMSEC_API void CreateConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, const char *filepath, GMSEC_STATUS_HANDLE status);

	/** @fn DestroyConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief Reclaim / cleanup a ConfigFile
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa CreateConfigFile()
	 */
	GMSEC_DEPRECATED GMSEC_API void DestroyConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, GMSEC_STATUS_HANDLE status);

	/** @fn LoadConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will cause the file to be loaded & parsed
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void LoadConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status);

	/** @fn LoadConfigFilePath(GMSEC_CONFIGFILE_HANDLE cfgfile, const char* filePath, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will cause the file to be loaded & parsed
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  filePath - The relative or absolute path to the file to load
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void LoadConfigFilePath(GMSEC_CONFIGFILE_HANDLE cfgfile, const char* filePath, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileSave(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *filePath, GMSEC_BOOL gbCompact, GMSEC_STATUS_HANDLE status);
	 * 
	 * @brief This function saves the currently open configuration file to
	 * the location specified.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param[in]  cfgfile - ConfigFile Handle
	 * @param[in]  filePath - The location to which the file will be saved; 
     * if the path is specified as NULL, then the path, if any, that
     * was used with Load(const char* filepath) will be used when saving the file.
	 * @param[in]  compact - determines whether the XML output will be compacted or not
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileSave(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *filePath, GMSEC_BOOL gbCompact, GMSEC_STATUS_HANDLE status);

	/** @fn FromXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will parse the xml string as a config file
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] cfgfile - ConfigFile handle
	 * @param[in]  xml - string xml input
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void FromXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status);

	/** @fn ToXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char **xml, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will output the config file in xml format
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile Handle
	 * @param[out] xml - string xml output
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ToXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char **xml, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileIsLoaded(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_BOOL loaded);
	 * 
	 * @brief This function will return true is the config file was successfully loaded
	 *
	 * @param[in]  cfgfile - ConfigFile Handle
	 * @param[out] GMSEC_BOOL - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_BOOL ConfigFileIsLoaded(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_BOOL loaded);

	/** @fn LookupConfigFileConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_CONFIG_HANDLE cfg, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will seed a config with values from a named \<CONFIG/\> block in the config file
	 *
	 *  Example config file load & get config:
	 *  @code
	 *
	 * // create configfile
	 * CreateConfigFile(&cfgfile, "path/to/file.xml");
	 * if( isStatusError(result) )
	 *	// handle error
	 *
	 * // load & parse
	 * LoadConfigFile(cfgfile,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * // create config
	 * CreateConfig(&cfg,result);
	 * if( isStatusError(result) )
	 *	// handle error
	 *
	 * // get config from file
	 * LookupConfigFileConfig(cfgfile,"config1",cfg,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *  @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile Handle
	 * @param[in]  name - value of name="" attribute in \<CONFIG/\> block to be loaded
	 * @param[out] cfg - config that has already been created with CreateConfig()
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void LookupConfigFileConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_CONFIG_HANDLE cfg, GMSEC_STATUS_HANDLE status);

	/** @fn LookupConfigFileMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will seed a message with values from a named \<MESSAGE/\> block in the config file
	 *
	 *  Example config file load & get message:
	 *  @code
	 *
	 * // create configfile
	 * CreateConfigFile(&cfgfile, "path/to/file.xml");
	 * if( isStatusError(result) )
	 *	// handle error
	 *
	 * // load & parse
	 * LoadConfigFile(cfgfile,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * // create message
	 * CreateMessageDflt(conn,&msg,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * // Get Message from ConfigFile
	 * LookupConfigFileMessage(cfgfile,"msg1",msg,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *  @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile Handle
	 * @param[in]  name - value of name="" attribute in \<MESSAGE/\> block to be loaded
	 * @param[out] msg - message that has already been created with CreateMessage()
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void LookupConfigFileMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status);

	/** @fn LookupConfigFileSubscription( GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, const char **pattern, GMSEC_STATUS_HANDLE status )
	 * @brief This function will look up a subscription pattern defined in
	 * the config file. This is usefull to allow easy modification of subject
	 * names without code changes.
	 *
	 * Example
	 * @code
	 * char *pattern = NULL;
	 *
	 * // lookup subject pattern by name
	 * LookupConfigFileSubscription( cfgfile, "events", &pattern, result );
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * printf("Subscribe to: %s\n",pattern);
	 *
	 * // subscribe on the connection
	 * Subscribe( conn, pattern, result );
	 * if( isStatusError(result) )
	 *	//handle error
	 * @endcode
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  name - value in the name="" attribute of the \<SUBSCRIPTION/\> block containing the subscription pattern
	 * @param[out] pattern - out parameter, the pattern defined in the config file
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void LookupConfigFileSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, const char **pattern, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileAddSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status, const char *subscription, const char *name);
	 *
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
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  name - name to be associated with the subscription
	 * @param[in]  subscription - XML string representation of a gmsec Subscription.
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileAddSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, const char *subscription, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileAddMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status, GMSEC_MESSAGE_HANDLE message, const char *name);
	 * 
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
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  name - name to be associated with the message definition
	 * @param[in]  message - gmsec message
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileAddMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_MESSAGE_HANDLE message, GMSEC_STATUS_HANDLE status);

	/** ConfigFileAddConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status, GMSEC_CONFIG_HANDLE config, const char *name);
	 *
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
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  name - name to be associated with the configuration
	 * @param[in]  config - gmsec config
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileAddConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_CONFIG_HANDLE config, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileAddCustomXML(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status, const char *xml);
	 *
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
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  xml - a custom, user-created XML string
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileAddCustomXML(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileRemoveSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status, const char *name);
	 * 
	 * @brief Removes the Subscription definition which is mapped to the provided name.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  name - name associated with the subscription
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileRemoveSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileRemoveConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status, const char *name);
	 *
	 * @brief Removes the Config definition which is mapped to the provided name.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  name - name associated with the configuration definition
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileRemoveConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileRemoveMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status, const char *name);
	 *
	 * @brief Removes the Message definition which is mapped to the provided name.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  name - name associated with the message definition
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileRemoveMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFileRemoveCustomXML(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status, const char *xml);
	 * @brief Removes the provided xml element.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  xml - xml element string matching the custom xml definition
	 * @param[out] status - out parameter, operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFileRemoveCustomXML(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_c_configfile_c_h */


