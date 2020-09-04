/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F64Field.java
 *
 * @brief Specialized Field class that can be used to retain a 64-bit floating point value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.jni.JNIF64Field;


/**
 * @class F64Field
 *
 * @brief Specialized Field class that can be used to retain a 64-bit floating point value.
 */
public class F64Field extends Field
{
	public F64Field(JNIF64Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn F64Field(String name, double data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 */
	public F64Field(String name, double data) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNIF64Field(name, data));
	}


	/**
	 * @fn F64Field(F64Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other F64Field object to copy.
	 */
	public F64Field(F64Field other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("F64Field object is null");
		}

		setInternal(new JNIF64Field((JNIF64Field) Field.getInternal(other)));
	}


	/**
	 * @fn double getValue()
	 *
	 * @return Returns the data stored within the F64Field.
	 */
	public double getValue()
	{
		JNIF64Field jF64Field = (JNIF64Field) getInternal();

		return jF64Field.getValue();
	}
}
