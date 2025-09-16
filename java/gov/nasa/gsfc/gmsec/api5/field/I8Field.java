/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I8Field.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII8Field;


/**
 * Specialized Field class that can be used to retain a signed 8-bit value.
 */
public class I8Field extends Field
{
	//! @cond
	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNII8Field object.
	 */
	public I8Field(JNII8Field field)
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
	 */
	public I8Field(String name, byte data) throws IllegalArgumentException
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
	 */
	public I8Field(String name, byte data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNII8Field(name, data, isHeader));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other I8Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given I8Field object is null.
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
	 * Returns the data stored within the I8Field.
	 *
	 * @return A byte value.
	 */
	public byte getValue()
	{
		JNII8Field jI8Field = (JNII8Field) getInternal();

		return jI8Field.getValue();
	}
}
