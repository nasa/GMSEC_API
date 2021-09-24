/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFileIterator.java
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.ConfigFile.*;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfigFileIterator;


/**
 * This class is used to access elements of a configuration file that has been loaded
 * into a {@link ConfigFile} object.
 *
 * @see ConfigFile#getIterator()
 */
public class ConfigFileIterator
{
	private JNIConfigFileIterator m_jniConfigFileIterator = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param iter Object to reference for acquiring internal JNIConfigFileIterator
	 * @return Internal JNIConfigFileIterator object
	 */
	public static JNIConfigFileIterator getInternal(ConfigFileIterator iter)
	{
		return (iter == null ? null : iter.m_jniConfigFileIterator);
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jIter Internal JNIConfigFileIterator object
	 */
	public ConfigFileIterator(JNIConfigFileIterator jIter)
	{
		m_jniConfigFileIterator = jIter;
	}


	private ConfigFileIterator()
	{
	}


	/**
	 * Reports back whether there is a next config attribute in the ConfigFile.
	 *
	 * @return Returns true if another config is available; false otherwise.
	 */
	public boolean hasNextConfig()
	{
		return m_jniConfigFileIterator.hasNextConfig();
	}


	/**
	 * Reports back whether there is a next message attribute in the ConfigFile.
	 *
	 * @return Returns true if another message is available; false otherwise.
	 */
	public boolean hasNextMessage()
	{
		return m_jniConfigFileIterator.hasNextMessage();
	}


	/**
	 * Reports back whether there is a next subscription attribute in the ConfigFile.
	 *
	 * @return Returns true if another subscription is available; false otherwise.
	 */
	public boolean hasNextSubscription()
	{
		return m_jniConfigFileIterator.hasNextSubscription();
	}


	/**
	 * Reports back whether there is a next custom XML element in the ConfigFile.
	 *
	 * @return Returns true if another custom XML element is available; false otherwise.
	 */
	public boolean hasNextCustomElement()
	{
		return m_jniConfigFileIterator.hasNextCustomElement();
	}


	/**
	 * Reports back the next available Config object in the ConfigFile.
	 *
	 * @return Returns a Config object.
	 *
	 * @throws GMSEC_Exception Thrown if no more Config objects are available.
	 *
	 * @see ConfigFileIterator#hasNextConfig()
	 */
	public ConfigFile.ConfigEntry nextConfig() throws GMSEC_Exception
	{
		return m_jniConfigFileIterator.nextConfig();
	}


	/**
	 * Reports back the next available Message object in the ConfigFile.
	 *
	 * @return Returns a Message object.
	 *
	 * @throws GMSEC_Exception Thrown if no more Message objects are available.
	 *
	 * @see ConfigFileIterator#hasNextMessage()
	 */
	public ConfigFile.MessageEntry nextMessage() throws GMSEC_Exception
	{
		return m_jniConfigFileIterator.nextMessage();
	}


	/**
	 * Reports the next available subscription subject/topic in the ConfigFile.
	 *
	 * @return Returns subscription subject/topic.
	 *
	 * @throws GMSEC_Exception Thrown if no more subscription subject/topics are available.
	 *
	 * @see ConfigFileIterator#hasNextSubscription()
	 */
	public ConfigFile.SubscriptionEntry nextSubscription() throws GMSEC_Exception
	{
		return m_jniConfigFileIterator.nextSubscription();
	}


	/**
	 * Reports back the next available custom XML element in the ConfigFile.
	 *
	 * @return Returns an XML string of the custom element.
	 *
	 * @throws GMSEC_Exception Thrown if no more custom XML elements are available.
	 *
	 * @see ConfigFileIterator#hasNextCustomElement()
	 */
	public String nextCustomElement() throws GMSEC_Exception
	{
		return m_jniConfigFileIterator.nextCustomElement();
	}


	/**
	 * Resets all iterators to the beginning of the respective lists that are maintained
	 * by the ConfigFile.
	 */
	public void reset()
	{
		m_jniConfigFileIterator.reset();
	}
}
