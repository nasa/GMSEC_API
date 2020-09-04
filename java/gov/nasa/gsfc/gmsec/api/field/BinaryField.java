/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BinaryField.java
 *
 * @brief Specialized Field class that can be used to retain raw data.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNIBinaryField;



/**
 * @class BinaryField
 *
 * @brief Specialized Field class that can be used to retain raw data.
 */
public class BinaryField extends Field
{
	public BinaryField(JNIBinaryField field)
	{
		setInternal(field);
	}


	/**
	 * @fn BinaryField(String name, byte[] data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - raw data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name string is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the byte array is null.
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
	 * @fn BinaryField(BinaryField other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other BinaryField object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given BinaryField object is null.
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
	 * @fn getValue()
	 *
	 * @return Returns the data stored within the BinaryField.
	 */
	public byte[] getValue()
	{
		JNIBinaryField jBinField = (JNIBinaryField) getInternal();

		return jBinField.getValue();
	}
}
