/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigEntry.java
 */

package gmsec;


/**
 * This class provides access to the attributes associated with a ConfigFile's
 * {@link Config} entry.
 */
public class ConfigEntry
{
	/**
	 * Returns the name associated with the entry.
	 *
	 * @return The name of the entry.
	 */
	public getName();

	/**
	 * Returns the {@link Config} object associated with the entry.
	 *
	 * @return A Config object.
	 */
	public getConfig();
}
