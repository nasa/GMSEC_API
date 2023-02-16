/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F32Field.java
 */
package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.jni.field.JNIF32Field;


/**
 * Specialized Field class that can be used to retain a 32-bit floating point value.
 */
public class F32Field extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIF32Field object.
	 */
	public F32Field(JNIF32Field field)
	{
		setInternal(field);
	}


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null, or contains an empty string.
	 */
	public F32Field(String name, float data) throws IllegalArgumentException
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
	 * @throws IllegalArgumentException Thrown if the field name is null, or contains an empty string.
	 */
	public F32Field(String name, float data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNIF32Field(name, data, isHeader));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other F32Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given F32Field object is null.
	 */
	public F32Field(F32Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("F32Field object is null");
		}

		setInternal(new JNIF32Field((JNIF32Field) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the F32Field.
	 *
	 * @return A float value.
	 */
	public float getValue()
	{
		JNIF32Field jF32Field = (JNIF32Field) getInternal();

		return jF32Field.getValue();
	}
}
