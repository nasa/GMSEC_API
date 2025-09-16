/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file StringField.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIStringField;


/**
 * Specialized Field class that can be used to retain a string value.
 */
public class StringField extends Field
{
	//! @cond
	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIStringField object.
	 */
	public StringField(JNIStringField field)
	{
		setInternal(field);
	}
	//! @endcond


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, is an empty string, or is not compliant.
	 * @throws IllegalArgumentException Thrown if the data string is null.
	 */
	public StringField(String name, String data) throws IllegalArgumentException
	{
		this(name, data, false);
	}


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Data to associate with the field.
	 * @param isHeader used to indicate if Field is a header field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, is an empty string, or is not compliant.
	 * @throws IllegalArgumentException Thrown if the data string is null.
	 */
	public StringField(String name, String data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("StringField Data is null");
		}

		setInternal(new JNIStringField(name, data, isHeader));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other StringField object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given StringField object is null.
	 */
	public StringField(StringField other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("StringField object is null");
		}

		setInternal(new JNIStringField((JNIStringField) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the StringField.
	 *
	 * @return A string value.
	 */
	public String getValue()
	{
		JNIStringField jStringField = (JNIStringField) getInternal();

		return jStringField.getValue();
	}
}
