/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CharField.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.field.JNICharField;


/**
 * Specialized Field class that can be used to retain a single character value.
 */
public class CharField extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNICharField object.
	 */
	public CharField(JNICharField field)
	{
		setInternal(field);
	}


	/**
	 * Default constructor.
	 *
	 * @param name Name of the field.
	 * @param data Character data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null, or contains an empty string.
	 */
	public CharField(String name, char data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNICharField(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other CharField object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given CharField is null.
	 */
	public CharField(CharField other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("CharField object is null");
		}

		setInternal(new JNICharField((JNICharField) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the CharField.
	 *
	 * @return A char value.
	 */
	public char getValue()
	{
		JNICharField jCharField = (JNICharField) getInternal();

		return jCharField.getValue();
	}
}
