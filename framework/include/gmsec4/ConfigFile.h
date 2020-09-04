/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ConfigFile.h
 *
 *  @brief This file contains a class for managing configuration files.
 *
**/

#ifndef GMSEC_API_CONFIGFILE_H
#define GMSEC_API_CONFIGFILE_H

#include <gmsec4/util/wdllexp.h>

#include <stddef.h>   // for NULL


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Config;
	class ConfigFileIterator;
	class Message;

	namespace internal
	{
		class InternalConfigFile;
		class InternalConfigFileIterator;

		class InternalConfigEntry;
		class InternalMessageEntry;
		class InternalSubscriptionEntry;
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
	/**
	 * @class ConfigEntry
	 *
	 * @brief This class provides access to the attributes associated with a ConfigFile's %Config entry.
	 */
	class GMSEC_API ConfigEntry
	{
	public:
		/**
		 * @fn const char* getName() const
		 *
		 * @desc Returns the name associated with the %Config entry.
		 */
		const char* CALL_TYPE getName() const;

		/**
		 * @fn const Config& getConfig() const
		 *
		 * @desc Returns the Config object associated with the %Config entry.
		 */
		const Config& CALL_TYPE getConfig() const;

	private:
		friend class internal::InternalConfigFile;
		friend class internal::InternalConfigFileIterator;

		ConfigEntry();
		ConfigEntry(const ConfigEntry& other);
		~ConfigEntry();
		ConfigEntry& operator=(const ConfigEntry& other);

		void CALL_TYPE setName(const char* name);
		void CALL_TYPE setConfig(const Config& cfg);

		internal::InternalConfigEntry* m_internal;
	};


	/**
	 * @class MessageEntry
	 *
	 * @brief This class provides access to the attributes associated with a ConfigFile's %Message entry.
	 */
	class GMSEC_API MessageEntry
	{
	public:
		/**
		 * @fn const char* getName() const
		 *
		 * @desc Returns the name associated with the %Message entry.
		 */
		const char* CALL_TYPE getName() const;

		/**
		 * @fn const Message& getMessage() const
		 *
		 * @desc Returns the Message object associated with the %Message entry.
		 */
		const Message& CALL_TYPE getMessage() const;

	private:
		friend class internal::InternalConfigFile;
		friend class internal::InternalConfigFileIterator;

		MessageEntry();
		MessageEntry(const MessageEntry& other);
		~MessageEntry();
		MessageEntry& operator=(const MessageEntry& other);

		void CALL_TYPE setName(const char* name);
		void CALL_TYPE setMessage(const Message& msg);

		internal::InternalMessageEntry* m_internal;
	};


	/**
	 * @class SubscriptionEntry
	 *
	 * @brief This class provides access to the attributes associated with a ConfigFile's Subscription entry.
	 */
	class GMSEC_API SubscriptionEntry
	{
	public:
		/**
		 * @fn const char* getName() const
		 *
		 * @desc Returns the name associated with the Subscription entry.
		 */
		const char* CALL_TYPE getName() const;

		/**
		 * @fn const char* getSubject() const
		 *
		 * @desc Returns the subject/topic associated with the Subscription entry.
		 */
		const char* CALL_TYPE getSubject() const;

	private:
		friend class internal::InternalConfigFile;
		friend class internal::InternalConfigFileIterator;

		SubscriptionEntry();
		SubscriptionEntry(const SubscriptionEntry& other);
		~SubscriptionEntry();
		SubscriptionEntry& operator=(const SubscriptionEntry& other);

		void CALL_TYPE setName(const char* name);
		void CALL_TYPE setSubject(const char* subject);

		internal::InternalSubscriptionEntry* m_internal;
	};


	/** @fn ConfigFile()
	 * @description Once the ConfigFile object has been created, use
	 * Load(const char* filepath) to load a configuration file, or if
	 * the plan is to add elements to the empty ConfigFile object, then
	 * use Save(const char* filepath, bool compact) to save any changes.
	 *
	 * @brief Basic constructor that creates an empty configuration file object.
	 */
	ConfigFile();


	/** @fn ~ConfigFile()
	 * @brief Destructor.
	 */
	~ConfigFile();


	/** @fn load(const char* filepath)
	 * @brief This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param const char* filePath - The location to which the file will be saved
	 *
	 * @throws Exception if file parsing error occurs
	 */
	void CALL_TYPE load(const char* filepath);


	/** @fn save(const char* filePath, bool compact = false) const
	 * @brief This function saves the currently open configuration file to
	 * the location specified.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param const char * filePath - The location to which the file will be saved;
	 * if no path is specified or is specified as NULL, then the path, if any, that
	 * was used with Load(const char* filepath) will be used.
	 * @param compact - determines whether the XML output will be compacted or not
	 *
	 * @throws Exception if error occurs writing to the file
	 */
	void CALL_TYPE save(const char* filePath = NULL, bool compact = false) const;


	/** @fn fromXML(const char* xml)
	 * @brief This function parses the XML in the passed in string.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @throws Exception if error occurs while parsing XML string
	 */
	void CALL_TYPE fromXML(const char* xml);


	/** @fn toXML() const
	 * @brief This function produces an XML config file containing all known config file tags.
	 *
	 * @return The XML string representation of the ConfigFile object
	 */
	const char* CALL_TYPE toXML() const;


	/** @fn isLoaded()
	 * @brief This function will return true is the config file was successfully loaded
	 *
	 * @return true if file loaded
	 */
	bool CALL_TYPE isLoaded() const;


	/** @fn lookupConfig(const char* name) const
	 * @brief This function will attempt to find a named Config in the
	 * config file and set the passed in config object with
	 * the loaded values.
	 *
	 * Example config file load and get config:
	 * @code
	 * try {
	 *     ConfigFile cfgFile;
	 *
	 *     // load & parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *
	 *     // attempt to get Config from ConfigFile
	 *     try {
	 *         Config cfg = cfgFile.lookupConfig("MyConfig");
	 *
	 *         // do something with cfg
	 *     }
	 *     catch (Exception& ce) {
	 *         // handle error Config lookup error
	 *     }
	 * }
	 * catch (Exception& e) {
	 *     // handle configuration file parsing error
	 * }
	 * @endcode
	 *
	 * @param name - value in the name="" attribute of the \<CONFIG/\> block to seed the Config object
	 * @return Config object that correlates with the given name
	 * @throw Exception for cases where configuration for the given name does not exist
	 */
	Config CALL_TYPE lookupConfig(const char* name) const;


	/** @fn lookupMessage(const char* name) const
	 * @brief This function will attempt to find a named message in the
	 * config file and create a new message from that xml
	 * the loaded fields and values.
	 *
	 * Example config file load and get message:
	 * @code
	 * try {
	 *     ConfigFile cfgFile;
	 *
	 *     // load & parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *
	 *     // attempt to get Message from ConfigFile
	 *     try {
	 *         Message msg = cfgFile.lookupMessage("MyMessage");
	 *
	 *         // do something with msg
	 *     }
	 *     catch (Exception& me) {
	 *         // handle error Message lookup error
	 *     }
	 * }
	 * catch (Exception& e) {
	 *     // handle configuration file parsing error
	 * }
	 * @endcode
	 * 
	 * @param name - value in the name="" attribute of the \<MESSAGE/\> block to seed the Message object
	 * @return Message object that correlates with the given name
	 * @throw Exception for cases where message configuration for the given name does not exist
	 */
	Message CALL_TYPE lookupMessage(const char* name) const;


	/** @fn lookupSubscription(const char* name) const
	 * @brief This function will look up a subscription pattern defined in
	 * the config file. This is useful to allow easy modification of subject
	 * names without code changes.
	 *
	 * Example
	 * @code
	 * try {
	 *     ConfigFile cfgFile;
	 *     const char* pattern = 0;
	 *
	 *     // load & parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *
	 *     // get Message from file
	 *     const char* pattern = cfgFile.lookupSubscription("MySubscription");
	 *     if (pattern) {
	 *         // success
	 *     }
	 *     else {
	 *         // handle subscription pattern lookup error
	 *     }
	 * }
	 * catch (Exception& e) {
	 *     // handle configuration file parsing error
	 * }
	 * @endcode
	 *
	 * @param name - value in the name="" attribute of the \<SUBSCRIPTION/\> block containing the subscription pattern
	 * @return subject pattern topic associated with SUBSCRIPTION block, or NULL if not found
	 */
	const char* CALL_TYPE lookupSubscription(const char* name) const;


	/** @fn addSubscription(const char* name, const char* subscription)
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
	 *
	 * @throws Exception if either name or subscription parameters are NULL or if either contains an empty string
	 */
	void CALL_TYPE addSubscription(const char* name, const char* subscription);


	/** @fn addConfig(const char* name, const Config& config)
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
	 *
	 * @throws Exception if name is NULL or contains an empty string
	 */
	void CALL_TYPE addConfig(const char* name, const Config& config);


	/** @fn addMessage(const char* name, const Message& message)
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
	 *
	 * @throws Exception if name is NULL or contains an empty string
	 */
	void CALL_TYPE addMessage(const char* name, const Message& message);


	/** @fn addCustomXML(const char* xml)
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
	 *
	 * @throws Exception if XML string is NULL, contains an empty string, or if an XML parsing error occurs
	 */
	void CALL_TYPE addCustomXML(const char* xml);


	/** @fn removeSubscription(const char* name)
	 * @brief Removes the Subscription definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the subscription
	 * @return whether successful or not
	 */
	bool CALL_TYPE removeSubscription(const char* name);


	/** @fn removeConfig(const char* name)
	 * @brief Removes the Config definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the configuration definition
	 * @return whether successful or not
	 */
	bool CALL_TYPE removeConfig(const char* name);


	/** @fn removeMessage(const char* name)
	 * @brief Removes the Message definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the message definition
	 * @return whether successful or not
	 */
	bool CALL_TYPE removeMessage(const char* name);


	/** @fn removeCustomXML(const char* xml)
	 * @brief Removes the provided xml element.
	 *
	 * @param xml - xml element string matching the custom xml definition
	 * @return whether successful or not
	 *
	 * @throws Exception if XML string is NULL, contains an empty string, or if an XML parsing error occurs
	 */
	bool CALL_TYPE removeCustomXML(const char* xml);


	/**
	 * @fn ConfigFileIterator& getIterator() const
	 *
	 * @desc Method that allows the callee to get the ConfigFileIterator associated with the %ConfigFile.
	 * This iterator will allow for applications to iterate over the entries stored within the %ConfigFile.
	 * The iterator is reset each time getIterator() is called.  The iterator itself is destroyed when the
	 * %ConfigFile object is destroyed.
	 *
	 * @note Only one ConfigFileIterator object is associated with a %ConfigFile object; multiple calls to
	 * getIterator() will return a reference to the same ConfigFileIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a ConfigFileIterator object.
	 */
	ConfigFileIterator& CALL_TYPE getIterator() const;


private:
	friend class gmsec::api::internal::InternalConfigFileIterator;

	// defined, but not implemented
	ConfigFile(const ConfigFile&);
	ConfigFile& operator=(const ConfigFile&);


	internal::InternalConfigFile* m_internal;
};

} // namespace api
} // namespace gmsec

#endif
