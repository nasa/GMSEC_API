/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U8Field.java
 *
 * @brief Specialized Field class that can be used to retain an unsigned 8-bit value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U8;
import gov.nasa.gsfc.gmsec.api.jni.JNIU8Field;


/**
 * @class U8Field
 *
 * @brief Specialized Field class that can be used to retain an unsigned 8-bit value.
 */
public class U8Field extends Field
{
	public U8Field(JNIU8Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn U8Field(String name, U8 data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the data value is null.
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
	 * @fn U8Field(U8Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other U8Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given U8Field object is null.
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
	 * @fn U8 getValue()
	 *
	 * @return Returns the data stored within the U8Field.
	 */
	public U8 getValue()
	{
		JNIU8Field jU8Field = (JNIU8Field) getInternal();

		return jU8Field.getValue();
	}
}
