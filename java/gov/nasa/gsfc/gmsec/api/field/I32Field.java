/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I32Field.java
 *
 * @brief Specialized Field class that can be used to retain a signed 32-bit value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNII32Field;


/**
 * @class I32Field
 *
 * @brief Specialized Field class that can be used to retain a signed 32-bit value.
 */
public class I32Field extends Field
{
	public I32Field(JNII32Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn I32Field(String name, int data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 */
	public I32Field(String name, int data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNII32Field(name, data));
	}


	/**
	 * @fn I32Field(I32Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other I32Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given I32Field is null.
	 */
	public I32Field(I32Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("I32Field object is null");
		}

		setInternal(new JNII32Field((JNII32Field) Field.getInternal(other)));
	}


	/**
	 * @fn int getValue()
	 *
	 * @return Returns the data stored within the I32Field.
	 */
	public int getValue()
	{
		JNII32Field jI32Field = (JNII32Field) getInternal();

		return jI32Field.getValue();
	}
}
