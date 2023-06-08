/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U64Field.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain an unsigned 64-bit value.
 */
public class U64Field extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data An unsigned 64-bit value.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 */
	public U64Field(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      An unsigned 64-bit value.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 */
	public U64Field(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other U64Field object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given U64Field is null.
	 */
	public U64Field(other);


	/**
	 * Returns the data stored within the U64Field.
	 *
	 * @return An unsigned 64-bit value.
	 */
	public getValue();
}
