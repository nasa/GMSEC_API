/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I16Field.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain a signed 16-bit value.
 */
public class I16Field extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data A signed 16-bit value.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 */
	public I16Field(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      A signed 16-bit value.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 */
	public I16Field(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other I16Field object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given I16Field is null.
	 */
	public I16Field(other);


	/**
	 * Returns the data stored within the I16Field.
	 *
	 * @return A signed 16-bit value.
	 */
	public getValue();
}
