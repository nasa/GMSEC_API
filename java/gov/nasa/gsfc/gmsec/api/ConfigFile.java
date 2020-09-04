/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFile.java
 *
 * @brief Class that can be used to manage standard configuration files.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNIConfigFile;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfigEntry;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessageEntry;
import gov.nasa.gsfc.gmsec.api.jni.JNISubscriptionEntry;


/** @class ConfigFile
 *
 * @brief This class is used to manage standard configuration files.
 *
 * Example configuration file format:
 *
 * @code
 * <?xml version="1.0" encoding="UTF-8"?>
 * <DEFINITIONS>
 *        <SUBSCRIPTION NAME="events" PATTERN="GMSEC.MISSION.CONST.SAT.EVT.MSG.>"/>
 *        <SUBSCRIPTION NAME="custom1" PATTERN="CUSTOM.MESSAGE.SUBJECTS.*"/>
 *        <CONFIG NAME="config1">
 *                <PARAMETER NAME="connectiontype">gmsec_icsswb</PARAMETER>
 *                <PARAMETER NAME="hostname">localhost</PARAMETER>
 *                <PARAMETER NAME="port">10005</PARAMETER>
 *        </CONFIG>
 *        <CONFIG NAME="config2">
 *                <PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
 *                <PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
 *                <PARAMETER NAME="tracking">true</PARAMETER>
 *                <PARAMETER NAME="isthreaded">true</PARAMETER>
 *                <PARAMETER NAME="compress">true</PARAMETER>
 *        </CONFIG>
 *        <MESSAGE NAME="msg1" SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
 *                <CONFIG NAME="msg_config">
 *                        <PARAMETER NAME="KIND">GMSEC_MSG_PUBLISH</PARAMETER>
 *                </CONFIG>
 *                <FIELD TYPE="CHAR" NAME="char_field">c</FIELD>
 *                <FIELD TYPE="BOOL" NAME="bool_field">TRUE</FIELD>
 *                <FIELD TYPE="SHORT" NAME="short_field">123</FIELD>
 *                <FIELD TYPE="USHORT" NAME="ushort_field">123</FIELD>
 *                <FIELD TYPE="LONG" NAME="long_field">123</FIELD>
 *                <FIELD TYPE="ULONG" NAME="ulong_field">123</FIELD>
 *                <FIELD TYPE="STRING" NAME="string_field">This is a test</FIELD>
 *                <FIELD TYPE="FLOAT" NAME="float_field">123</FIELD>
 *                <FIELD TYPE="DOUBLE" NAME="double_field">123</FIELD>
 *                <FIELD TYPE="BIN" NAME="bin_field">4a4c4d4e4f5051</FIELD>
 *        </MESSAGE>
 * </DEFINITIONS>
 * @endcode
 *
 * A ConfigFile object has a one-to-many mapping to connection configurations. If a user
 * wishes to maintain only one connection per object, the use of a Config object is suggested. 
 */
public class ConfigFile
{
	private JNIConfigFile m_jniConfigFile = null;


	public static JNIConfigFile getInternal(ConfigFile obj)
	{
		return (obj == null ? null : obj.m_jniConfigFile);
	}


	public ConfigFile(JNIConfigFile jCfgFile)
	{
		m_jniConfigFile = jCfgFile;
	}


	/**
	 * @class ConfigEntry
	 * @brief This class provides access to the attributes associated with a ConfigFile's %Config entry.
	 */
	public static class ConfigEntry
	{
		private JNIConfigEntry m_jniEntry = null;

		public ConfigEntry(JNIConfigEntry entry)
		{
			m_jniEntry = entry;
		}

		/**
		 * @fn String getName()
		 * @desc Returns the name associated with the %Config entry.
		 */
		public String getName()
		{
			return m_jniEntry.getName();
		}

		/**
		 * @fn Config getConfig()
		 * @desc Returns the Config object associated with the %Config entry.
		 */
		public Config getConfig()
		{
			return m_jniEntry.getConfig();
		}
	}


	/**
	 * @class MessageEntry
	 * @brief This class provides access to the attributes associated with a ConfigFile's %Message entry.
	 */
	public static class MessageEntry
	{
		private JNIMessageEntry m_jniEntry = null;

		public MessageEntry(JNIMessageEntry entry)
		{
			m_jniEntry = entry;
		}

		/**
		 * @fn String getName()
		 * @desc Returns the name associated with the %Message entry.
		 */
		public String getName()
		{
			return m_jniEntry.getName();
		}

		/**
		 * @fn Message getMessage()
		 * @desc Returns the Message object associated with the %Message entry.
		 */
		public Message getMessage()
		{
			return m_jniEntry.getMessage();
		}
	}


	/**
	 * @class SubscriptionEntry
	 * @brief This class provides access to the attributes associated with a ConfigFile's Subscription entry.
	 */
	public static class SubscriptionEntry
	{
		private JNISubscriptionEntry m_jniEntry = null;

		public SubscriptionEntry(JNISubscriptionEntry entry)
		{
			m_jniEntry = entry;
		}

		/**
		 * @fn String getName()
		 * @desc Returns the name associated with the Subscription entry.
		 */
		public String getName()
		{
			return m_jniEntry.getName();
		}

		/**
		 * @fn String getSubject()
		 * @desc Returns the subject/topic associated with the Subscription entry.
		 */
		public String getSubject()
		{
			return m_jniEntry.getSubject();
		}
	}


	/**
	 * @fn ConfigFile()
	 *
	 * @description Once the ConfigFile object has been created, use
	 * load(String filepath) to load a configuration file, or if
	 * the plan is to add elements to the empty ConfigFile object, then
	 * use save(String filepath, boolean compact) to save any changes.
	 *
	 * @brief Basic constructor that creates an empty configuration file object.
	 */
	public ConfigFile()
	{
		m_jniConfigFile = new JNIConfigFile();
	}


	/**
	 * @fn void load(String filePath)
	 *
	 * @brief This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param filePath - The absolute or relative path of the file to be loaded.
	 *
	 * @throws An IllegalArgumentException if the given file path is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if file parsing error occur.
	 */
	public void load(String filePath) throws IllegalArgumentException, GMSEC_Exception
	{
		if (filePath == null || filePath.isEmpty())
		{
			throw new IllegalArgumentException("File Path is null or contains an empty string");
		}

		m_jniConfigFile.load(filePath);
	}


	/**
	 * @fn void save()
	 *
	 * @brief This function saves the currently open configuration file to
	 * the file path specified when load() was called.
	 *
	 * @throws A GMSEC_Exception is thrown if an error occurs while attempting to write to the file
	 */
	public void save() throws GMSEC_Exception
	{
		save(null, false);
	}


	/**
	 * @fn void save(String filePath)
	 *
	 * @brief This function saves the currently open configuration file to
	 * the file location specified.
	 *
	 * @param filePath - The location to which the file will be saved; if the specified path is null,
	 * then the file path used when load() was called will be used.
	 *
	 * @throws An IllegalArgumentException if the given file path contains an empty string.
	 * @throws A GMSEC_Exception is thrown if an error occurs while attempting to write to the file
	 */
	public void save(String filePath) throws IllegalArgumentException, GMSEC_Exception
	{
		save(filePath, false);
	}


	/**
	 * @fn void save(String filePath, boolean compact)
	 *
	 * @brief This function saves the currently open configuration file to
	 * the file location specified.
	 *
	 * @param filePath - The location to which the file will be saved; if the specified path is null,
	 * then the file path used when load() was called will be used.
	 * @param compact  - used to indicate whether the XML written to the file should be compacted or not
	 *
	 * @throws An IllegalArgumentException if the given file path contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the filePath string is null, or if an error occurs while
	 * attempting to write to the file
	 */
	public void save(String filePath, boolean compact) throws IllegalArgumentException, GMSEC_Exception
	{
		if (filePath != null && filePath.isEmpty())
		{
			throw new IllegalArgumentException("File Path contains an empty string");
		}

		m_jniConfigFile.save(filePath, compact);
	}


	/**
	 * @fn void fromXML(String xml)
	 *
	 * @brief This function parses the given XML string to fill in the contents of the ConfigFile object.
	 *
	 * @throws An IllegalArgumentException if the given XML string is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if an error occurs while parsing the XML string
	 */
	public void fromXML(String xml) throws GMSEC_Exception
	{
		if (xml == null || xml.isEmpty())
		{
			throw new IllegalArgumentException("XML string is null or contains an empty string");
		}

		m_jniConfigFile.fromXML(xml);
	}


	/**
	 * @fn String toXML()
	 *
	 * @brief This method converts the ConfigFile object to an XML string representation.
	 *
	 * @return The XML string representation of the ConfigFile object
	 */
	public String toXML()
	{
		return m_jniConfigFile.toXML();
	}


	/**
	 * @fn boolean isLoaded()
	 *
	 * @brief This method indicates whether a configuration file has been loaded into the ConfigFile object.
	 *
	 * @return Returns true if configuration file loaded; false otherwise.
	 */
	public boolean isLoaded()
	{
		return m_jniConfigFile.isLoaded();
	}


	/**
	 * @fn Config lookupConfig(String name)
	 *
	 * @brief This function will attempt to locate within the ConfigFile a Config object associated
	 * with the given name.
	 *
	 * Example config file load and get config:
	 * @code
	 * try {
	 *     ConfigFile cfgFile = new ConfigFile();
	 *
	 *     // load and parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *
	 *     // attempt to get Config from ConfigFile
	 *     Config cfg = cfgFile.lookupConfig("MyConfig");
	 *
	 *     // do something with cfg
	 * }
	 * catch (IllegalArgumentException | GMSEC_Exception e) {
	 *     // handle error
	 * }
	 * @endcode
	 *
	 * @param name - name of the Config object to retrieve.
	 *
	 * @return Config object that correlates with the given name
	 *
	 * @throws An IllegalArgumentException if the given Config name is null or contains an empty string.
	 * @throw A GMSEC_Exception is thrown if the named Config object does not exist.
	 */
	public Config lookupConfig(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Config name is null or contains an empty string");
		}

		return m_jniConfigFile.lookupConfig(name);
	}


	/**
	 * @fn Message lookupMessage(String name)
	 *
	 * @brief This function will attempt to locate within the ConfigFile a Message object associated
	 * with the given name.
	 *
	 * Example config file load and get message:
	 * @code
	 * try {
	 *     ConfigFile cfgFile = new ConfigFile();
	 *
	 *     // load and parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *
	 *     // attempt to get Message from ConfigFile
	 *     Message msg = cfgFile.lookupMessage("MyMessage");
	 *
	 *     // do something with msg
	 * }
	 * catch (IllegalArgumentException | GMSEC_Exception e) {
	 *     // handle error
	 * }
	 * @endcode
	 * 
	 * @param name - name of the Message object to retrieve
	 *
	 * @return Message object that correlates with the given name
	 *
	 * @throws An IllegalArgumentException if the given Message name is null or contains an empty string.
	 * @throw A GMSEC_Exception is thrown if the name is null, or if the named Message object does not exist
	 */
	public Message lookupMessage(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Message name is null or contains an empty string");
		}

		return m_jniConfigFile.lookupMessage(name);
	}


	/**
	 * @fn String lookupSubscription(String name)
	 *
	 * @brief This function will attempt to locate within the ConfigFile a Subscription entry associated
	 * with the given name, and return the subject/topic associated with such.
	 *
	 * Example
	 * @code
	 * try {
	 *     ConfigFile cfgFile = new ConfigFile();
	 *
	 *     // load and parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *
	 *     // get subscription topic from file
	 *     String topic = cfgFile.lookupSubscription("MySubscription");
	 *
	 *     if (topic != null) {
	 *         // success
	 *     }
	 *     else {
	 *         // handle subscription pattern lookup error
	 *     }
	 * }
	 * catch (IllegalArgumentException | GMSEC_Exception e) {
	 *     // handle error
	 * }
	 * @endcode
	 *
	 * @param name - name of the Subscription entry
	 *
	 * @return subject pattern topic associated with SUBSCRIPTION block, or NULL if not found
	 *
	 * @throws An IllegalArgumentException if the given Subscription name is null or contains an empty string.
	 * @throw A GMSEC_Exception is thrown if the name is null.
	 */
	public String lookupSubscription(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Subscription name is null or contains an empty string");
		}

		return m_jniConfigFile.lookupSubscription(name);
	}


	/**
	 * @fn void addSubscription(String name, String subject)
	 *
	 * @brief Adds a Subscription definition to the ConfigFile object.
	 *
	 * @note Adding a subscription with a name that currently exists will overwrite the current subscription.
	 * @note Adding a subscription definition to a newly instantiated ConfigFile object that has not been
	 * loaded will create a new XML document.
	 *
	 * @param name    - name to be associated with the subscription definition
	 * @param subject - subject/topic to associate with the subscription definition
	 *
	 * @throws An IllegalArgumentException is thrown if either name or subject parameters are null, or if either contains
	 * an empty string
	 */
	public void addSubscription(String name, String subject) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Subscription name is null or contain an empty string");
		}
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subscription subject is null or contain an empty string");
		}

		m_jniConfigFile.addSubscription(name, subject);
	}


	/**
	 * @fn void addConfig(String name, Config config)
	 *
	 * @brief Adds a Config definition to the ConfigFile object.
	 *
	 * @note Adding a Config with a name that currently exists will overwrite the current
	 * configuration definition.
	 * @note Adding a Config to a newly instantiated ConfigFile object that has not been loaded
	 * will create a new XML document.
	 *
	 * @param name   - name to be associated with the configuration definition
	 * @param config - the configuration object
	 *
	 * @throws An IllegalArgumentException is thrown if name is null or contains an empty string,
	 * of if the Config object is null.
	 */
	public void addConfig(String name, Config config) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Config name is null or contains an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config object handle is null");
		}

		m_jniConfigFile.addConfig(name, config);
	}


	/**
	 * @fn void addMessage(String name, Message message)
	 *
	 * @brief Adds a Message definition to the ConfigFile object.
	 *
	 * @note Adding a message with a name that currently exists will overwrite the current
	 * message definition.
	 * @note Adding a Message to a newly instantiated ConfigFile object that has not been loaded
	 * will create a new XML document.
	 *
	 * @param name    - name to be associated with the message definition
	 * @param message - the message object
	 *
	 * @throws An IllegalArgumentException is thrown if name is null or contains an empty string,
	 * of if the Message object is null.
	 */
	public void addMessage(String name, Message message) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Message name is null or contains an empty string");
		}
		if (message == null)
		{
			throw new IllegalArgumentException("Message object handle is null");
		}

		m_jniConfigFile.addMessage(name, message);
	}


	/**
	 * @fn void addCustomXML(String xml)
	 *
	 * @brief Adds a custom XML string to the ConfigFile.
	 *
	 * @note Adding a custom XML to a newly instantiated ConfigFile object that has not loaded
	 * will create a new XML document.
	 *
	 * @param xml - a custom, user-created XML string
	 *
	 * @throws An IllegalArgumentException is thrown if the XML string is null, or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if an XML parsing error occurs.
	 */
	public void addCustomXML(String xml) throws IllegalArgumentException, GMSEC_Exception
	{
		if (xml == null || xml.isEmpty())
		{
			throw new IllegalArgumentException("Custom XML string is null or contains an empty string");
		}

		m_jniConfigFile.addCustomXML(xml);
	}


	/**
	 * @fn boolean removeSubscription(String name)
	 *
	 * @brief Removes the Subscription definition which is associated with the given name.
	 *
	 * @param name - name associated with the subscription definition
	 *
	 * @return Returns true if the subscription definition was removed; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the subscription name is null, or contains an empty string.
	 */
	public boolean removeSubscription(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Subscription name is null or contains an empty string");
		}

		return m_jniConfigFile.removeSubscription(name);
	}


	/**
	 * @fn boolean removeConfig(String name)
	 *
	 * @brief Removes the configuration definition which is associated with the given name.
	 *
	 * @param name - name associated with the configuration definition
	 *
	 * @return Returns true if the configuration definition was removed; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the Config name is null, or contains an empty string.
	 */
	public boolean removeConfig(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Config name is null or contains an empty string");
		}

		return m_jniConfigFile.removeConfig(name);
	}


	/**
	 * @fn boolean removeMessage(String name)
	 *
	 * @brief Removes the message definition which is associated with the given name.
	 *
	 * @param name - name associated with the message definition
	 *
	 * @return Returns true if the message definition was removed; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the Message name is null, or contains an empty string.
	 */
	public boolean removeMessage(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Message name is null or contains an empty string");
		}

		return m_jniConfigFile.removeMessage(name);
	}


	/**
	 * @fn boolean removeCustomXML(String xml)
	 *
	 * @brief Removes the given XML string from the ConfigFile.
	 *
	 * @param xml - XML string containing the custom XML definition
	 *
	 * @return Returns true if the custom XML definition was removed; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the XML string is null, or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if an XML parsing error occurs.
	 */
	public boolean removeCustomXML(String xml) throws IllegalArgumentException, GMSEC_Exception
	{
		if (xml == null || xml.isEmpty())
		{
			throw new IllegalArgumentException("Custom XML string is null or contains an empty string");
		}

		return m_jniConfigFile.removeCustomXML(xml);
	}


	/**
	 * @fn ConfigFileIterator getIterator()
	 *
	 * @desc Returns the ConfigFileIterator that may be used to iterate over the entries of the ConfigFile.
	 */
	public ConfigFileIterator getIterator()
	{
		return m_jniConfigFile.getIterator();
	}
}
