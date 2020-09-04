/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U32Field.java
 *
 * @brief Specialized Field class that can be used to retain an unsigned 32-bit value.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U32;
import gov.nasa.gsfc.gmsec.api.jni.JNIU32Field;


/**
 * @class U32Field
 *
 * @brief Specialized Field class that can be used to retain an unsigned 32-bit value.
 */
public class U32Field extends Field
{
	public U32Field(JNIU32Field field)
	{
		setInternal(field);
	}


	/**
	 * @fn U32Field(String name, U32 data)
	 *
	 * @brief Default constructor.
	 *
	 * @param name - name of the field.
	 * @param data - data to associate with the field.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the data value is null.
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
	 * @fn U32Field(U32Field other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other U32Field object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given U32Field object is null.
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
	 * @fn U32 getValue()
	 *
	 * @return Returns the data stored within the U32Field.
	 */
	public U32 getValue()
	{
		JNIU32Field jU32Field = (JNIU32Field) getInternal();

		return jU32Field.getValue();
	}
}
