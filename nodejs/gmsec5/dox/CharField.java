/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CharField.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain a single character value.
 */
public class CharField extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Character data to associate with the field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 */
	public CharField(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      Character data to associate with the field.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 */
	public CharField(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other CharField object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given CharField is null.
	 */
	public CharField(other);


	/**
	 * Returns the data stored within the CharField.
	 *
	 * @return A char value.
	 */
	public getValue();
}
