/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFile.java
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNIConfigFile;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfigEntry;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessageEntry;
import gov.nasa.gsfc.gmsec.api.jni.JNISubscriptionEntry;


/**
 * This class is used to manage standard configuration files.
 * <p>
 * Example configuration file format:
 * <pre>{@code
 * <?xml version="1.0" encoding="UTF-8"?>
 * <DEFINITIONS>
 *        <SUBSCRIPTION NAME="events" PATTERN="GMSEC.MISSION.CONST.SAT.EVT.MSG.>"/>
 *
 *        <SUBSCRIPTION NAME="custom1" PATTERN="CUSTOM.MESSAGE.SUBJECTS.*"/>
 *
 *        <CONFIG NAME="config1">
 *                <PARAMETER NAME="connectiontype">gmsec_icsswb</PARAMETER>
 *                <PARAMETER NAME="hostname">localhost</PARAMETER>
 *                <PARAMETER NAME="port">10005</PARAMETER>
 *        </CONFIG>
 *
 *        <CONFIG NAME="config2">
 *                <PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
 *                <PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
 *                <PARAMETER NAME="tracking">true</PARAMETER>
 *                <PARAMETER NAME="isthreaded">true</PARAMETER>
 *                <PARAMETER NAME="compress">true</PARAMETER>
 *        </CONFIG>
 *
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
 * }</pre>
 * <p>
 * A ConfigFile object has a one-to-many mapping to connection configurations. If a user
 * wishes to maintain only one connection per object, the use of a Config object is suggested. 
 */
public class ConfigFile
{
	private JNIConfigFile m_jniConfigFile = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param obj Object to reference for acquiring internal JNIConfigFile
	 * @return Internal JNIConfigFile object
	 */
	public static JNIConfigFile getInternal(ConfigFile obj)
	{
		return (obj == null ? null : obj.m_jniConfigFile);
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jCfgFile Internal JNIConfigFile object
	 */
	public ConfigFile(JNIConfigFile jCfgFile)
	{
		m_jniConfigFile = jCfgFile;
	}


	/**
	 * This class provides access to the attributes associated with a ConfigFile's
	 * {@link Config} entry.
	 */
	public static class ConfigEntry
	{
		private JNIConfigEntry m_jniEntry = null;

		/**
		 * This method is for internal GMSEC API use only.
		 * @param entry Internal JNIConfigEntry object
		 */
		public ConfigEntry(JNIConfigEntry entry)
		{
			m_jniEntry = entry;
		}

		/**
		 * Returns the name associated with the entry.
		 *
		 * @return The name of the entry.
		 */
		public String getName()
		{
			return m_jniEntry.getName();
		}

		/**
		 * Returns the {@link Config} object associated with the entry.
		 *
		 * @return A Config object.
		 */
		public Config getConfig()
		{
			return m_jniEntry.getConfig();
		}
	}


	/**
	 * This class provides access to the attributes associated with a ConfigFile's
	 * {@link Message} entry.
	 */
	public static class MessageEntry
	{
		private JNIMessageEntry m_jniEntry = null;

		/**
		 * This method is for internal GMSEC API use only.
		 * @param entry Internal JNIMessageEntry object
		 */
		public MessageEntry(JNIMessageEntry entry)
		{
			m_jniEntry = entry;
		}

		/**
		 * Returns the name associated with the entry.
		 *
		 * @return The name of the entry.
		 */
		public String getName()
		{
			return m_jniEntry.getName();
		}

		/**
		 * Returns the {@link Message} object associated with the entry.
		 *
		 * @return A Message object.
		 */
		public Message getMessage()
		{
			return m_jniEntry.getMessage();
		}
	}


	/**
	 * This class provides access to the attributes associated with a ConfigFile's
	 * subscription entry.
	 */
	public static class SubscriptionEntry
	{
		private JNISubscriptionEntry m_jniEntry = null;

		/**
		 * This method is for internal GMSEC API use only.
		 * @param entry Internal JNISubscriptionEntry object
		 */
		public SubscriptionEntry(JNISubscriptionEntry entry)
		{
			m_jniEntry = entry;
		}

		/**
		 * Returns the name associated with the subscription entry.
		 *
		 * @return The subscription name.
		 */
		public String getName()
		{
			return m_jniEntry.getName();
		}

		/**
		 * Returns the subject/topic associated with the subscription entry.
		 *
		 * @return A subject/topic string.
		 *
		 * @see SubscriptionEntry#getPattern()
		 */
		@Deprecated
		public String getSubject()
		{
			return m_jniEntry.getPattern();
		}

		/**
		 * Returns the subject/topic associated with the subscription entry.
		 *
		 * @return A subject/topic string.
		 */
		public String getPattern()
		{
			return m_jniEntry.getPattern();
		}
		
		/**
		 * Returns true if there is a next excluded pattern associated with the subscription entry;
		 * false otherwise.
		 *
		 * @return A boolean.
		 */
		public boolean hasNextExcludedPattern()
		{
			return m_jniEntry.hasNextExcludedPattern();
		}

		/**
		 * Returns the next excluded pattern associated with the subscription entry. 
		 *
		 * @return A string containing the excluded pattern.
		 */
		public String nextExcludedPattern()
		{
			return m_jniEntry.nextExcludedPattern();
		}
	}


	/**
	 * Basic constructor that creates an empty configuration file object.
	 * <p>
	 * Once the ConfigFile object has been created, use
	 * {@link ConfigFile#load(String)} to load a configuration file, or if
	 * the plan is to add elements to the empty ConfigFile object, then
	 * use {@link ConfigFile#save(String, boolean)} to save any changes.
	 */
	public ConfigFile()
	{
		m_jniConfigFile = new JNIConfigFile();
	}


	/**
	 * This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param filePath The absolute or relative path of the file to be loaded.
	 *
	 * @throws IllegalArgumentException Thrown if the given file path is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if file parsing error occur.
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
	 * This function saves the currently open configuration file to
	 * the file path specified when load() was called.
	 *
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to write to the file
	 */
	public void save() throws GMSEC_Exception
	{
		save(null, false);
	}


	/**
	 * This function saves the currently open configuration file to
	 * the file location specified.
	 *
	 * @param filePath The location to which the file will be saved; if the specified path is null,
	 * then the file path used when load() was called will be used.
	 *
	 * @throws IllegalArgumentException Thrown if the given file path contains an empty string.
	 * @throws GMSEC_Exception Thrown if an error occurs while attempting to write to the file
	 */
	public void save(String filePath) throws IllegalArgumentException, GMSEC_Exception
	{
		save(filePath, false);
	}


	/**
	 * This function saves the currently open configuration file to
	 * the file location specified.
	 *
	 * @param filePath The location to which the file will be saved; if the specified path is null,
	 *                 then the file path used when load() was called will be used.
	 * @param compact  used to indicate whether the XML written to the file should be compacted or not
	 *
	 * @throws IllegalArgumentException Throw if the given file path contains an empty string.
	 * @throws GMSEC_Exception Thrown if the filePath string is null, or if an error occurs while
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
	 * This function parses the given XML string to fill in the contents of the ConfigFile object.
	 *
	 * @param xml The XML string to parse.
	 *
	 * @throws IllegalArgumentException Thrown if the given XML string is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if an error occurs while parsing the XML string
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
	 * This method converts the ConfigFile object to an XML string representation.
	 *
	 * @return The XML string representation of the ConfigFile object
	 */
	public String toXML()
	{
		return m_jniConfigFile.toXML();
	}


	/**
	 * This method indicates whether a configuration file has been loaded into the ConfigFile object.
	 *
	 * @return Returns true if configuration file loaded; false otherwise.
	 */
	public boolean isLoaded()
	{
		return m_jniConfigFile.isLoaded();
	}


	/**
	 * This function will attempt to locate within the ConfigFile a Config object associated
	 * with the given name.
	 * <p>
	 * Example config file load and get config:
	 * <pre>{@code
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
	 * }</pre>
	 *
	 * @param name name of the Config object to retrieve.
	 *
	 * @return Config object that correlates with the given name
	 *
	 * @throws IllegalArgumentException Thrown if the given Config name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the named Config object does not exist.
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
	 * This function will attempt to locate within the ConfigFile a Message object associated
	 * with the given name.
	 * <p>
	 * Example config file load and get message:
	 * <pre>{@code
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
	 * }</pre>
	 * 
	 * @param name name of the Message object to retrieve
	 *
	 * @return Message object that correlates with the given name
	 *
	 * @throws IllegalArgumentException Thrown if the given Message name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the name is null, or if the named Message object does not exist
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
	 * This function will attempt to locate within the ConfigFile a Subscription entry associated
	 * with the given name, and return the subject/topic associated with such.
	 * <p>
	 * Example
	 * <pre>{@code
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
	 * }</pre>
	 *
	 * @param name Name of the Subscription entry
	 *
	 * @return Subject pattern topic associated with SUBSCRIPTION block, or NULL if not found
	 *
	 * @throws IllegalArgumentException Thrown if the given Subscription name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the name is null.
	 * 
	 * @see ConfigFile#lookupSubscriptionEntry(String)
	 */
	@Deprecated
	public String lookupSubscription(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Subscription name is null or contains an empty string");
		}

		return m_jniConfigFile.lookupSubscription(name);
	}


	/**
	 * This function will attempt to locate within the ConfigFile a Subscription entry associated
	 * with the given name, and return the subject/topic associated with such.
	 * <p>
	 * Example
	 * <pre>{@code
	 * try {
	 *     ConfigFile cfgFile = new ConfigFile();
	 *
	 *     // load and parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *
	 *     // attempt to acquire Subscription Entry
	 *     try {
	 *         SubscriptionEntry entry = cfgFile.lookupSubscriptionEntry("MySubscription");
	 *
	 *         ...
	 *     }
	 *     catch (GMSEC_Exception e) {
	 *         // handle lookup error for Subscription Entry
	 *     }
	 * }
	 * catch (IllegalArgumentException | GMSEC_Exception e) {
	 *     // handle error
	 * }
	 * }</pre>
	 *
	 * @param name name of the Subscription entry
	 *
	 * @return A SubscriptionEntry object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Subscription name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the Subscription Entry cannot be returned.
	 */ 
	public SubscriptionEntry lookupSubscriptionEntry(String name)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Subscription name is null or contains an empty string");
		}

		return m_jniConfigFile.lookupSubscriptionEntry(name);
	}


	/**
	 * Adds a Subscription definition to the ConfigFile object.
	 * <p>
	 * Note 1: Adding a subscription with a name that currently exists will overwrite the current subscription.
	 * <p>
	 * Note 2: Adding a subscription definition to a newly instantiated ConfigFile object that has not been
	 * loaded will create a new XML document.
	 *
	 * @param name    name to be associated with the subscription definition
	 * @param subject subject/topic to associate with the subscription definition
	 *
	 * @throws IllegalArgumentException Thrown if either name or subject parameters are null, or if either contains
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
	 * Adds a Config definition to the ConfigFile object.
	 * <p>
	 * Note 1: Adding a Config with a name that currently exists will overwrite the current
	 * configuration definition.
	 * <p>
	 * Note 2: Adding a Config to a newly instantiated ConfigFile object that has not been loaded
	 * will create a new XML document.
	 *
	 * @param name   name to be associated with the configuration definition
	 * @param config the configuration object
	 *
	 * @throws IllegalArgumentException Thrown if name is null or contains an empty string,
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
	 * Adds a Message definition to the ConfigFile object.
	 * <p>
	 * Note 1: Adding a message with a name that currently exists will overwrite the current
	 * message definition.
	 * <p>
	 * Note 2: Adding a Message to a newly instantiated ConfigFile object that has not been loaded
	 * will create a new XML document.
	 *
	 * @param name    name to be associated with the message definition
	 * @param message the message object
	 *
	 * @throws IllegalArgumentException Thrown if name is null or contains an empty string,
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
	 * Adds a custom XML string to the ConfigFile.
	 * <p>
	 * Note: Adding a custom XML to a newly instantiated ConfigFile object that has not loaded
	 * will create a new XML document.
	 *
	 * @param xml a custom, user-created XML string
	 *
	 * @throws IllegalArgumentException Thrown if the XML string is null, or contains an empty string.
	 * @throws GMSEC_Exception Thrown if an XML parsing error occurs.
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
	 * Removes the Subscription definition which is associated with the given name.
	 *
	 * @param name name associated with the subscription definition
	 *
	 * @return Returns true if the subscription definition was removed; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the subscription name is null, or contains an empty string.
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
	 * Removes the configuration definition which is associated with the given name.
	 *
	 * @param name name associated with the configuration definition
	 *
	 * @return Returns true if the configuration definition was removed; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the Config name is null, or contains an empty string.
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
	 * Removes the message definition which is associated with the given name.
	 *
	 * @param name name associated with the message definition
	 *
	 * @return Returns true if the message definition was removed; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the Message name is null, or contains an empty string.
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
	 * Removes the given XML string from the ConfigFile.
	 *
	 * @param xml XML string containing the custom XML definition
	 *
	 * @return Returns true if the custom XML definition was removed; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the XML string is null, or contains an empty string.
	 * @throws GMSEC_Exception Thrown if an XML parsing error occurs.
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
	 * Returns the {@link ConfigFileIterator} that may be used to iterate over the entries of the ConfigFile.
	 *
	 * @return A ConfigFileIterator object.
	 */
	public ConfigFileIterator getIterator()
	{
		return m_jniConfigFile.getIterator();
	}
}
