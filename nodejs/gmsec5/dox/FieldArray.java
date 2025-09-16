/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file FieldArray.java
 */

package gmsec;


/**
 * This class holds an array of Field objects.
 */
public class FieldArray
{
	/**
	 * Provides insight into the number of entries in the array.
	 *
	 * @return The number of entries in the array.
	 */
	public size();


	/**
	 * Returns the Field object at the given index.
	 *
	 * @return A Field object.
	 *
	 * @throws SWIG_Exception Thrown if the index is out-of-bounds.
	 *
	 * @see size()
	 */
	public get(index);


	/**
	 * Adds the given Field object to the array.
	 *
	 * @param field The Field object to add to the array.
	 *
	 * @throws SWIG_Exception Thrown if the given Field object is null.
	 */
	public add(field);
}
