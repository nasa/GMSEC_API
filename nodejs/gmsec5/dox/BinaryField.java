/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BinaryField.java
 */

package gmsec;


/**
 * Specialized Field class that can be used to retain raw data.
 */
public class BinaryField extends Field
{
	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data A BlobArray containing raw data to associate with the field.
	 *
	 * @throws SWIG_Exception Thrown if the field name string is null.
	 * @throws SWIG_Exception Thrown if the BlobArray is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public BinaryField(name, data);


	/**
	 * Constructor.
	 *
	 * @param name      Name of the field.
	 * @param data      A BlobArray containing raw data to associate with the field.
	 * @param is_header Used to indicate if Field is a header field.
	 *
	 * @throws SWIG_Exception Thrown if the field name string is null.
	 * @throws SWIG_Exception Thrown if the BlobArray is null.
	 * @throws GmsecException Thrown if the field name string is an empty string or is otherwise non-compliant.
	 */
	public BinaryField(name, data, is_header);


	/**
	 * Copy constructor.
	 *
	 * @param other The other BinaryField object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given BinaryField object is null.
	 */
	public BinaryField(other);


	/**
	 * Returns the data stored within the BinaryField.
	 *
	 * @return A BlobArray object.
	 */
	public getValue();
}
