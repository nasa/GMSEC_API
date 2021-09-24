/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U32Field.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U32;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU32Field;


/**
 * Specialized Field class that can be used to retain an unsigned 32-bit value.
 */
public class U32Field extends Field
{
	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIU32Field object.
	 */
	public U32Field(JNIU32Field field)
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
	public U32Field(String name, U32 data) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("U32Field object is null");
		}

		setInternal(new JNIU32Field(name, data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other U32Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given U32Field object is null.
	 */
	public U32Field(U32Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("U32Field object is null");
		}

		setInternal(new JNIU32Field((JNIU32Field) Field.getInternal(other)));
	}


	/**
	 * Returns the data stored within the U32Field.
	 *
	 * @return A U32 object.
	 */
	public U32 getValue()
	{
		JNIU32Field jU32Field = (JNIU32Field) getInternal();

		return jU32Field.getValue();
	}
}
