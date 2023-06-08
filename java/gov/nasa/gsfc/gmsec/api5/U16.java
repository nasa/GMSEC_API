/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U16.java
 */

package gov.nasa.gsfc.gmsec.api5;


/**
 * This is a container class for the GMSEC unsigned 16-bit integer field type.
 */
public final class U16
{
	private int value;


	/**
	 * Minimum value (0) allowed for U16 type.
	 */
	public static final int MIN_VALUE = 0;


	/**
	 * Maximum value (2^16 - 1) allowed for U16 type.
	 */
	public static final int MAX_VALUE = 65535;


	/**
	 * Enforce allowable range for U16 values.
	 */
	private int checkRange(int arg) throws IllegalArgumentException
	{
		if (arg < MIN_VALUE)
		{
			throw new IllegalArgumentException("Value is less than U16.MIN_VALUE");
		}
		if (arg > MAX_VALUE)
		{
			throw new IllegalArgumentException("Value is greater than U16.MAX_VALUE");
		}
		return arg;
	}


	/**
	 * Construct U16 object with value equal to MIN_VALUE
	 */
	public U16()
	{
		this.value = MIN_VALUE;
	}


	/**
	 * Construct U16 object with the given value
	 *
	 * @param value The value to assign to the U16 object.
	 *
	 * @throws IllegalArgumentException Thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public U16(int value)
	{
		this.value = checkRange(value);
	}


	/**
	 * Copy constructor
	 *
	 * @param other The U16 object to copy
	 */
	public U16(U16 other)
	{
		this.value = other.get();
	}


	/**
	 * Set U16 object with given value.
	 *
	 * @param value The value to assign to the U16 object.
	 *
	 * @throws IllegalArgumentException Thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public void set(int value)
	{
		this.value = value;
	}


	/**
	 * Returns the underlying value within the U16 object.
	 * @return The integer value held by the U16 object.
	 */
	public int get()
	{
		return this.value;
	}


	/**
	 * Returns the U16 value as a printable string.
	 */
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		buf.append(this.value);
		return buf.toString();
	}
}
