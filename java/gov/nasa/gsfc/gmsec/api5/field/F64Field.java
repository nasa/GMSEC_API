/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file F64Field.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIF64Field;


/**
 * Specialized Field class that can be used to retain a 64-bit floating point value.
 */
public class F64Field extends Field
{
	//! @cond
	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param field A JNIF64Field object.
	 */
	public F64Field(JNIF64Field field)
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
	public F64Field(String name, double data) throws IllegalArgumentException
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
	public F64Field(String name, double data, boolean isHeader) throws IllegalArgumentException
	{
		validateName(name);

		setInternal(new JNIF64Field(name, data, isHeader));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other F64Field object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given F64Field object is null.
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
	 * Returns the data stored within the F64Field.
	 *
	 * @return A double value.
	 */
	public double getValue()
	{
		JNIF64Field jF64Field = (JNIF64Field) getInternal();

		return jF64Field.getValue();
	}
}
