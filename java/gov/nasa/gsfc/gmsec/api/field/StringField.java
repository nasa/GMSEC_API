/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file StringField.java
 *
 * @brief Specialized Field class that can be used to retain a string value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNIStringField;


/**
 * @class StringField
 *
 * @brief Specialized Field class that can be used to retain a string value.
 */
public class StringField extends Field
{
	public StringField(JNIStringField field)
	{
		setInternal(field);
	}


	/**
	 * @fn StringField(String name, String data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the data string is null.
	 */
	public StringField(String name, String data) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("StringField Data is null");
		}

		setInternal(new JNIStringField(name, data));
	}


	/**
	 * @fn StringField(StringField other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other StringField object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given StringField object is null.
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
	 * @fn String getValue()
	 *
	 * @return Returns the data stored within the StringField.
	 */
	public String getValue()
	{
		JNIStringField jStringField = (JNIStringField) getInternal();

		return jStringField.getValue();
	}
}
