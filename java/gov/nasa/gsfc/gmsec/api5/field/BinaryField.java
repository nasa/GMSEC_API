/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BinaryField.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIBinaryField;



/**
 * Specialized Field class that can be used to retain raw data.
 */
public class BinaryField extends Field
{
	//! @cond
	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIBinaryField object.
	 */
	public BinaryField(JNIBinaryField field)
	{
		setInternal(field);
	}
	//! @endcond


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Raw data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, is an empty string, or is not compliant.
	 * @throws IllegalArgumentException Thrown if the byte array is null.
	 */
	public BinaryField(String name, byte[] data) throws IllegalArgumentException
	{
		this(name, data, false);
	}


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Raw data to associate with the field.
	 * @param isHeader used to indicate if Field is a header field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, is an empty string, or is not compliant.
	 * @throws IllegalArgumentException Thrown if the byte array is null.
	 */
	public BinaryField(String name, byte[] data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("Byte array is null");
		}

		setInternal(new JNIBinaryField(name, data, isHeader));
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
