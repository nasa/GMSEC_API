/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BlobArray.java
 */

package gmsec;


/**
 * This class holds an array of integer values in the range oc 0x00 thru 0xFF (not enforced!).
 */
public class BlobArray
{
	/**
	 * Provides insight into the number of entries in the array.
	 *
	 * @return The number of entries in the array.
	 */
	public size();


	/**
	 * Returns the value at the given index.
	 *
	 * @return An integer value in the range of 0x00 - 0xFF .
	 *
	 * @throws SWIG_Exception Thrown if the index is out-of-bounds.
	 *
	 * @see size()
	 */
	public get(index);


	/**
	 * Adds the given Field object to the array.
	 *
	 * @param value The integer value in the range of 0x00 - 0xFF.
	 *
	 * @throws SWIG_Exception Thrown if the given value is null.
	 */
	public add(value);
}
