/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFile.h
 *
 *  @brief This file contains a class for managing configuration files.
 */

#ifndef GMSEC_API5_CONFIGFILE_H
#define GMSEC_API5_CONFIGFILE_H

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>

#include <gmsec5/util/wdllexp.h>

#include <stddef.h>   // for NULL


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class ConfigFileIterator;

	namespace internal
	{
		class InternalConfigFile;
		class InternalConfigFileIterator;

		class InternalConfigEntry;
		class InternalMessageEntry;
		class InternalSubscriptionEntry;
	}


/**
 * @class ConfigFile
 *
 * @brief This class is for the managment of standard configuration files.
 *
 * Example configuration file format:
 * @code
 * <?xml version="1.0" encoding="UTF-8"?>
 *
 * <DEFINITIONS>
 *     <SUBSCRIPTION NAME="all-messages" PATTERN="GMSEC.*.CONST.SAT.>">
 *         <EXCLUDE PATTERN="GMSEC.*.CONST.SAT.MSG.C2CX.>"/>
 *         <EXCLUDE PATTERN="GMSEC.*.CONST.SAT.MSG.LOG.*"/>
 *     </SUBSCRIPTION>
 *
 *     <SUBSCRIPTION NAME="custom1" PATTERN="CUSTOM.MESSAGE.SUBJECTS.*"/>
 *
 *     <CONFIG NAME="config1">
 *         <PARAMETER NAME="connectionType">gmsec_activemq394</PARAMETER>
 *         <PARAMETER NAME="server">tcp://myserver:61616</PARAMETER>
 *     </CONFIG>
 *
 *     <CONFIG NAME="config2">
 *         <PARAMETER NAME="mw-id">bolt</PARAMETER>
 *         <PARAMETER NAME="server">10.1.2.159</PARAMETER>
 *         <PARAMETER NAME="tracking">false</PARAMETER>
 *         <PARAMETER NAME="compress">true</PARAMETER>
 *     </CONFIG>
 *
 *     <MESSAGE NAME="msg1" SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
 *         <CONFIG NAME="msg_config">
 *             <PARAMETER NAME="GMSEC-MSGFLD-STORE-SIZE">15</PARAMETER>
 *             <PARAMETER NAME="GMSEC-MSGFLD-STORE-TYPE">tree</PARAMETER>
 *         </CONFIG>
 *         <FIELD NAME="BIN-FIELD" TYPE="BIN">4A4C4D4E4F5051</FIELD>
 *         <FIELD NAME="BOOL-FIELD-FALSE" TYPE="BOOL">FALSE</FIELD>
 *         <FIELD NAME="CHAR-FIELD" TYPE="CHAR">c</FIELD>
 *         <FIELD NAME="COUNT" TYPE="I32">0</FIELD>
 *         <FIELD NAME="F32-FIELD" TYPE="F32" BITS="40000000">2</FIELD>
 *         <FIELD NAME="F64-FIELD" TYPE="F64" BITS="4000000000000000">2</FIELD>
 *         <FIELD NAME="I16-FIELD" TYPE="I16">1</FIELD>
 *         <FIELD NAME="I32-FIELD" TYPE="I32">1</FIELD>
 *         <FIELD NAME="I64-FIELD" TYPE="I64">1</FIELD>
 *         <FIELD NAME="I8-FIELD" TYPE="I8">1</FIELD>
 *         <FIELD NAME="STRING-FIELD" TYPE="STRING">This is a test</FIELD>
 *         <FIELD NAME="U16-FIELD" TYPE="U16">1</FIELD>
 *         <FIELD NAME="U32-FIELD" TYPE="U32">1</FIELD>
 *         <FIELD NAME="U64-FIELD" TYPE="U64">1</FIELD>
 *         <FIELD NAME="U8-FIELD" TYPE="U8">1</FIELD>
 *     </MESSAGE>
 * </DEFINITIONS>
 * @endcode
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
		/** @cond
		 *  Copy constructor.
		 */
		ConfigEntry(const ConfigEntry& other);
		/** @endcond */

		/** @cond
		 *  Destructor.
		 */
		~ConfigEntry();
		/** @endcond */

		/** @cond
		 *  Overloaded assignment operator.
		 */
		ConfigEntry& CALL_TYPE operator=(const ConfigEntry& other);
		/** @endcond */

		/**
		 * @fn const char* getName() const
		 *
		 * @brief Returns the name associated with the %Config entry.
		 */
		const char* CALL_TYPE getName() const;

		/**
		 * @fn const Config& getConfig() const
		 *
		 * @brief Returns the Config object associated with the %Config entry.
		 */
		const Config& CALL_TYPE getConfig() const;

	private:
		friend class internal::InternalConfigFile;
		friend class internal::InternalConfigFileIterator;

		ConfigEntry();

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
		/** @cond
		 *  Copy constructor.
		 */
		MessageEntry(const MessageEntry& other);
		/** @endcond */

		/** @cond
		 *  Destructor.
		 */
		~MessageEntry();
		/** @endcond */

		/** @cond
		 *  Overloaded assignment operator.
		 */
		MessageEntry& CALL_TYPE operator=(const MessageEntry& other);
		/** @endcond */

		/**
		 * @fn const char* getName() const
		 *
		 * @brief Returns the name associated with the %Message entry.
		 */
		const char* CALL_TYPE getName() const;

		/**
		 * @fn const Message& getMessage() const
		 *
		 * @brief Returns the Message object associated with the %Message entry.
		 */
		const Message& CALL_TYPE getMessage() const;

	private:
		friend class internal::InternalConfigFile;
		friend class internal::InternalConfigFileIterator;

		MessageEntry();

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
		 * @fn SubscriptionEntry()
		 *
		 * @brief Basic constructor.
		 */
		SubscriptionEntry(const char* name, const char* pattern);

		/**
		 * @fn SubscriptionEntry(const SubscriptionEntry& other)
		 *
		 * @brief Copy constructor.
		 *
		 * @param other - The %SubscriptionEntry object to copy.
		 */
		SubscriptionEntry(const SubscriptionEntry& other);

		/**
		 * @fn ~SubscriptionEntry()
		 *
		 * @brief Destructor.
		 */
		~SubscriptionEntry();

		/**
		 * @fn SubscriptionEntry& operator=(const SubscriptionEntry& other)
		 *
		 * @brief Overloaded assignment operator.
		 *
		 * @param other - The %SubscriptionEntry object to copy.
		 */
		SubscriptionEntry& CALL_TYPE operator=(const SubscriptionEntry& other);

		/**
		 * @fn const char* getName() const
		 *
		 * @brief Returns the name associated with the Subscription entry.
		 */
		const char* CALL_TYPE getName() const;

		/**
		 * @fn const char* getPattern() const
		 *
		 * @brief Returns the pattern associated with the Subscription entry.
		 */	 
		const char* CALL_TYPE getPattern() const;

		/**
		 * @fn void CALL_TYPE addExcludedPattern(const char* pattern)
		 *
		 * @brief Adds an excluded subject pattern/topic to the %SubscriptionEntry.
		 *
		 * @param pattern - A subject pattern/topic to exclude.
		 *
		 * @throw GmsecException is thrown if the pattern is NULL or is an empty string.
		 */
		void CALL_TYPE addExcludedPattern(const char* pattern);

		/**
		 * @fn bool hasNextExcludedPattern() const
		 *
		 * @brief  Returns whether or not there is a next excluded pattern.
		 */ 
		bool CALL_TYPE hasNextExcludedPattern() const;

		/**
		 * @fn const char* nextExcludedPattern() const
		 *
		 * @brief Returns next excluded pattern associated with the Subscription entry
		 */ 
		const char* CALL_TYPE nextExcludedPattern() const;

	private:
		friend class gmsec::api5::internal::InternalConfigFileIterator;

		SubscriptionEntry();

		internal::InternalSubscriptionEntry* m_internal;
	};


	/**
	 * @fn ConfigFile()
	 *
	 * @brief Constructor that is devoid of any definitions.
	 * The internally held MessageFactory will be configured with the default message specification used by the API.
	 *
	 * @throw GmsecException is thrown if the MessageFactory cannot be instantiated.
	 */
	ConfigFile();


	/**
	 * @fn ConfigFile(const Config& config)
	 *
	 * @brief Constructor that is devoid of any definitions.
	 * The internally held MessageFactory will be configured with the given configuration.
	 *
	 * Configuration options referenced:
	 * <ul>
	 *   <li>gmsec-specification-level</li>
	 *   <li>gmsec-schema-level</li>
	 *   <li>gmsec-msg-content-validate</li>
	 * </ul>
	 *
	 * @param config - The configuration to apply to the internally held %MessageFactory.
	 *
	 * @throw GmsecException is thrown if the MessageFactory cannot be instantiated.
	 */
	ConfigFile(const Config& config);


	/**
	 * @fn ~ConfigFile()
	 * @brief Destructor.
	 */
	~ConfigFile();


	/**
	 * @fn load(const char* filepath)
	 *
	 * @brief This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param const char* filePath - The location to which the file will be saved
	 *
	 * @throw GmsecException is thrown if file parsing error occurs
	 */
	void CALL_TYPE load(const char* filepath);


	/**
	 * @fn save(const char* filePath, bool compact = false) const
	 *
	 * @brief This function saves the currently open configuration file to
	 * the location specified.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param const char * filePath - The location to which the file will be saved;
	 * if no path is specified or is specified as NULL, then the path, if any, that
	 * was used with Load(const char* filepath) will be used.
	 * @param compact - determines whether the XML output will be compacted or not
	 *
	 * @throw GmsecException is thrown if error occurs writing to the file
	 */
	void CALL_TYPE save(const char* filePath = NULL, bool compact = false) const;


	/**
	 * @fn fromXML(const char* xml)
	 *
	 * @brief This function parses the XML in the passed in string.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @throw GmsecException is thrown if error occurs while parsing XML string
	 */
	void CALL_TYPE fromXML(const char* xml);


	/**
	 * @fn toXML() const
	 *
	 * @brief This function produces an XML config file containing all known config file tags.
	 *
	 * @return The XML string representation of the ConfigFile object
	 */
	const char* CALL_TYPE toXML() const;


	/**
	 * @fn isLoaded()
	 *
	 * @brief This function will return true is the config file was successfully loaded
	 *
	 * @return true if file loaded
	 */
	bool CALL_TYPE isLoaded() const;


	/**
	 * @fn addConfig(const char* name, const Config& config)
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
	 * @param name - name to be associated with the configuration
	 * @param config - gmsec config
	 *
	 * @throw GmsecException is thrown if name is NULL or contains an empty string
	 */
	void CALL_TYPE addConfig(const char* name, const Config& config);


	/**
	 * @fn lookupConfig(const char* name) const
	 *
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
	 *     catch (const GmsecException& ce) {
	 *         // handle error Config lookup error
	 *     }
	 * }
	 * catch (const GmsecException& e) {
	 *     // handle configuration file parsing error
	 * }
	 * @endcode
	 *
	 * @param name - value in the name="" attribute of the \<CONFIG/\> block to seed the Config object
	 *
	 * @return Config object that correlates with the given name
	 *
	 * @throw GmsecException is thrown for cases where configuration for the given name does not exist
	 */
	Config CALL_TYPE lookupConfig(const char* name) const;


	/**
	 * @fn removeConfig(const char* name)
	 *
	 * @brief Removes the Config definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the configuration definition
	 *
	 * @return whether successful or not
	 */
	bool CALL_TYPE removeConfig(const char* name);


	/**
	 * @fn addMessage(const char* name, const Message& message)
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
	 * @param name - name to be associated with the message definition
	 * @param message - gmsec message
	 *
	 * @throw GmsecException is thrown if name is NULL or contains an empty string
	 */
	void CALL_TYPE addMessage(const char* name, const Message& message);


	/**
	 * @fn lookupMessage(const char* name) const
	 *
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
	 *     catch (const GmsecException& me) {
	 *         // handle error Message lookup error
	 *     }
	 * }
	 * catch (const GmsecException& e) {
	 *     // handle configuration file parsing error
	 * }
	 * @endcode
	 * 
	 * @param name - value in the name="" attribute of the \<MESSAGE/\> block to seed the Message object
	 *
	 * @return Message object that correlates with the given name
	 *
	 * @throw GmsecException is thrown for cases where message configuration for the given name does not exist
	 */
	Message CALL_TYPE lookupMessage(const char* name) const;


	/**
	 * @fn removeMessage(const char* name)
	 *
	 * @brief Removes the Message definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the message definition
	 *
	 * @return whether successful or not
	 */
	bool CALL_TYPE removeMessage(const char* name);


	/**
	 * @fn addSubscriptionEntry(const SubscriptionEntry& entry)
	 *
	 * @brief Adds a SubscriptionEntry definition to the list of subscriptions.
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
	 * @param entry - A SubscriptionEntry object.
	 */
	void CALL_TYPE addSubscriptionEntry(const SubscriptionEntry& entry);


	/**
	 * @fn lookupSubscriptionEntry(const char* name) const
	 *
	 * @brief This function will look up a subscription entry pattern defined in
	 * the config file. This is useful to allow easy modification of subject
	 * names without code changes.
	 *
	 * Example
	 * @code
	 * try {
	 *     ConfigFile cfgFile;
	 *
	 *     // load & parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *
	 *     // attempt to acquire Subscription Entry
	 *     try {
	 *         const ConfigFile::SubscriptionEntry& entry = cfgFile.lookupSubscriptionEntry("MySubscription");
	 *
	 *         // entry found!
	 *         // ...
	 *     }
	 *     catch (const GmsecException& e) {
	 *         // handle lookup error for Subscription Entry
	 *     }
	 * }
	 * catch (const GmsecException& e) {
	 *     // handle configuration file parsing error
	 * }
	 * @endcode
	 *
	 * @param name - value in the name="" attribute of the \<SUBSCRIPTION/\> block containing the subscription pattern
	 *
	 * @return SubscriptionEntry associated with a given name
	 *
	 * @throw GmsecException is thrown if the subscription entry cannot be found.
	 */
	const ConfigFile::SubscriptionEntry& CALL_TYPE lookupSubscriptionEntry(const char* name);


	/**
	 * @fn removeSubscriptionEntry(const char* name)
	 *
	 * @brief Removes the SubscriptionEntry definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the %SubscriptionEntry
	 *
	 * @return whether successful or not
	 */
	bool CALL_TYPE removeSubscriptionEntry(const char* name);


	/**
	 * @fn ConfigFileIterator& getIterator() const
	 *
	 * @brief Method that allows the callee to get the ConfigFileIterator associated with the %ConfigFile.
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
	friend class gmsec::api5::internal::InternalConfigFileIterator;

	// defined, but not implemented
	ConfigFile(const ConfigFile&);
	ConfigFile& operator=(const ConfigFile&);

	internal::InternalConfigFile* m_internal;
};

} // end namespace api5
} // end namespace gmsec

#endif
