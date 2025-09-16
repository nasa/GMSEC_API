/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFile.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.jni.JNIConfigFile;
import gov.nasa.gsfc.gmsec.api5.jni.JNIConfigEntry;
import gov.nasa.gsfc.gmsec.api5.jni.JNIMessageEntry;
import gov.nasa.gsfc.gmsec.api5.jni.JNISubscriptionEntry;


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
 *                <PARAMETER NAME="mw-id">artemis</PARAMETER>
 *                <PARAMETER NAME="hostname">tcp://localhost:61616</PARAMETER>
 *        </CONFIG>
 *
 *        <CONFIG NAME="config2">
 *                <PARAMETER NAME="mw-id">bolt</PARAMETER>
 *                <PARAMETER NAME="server">10.1.2.159</PARAMETER>
 *        </CONFIG>
 *
 *        <MESSAGE NAME="msg1" SUBJECT="GMSEC.MISSION.SAT.MSG.HB.MYAPP" KIND="PUBLISH">
 *                <CONFIG NAME="msg_config">
 *                        <PARAMETER NAME="GMSEC-SHOW-MSG-CONFIG">true</PARAMETER>
 *                </CONFIG>
 *                <FIELD NAME="COMPONENT" TYPE="STRING">MYAPP</FIELD>
 *                <FIELD NAME="CONSTELLATION-ID" TYPE="STRING">CNS1</FIELD>
 *                <FIELD NAME="CONTENT-VERSION" TYPE="F32" BITS="44FC6000">2019</FIELD>
 *                <FIELD NAME="COUNTER" TYPE="U16">1</FIELD>
 *                <FIELD NAME="DOMAIN1" TYPE="STRING">DOM1</FIELD>
 *                <FIELD NAME="DOMAIN2" TYPE="STRING">DOM2</FIELD>
 *                <FIELD NAME="FACILITY" TYPE="STRING">MY-FACILITY</FIELD>
 *                <FIELD NAME="HEADER-VERSION" TYPE="F32" BITS="44FC6000">2019</FIELD>
 *                <FIELD NAME="MESSAGE-SUBTYPE" TYPE="STRING">HB</FIELD>
 *                <FIELD NAME="MESSAGE-TYPE" TYPE="STRING">MSG</FIELD>
 *                <FIELD NAME="MISSION-ID" TYPE="STRING">MISSION</FIELD>
 *                <FIELD NAME="PUB-RATE" TYPE="U16">1</FIELD>
 *                <FIELD NAME="SAT-ID-LOGICAL" TYPE="STRING">SAT</FIELD>
 *                <FIELD NAME="SAT-ID-PHYSICAL" TYPE="STRING">SAT</FIELD>
 *                <FIELD NAME="SPECIFICATION" TYPE="STRING">GMSEC</FIELD>
 *                <FIELD NAME="SW-VERSION" TYPE="STRING">5.0</FIELD>
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


	//! @cond
	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param obj Object to reference for acquiring internal JNIConfigFile
	 * @return Internal JNIConfigFile object
	 */
	public static JNIConfigFile getInternal(ConfigFile obj)
	{
		return (obj == null ? null : obj.m_jniConfigFile);
	}
	//! @endcond


	/**
	 * This class provides access to the attributes associated with a ConfigFile's
	 * {@link Config} entry.
	 */
	public static class ConfigEntry
	{
		private JNIConfigEntry m_jniEntry = null;

		//! @cond
		/**
		 * @hidden
		 * This method is for internal GMSEC API use only.
		 * @param entry Internal JNIConfigEntry object
		 */
		public ConfigEntry(JNIConfigEntry entry)
		{
			m_jniEntry = entry;
		}
		//! @endcond

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

		//! @cond
		/**
		 * @hidden
		 * This method is for internal GMSEC API use only.
		 * @param entry Internal JNIMessageEntry object
		 */
		public MessageEntry(JNIMessageEntry entry)
		{
			m_jniEntry = entry;
		}
		//! @endcond

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

		//! @cond
		/**
		 * @hidden
		 * This method is for internal GMSEC API use only.
		 * @param entry Internal JNISubscriptionEntry object
		 */
		public SubscriptionEntry(JNISubscriptionEntry entry)
		{
			m_jniEntry = entry;
		}
		//! @endcond

		/**
		 * Constructor
		 *
		 * @param name    The name to associate with the SubscriptionEntry
		 * @param pattern A subscription pattern/topic to associate with the SubscriptionEntry
		 *
		 * @throws IllegalArgumentException is thrown if either the name or the patter are null, or if
		 * either consist of an empty string
		 */
		public SubscriptionEntry(String name, String pattern)
			throws IllegalArgumentException
		{
			if (name == null || name.isEmpty())
			{
				throw new IllegalArgumentException("SubscriptionEntry name is null or is an empty string");
			}
			if (pattern == null || pattern.isEmpty())
			{
				throw new IllegalArgumentException("SubscriptionEntry pattern is null or is an empty string");
			}

			m_jniEntry = new JNISubscriptionEntry(name, pattern);
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
		 */
		public String getPattern()
		{
			return m_jniEntry.getPattern();
		}
		
		/**
		 * Associates an excluded subscription pattern/topic with the SubscriptionEntry.
		 * @param pattern The pattern/topic to exclude
		 * @throws IllegalArgumentException is thrown if the pattern is null or is an empty string
		 */
		public void addExcludedPattern(String pattern)
			throws IllegalArgumentException
		{
			if (pattern == null || pattern.isEmpty())
			{
				throw new IllegalArgumentException("Excluded pattern is null or is an empty string");
			}

			m_jniEntry.addExcludedPattern(pattern);
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
	 * Constructor that creates an empty configuration file object.
	 * The internally held MessageFactory will be configured with the default message specification used by the API.
	 * <p>
	 * Once the ConfigFile object has been created, use
	 * {@link ConfigFile#load(String)} to load a configuration file, or if
	 * the plan is to add elements to the empty ConfigFile object, then
	 * use {@link ConfigFile#save(String, boolean)} to save any changes.
	 *
	 * @throws GmsecException Thrown if the MessageFactory cannot be instantiated.
	 */
	public ConfigFile()
		throws GmsecException
	{
		m_jniConfigFile = new JNIConfigFile();
	}


	/**
	 * Constructor that creates an empty configuration file object.
	 * The internally held MessageFactory will be configured with the given configuration.
	 * <p>
	 * Configuration options referenced:
	 * <ul>
	 *   <li>gmsec-specification-version</li>
	 *   <li>gmsec-schema-level</li>
	 *   <li>gmsec-msg-content-validate</li>
	 * </ul>
	 * <p>
	 * Once the ConfigFile object has been created, use
	 * {@link ConfigFile#load(String)} to load a configuration file, or if
	 * the plan is to add elements to the empty ConfigFile object, then
	 * use {@link ConfigFile#save(String, boolean)} to save any changes.
	 *
	 * @param config The configuration to apply to the internally held MessageFactory.
	 *
	 * @throws GmsecException Thrown if the MessageFactory cannot be instantiated.
	 */
	public ConfigFile(Config config)
		throws IllegalArgumentException, GmsecException
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}

		m_jniConfigFile = new JNIConfigFile(config);
	}


	/**
	 * This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param filePath The absolute or relative path of the file to be loaded.
	 *
	 * @throws IllegalArgumentException Thrown if the given file path is null or contains an empty string.
	 * @throws GmsecException Thrown if file parsing error occur.
	 */
	public void load(String filePath) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if an error occurs while attempting to write to the file
	 */
	public void save() throws GmsecException
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
	 * @throws GmsecException Thrown if an error occurs while attempting to write to the file
	 */
	public void save(String filePath) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the filePath string is null, or if an error occurs while
	 * attempting to write to the file
	 */
	public void save(String filePath, boolean compact) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if an error occurs while parsing the XML string
	 */
	public void fromXML(String xml) throws IllegalArgumentException, GmsecException
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
	 * catch (IllegalArgumentException | GmsecException e) {
	 *     // handle error
	 * }
	 * }</pre>
	 *
	 * @param name name of the Config object to retrieve.
	 *
	 * @return Config object that correlates with the given name
	 *
	 * @throws IllegalArgumentException Thrown if the given Config name is null or contains an empty string.
	 * @throws GmsecException Thrown if the named Config object does not exist.
	 */
	public Config lookupConfig(String name) throws IllegalArgumentException, GmsecException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Config name is null or contains an empty string");
		}

		return m_jniConfigFile.lookupConfig(name);
	}


	/**
	 * Removes the configuration definition which is associated with the given name.
	 *
	 * @param name name associated with the configuration definition
	 *
	 * @return Returns true if the configuration definition was removed; false otherwise.
	 */
	public boolean removeConfig(String name)
	{
		if (name == null || name.isEmpty())
		{
			return false;
		}

		return m_jniConfigFile.removeConfig(name);
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
	 * catch (IllegalArgumentException | GmsecException e) {
	 *     // handle error
	 * }
	 * }</pre>
	 * 
	 * @param name name of the Message object to retrieve
	 *
	 * @return Message object that correlates with the given name
	 *
	 * @throws IllegalArgumentException Thrown if the given Message name is null or contains an empty string.
	 * @throws GmsecException Thrown if the name is null, or if the named Message object does not exist
	 */
	public Message lookupMessage(String name) throws IllegalArgumentException, GmsecException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Message name is null or contains an empty string");
		}

		return m_jniConfigFile.lookupMessage(name);
	}


	/**
	 * Removes the message definition which is associated with the given name.
	 *
	 * @param name name associated with the message definition
	 *
	 * @return Returns true if the message definition was removed; false otherwise.
	 */
	public boolean removeMessage(String name)
	{
		if (name == null || name.isEmpty())
		{
			return false;
		}

		return m_jniConfigFile.removeMessage(name);
	}


	/**
	 * Adds a SubscriptionEntry to the ConfigFile object.
	 * <p>
	 * Note 1: Adding a subscription with a name that currently exists will overwrite the current subscription.
	 * <p>
	 * Note 2: Adding a subscription definition to a newly instantiated ConfigFile object that has not been
	 * loaded will create a new XML document.
	 *
	 * @param entry   The SubscriptionEntry to associate with the ConfigFile
	 *
	 * @throws IllegalArgumentException Thrown if the given SubscriptionEntry object is null
	 */
	public void addSubscriptionEntry(SubscriptionEntry entry) throws IllegalArgumentException
	{
		if (entry == null)
		{
			throw new IllegalArgumentException("SubscriptionEntry is null");
		}

		m_jniConfigFile.addSubscriptionEntry(entry.m_jniEntry);
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
	 *     catch (GmsecException e) {
	 *         // handle lookup error for Subscription Entry
	 *     }
	 * }
	 * catch (IllegalArgumentException | GmsecException e) {
	 *     // handle error
	 * }
	 * }</pre>
	 *
	 * @param name name of the Subscription entry
	 *
	 * @return A SubscriptionEntry object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Subscription name is null or contains an empty string.
	 * @throws GmsecException Thrown if the Subscription Entry cannot be returned.
	 */ 
	public SubscriptionEntry lookupSubscriptionEntry(String name)
		throws IllegalArgumentException, GmsecException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("SubscriptionEntry name is null or contains an empty string");
		}

		return m_jniConfigFile.lookupSubscriptionEntry(name);
	}


	/**
	 * Removes the SubscriptionEntry definition which is associated with the given name.
	 *
	 * @param name name associated with the subscription definition
	 *
	 * @return Returns true if the subscription definition was removed; false otherwise.
	 */
	public boolean removeSubscriptionEntry(String name)
	{
		if (name == null || name.isEmpty())
		{
			return false;
		}

		return m_jniConfigFile.removeSubscriptionEntry(name);
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
