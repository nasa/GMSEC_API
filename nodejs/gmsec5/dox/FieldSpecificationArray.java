/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file FieldSpecificationArray.java
 */

package gmsec;


/**
 * This class holds an array of FieldSpecification objects.
 */
public class FieldSpecificationArray
{
	/**
	 * Provides insight into the number of entries in the array.
	 *
	 * @return The number of entries in the array.
	 */
	public size();


	/**
	 * Returns the FieldSpecification object at the given index.
	 *
	 * @return A FieldSpecification object.
	 *
	 * @throws SWIG_Exception Thrown if the index is out-of-bounds.
	 *
	 * @see size()
	 */
	public get(index);
}
