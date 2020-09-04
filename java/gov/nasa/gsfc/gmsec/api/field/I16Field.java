/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I16Field.java
 *
 * @brief Specialized Field class that can be used to retain a signed 16-bit value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNII16Field;


/**
 * @class I16Field
 *
 * @brief Specialized Field class that can be used to retain a signed 16-bit value.
 */
public class I16Field extends Field
{
	public I16Field(JNII16Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn I16Field(String name, short data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 */
	public I16Field(String name, short data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNII16Field(name, data));
	}


	/**
	 * @fn I16Field(I16Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other I16Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given I16Field is null.
	 */
	public I16Field(I16Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("I16Field object is null");
		}

		setInternal(new JNII16Field((JNII16Field) Field.getInternal(other)));
	}


	/**
	 * @fn short getValue()
	 *
	 * @return Returns the data stored within the I16Field.
	 */
	public short getValue()
	{
		JNII16Field jI16Field = (JNII16Field) getInternal();

		return jI16Field.getValue();
	}
}
