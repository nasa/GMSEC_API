/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F32Field.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain a 32-bit floating point value.
 */
public class F32Field extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data A 32-bit floating point value.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public F32Field(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      A 32-bit floating point value.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public F32Field(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other F32Field object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given F32Field object is null.
	 */
	public F32Field(other);


	/**
	 * Returns the data stored within the F32Field.
	 *
	 * @return A 32-bit floating point value.
	 */
	public getValue();
}
