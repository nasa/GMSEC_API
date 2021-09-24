/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file BooleanField.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.field.JNIBooleanField;


/**
 * Specialized Field class that can be used to retain a boolean value.
 */
public class BooleanField extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIBooleanField object.
	 */
	public BooleanField(JNIBooleanField field)
	{
		setInternal(field);
	}


	/**
	 * Default constructor.
	 *
	 * @param name Name of the field.
	 * @param data Boolean data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null, or contains an empty string.
	 */
	public BooleanField(String name, boolean data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNIBooleanField(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other BooleanField object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given BooleanField is null.
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
	 * Returns the data stored within the BooleanField.
	 *
	 * @return A boolean value.
	 */
	public boolean getValue()
	{
		JNIBooleanField jboolField = (JNIBooleanField) getInternal();

		return jboolField.getValue();
	}
}
