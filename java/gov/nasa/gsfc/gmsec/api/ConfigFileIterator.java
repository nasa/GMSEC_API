/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFileIterator.java
 *
 * @brief Class that can be used to access elements of a configuration file (ConfigFile) object.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.ConfigFile.*;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfigFileIterator;


/** @class ConfigFileIterator
 *
 * @brief This class is used to access elements of a configuration file (ConfigFile) object.
 *
 * @sa ConfigFile.getIterator()
 */
public class ConfigFileIterator
{
	private JNIConfigFileIterator m_jniConfigFileIterator = null;


	public static JNIConfigFileIterator getInternal(ConfigFileIterator iter)
	{
		return (iter == null ? null : iter.m_jniConfigFileIterator);
	}


	public ConfigFileIterator(JNIConfigFileIterator jIter)
	{
		m_jniConfigFileIterator = jIter;
	}


	private ConfigFileIterator()
	{
	}


	/**
	 * @fn boolean hasNextConfig()
	 *
	 * @brief Reports back whether there is a next config attribute in the ConfigFile.
	 *
	 * @return Returns true if another config is available; false otherwise.
	 */
	public boolean hasNextConfig()
	{
		return m_jniConfigFileIterator.hasNextConfig();
	}


	/**
	 * @fn boolean hasNextMessage()
	 *
	 * @brief Reports back whether there is a next message attribute in the ConfigFile.
	 *
	 * @return Returns true if another message is available; false otherwise.
	 */
	public boolean hasNextMessage()
	{
		return m_jniConfigFileIterator.hasNextMessage();
	}


	/**
	 * @fn boolean hasNextSubscription()
	 *
	 * @brief Reports back whether there is a next subscription attribute in the ConfigFile.
	 *
	 * @return Returns true if another subscription is available; false otherwise.
	 */
	public boolean hasNextSubscription()
	{
		return m_jniConfigFileIterator.hasNextSubscription();
	}


	/**
	 * @fn boolean hasNextCustomElement()
	 *
	 * @brief Reports back whether there is a next custom XML element in the ConfigFile.
	 *
	 * @return Returns true if another custom XML element is available; false otherwise.
	 */
	public boolean hasNextCustomElement()
	{
		return m_jniConfigFileIterator.hasNextCustomElement();
	}


	/**
	 * @fn ConfigFile.ConfigEntry nextConfig()
	 *
	 * @brief Reports back the next available Config object in the ConfigFile.
	 *
	 * @return Returns a Config object.
	 *
	 * @throw A GMSEC_Exception is thrown if no more Config objects are available.
	 *
	 * @sa hasNextConfig()
	 */
	public ConfigFile.ConfigEntry nextConfig() throws GMSEC_Exception
	{
		return m_jniConfigFileIterator.nextConfig();
	}


	/**
	 * @fn ConfigFile.MessageEntry nextMessage()
	 *
	 * @brief Reports back the next available Message object in the ConfigFile.
	 *
	 * @return Returns a Message object.
	 *
	 * @throw A GMSEC_Exception is thrown if no more Message objects are available.
	 *
	 * @sa hasNextMessage()
	 */
	public ConfigFile.MessageEntry nextMessage() throws GMSEC_Exception
	{
		return m_jniConfigFileIterator.nextMessage();
	}


	/**
	 * @fn ConfigFile.SubscriptionEntry nextSubscription()
	 *
	 * @brief Reports the next available subscription subject/topic in the ConfigFile.
	 *
	 * @return Returns subscription subject/topic.
	 *
	 * @throw A GMSEC_Exception is thrown if no more subscription subject/topics are available.
	 *
	 * @sa hasNextSubscription()
	 */
	public ConfigFile.SubscriptionEntry nextSubscription() throws GMSEC_Exception
	{
		return m_jniConfigFileIterator.nextSubscription();
	}


	/**
	 * @fn Message nextCustomElement()
	 *
	 * @brief Reports back the next available custom XML element in the ConfigFile.
	 *
	 * @return Returns an XML string of the custom element.
	 *
	 * @throw A GMSEC_Exception is thrown if no more custom XML elements are available.
	 *
	 * @sa hasNextCustomElement()
	 */
	public String nextCustomElement() throws GMSEC_Exception
	{
		return m_jniConfigFileIterator.nextCustomElement();
	}


	/**
	 * @fn void reset()
	 *
	 * @desc Resets all iterators to the beginning of the respective lists that are maintained by the ConfigFile.
	 */
	public void reset()
	{
		m_jniConfigFileIterator.reset();
	}
}
