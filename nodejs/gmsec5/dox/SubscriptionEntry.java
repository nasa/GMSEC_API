/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SubscriptionEntry.java
 */

package gmsec;


/**
 * This class provides access to the attributes associated with a ConfigFile's
 * subscription entry.
 */
public class SubscriptionEntry
{
	/**
	 * Constructor
	 *
	 * @param name    The name to associate with the SubscriptionEntry
	 * @param pattern A subscription pattern/topic to associate with the SubscriptionEntry
	 *
	 * @throws SWIG_Exception Thrown if either the name or the pattern are null, or if
	 * either consist of an empty string
	 */
	public SubscriptionEntry(name, pattern);

	/**
	 * Returns the name associated with the subscription entry.
	 *
	 * @return The subscription name.
	 */
	public getName();

	/**
	 * Returns the subject/topic associated with the subscription entry.
	 *
	 * @return A subject/topic string.
	 */
	public getPattern();

	/**
	 * Associates an excluded subscription pattern/topic with the SubscriptionEntry.
	 * @param pattern The pattern/topic to exclude
	 * @throws SWIG_Exception Thrown if the pattern is null or is an empty string
	 */
	public addExcludedPattern(pattern);

	/**
	 * Returns true if there is a next excluded pattern associated with the subscription entry;
	 * false otherwise.
	 *
	 * @return A boolean.
	 */
	public hasNextExcludedPattern();

	/**
	 * Returns the next excluded pattern associated with the subscription entry. 
	 *
	 * @return A string containing the excluded pattern.
	 */
	public nextExcludedPattern();
}
