/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Config.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.jni.JNIConfig;


/**
 * Encapsulates key/value parameters.
 * <p>
 * Example creation and use of Config:
 * <pre>{@code
 * import gov.nasa.gsfc.gmsec.api5.*;
 *
 * import java.util.Collection;
 * import java.util.Map;
 * import java.util.Map.Entry;
 *
 * public static void main(String[] args)
 * {
 *     Config config = new Config(args);
 *
 *     Collection<Map.Entry<String, String>> entries = config.entrySet();
 *
 *     for (Map.Entry<String, String> entry : entries)
 *     {
 *         System.out.println("[" + entry.getKey() + ", " + entry.getValue() + "]");
 *     }
 *
 *     Connection conn = null;
 *     try
 *     {
 *         conn = Connection.create(config);
 *
 *         conn.connect();
 *
 *         // ...
 *
 *         conn.disconnect();
 *     }
 *     catch (IllegalArgumentException | GmsecException e)
 *     {
 *         System.err.println(e.toString());
 *     }
 *     finally
 *     {
 *       if (conn != null) {
 *           Connection.destroy(conn);
 *       }
 *     }
 * }
 * }</pre>
 *
 * @see Connection
 * @see MessageFactory
 */
public class Config
{
	private JNIConfig m_jniConfig = null;


	//! @cond
	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param cfg Object to reference for acquiring internal JNIConfig
	 * @return Internal JNIConfig object
	 */
	public static JNIConfig getInternal(Config cfg)
	{
		return (cfg == null ? null : cfg.m_jniConfig);
	}


	/**
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param jcfg Internal JNIConfig object
	 */
	public Config(JNIConfig jcfg)
	{
		m_jniConfig = jcfg;
	}
	//! @endcond


	/**
	 * Creates an empty configuration object
	 */
	public Config()
	{
		m_jniConfig = new JNIConfig();
	}


	/**
	 * Creates an configuration object using the key/value pairs from the
	 * command-line parameters that are in the form of key=value.
	 * <p>
	 * For example, an application could instantiate a Config object when passed parameters such as:
	 * <pre>{@code
	 * java gmpub mw-id=bolt server=10.1.2.159
	 * }</pre>
	 *
	 * @param args an array of string parameters
	 */
	public Config(String[] args)
	{
		m_jniConfig = new JNIConfig(args);
	}


	/**
	 * Creates an configuration object by parsing the key/value pairs from the
	 * given data string.
	 * <p>
	 * The data string can contain either XML or JSON formatted configuration
	 * data, or contain raw data consisting of white-space separated key=value pairs.
	 * <p>
	 * Example XML string:
	 * <pre>{@code
	 * <CONFIG>
	 *     <PARAMETER NAME="mw-id">bolt</PARAMETER>
	 *     <PARAMETER NAME="server">10.1.2.159</PARAMETER>
	 *     <PARAMETER NAME="tracking">true</PARAMETER>
	 *     <PARAMETER NAME="loglevel">info</PARAMETER>
	 * </CONFIG>
	 * }</pre>
	 * <p>
	 * Example key=value string:
	 * <pre>{@code
	 * mw-id=bolt server=10.1.2.159 tracking=true loglevel=info
	 * }</pre>
	 *
	 * @param data data string containing configuration information
	 * @param type the type of data being provided
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null, or contains an empty string.
	 * @throws GmsecException Thrown if the given data string is not parsable.
	 *
	 * @see Config#toXML()
	 */
	public Config(String data, Gmsec.DataType type)
		throws IllegalArgumentException, GmsecException
	{
		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data string is null or contains an empty string");
		}

		m_jniConfig = new JNIConfig(data, type);
	}


	/**
	 * Copy-constructor for initializing a Config object using the configuration
	 * values from an existing Config object.
	 *
	 * @param other the existing Config object to copy.
	 */
	public Config(Config other)
	{
		if (other != null)
		{
			m_jniConfig = new JNIConfig(other.m_jniConfig);
		}
		else
		{
			m_jniConfig = new JNIConfig();
		}
	}


	/**
	 * Stores a key/value pair within the Config object.
	 *
	 * @param key   the key associated with the value.
	 * @param value the value associated with the key.
	 *
	 * @throws IllegalArgumentException Thrown if the key is null or contains an empty string,
	 * or if the value is null.
	 */
	public void addValue(String key, String value) throws IllegalArgumentException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}
		if (value == null)
		{
			throw new IllegalArgumentException("Value is null");
		}

		m_jniConfig.addValue(key, value);
	}


	/**
	 * Clears the key, and associated value, from the Config object.
	 *
	 * @param key key of value to clear
	 *
	 * @return True if the operation was successful; false if the key was not found.
	 *
	 * @throws IllegalArgumentException Thrown if the key string is null, or contains an empty string.
	 */
	public boolean clearValue(String key) throws IllegalArgumentException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.clearValue(key);
	}


	/**
	 * Clears all key/value pairs from the Config object.
	 */
	public void clear()
	{
		m_jniConfig.clear();
	}


	/**
	 * Returns a collection of the key/value pairs stored in the Config object.
	 *
	 * @return A Collection of key/value entries.
	 */
	public java.util.Collection<java.util.Map.Entry<String, String>> entrySet()
	{
		return m_jniConfig.entrySet();
	}


	/**
	 * Returns the collection of keys within the Config object.
	 *
	 * @return A Collection of key strings.
	 */
	public java.util.Collection<String> keySet()
	{
		return m_jniConfig.keySet();
	}


	/**
	 * This method gets a value by key.
	 *
	 * @param key key of value to get
	 *
	 * @return If the key entry is found, then the associated value is returned; otherwise null is returned.
	 *
	 * @throws IllegalArgumentException Thrown if the key string is null, or contains an empty string.
	 */
	public String getValue(String key) throws IllegalArgumentException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getValue(key);
	}

	
	/**
     * This method gets a value by key.
     *
     * @param key Key of value to get
	 * @param defaultValue Value to return if the configuration element does not exist.
     *
     * @return If the key entry is found, then the associated value is returned; otherwise defaultValue is returned.
     */
	public String getValue(String key, String defaultValue)
	{
		return m_jniConfig.getValue(key, defaultValue);
	}	
	

	/**
	 * Attempts to return the boolean representation of the configuration element.
	 *
	 * @param key key of value to get
	 *
	 * @return Boolean value of the keyd configuration element.
	 *
	 * @throws IllegalArgumentException Thrown if the key string is null, or contains an empty string.
	 * @throws GmsecException Thrown if the value associated with the configuration element cannot be interpreted as a boolean value.
	 */
	public boolean getBooleanValue(String key) throws IllegalArgumentException, GmsecException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getBooleanValue(key);
	}


	/**
	 * Attempts to return the boolean representation of the configuration element.
	 * <p>
	 * If the element does not exist or if there is no valid boolean interpretation of
	 * the value, then the default value is returned.
	 *
	 * @param key key of value to get
	 * @param defaultValue Value to return if the configuration element does not exist.
	 *
	 * @return Boolean value of the keyd configuration element, or possibly the default value.
	 */
	public boolean getBooleanValue(String key, boolean defaultValue)
	{
		return m_jniConfig.getBooleanValue(key, defaultValue);
	}


	/**
	 * Attempts to return the integer representation of the configuration element.
	 *
	 * @param key Key of value to get
	 *
	 * @return integer value of the key configuration element.
	 *
	 * @throws IllegalArgumentException Thrown if the key string is null, or contains an empty string.
	 * @throws GmsecException Thrown if the value associated with the configuration element cannot be interpreted as an integer value.
	 */
	public int getIntegerValue(String key) throws IllegalArgumentException, GmsecException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getIntegerValue(key);
	}


	/**
	 * Attempts to return the boolean representation of the configuration element.
	 * <p>
	 * If the element does not exist or if there is no valid integer interpretation of
	 * the value, then the default value is returned.
	 *
	 * @param key Key of value to get
	 * @param defaultValue Value to return if the configuration element does not exist.
	 *
	 * @return integer value of the keyd configuration element, or possibly the default value.
	 */
	public int getIntegerValue(String key, int defaultValue)
	{
		return m_jniConfig.getIntegerValue(key, defaultValue);
	}


	/**
	 * Attempts to return the double representation of the configuration element.
	 *
	 * @param key Key of value to get
	 *
	 * @return double value of the keyd configuration element.
	 *
	 * @throws IllegalArgumentException Thrown if the key string is null, or contains an empty string.
	 * @throws GmsecException Thrown if the value associated with the configuration element cannot be interpreted as a double value.
	 */
	public double getDoubleValue(String key) throws IllegalArgumentException, GmsecException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getDoubleValue(key);
	}


	/**
	 * Attempts to return the double representation of the configuration element.
	 * <p>
	 * If the element does not exist or if there is no valid double interpretation of
	 * the value, then the default value is returned.
	 *
	 * @param key Key of value to get
	 * @param defaultValue Value to return if the configuration element does not exist.
	 *
	 * @return double value of the keyd configuration element, or possibly the default value.
	 */
	public double getDoubleValue(String key, double defaultValue)
	{
		return m_jniConfig.getDoubleValue(key, defaultValue);
	}


	/**
	 * Merge the given Config object with the current Config object.
	 *
	 * @param other the Config object from which to read key/value pairs.
	 * @param overwriteExisting indicates whether to overwrite any existing key/value
	 * pairs within the working Config object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 */
	public void merge(Config other, boolean overwriteExisting) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("Config object is null");
		}

		m_jniConfig.merge(other, overwriteExisting);
	}


	/**
	 * Convenience method for converting the Config object into XML representation.
	 * The format is the same as that which is accepted by {@link Config#Config(String, Gmsec.DataType)}.
	 *
	 * @return XML string
	 *
	 * @see Config#Config(String, Gmsec.DataType)
	 * @see Config#fromXML(String)
	 */
	public String toXML()
	{
		return m_jniConfig.toXML();
	}


	/**
	 * This function will read a configuration from an XML string.
	 * The format is the same as that which is generated by toXML().
	 * <p>
	 * Example XML string:
	 * <pre>{@code
	 * <CONFIG NAME="Bolt">
	 *     <PARAMETER NAME="mw-id">bolt</PARAMETER>
	 *     <PARAMETER NAME="server">10.0.0.159</PARAMETER>
	 *     <PARAMETER NAME="tracking">false</PARAMETER>
	 *     <PARAMETER NAME="loglevel">info</PARAMETER>
	 * </CONFIG>
	 * }</pre>
	 *
	 * @param xml XML string representation of a Config object.
	 *
	 * @throws IllegalArgumentException Thrown if the XML string is null or contains an empty string.
	 * @throws GmsecException Thrown if the given XML string is not parsable.
	 *
	 * @see Config#toXML()
	 */
	public void fromXML(String xml) throws IllegalArgumentException, GmsecException
	{
		if (xml == null || xml.isEmpty())
		{
			throw new IllegalArgumentException("XML string is null or contains an empty string");
		}

		m_jniConfig.fromXML(xml);
	}


	/**
	 * Attempts to open the given file and use the contents to create a Config object.
	 *
	 * @param filename   The configuration file to open.
	 * @param configName The name of the configuration to load/process.
	 *
	 * @return A Config object.
	 *
	 * @throws IllegalArgumentException Thrown if the filename is null or contains an empty string.
	 * @throws GmsecException Thrown if the file cannot be opened.
	 * @throws GmsecException Thrown if the file does not contain a valid configuration construct.
	 * @throws GmsecException Thrown if the file does not contain the named configuration construct.
	 *
	 * @see Config#fromXML
	 */
	public static Config getFromFile(String filename, String configName) throws IllegalArgumentException, GmsecException
	{
		if (filename == null || filename.isEmpty())
		{
			throw new IllegalArgumentException("Configuration file name is null or contains an empty string");
		}

		return JNIConfig.getFromFile(filename, configName);
	}


	/**
	 * This function will dump the Config into JSON format.
	 * The format is the same as that which is accepted by {@link Config#Config(String, Gmsec.DataType)}.
	 *
	 * @return JSON formatted string
	 *
	 * @see Config#Config(String, Gmsec.DataType)
	 */
	public String toJSON()
	{
		return m_jniConfig.toJSON();
	}
}
