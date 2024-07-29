/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U32Field.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain an unsigned 32-bit value.
 */
public class U32Field extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data An unsigned 32-bit value.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public U32Field(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      An unsigned 32-bit value.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public U32Field(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other U32Field object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given U32Field is null.
	 */
	public U32Field(other);


	/**
	 * Returns the data stored within the U32Field.
	 *
	 * @return An unsigned 32-bit value.
	 */
	public getValue();
}
