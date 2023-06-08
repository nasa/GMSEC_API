/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFileIterator.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.ConfigFile.*;
import gov.nasa.gsfc.gmsec.api5.jni.JNIConfigFileIterator;


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
	 * Reports back the next available ConfigFile.ConfigEntry in the ConfigFile.
	 *
	 * @return Returns the next ConfigFile.ConfigEntry.
	 *
	 * @throws GmsecException Thrown if no more ConfigFile.ConfigEntry objects are available.
	 *
	 * @see ConfigFileIterator#hasNextConfig()
	 */
	public ConfigFile.ConfigEntry nextConfig() throws GmsecException
	{
		return m_jniConfigFileIterator.nextConfig();
	}


	/**
	 * Reports back the next available ConfigFile.MessageEntry in the ConfigFile.
	 *
	 * @return Returns the next ConfigFile.MessageEntry.
	 *
	 * @throws GmsecException Thrown if no more ConfigFile.MessageEntry objects are available.
	 *
	 * @see ConfigFileIterator#hasNextMessage()
	 */
	public ConfigFile.MessageEntry nextMessage() throws GmsecException
	{
		return m_jniConfigFileIterator.nextMessage();
	}


	/**
	 * Reports the next available ConfigFile.SubscriptionEntry in the ConfigFile.
	 *
	 * @return Returns the next ConfigFile.SubscriptionEntry.
	 *
	 * @throws GmsecException Thrown if no more ConfigFile.SubscriptionEntry objects are available.
	 *
	 * @see ConfigFileIterator#hasNextSubscription()
	 */
	public ConfigFile.SubscriptionEntry nextSubscription() throws GmsecException
	{
		return m_jniConfigFileIterator.nextSubscription();
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
