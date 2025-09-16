/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFile.java
 */

package gmsec;


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
	/**
	 * Constructor that creates an empty configuration file object.
	 * The internally held MessageFactory will be configured with the default message specification used by the API.
	 * <p>
	 * Once the ConfigFile object has been created, use
	 * {@link ConfigFile.load(String)} to load a configuration file, or if
	 * the plan is to add elements to the empty ConfigFile object, then
	 * use {@link ConfigFile.save(String, boolean)} to save any changes.
	 *
	 * @throws GmsecException Thrown if the MessageFactory cannot be instantiated.
	 */
	public ConfigFile();


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
	 * {@link ConfigFile.load(String)} to load a configuration file, or if
	 * the plan is to add elements to the empty ConfigFile object, then
	 * use {@link ConfigFile.save(String, boolean)} to save any changes.
	 *
	 * @param config The configuration to apply to the internally held MessageFactory.
	 *
	 * @throws SWIG_Exception Thrown if the given Config object is null.
	 * @throws GmsecException Thrown if the MessageFactory cannot be instantiated.
	 */
	public ConfigFile(config);


	/**
	 * Loads the contents of the configuration file.
	 *
	 * @param filePath The absolute or relative path of the file to be loaded.
	 *
	 * @throws SWIG_Exception Thrown if the given file path is null.
	 * @throws GmsecException Thrown if file parsing error occur.
	 */
	public load(filePath);


	/**
	 * This function saves the currently open configuration file to
	 * the file path specified when load() was called.
	 *
	 * @throws GmsecException Thrown if an error occurs while attempting to write to the file.
	 */
	public save();


	/**
	 * Saves the currently open configuration to the file location specified.
	 *
	 * @param filePath The location to which the file will be saved; if the specified path is null,
	 * then the file path used when load() was called will be used.
	 *
	 * @throws SWIG_Exception Thrown if the given file path is null.
	 * @throws GmsecException Thrown if an error occurs while attempting to write to the file.
	 */
	public save(filePath);


	/**
	 * Saves the currently open configuration to the file location specified.
	 *
	 * @param filePath The location to which the file will be saved; if the specified path is null,
	 *                 then the file path used when load() was called will be used.
	 * @param compact  used to indicate whether the XML written to the file should be compacted or not
	 *
	 * @throws SWIG_Exception Thrown if the given file path is null.
	 * @throws GmsecException Thrown if if an error occurs while attempting to write to the file.
	 */
	public save(filePath, compact);


	/**
	 * Parses the given XML string to fill in the contents of the ConfigFile object.
	 *
	 * @param xml The XML string to parse.
	 *
	 * @throws SWIG_Exception Thrown if the given XML string is null.
	 * @throws GmsecException Thrown if an error occurs while parsing the XML string.
	 */
	public fromXML(xml);


	/**
	 * Converts the ConfigFile object to an XML string representation.
	 *
	 * @return The XML string representation of the ConfigFile object
	 */
	public toXML();


	/**
	 * Indicates whether a configuration file has been loaded into the ConfigFile object.
	 *
	 * @return Returns true if configuration file loaded; false otherwise.
	 */
	public isLoaded();


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
	 * @throws SWIG_Exception Thrown if name is null, or if the Config object is null.
	 * @throws GmsecException Thrown if the name is an empty string.
	 */
	public addConfig(name, config);


	/**
	 * Attempts to locate within the ConfigFile a Config object associated with the given name.
	 * <p>
	 * Example config file load and get config:
	 * <pre>{@code
	 * try {
	 *     let cfgFile = new gmsec.ConfigFile();
	 *     // load and parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *     // attempt to get Config from ConfigFile
	 *     const cfg = cfgFile.lookupConfig("MyConfig");
	 *     // do something with cfg
	 * }
	 * catch (e) {
	 *     // handle error
	 * }
	 * }</pre>
	 *
	 * @param name name of the Config object to retrieve.
	 *
	 * @return Config object that correlates with the given name
	 *
	 * @throws SWIG_Exception Thrown if the given Config name is null.
	 * @throws GmsecException Thrown if the named Config object does not exist.
	 */
	public lookupConfig(name);


	/**
	 * Removes the configuration definition which is associated with the given name.
	 *
	 * @param name name associated with the configuration definition
	 *
	 * @return Returns true if the configuration definition was removed; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the given Config name is null.
	 */
	public removeConfig(name);


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
	 * @throws SWIG_Exception Thrown if the name is null.
	 * @throws SWIG_Exception Thrown if the Message object is null.
	 * @throws GmsecException Thrown if the name is an empty string.
	 */
	public addMessage(name, message);


	/**
	 * This function will attempt to locate within the ConfigFile a Message object associated
	 * with the given name.
	 * <p>
	 * Example config file load and get message:
	 * <pre>{@code
	 * try {
	 *     let cfgFile = new gmsec.ConfigFile();
	 *     // load and parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *     // attempt to get Message from ConfigFile
	 *     const msg = cfgFile.lookupMessage("MyMessage");
	 *     // do something with msg
	 * }
	 * catch (e) {
	 *     // handle error
	 * }
	 * }</pre>
	 * 
	 * @param name name of the Message object to retrieve
	 *
	 * @return Message object that correlates with the given name
	 *
	 * @throws SWIG_Exception Thrown if the given name is null.
	 * @throws GmsecException Thrown if if the named Message object does not exist.
	 */
	public lookupMessage(name);


	/**
	 * Removes the message definition which is associated with the given name.
	 *
	 * @param name name associated with the message definition
	 *
	 * @return Returns true if the message definition was removed; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the given name is null.
	 */
	public removeMessage(name);


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
	 * @throws SWIG_Exception Thrown if the given SubscriptionEntry object is null.
	 */
	public addSubscriptionEntry(entry);


	/**
	 * Attempts to locate within the ConfigFile a Subscription entry associated
	 * with the given name, and return the subject/topic associated with such.
	 * <p>
	 * Example
	 * <pre>{@code
	 * try {
	 *     let cfgFile = new ConfigFile();
	 *     // load and parse configuration file
	 *     cfgFile.load("/some/path/to/config.xml");
	 *     // attempt to acquire Subscription Entry
	 *     try {
	 *         const entry = cfgFile.lookupSubscriptionEntry("MySubscription");
	 *         ...
	 *     }
	 *     catch (e2) {
	 *         // handle lookup error for Subscription Entry
	 *     }
	 * }
	 * catch (e1) {
	 *     // handle error
	 * }
	 * }</pre>
	 *
	 * @param name name of the Subscription entry
	 *
	 * @return A SubscriptionEntry object.
	 *
	 * @throws SWIG_Exception Thrown if the given Subscription name is null.
	 * @throws GmsecException Thrown if the Subscription Entry cannot be returned.
	 */ 
	public lookupSubscriptionEntry(name);


	/**
	 * Removes the SubscriptionEntry definition which is associated with the given name.
	 *
	 * @param name name associated with the subscription definition
	 *
	 * @return Returns true if the subscription definition was removed; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the given Subscription name is null.
	 */
	public removeSubscriptionEntry(name);


	/**
	 * Returns the {@link ConfigFileIterator} that may be used to iterate over the entries of the ConfigFile.
	 *
	 * @return A ConfigFileIterator object.
	 */
	public getIterator();
}
