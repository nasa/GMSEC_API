/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U64Field.java
 *
 * @brief Specialized Field class that can be used to retain an unsigned 64-bit value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U64;
import gov.nasa.gsfc.gmsec.api.jni.JNIU64Field;


/**
 * @class U64Field
 *
 * @brief Specialized Field class that can be used to retain an unsigned 64-bit value.
 */
public class U64Field extends Field
{
	public U64Field(JNIU64Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn U64Field(String name, U64 data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the data value is null.
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
	 * @fn U64Field(U64Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other U64Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given U64Field object is null.
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
	 * @fn U64 getValue()
	 *
	 * @return Returns the data stored within the U64Field.
	 */
	public U64 getValue()
	{
		JNIU64Field jU64Field = (JNIU64Field) getInternal();

		return jU64Field.getValue();
	}
}
