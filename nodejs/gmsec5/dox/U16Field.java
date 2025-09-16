/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U16Field.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain an unsigned 16-bit value.
 */
public class U16Field extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data An unsigned 16-bit value.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public U16Field(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      An unsigned 16-bit value.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public U16Field(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other U16Field object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given U16Field is null.
	 */
	public U16Field(other);


	/**
	 * Returns the data stored within the U16Field.
	 *
	 * @return An unsigned 16-bit value.
	 */
	public getValue();
}
