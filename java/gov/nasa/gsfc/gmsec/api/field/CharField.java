/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CharField.java
 *
 * @brief Specialized Field class that can be used to retain a single character value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNICharField;


/**
 * @class CharField
 *
 * @brief Specialized Field class that can be used to retain a single character value.
 */
public class CharField extends Field
{
	public CharField(JNICharField field)
	{
		setInternal(field);
	}


	/**
	 * @fn CharField(String name, char data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - character data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 */
	public CharField(String name, char data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNICharField(name, data));
	}


	/**
	 * @fn CharField(CharField other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other CharField object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given CharField is null.
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
	 * @fn char getValue()
	 *
	 * @return Returns the data stored within the CharField.
	 */
	public char getValue()
	{
		JNICharField jCharField = (JNICharField) getInternal();

		return jCharField.getValue();
	}
}
