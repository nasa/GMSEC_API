/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U16Field.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U16;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU16Field;


/**
 * Specialized Field class that can be used to retain an unsigned 16-bit value.
 */
public class U16Field extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIU16Field object.
	 */
	public U16Field(JNIU16Field field)
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
	 * @throws IllegalArgumentException Thrown if the data value is null.
	 */
	public U16Field(String name, U16 data) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("U16Field object is null");
		}

		setInternal(new JNIU16Field(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other U16Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given U16Field object is null.
	 */
	public U16Field(U16Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("U16Field object is null");
		}

		setInternal(new JNIU16Field((JNIU16Field) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the U16Field.
	 *
	 * @return A U16 object.
	 */
	public U16 getValue()
	{
		JNIU16Field jU16Field = (JNIU16Field) getInternal();

		return jU16Field.getValue();
	}
}
