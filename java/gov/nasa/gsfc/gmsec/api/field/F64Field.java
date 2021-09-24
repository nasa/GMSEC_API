/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F64Field.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.field.JNIF64Field;


/**
 * Specialized Field class that can be used to retain a 64-bit floating point value.
 */
public class F64Field extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIF64Field object.
	 */
	public F64Field(JNIF64Field field)
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
	public F64Field(String name, double data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNIF64Field(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other F64Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given F64Field object is null.
	 */
	public F64Field(F64Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("F64Field object is null");
		}

		setInternal(new JNIF64Field((JNIF64Field) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the F64Field.
	 *
	 * @return A double value.
	 */
	public double getValue()
	{
		JNIF64Field jF64Field = (JNIF64Field) getInternal();

		return jF64Field.getValue();
	}
}
