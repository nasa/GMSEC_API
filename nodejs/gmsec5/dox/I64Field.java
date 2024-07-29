/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I64Field.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain a signed 64-bit value.
 */
public class I64Field extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data A signed 64-bit value.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public I64Field(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      A signed 64-bit value.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public I64Field(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other I64Field object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given I64Field is null.
	 */
	public I64Field(other);


	/**
	 * Returns the data stored within the I64Field.
	 *
	 * @return A signed 64-bit value.
	 */
	public getValue();
}
