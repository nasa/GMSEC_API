/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BooleanField.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain a boolean value.
 */
public class BooleanField extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Boolean data to associate with the field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public BooleanField(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      Boolean data to associate with the field.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public BooleanField(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other BooleanField object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given BooleanField is null.
	 */
	public BooleanField(other);


	/**
	 * Returns the data stored within the BooleanField.
	 *
	 * @return A Boolean value.
	 */
	public getValue();
}
