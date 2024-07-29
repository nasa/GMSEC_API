/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SchemaIDIterator.java
 */

package gmsec;


/**
 * Supports the iteration over the list of schema templates maintained by a Specification object.
 *
 * @see Specification
 */
public class SchemaIDIterator
{
	/**
	 * Indicates whether there are additional Schema IDs that can be referenced using next().
	 *
	 * @return True is returned if additional fields are available, false otherwise.
	 */
	public hasNext();


	/**
	 * Returns the next available Schema ID.
	 *
	 * @return A Schema ID string.
	 *
	 * @throws GmsecException Thrown if the iterator has reached the end (i.e. there are no more IDs).
	 */
	public next();


	/**
	 * Resets the iterator to the beginning of the schema list that is maintained by the Specification object.
	 */
	public reset();
}
