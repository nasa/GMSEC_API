/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file I32Field.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII32Field;


/**
 * Specialized Field class that can be used to retain a signed 32-bit value.
 */
public class I32Field extends Field
{
	//! @cond
	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNII32Field object.
	 */
	public I32Field(JNII32Field field)
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
	public I32Field(String name, int data) throws IllegalArgumentException
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
	public I32Field(String name, int data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNII32Field(name, data, isHeader));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other I32Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given I32Field is null.
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
	 * Returns the data stored within the I32Field.
	 *
	 * @return An integer value.
	 */
	public int getValue()
	{
		JNII32Field jI32Field = (JNII32Field) getInternal();

		return jI32Field.getValue();
	}
}
