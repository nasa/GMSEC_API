/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I8Field.java
 *
 * @brief Specialized Field class that can be used to retain a signed 8-bit value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNII8Field;


/**
 * @class I8Field
 *
 * @brief Specialized Field class that can be used to retain a signed 8-bit value.
 */
public class I8Field extends Field
{
	public I8Field(JNII8Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn I8Field(String name, byte data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 */
	public I8Field(String name, byte data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNII8Field(name, data));
	}


	/**
	 * @fn I8Field(I8Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other I8Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given I8Field object is null.
	 */
	public I8Field(I8Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("I8Field object is null");
		}

		setInternal(new JNII8Field((JNII8Field) Field.getInternal(other)));
	}


	/**
	 * @fn byte getValue()
	 *
	 * @return Returns the data stored within the I8Field.
	 */
	public byte getValue()
	{
		JNII8Field jI8Field = (JNII8Field) getInternal();

		return jI8Field.getValue();
	}
}
