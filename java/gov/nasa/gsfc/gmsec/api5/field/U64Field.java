/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U64Field.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.U64;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU64Field;


/**
 * Specialized Field class that can be used to retain an unsigned 64-bit value.
 */
public class U64Field extends Field
{
	//! @cond
	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIU64Field object.
	 */
	public U64Field(JNIU64Field field)
	{
		setInternal(field);
	}
	//! @endcond


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Data to associate with the field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, is an empty string, or is not compliant.
	 * @throws IllegalArgumentException Thrown if the data value is null.
	 */
	public U64Field(String name, U64 data) throws IllegalArgumentException
	{
		this(name, data, false);
	}


	/**
	 * Constructor.
	 *
	 * @param name Name of the field.
	 * @param data Data to associate with the field.
	 * @param isHeader used to indicate if Field is a header field.
	 *
	 * @throws IllegalArgumentException Thrown if the field name string is null, is an empty string, or is not compliant.
	 * @throws IllegalArgumentException Thrown if the data value is null.
	 */
	public U64Field(String name, U64 data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		if (data == null)
		{
			throw new IllegalArgumentException("U64 object is null");
		}

		setInternal(new JNIU64Field(name, data, isHeader));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other U64Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given U64Field object is null.
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
	 * Returns the data stored within the U64Field.
	 *
	 * @return A U64 object.
	 */
	public U64 getValue()
	{
		JNIU64Field jU64Field = (JNIU64Field) getInternal();

		return jU64Field.getValue();
	}
}
