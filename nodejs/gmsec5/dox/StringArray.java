/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file StringArray.java
 */

package gmsec;


/**
 * This class holds an array of String objects.
 */
public class StringArray
{
	/**
	 * Provides insight into the number of entries in the array.
	 *
	 * @return The number of entries in the array.
	 */
	public size();


	/**
	 * Returns the String object at the given index.
	 *
	 * @return A String object.
	 *
	 * @throws SWIG_Exception Thrown if the index is out-of-bounds.
	 *
	 * @see size()
	 */
	public get(index);


	/**
	 * Adds the given String object to the array.
	 *
	 * @param entry The String object to add to the array.
	 *
	 * @throws SWIG_Exception Thrown if the given Field object is null.
	 */
	public add(entry);
}
