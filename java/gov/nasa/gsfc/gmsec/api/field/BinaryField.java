/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BinaryField.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.field.JNIBinaryField;



/**
 * Specialized Field class that can be used to retain raw data.
 */
public class BinaryField extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIBinaryField object.
	 */
	public BinaryField(JNIBinaryField field)
	{
		setInternal(field);
	}


	/**
	 * Default constructor.
	 *
	 * @param name Name of the field.
	 * @param data Raw data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the byte array is null.
	 */
	public BinaryField(String name, byte[] data) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("Byte array is null");
		}

		setInternal(new JNIBinaryField(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other BinaryField object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given BinaryField object is null.
	 */
	public BinaryField(BinaryField other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("BinaryField object is null");
		}

		setInternal(new JNIBinaryField((JNIBinaryField) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the BinaryField.
	 *
	 * @return A byte array.
	 */
	public byte[] getValue()
	{
		JNIBinaryField jBinField = (JNIBinaryField) getInternal();

		return jBinField.getValue();
	}
}
