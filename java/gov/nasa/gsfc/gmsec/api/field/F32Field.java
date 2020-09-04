/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F32Field.java
 *
 * @brief Specialized Field class that can be used to retain a 32-bit floating point value.
 */
package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNIF32Field;


/**
 * @class F32Field
 *
 * @brief Specialized Field class that can be used to retain a 32-bit floating point value.
 */
public class F32Field extends Field
{
	public F32Field(JNIF32Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn F32Field(String name, float data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 */
	public F32Field(String name, float data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNIF32Field(name, data));
	}


	/**
	 * @fn F32Field(F32Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other F32Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given F32Field object is null.
	 */
	public F32Field(F32Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("F32Field object is null");
		}

		setInternal(new JNIF32Field((JNIF32Field) Field.getInternal(other)));
	}


	/**
	 * @fn float getValue()
	 *
	 * @return Returns the data stored within the F32Field.
	 */
	public float getValue()
	{
		JNIF32Field jF32Field = (JNIF32Field) getInternal();

		return jF32Field.getValue();
	}
}
