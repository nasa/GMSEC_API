/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U16Field.java
 *
 * @brief Specialized Field class that can be used to retain an unsigned 16-bit value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U16;
import gov.nasa.gsfc.gmsec.api.jni.JNIU16Field;


/**
 * @class U16Field
 *
 * @brief Specialized Field class that can be used to retain an unsigned 16-bit value.
 */
public class U16Field extends Field
{
	public U16Field(JNIU16Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn U16Field(String name, U16 data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the data value is null.
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
	 * @fn U16Field(U16Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other U16Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given U16Field object is null.
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
	 * @fn U16 getValue()
	 *
	 * @return Returns the data stored within the U16Field.
	 */
	public U16 getValue()
	{
		JNIU16Field jU16Field = (JNIU16Field) getInternal();

		return jU16Field.getValue();
	}
}
