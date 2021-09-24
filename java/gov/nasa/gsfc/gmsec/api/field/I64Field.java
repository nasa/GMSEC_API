/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I64Field.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.field.JNII64Field;


/**
 * Specialized Field class that can be used to retain a signed 64-bit value.
 */
public class I64Field extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNII64Field object.
	 */
	public I64Field(JNII64Field field)
	{
		setInternal(field);
	}


	/**
	 * Default constructor.
	 *
	 * @param name Name of the field.
	 * @param data Data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null, or contains an empty string.
	 */
	public I64Field(String name, long data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNII64Field(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other I64Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given I64Field object is null.
	 */
	public I64Field(I64Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("I64Field object is null");
		}

		setInternal(new JNII64Field((JNII64Field) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the I64Field.
	 *
	 * @return A long value.
	 */
	public long getValue()
	{
		JNII64Field jI64Field = (JNII64Field) getInternal();

		return jI64Field.getValue();
	}
}
