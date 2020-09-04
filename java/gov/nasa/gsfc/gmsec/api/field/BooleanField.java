/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BooleanField.java
 *
 * @brief Specialized Field class that can be used to retain a boolean value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNIBooleanField;


/**
 * @class BooleanField
 *
 * @brief Specialized Field class that can be used to retain a boolean value.
 */
public class BooleanField extends Field
{
	public BooleanField(JNIBooleanField field)
	{
		setInternal(field);
	}


	/**
	 * @fn BooleanField(String name, boolean data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - boolean data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 */
	public BooleanField(String name, boolean data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNIBooleanField(name, data));
	}


	/**
	 * @fn BooleanField(BooleanField other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other BooleanField object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given BooleanField is null.
	 */
	public BooleanField(BooleanField other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("BooleanField object is null");
		}

		setInternal(new JNIBooleanField((JNIBooleanField) Field.getInternal(other)));
	}


	/**
	 * @fn boolean getValue()
	 *
	 * @return Returns the data stored within the BooleanField.
	 */
	public boolean getValue()
	{
		JNIBooleanField jboolField = (JNIBooleanField) getInternal();

		return jboolField.getValue();
	}
}
