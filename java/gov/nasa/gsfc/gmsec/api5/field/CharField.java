/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CharField.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNICharField;


/**
 * Specialized Field class that can be used to retain a single character value.
 */
public class CharField extends Field
{
	//! @cond
	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNICharField object.
	 */
	public CharField(JNICharField field)
	{
		setInternal(field);
	}
	//! @endcond


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Character data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, is an empty string, or is not compliant.
	 */
	public CharField(String name, char data) throws IllegalArgumentException
	{
		this(name, data, false);
	}


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Character data to associate with the field.
	 * @param isHeader used to indicate if Field is a header field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, is an empty string, or is not compliant.
	 */
	public CharField(String name, char data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNICharField(name, data, isHeader));
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
