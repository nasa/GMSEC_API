/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U8Field.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U8;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU8Field;


/**
 * Specialized Field class that can be used to retain an unsigned 8-bit value.
 */
public class U8Field extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIU8Field object.
	 */
	public U8Field(JNIU8Field field)
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
	public U8Field(String name, U8 data) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("U8Field object is null");
		}

		setInternal(new JNIU8Field(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other U8Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given U8Field object is null.
	 */
	public U8Field(U8Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("U8Field object is null");
		}

		setInternal(new JNIU8Field((JNIU8Field) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the U8Field.
	 *
	 * @return A U8 object.
	 */
	public U8 getValue()
	{
		JNIU8Field jU8Field = (JNIU8Field) getInternal();

		return jU8Field.getValue();
	}
}
