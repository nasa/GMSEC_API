/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U64Field.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U64;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU64Field;


/**
 * Specialized Field class that can be used to retain an unsigned 64-bit value.
 */
public class U64Field extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIU64Field object.
	 */
	public U64Field(JNIU64Field field)
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
	public U64Field(String name, U64 data) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("U64Field object is null");
		}

		setInternal(new JNIU64Field(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other U64Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given U64Field object is null.
	 */
	public U64Field(U64Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("U64Field object is null");
		}

		setInternal(new JNIU64Field((JNIU64Field) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the U64Field.
	 *
	 * @return A U64 object.
	 */
	public U64 getValue()
	{
		JNIU64Field jU64Field = (JNIU64Field) getInternal();

		return jU64Field.getValue();
	}
}
