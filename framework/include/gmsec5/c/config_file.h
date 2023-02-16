/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config_file.h
 *
 * @brief This file contains functions for interfacing with a ConfigFile object.
 */


#ifndef GMSEC_API5_C_CONFIG_FILE_H
#define GMSEC_API5_C_CONFIG_FILE_H

#include <gmsec5_defs.h>

#include <gmsec5/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_ConfigFile configFileCreate(GMSEC_Status status)
	 *
	 * @brief Creates a ConfigFile object devoid of any definitions.
	 * The internally held MessageFactory will be configured with the default message specification used by the API.
	 *
	 * param[out] status - the result of the operation
	 *
	 * @return A handle to a ConfigFile object
	 */
	GMSEC_API GMSEC_ConfigFile configFileCreate(GMSEC_Status status);


	/**
	 * @fn GMSEC_ConfigFile configFileCreateUsingConfig(GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief Creates a ConfigFile object devoid of any definitions.
	 * The internally held MessageFactory will be configured with the given configuration.
	 *
	 * Configuration options referenced:
	 * <ul>
	 *   <li>gmsec-specification-level</li>
	 *   <li>gmsec-schema-level</li>
	 *   <li>gmsec-msg-content-validate</li>
	 * </ul>
	 *
	 * @param[in] config - the configuration to apply to the internally held MessageFactory
	 * param[out] status - the result of the operation
	 *
	 * @return A handle to a ConfigFile object
	 */
	GMSEC_API GMSEC_ConfigFile configFileCreateUsingConfig(GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn void configFileDestroy(GMSEC_ConfigFile* cfgFile)
	 *
	 * @brief Destroys the ConfigFile object.
	 *
	 * @param[in,out] cfgFile - the handle to the ConfigFile object to destroy.
	 */
	GMSEC_API void configFileDestroy(GMSEC_ConfigFile* cfgFile);


	/**
	 * @fn void configFileLoad(GMSEC_ConfigFile cfgFile, const char* filePath, GMSEC_Status status)
	 *
	 * @brief Opens the configuration file and parses the XML.
	 * It will return an error explaining what, if anything, went wrong.
	 *
	 * param[in]  cfgFile  - the handle to the ConfigFile object
	 * param[in]  filePath - the absolute path to the configuration file to load.
	 * param[out] status   - the result of the operation
	 */
	GMSEC_API void configFileLoad(GMSEC_ConfigFile cfgFile, const char* filePath, GMSEC_Status status);


	/**
	 * @fn void configFileSave(GMSEC_ConfigFile cfgFile, GMSEC_Status status)
	 *
	 * @brief Saves the currently open configuration file.
	 * It will return an error explaining what, if anything, went wrong.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void configFileSave(GMSEC_ConfigFile cfgFile, GMSEC_Status status);


	/**
	 * @fn void configFileSaveToFile(GMSEC_ConfigFile cfgFile, const char* filePath, GMSEC_BOOL compact, GMSEC_Status status)
	 *
	 * @brief Saves the currently open configuration file to the location specified.
	 * It will return an error explaining what, if anything, went wrong.
	 *
	 * @param[in]  cfgFile  - the handle to the ConfigFile object
	 * @param[in]  filePath - the absolute path to which to save the configuration file
	 * @param[out] status   - the result of the operation
	 */
	GMSEC_API void configFileSaveToFile(GMSEC_ConfigFile cfgFile, const char* filePath, GMSEC_BOOL compact, GMSEC_Status status);


	/**
	 * @fn void configFileFromXML(GMSEC_ConfigFile cfgFile, const char* xml, GMSEC_Status status)
	 *
	 * @brief Parses the XML content from the given string to form a ConfigFile object.
	 * It will return an error explaining what, if anything, went wrong.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  xml     - the XML data to parse
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void configFileFromXML(GMSEC_ConfigFile cfgFile, const char* xml, GMSEC_Status status);


	/**
	 * @fn const char* configFileToXML(GMSEC_ConfigFile cfgFile, GMSEC_Status status)
	 *
	 * @brief Returns the XML representation of the ConfigFile object as a string.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[out] status  - the result of the operation
	 *
	 * @return An XML string representation of the configuration file.
	 */
	GMSEC_API const char* configFileToXML(GMSEC_ConfigFile cfgFile, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configFileIsLoaded(GMSEC_ConfigFile cfgFile, GMSEC_Status status)
	 *
	 * @brief Reports whether a configuration has been loaded into the ConfigFile.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[out] status  - the result of the operation
	 *
	 * @return GMSEC_TRUE if a configuration has been loaded; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL configFileIsLoaded(GMSEC_ConfigFile cfgFile, GMSEC_Status status);


	/**
	 * @fn void configFileAddConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Config config, GMSEC_Status status)
	 *
	 * @brief Adds the given Config definition to the list of configurations. The Config object will not be modified.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for determining the placement of elements within the
	 * document, if saved. When adding a config element, it will initially be inserted after the last <CONFIG> element in the document,
	 * if no other config definitions are present in the document, the new config definition will be added after the last <SUBSCRIPTION>
	 * element in the document, if there are no subscription definitions then it will be added as the very first child of the root element.
	 *
	 * @note Adding a config with a name that currently exists will overwrite the current configuration definition. 
	 * @note Adding a config to a newly instantiated ConfigFile object that has not loaded a document will create a new XML document.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  name    - the name to be associated with the Config object
	 * @param[in]  config  - the handle to the Config object to add
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void configFileAddConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Config config, GMSEC_Status status);


	/**
	 * @fn GMSEC_Config configFileLookupConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the named Config within the ConfigFile.
	 * It will return an error explaining what, if anything, went wrong.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  name    - the name of the configuration object to reference
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to the found Config object, or NULL if not found.  For the latter case, refer to the status for the error reason.
	 *
	 * @note It is the responsibility of the user to call configDestroy() when the config handle is no longer needed.
	 *
	 * @sa configDestroy
	 */
	GMSEC_API GMSEC_Config configFileLookupConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configFileRemoveConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
	 *
	 * @brief Removes the Config definition which is mapped to the provided name.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  name    - the name associated with the configuration definition
	 * @param[out] status  - the result of the operation
	 *
	 * @return GMSEC_TRUE if the operation was successful; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL configFileRemoveConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status);


	/**
	 * @fn void configFileAddMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Message msg, GMSEC_Status status)
	 *
	 * @brief Adds the given Message definition to the list of messages. The Message object will not be modified.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for determining the placement of elements within the
	 * document, if saved. When adding a message element, it will initially be inserted after the last <MESSAGE> element in the document,
	 * if no other message definitions are present in the document, it will be added after the last <CONFIG> element in the document, if
	 * no other definitions are present in the document, the new config definition will be added after the last <SUBSCRIPTION> element
	 * in the document, if there are no subscription definitions then it will be added as the very first child of the root element.
	 *
	 * @note Adding a message with a name that currently exists will overwrite the current message definition. 
	 * @note Adding a message definition to a newly instantiated ConfigFile object that has not loaded a document will create a new XML document.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  name    - the name to associate with the Message object
	 * @param[in]  msg     - the handle to the Message object
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void configFileAddMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Message msg, GMSEC_Status status);


	/**
	 * @fn GMSEC_Message configFileLookupMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the named Message within the ConfigFile.
	 * It will return an error explaining what, if anything, went wrong.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  name    - the name of the message object to reference
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to the found Message object, or NULL if not found.  For the latter case, refer to the status for the error reason.
	 *
	 * @note It is the responsibility of the user to call messageDestroy() when the message handle is no longer needed.
	 *
	 * @sa messageDestroy
	 */
	GMSEC_API GMSEC_Message configFileLookupMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configFileRemoveMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
	 *
	 * @brief Removes the Message definition which is mapped to the provided name.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  name    - the name associated with the message definition
	 * @param[out] status  - the result of the operation
	 *
	 * @return GMSEC_TRUE if the operation was successful; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL configFileRemoveMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status);


	/**
	 * @fn void configFileAddSubscriptionEntry(GMSEC_ConfigFile cfgFile, GMSEC_SubscriptionEntry entry, GMSEC_Status status)
	 *
	 * @brief Adds a SubscriptionEntry definition to the list of subscriptions.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for determining the placement of elements within
	 * the document, if saved. When adding a subscription element, it will initially be inserted after the last <SUBSCRIPTION>
	 * element in the document, or as the very first child of the root element if there are no other subscription definitions.
	 *
	 * @note Adding a subscription with a name that currently exists will overwrite the current subscription. 
	 * @note Adding a subscription definition to a newly instantiated ConfigFile object that has not loaded a document will create a new XML document.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  entry   - the SubscripitionEntry to associate with the ConfigFile object
	 * @param[out] status  - the result of the operation
	 */
	GMSEC_API void configFileAddSubscriptionEntry(GMSEC_ConfigFile cfgFile, GMSEC_SubscriptionEntry entry, GMSEC_Status status);


	/**
	 * @fn GMSEC_SubscriptionEntry configFileLookupSubscriptionEntry(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the named SubscriptionEntry within the ConfigFile.
	 * It will return an error explaining what, if anything, went wrong.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  name    - the name of the configuration object to reference
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to the found Config object, or NULL if not found.  For the latter case, refer to the status for the error reason.
	 */  
	GMSEC_API GMSEC_SubscriptionEntry configFileLookupSubscriptionEntry(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configFileRemoveSubscriptionEntry(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
	 *
	 * @brief Removes the SubscriptionEntry definition which is mapped to the provided name.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[in]  name    - the name associated with the subscription
	 * @param[out] status  - the result of the operation
	 *
	 * @return GMSEC_TRUE if the operation was successful; GMSEC_FALSE otherwise.
	 */
	GMSEC_API GMSEC_BOOL configFileRemoveSubscriptionEntry(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status);


	/**
	 * @fn GMSEC_ConfigFileIterator configFileGetIterator(GMSEC_ConfigFile cfgFile, GMSEC_Status status)
	 *
	 * @brief Returns the ConfigFileIterator which can be used to iteratate over the elements of the ConfigFile.
	 *
	 * @param[in]  cfgFile - the handle to the ConfigFile object
	 * @param[out] status  - the result of the operation; operation will always succeed if valid ConfigFile handle is provided.
	 *
	 * @return A handle to a ConfigFileIterator object.
	 */
	GMSEC_API GMSEC_ConfigFileIterator configFileGetIterator(GMSEC_ConfigFile cfgFile, GMSEC_Status status);


	/**
	 * Functions related to SubscriptionEntry
	 */

	/**
	 * @fn GMSEC_SubscriptionEntry subscriptionEntryCreate(const char* name, const char* pattern, GMSEC_Status status)
	 *
	 * @brief Creates an instance of a GMSEC_SubscriptionEntry object.
	 *
	 * @param[in]  name    - the name to associate with the subscription
	 * @param[in]  pattern - the subscription pattern/topic
	 * @param[out] status  - the result of the operation
	 *
	 * @return A handle to a SubscriptionEntry object.
	 */
	GMSEC_API GMSEC_SubscriptionEntry subscriptionEntryCreate(const char* name, const char* pattern, GMSEC_Status status);


	/**
	 * @fn void subscriptionEntryDestroy(GMSEC_SubscriptionEntry* entry)
	 *
	 * @brief Destroys the SubscriptionEntry object.
	 *
	 * @note This function should *not* be used to destroy SubscriptionEntry objects returned by ConfigFile.
	 */
	GMSEC_API void subscriptionEntryDestroy(GMSEC_SubscriptionEntry* entry);


	/**
	 * @fn const char* subscriptionEntryGetName(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status)
	 *
	 * @brief Returns the name of a subscription entry object
	 *
	 * @param[in]  subEntry - the handle to the SubscriptionEntry object
	 * @param[out] status   - the result of the operation
	 * 
	 * @return The name of a SubscriptionEntry object
	 */
	GMSEC_API const char* subscriptionEntryGetName(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status);


	/**
	 * @fn const char* subscriptionEntryGetPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status)
	 *
	 * @brief Returns the subscription pattern/topic for a subscription entry object
	 *
	 * @param[in]  subEntry - the handle to the SubscriptionEntry object
	 * @param[out] status   - the result of the operation
	 * 
	 * @return The pattern of a SubscriptionEntry object
	 */
	GMSEC_API const char* subscriptionEntryGetPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status);


	/**
	 * @fn void subscriptionEntryAddExcludedPattern(GMSEC_SubscriptionEntry subEntry, const char* pattern, GMSEC_Status status)
	 *
	 * @brief Adds an excluded subject pattern/topic to the SubscriptionEntry.
	 *
	 * @param[in]  subEntry - the handle to the SubscriptionEntry object
	 * @param[in]  pattern  - the subscription pattern/topic to exclude
	 * @param[out] status   - the result of the operation
	 */
	GMSEC_API void subscriptionEntryAddExcludedPattern(GMSEC_SubscriptionEntry subEntry, const char* pattern, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL subscriptionEntryHasNextExcludedPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status)
	 *
	 * @brief Returns true if there is a next excluded pattern in a subscription entry object
	 *
	 * @param[in]  subEntry - the handle to the SubscriptionEntry object
	 * @param[out] status   - the result of the operation
	 *
	 * @return GMSEC_BOOL value to indicate whether or not there is a next excluded pattern
	 */
	GMSEC_API GMSEC_BOOL subscriptionEntryHasNextExcludedPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status);


	/**
	 * @fn const char* subscriptionEntryNextExcludedPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status)
	 *
	 * @brief Returns the next excluded pattern in a subscription entry object
	 *
	 * @param[in]  subEntry - the handle to the SubscriptionEntry object
	 * @param[out] status   - the result of the operation
	 * 
	 * @return The next excluded pattern
	 */
	GMSEC_API const char* subscriptionEntryNextExcludedPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
