/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Config.java
 *
 * @brief Class that can be used to store key=value pairs.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNIConfig;


/** @class Config
 *
 * @brief Encapsulates the parameters necessary for initializing connections.  Used
 * by the Connection, this class provides a generic means of supplying initialization
 * data without being Connection specific.
 *
 * Example creation and use of Config:
 * @code
 * import gov.nasa.gsfc.gmsec.api.*;
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
 *
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
 *     catch (IllegalArgumentException | GMSEC_Exception e)
 *     {
 *         System.err.println(e.toString());
 *     }
 *     finally
 *     {
 *         if (conn != null)
 *         {
 *             Connection.destroy(conn);
 *         }
 *     }
 * }
 * @endcode
 *
 * @sa Connection
 * @sa mist.ConnectionManager
 * @sa Message
 */
public class Config
{
	private JNIConfig m_jniConfig = null;


	public static JNIConfig getInternal(Config cfg)
	{
		return (cfg == null ? null : cfg.m_jniConfig);
	}


	public Config(JNIConfig jcfg)
	{
		m_jniConfig = jcfg;
	}


	/**
	 * @fn Config()
	 *
	 * @desc Creates an empty configuration object
	 */
	public Config()
	{
		m_jniConfig = new JNIConfig();
	}


	/**
	 * @fn Config(String[] args)
	 *
	 * @brief Creates an configuration object using the key/value pairs from the
	 * command-line parameters that are in the form of \<key\>=\<value\>.
	 *
	 * Example:
	 * @code
	 * java gmpub connectionType=gmsec_bolt server=localhost 
	 * @endcode
	 *
	 * @param args - an array of string parameters
	 */
	public Config(String[] args)
	{
		m_jniConfig = new JNIConfig(args);
	}


	/**
	 * @fn Config(String xml)
	 *
	 * @brief Creates an configuration object using the XML string representation
	 * of a Config object.
	 *
	 * Example XML string:
	 * @code
	 * <CONFIG>
	 *     <PARAMETER NAME="connectionType">gmsec_bolt</PARAMETER>
	 *     <PARAMETER NAME="server">10.1.2.159</PARAMETER>
	 *     <PARAMETER NAME="tracking">true</PARAMETER>
	 *     <PARAMETER NAME="loglevel">info</PARAMETER>
	 * </CONFIG>
	 * @endcode
	 *
	 * @param xml - an XML string that represents a Config object.
	 *
	 * @throws An IllegalArgumentException is thrown if the XML string is null, or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the given XML string is not parsable.
	 *
	 * @sa toXML()
	 */
	public Config(String xml) throws IllegalArgumentException, GMSEC_Exception
	{
		if (xml == null || xml.isEmpty())
		{
			throw new IllegalArgumentException("XML string is null or contains an empty string");
		}

		m_jniConfig = new JNIConfig(xml);
	}


	/**
	 * @fn Config(Config other)
	 *
	 * @brief Copy-constructor for initializing a Config object using the configuration
	 * values from an existing Config object.
	 *
	 * @param other - the existing Config object to copy.
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
	 * @fn void addValue(String key, String value)
	 *
	 * @brief Stores a key/value pair within the Config object.
	 *
	 * @param key   - the key associated with the value.
	 * @param value - the value associated with the key.
	 *
	 * @throws An IllegalArgumentException is thrown if the key is null or contains an empty string,
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
	 * @fn boolean clearValue(String key);
	 *
	 * @brief Clears the key, and associated value, from the Config object.
	 *
	 * @param key - key of value to clear
	 *
	 * @return True if the operation was successful; false if the key was not found.
	 *
	 * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
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
	 * @fn void clear()
	 *
	 * @desc Clears all key/value pairs from the Config object.
	 */
	public void clear()
	{
		m_jniConfig.clear();
	}


	/**
	 * @fn java.util.Collection<java.util.Map.Entry<String, String> > entrySet()
	 *
	 * @brief Returns a collection of the key/value pairs stored in the Config object.
	 */
	public java.util.Collection<java.util.Map.Entry<String, String>> entrySet()
	{
		return m_jniConfig.entrySet();
	}


	/**
	 * @fn java.util.Collection<String> keySet()
	 *
	 * @brief Returns the collection of keys within the Config object.
	 *
	 * @return A collection of key strings.
	 */
	public java.util.Collection<String> keySet()
	{
		return m_jniConfig.keySet();
	}


	/**
	 * @fn String getValue(String key)
	 *
	 * @brief This method gets a value by key.
	 *
	 * @param key - key of value to get
	 *
	 * @return If the key entry is found, then the associated value is returned; otherwise null is returned.
	 *
	 * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
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
     * @fn String getValue(String key, String defaultValue)
     *
     * @brief This method gets a value by key.
     *
     * @param key - key of value to get
	 *
	 * @param defaultValue - value to be returned if key does not exist
     *
     * @return If the key entry is found, then the associated value is returned; otherwise defaultValue is returned.
     *
     * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
     */
	public String getValue(String key, String defaultValue) throws IllegalArgumentException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getValue(key, defaultValue);
	}	
	

	/**
	 * @fn boolean getBooleanValue(String key)
	 *
	 * @brief Attempts to return the boolean representation of the configuration element.
	 * An exception is thrown if the element does not exist or if there is no valid boolean
	 * interpretation for the value.
	 *
	 * @param key - key of value to get
	 *
	 * @return Boolean value of the keyd configuration element.
	 *
	 * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the value associated with the configuration element cannot be interpreted as a boolean value.
	 */
	public boolean getBooleanValue(String key) throws IllegalArgumentException, GMSEC_Exception
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getBooleanValue(key);
	}


	/**
	 * @fn boolean getBooleanValue(String key, boolean defaultValue)
	 *
	 * @brief Attempts to return the boolean representation of the configuration element.
	 * If the element does not exist or if there is no valid boolean interpretation of
	 * the value, then the default value is returned.
	 *
	 * @param key - key of value to get
	 *
	 * @return Boolean value of the keyd configuration element, or possibly the default value.
	 *
	 * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
	 */
	public boolean getBooleanValue(String key, boolean defaultValue) throws IllegalArgumentException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getBooleanValue(key, defaultValue);
	}


	/**
	 * @fn int getIntegerValue(String key)
	 *
	 * @brief Attempts to return the integer representation of the configuration element.
	 * An exception is thrown if the element does not exist or if there is no valid integer 
	 * interpretation for the value.
	 *
	 * @param key - key of value to get
	 *
	 * @return int value of the keyd configuration element.
	 *
	 * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the value associated with the configuration element cannot be interpreted as an integer value.
	 */
	public int getIntegerValue(String key) throws IllegalArgumentException, GMSEC_Exception
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getIntegerValue(key);
	}


	/**
	 * @fn int getIntegerValue(String key, int defaultValue)
	 *
	 * @brief Attempts to return the boolean representation of the configuration element.
	 * If the element does not exist or if there is no valid integer interpretation of
	 * the value, then the default value is returned.
	 *
	 * @param key - key of value to get
	 *
	 * @return int value of the keyd configuration element, or possibly the default value.
	 *
	 * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
	 */
	public int getIntegerValue(String key, int defaultValue) throws IllegalArgumentException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getIntegerValue(key, defaultValue);
	}


	/**
	 * @fn double getDoubleValue(String key)
	 *
	 * @brief Attempts to return the double representation of the configuration element.
	 * An exception is thrown if the element does not exist or if there is no valid double
	 * interpretation for the value.
	 *
	 * @param key - key of value to get
	 *
	 * @return double value of the keyd configuration element.
	 *
	 * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the value associated with the configuration element cannot be interpreted as a double value.
	 */
	public double getDoubleValue(String key) throws IllegalArgumentException, GMSEC_Exception
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getDoubleValue(key);
	}


	/**
	 * @fn double getDoubleValue(String key, double defaultValue)
	 *
	 * @brief Attempts to return the double representation of the configuration element.
	 * If the element does not exist or if there is no valid double interpretation of
	 * the value, then the default value is returned.
	 *
	 * @param key - key of value to get
	 *
	 * @return double value of the keyd configuration element, or possibly the default value.
	 *
	 * @throws An IllegalArgumentException is thrown if the key string is null, or contains an empty string.
	 */
	public double getDoubleValue(String key, double defaultValue) throws IllegalArgumentException
	{
		if (key == null || key.isEmpty())
		{
			throw new IllegalArgumentException("Key is null or contains an empty string");
		}

		return m_jniConfig.getDoubleValue(key, defaultValue);
	}


	/**
	 * @fn String toXML()
	 *
	 * @brief Convenience method for converting the Config object into XML representation.
	 *
	 * @return XML string
	 *
	 * @sa fromXML()
	 */
	public String toXML()
	{
		return m_jniConfig.toXML();
	}


	/**
	 * @fn merge(Config other, boolean overwriteExisting)
	 *
	 * @brief Merge the given %Config object with the current %Config object.
	 *
	 * @param other - the %Config object from which to read key/value pairs.
	 * @param overwriteExisting - indicates whether to overwrite any existing key/value
	 * pairs within the working %Config object.
	 */
	public void merge(Config other, boolean overwriteExisting)
	{
		m_jniConfig.merge(other, overwriteExisting);
	}


	/**
	 * @fn void fromXML(String xml)
	 *
	 * @brief This function will read a configuration from an XML string.
	 * The format is the same as that which is generated by toXML().
	 *
	 * Example XML string:
	 * @code
	 * <CONFIG>
	 *     <PARAMETER NAME="connectionType">gmsec_bolt</PARAMETER>
	 *     <PARAMETER NAME="server">10.1.2.159</PARAMETER>
	 *     <PARAMETER NAME="tracking">true</PARAMETER>
	 *     <PARAMETER NAME="loglevel">info</PARAMETER>
	 * </CONFIG>
	 * @endcode
	 *
	 * @param xml - XML string representation of a Config object.
	 *
	 * @throws An IllegalArgumentException is thrown if the XML string is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the given XML string is not parsable.
	 *
	 * @sa toXML()
	 */
	public void fromXML(String xml) throws IllegalArgumentException, GMSEC_Exception
	{
		if (xml == null || xml.isEmpty())
		{
			throw new IllegalArgumentException("XML string is null or contains an empty string");
		}

		m_jniConfig.fromXML(xml);
	}
}
