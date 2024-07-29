/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageEntry.java
 */

package gmsec;


/**
 * This class provides access to the attributes associated with a ConfigFile's
 * {@link Message} entry.
 */
public class MessageEntry
{
	/**
	 * Returns the name associated with the entry.
	 *
	 * @return The name of the entry.
	 */
	public getName();

	/**
	 * Returns the {@link Message} object associated with the entry.
	 *
	 * @return A Message object.
	 */
	public getMessage();
}
