/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Config.java
 */

package gmsec;


/**
 * Encapsulates name/value parameters.
 * <p>
 * Example creation and use of Config:
 * <pre>{@code
 * const gmsec_api_home = process.env.GMSEC_API_HOME
 * const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");
 * let config = new gmsec.Config();
 * config.addValue("mw-id", "activemq39");
 * config.addValue("mw-server", "tcp://myserver:61616");
 * try {
 *    let conn = new gmsec.Connection(config);
 *    conn.connect();
 *    // ...
 *    conn.disconnect();
 * }
 * catch (e) {
 *    console.log(e.message);
 * }
 * </pre>
 *
 * @see Connection
 * @see MessageFactory
 */
public class Config
{
	/**
	 * Creates an empty configuration object
	 */
	public Config();


	/**
	 * Creates an configuration object using the StringArray containing name/value pairs
	 * that are in the form of name=value.
	 *
	 * @param args A StringArray of name=value entries
	 */
	public Config(args);


	/**
	 * Creates an configuration object by parsing the name/value pairs from the
	 * given JSON string.
	 * <p>
	 * Example usage:
	 * <pre>
	 * const config = new gmsec.Config(JSON.stringify(process.argv));
	 * </pre>
	 *
	 * @param jsonString The string containing configuration information
	 *
	 * @throws SWIG_Exception Thrown if the string is null.
	 * @throws SWIG_Exception Thrown if the string is not parseable.
	 */
	public Config(jsonString);


	/**
	 * Creates an configuration object by parsing the name/value pairs from the
	 * given data string.
	 * <p>
	 * Note: XML and JSON configuration data strings have a defined format.
	 * See Config.toXML() and Config.toJSON() for more information.
	 * <p>
	 * The types of data supported are:
	 * <ul>
	 * <li> gmsec.DataType_KEY_VALUE_DATA
	 * <li> gmsec.DataType_XML_DATA
	 * <li> gmsec.DataType_JSON_DATA
	 * </ul><p>
	 * Example usage:
	 * <pre>
	 * const config = new gmsec.Config("mw-id=bolt mw-server=localhost", gmsec.DataType_KEY_VALUE_DATA);
	 * </pre>
	 *
	 * @param data The string containing configuration information
	 * @param type The type of data being provided
	 *
	 * @throws SWIG_Exception Thrown if the data string is null.
	 * @throws GmsecException Thrown if the given data string is not parsable.
	 *
	 * @see Config.toXML()
	 * @see Config.toJSON()
	 */
	public Config(data, type);


	/**
	 * Copy-constructor for initializing a Config object using the configuration
	 * values from an existing Config object.
	 *
	 * @param other The existing Config object to copy.
	 */
	public Config(other);


	/**
	 * Stores a name/value pair within the Config object.
	 *
	 * @param name  The name associated with the value.
	 * @param value The value associated with the name.
	 *
	 * @throws SWIG_Exception Thrown if the name is null.
	 * @throws GmsecException Thrown if the name is an empty string.
	 */
	public addValue(name, value);


	/**
	 * Clears the name, and associated value, from the Config object.
	 *
	 * @param name The name of value to clear.
	 *
	 * @return True if the operation was successful; false if the name was not found.
	 *
	 * @throws SWIG_Exception Thrown if the name is null.
	 * @throws GmsecException Thrown if the name is an empty string.
	 */
	public clearValue(name);


	/**
	 * Clears all name/value pairs from the Config object.
	 */
	public clear();


	/**
	 * Returns the first name and value pair for iteration.
	 * <p>
	 * Example usage:
	 * <pre>
	 * let pair = new gmsec.ConfigPair();
	 * let havePair = config.getFirst(pair);
	 * while (havePair) {
	 *     console.log("Name: " + pair.getName() + " = " + pair.getValue());
	 *     havePair = config.getNext(pair);
	 * }
	 * </pre>
	 *
	 * @param pair The ConfigPair object where to store the name/value pair.
	 *
	 * @return Boolean status as to whether name/value pair are available.
	 *
	 * @throws SWIG_Exception Thrown if the ConfigPair object is null.
	 */
	public getFirst(pair);


	/**
	 * Returns the next name and value pair for iteration.
	 * <p>
	 * Example usage:
	 * <pre>
	 * let pair = new gmsec.ConfigPair();
	 * let havePair = config.getFirst(pair);
	 * while (havePair) {
	 *     console.log("Name: " + pair.getName() + " = " + pair.getValue());
	 *     havePair = config.getNext(pair);
	 * }
	 * </pre>
	 *
	 * @param pair The ConfigPair object where to store the name/value pair.
	 *
	 * @return Boolean status as to whether name/value pair are available.
	 *
	 * @throws SWIG_Exception Thrown if the ConfigPair object is null.
	 */
	public getNext(pair);


	/**
	 * This method gets a value by name.
	 *
	 * @param name The name of value to get.
	 *
	 * @return If the name entry is found, then the associated value is returned; otherwise null is returned.
	 *
	 * @throws SWIG_Exception Thrown if the name is null.
	 * @throws GmsecException Thrown if the name is an empty string.
	 */
	public getValue(name);

	
	/**
     * This method gets a value by name.
     *
     * @param name         The name of the value to get
	 * @param defaultValue Value to return if the configuration element does not exist.
     *
     * @return If the name entry is found, then the associated value is returned; otherwise defaultValue is returned.
	 *
	 * @throws SWIG_Exception Thrown if the name is null.
     */
	public getValue(name, defaultValue);
	

	/**
	 * Attempts to return the boolean representation of the configuration element.
	 *
     * @param name The name of the value to get
	 *
	 * @return Boolean value of the named configuration element.
	 *
	 * @throws SWIG_Exception Thrown if the name string is null
	 * @throws GmsecException Thrown if the name is an empty string.
	 * @throws GmsecException Thrown if the value associated with the configuration element cannot be interpreted as a boolean value.
	 */
	public getBooleanValue(name);


	/**
	 * Attempts to return the boolean representation of the configuration element.
	 * <p>
	 * If the element does not exist or if there is no valid boolean interpretation of
	 * the value, then the default value is returned.
	 *
     * @param name         The name of the value to get
	 * @param defaultValue Value to return if the configuration element does not exist.
	 *
	 * @return Boolean value of the named configuration element, or possibly the default value.
	 *
	 * @throws SWIG_Exception Thrown if the name string is null
	 */
	public getBooleanValue(name, defaultValue);


	/**
	 * Attempts to return the integer representation of the configuration element.
	 *
	 * @param name The name of the value to get
	 *
	 * @return integer value of the name configuration element.
	 *
	 * @throws SWIG_Exception Thrown if the name string is null
	 * @throws GmsecException Thrown if the name is an empty string.
	 * @throws GmsecException Thrown if the value associated with the configuration element cannot be interpreted as an integer value.
	 */
	public getIntegerValue(name);


	/**
	 * Attempts to return the boolean representation of the configuration element.
	 * <p>
	 * If the element does not exist or if there is no valid integer interpretation of
	 * the value, then the default value is returned.
	 *
	 * @param name The name of the value to get
	 * @param defaultValue Value to return if the configuration element does not exist.
	 *
	 * @return integer value of the named configuration element, or possibly the default value.
	 *
	 * @throws SWIG_Exception Thrown if the name string is null
	 */
	public getIntegerValue(name, defaultValue);


	/**
	 * Attempts to return the double representation of the configuration element.
	 *
	 * @param name The name of the value to get
	 *
	 * @return double value of the named configuration element.
	 *
	 * @throws SWIG_Exception Thrown if the name string is null
	 * @throws GmsecException Thrown if the name is an empty string.
	 * @throws GmsecException Thrown if the value associated with the configuration element cannot be interpreted as a double value.
	 */
	public getDoubleValue(name);


	/**
	 * Attempts to return the double representation of the configuration element.
	 * <p>
	 * If the element does not exist or if there is no valid double interpretation of
	 * the value, then the default value is returned.
	 *
	 * @param name The name of the value to get
	 * @param defaultValue Value to return if the configuration element does not exist.
	 *
	 * @return double value of the named configuration element, or possibly the default value.
	 *
	 * @throws SWIG_Exception Thrown if the name string is null
	 */
	public getDoubleValue(name, defaultValue);


	/**
	 * Merge the given Config object with the current Config object.
	 *
	 * @param other The Config object from which to read name/value pairs.
	 * @param overwriteExisting Indicates whether to overwrite any existing name/value
	 * pairs within the working Config object.
	 *
	 * @throws SWIG_Exception Thrown if the given Config object is null.
	 */
	public merge(other, overwriteExisting);


	/**
	 * Convenience method for converting the Config object into XML representation.
	 * <p>
	 * Example XML configuration string:
	 * <pre>
	 * <CONFIG NAME="Bolt">
	 *     <PARAMETER NAME="mw-id">bolt</PARAMETER>
	 *     <PARAMETER NAME="mw-server">10.0.0.159</PARAMETER>
	 *     <PARAMETER NAME="tracking">false</PARAMETER>
	 *     <PARAMETER NAME="loglevel">info</PARAMETER>
	 * </CONFIG>
	 * </pre>
	 *
	 * @return XML string
	 */
	public toXML();


	/**
	 * This function will read a configuration from an XML string.
	 * The format is the same as that which is generated by toXML().
	 * <p>
	 * Example XML configuration string:
	 * <pre>
	 * <CONFIG NAME="Bolt">
	 *     <PARAMETER NAME="mw-id">bolt</PARAMETER>
	 *     <PARAMETER NAME="mw-server">10.0.0.159</PARAMETER>
	 *     <PARAMETER NAME="tracking">false</PARAMETER>
	 *     <PARAMETER NAME="loglevel">info</PARAMETER>
	 * </CONFIG>
	 * </pre>
	 *
	 * @param xml The XML string representation of a Config object.
	 *
	 * @throws SWIG_Exception Thrown if the XML string is null or contains an empty string.
	 * @throws GmsecException Thrown if the given XML string is not parsable.
	 *
	 * @see Config.toXML()
	 */
	public fromXML(xml);


	/**
	 * Attempts to open the given file and use the contents to create a Config object.
	 *
	 * @param filename   The configuration file to open.
	 * @param configName The name of the configuration to load/process.
	 *
	 * @return A Config object.
	 *
	 * @throws SWIG_Exception Thrown if the filename is null or contains an empty string.
	 * @throws GmsecException Thrown if the file cannot be opened.
	 * @throws GmsecException Thrown if the file does not contain a valid configuration construct.
	 * @throws GmsecException Thrown if the file does not contain the named configuration construct.
	 *
	 * @see Config.fromXML()
	 */
	public static getFromFile(filename, configName);


	/**
	 * This function will dump the Config into JSON format.
	 * <p>
	 * Example JSON configuration string:
	 * <pre>
	 * {"CONFIG":{"PARAMETER":[{"NAME":"mw-id","VALUE":"bolt"},{"NAME":"mw-server","VALUE":"localhost"}]}}
	 * </pre>
	 *
	 * @return JSON formatted string
	 */
	public toJSON();
}
