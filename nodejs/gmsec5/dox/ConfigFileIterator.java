/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFileIterator.java
 */

package gmsec;


/**
 * This class is used to access elements of a configuration file that has been loaded
 * into a {@link ConfigFile} object.
 *
 * @see ConfigFile.getIterator()
 */
public class ConfigFileIterator
{
	private ConfigFileIterator();


	/**
	 * Reports back whether there is a next config attribute in the ConfigFile.
	 *
	 * @return Returns true if another config is available; false otherwise.
	 */
	public hasNextConfig();


	/**
	 * Reports back whether there is a next message attribute in the ConfigFile.
	 *
	 * @return Returns true if another message is available; false otherwise.
	 */
	public hasNextMessage();


	/**
	 * Reports back whether there is a next subscription attribute in the ConfigFile.
	 *
	 * @return Returns true if another subscription is available; false otherwise.
	 */
	public hasNextSubscription();


	/**
	 * Reports back the next available ConfigEntry object in the ConfigFile.
	 *
	 * @return Returns a ConfigEntry object.
	 *
	 * @throws GmsecException Thrown if no more ConfigEntry objects are available.
	 *
	 * @see ConfigFileIterator.hasNextConfig()
	 */
	public nextConfig();


	/**
	 * Reports back the next available MessageEntry object in the ConfigFile.
	 *
	 * @return Returns a MessageEntry object.
	 *
	 * @throws GmsecException Thrown if no more MessageEntry objects are available.
	 *
	 * @see ConfigFileIterator.hasNextMessage()
	 */
	public nextMessage();


	/**
	 * Reports the next available SubscriptionEntry object in the ConfigFile.
	 *
	 * @return Returns a SubscriptionEntry object.
	 *
	 * @throws GmsecException Thrown if no more SubscriptionEntry objects are available.
	 *
	 * @see ConfigFileIterator.hasNextSubscription()
	 */
	public nextSubscription();


	/**
	 * Resets all iterators to the beginning of the respective lists that are maintained
	 * by the ConfigFile.
	 */
	public reset();
}
