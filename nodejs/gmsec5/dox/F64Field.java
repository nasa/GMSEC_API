/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F64Field.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain a 64-bit floating point value.
 */
public class F64Field extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data A 64-bit floating point value.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public F64Field(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      A 64-bit floating point value.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public F64Field(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other F64Field object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given F64Field object is null.
	 */
	public F64Field(other);


	/**
	 * Returns the data stored within the F64Field.
	 *
	 * @return A 64-bit floating point value.
	 */
	public getValue();
}
