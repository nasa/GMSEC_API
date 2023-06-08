/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I8Field.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain a signed 8-bit value.
 */
public class I8Field extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data A signed 8-bit value.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 */
	public I8Field(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      A signed 8-bit value.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 */
	public I8Field(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other I8Field object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given I8Field is null.
	 */
	public I8Field(other);


	/**
	 * Returns the data stored within the I8Field.
	 *
	 * @return A signed 8-bit value.
	 */
	public getValue();
}
