/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I16Field.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.jni.field.JNII16Field;


/**
 * Specialized Field class that can be used to retain a signed 16-bit value.
 */
public class I16Field extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNII16Field object.
	 */
	public I16Field(JNII16Field field)
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
	public I16Field(String name, short data) throws IllegalArgumentException
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
	public I16Field(String name, short data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNII16Field(name, data, isHeader));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other I16Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given I16Field is null.
	 */
	public I16Field(I16Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("I16Field object is null");
		}

		setInternal(new JNII16Field((JNII16Field) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the I16Field.
	 *
	 * @return A short value.
	 */
	public short getValue()
	{
		JNII16Field jI16Field = (JNII16Field) getInternal();

		return jI16Field.getValue();
	}
}
