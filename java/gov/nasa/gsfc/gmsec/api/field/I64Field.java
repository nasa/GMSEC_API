/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I64Field.java
 *
 * @brief Specialized Field class that can be used to retain a signed 64-bit value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNII64Field;


/**
 * @class I64Field
 *
 * @brief Specialized Field class that can be used to retain a signed 64-bit value.
 */
public class I64Field extends Field
{
	public I64Field(JNII64Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn I64Field(String name, long data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 */
	public I64Field(String name, long data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNII64Field(name, data));
	}


	/**
	 * @fn I64Field(I64Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other I64Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given I64Field object is null.
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
	 * @fn long getValue()
	 *
	 * @return Returns the data stored within the I64Field.
	 */
	public long getValue()
	{
		JNII64Field jI64Field = (JNII64Field) getInternal();

		return jI64Field.getValue();
	}
}
