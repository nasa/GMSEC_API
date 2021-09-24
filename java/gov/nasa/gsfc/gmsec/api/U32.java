/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U32.java
 */

package gov.nasa.gsfc.gmsec.api;


/**
 * This is a container class for the GMSEC unsigned 32-bit integer field type.
 */
public class U32
{
	private long value;


	/**
	 * Minimum value (0) allowed for U32 type.
	 */
	public static final long MIN_VALUE = 0L;


	/**
	 * Maximum value (2^32 - 1) allowed for U32 type.
	 */
	public static final long MAX_VALUE = 4294967295L;


	/**
	 * Enforce allowable range for U32 values.
	 */
	private long checkRange(long arg) throws IllegalArgumentException
	{
		if (arg < MIN_VALUE)
		{
			throw new IllegalArgumentException("Value is less than U32.MIN_VALUE");
		}
		if (arg > MAX_VALUE)
		{
			throw new IllegalArgumentException("Value is greater than U32.MAX_VALUE");
		}
		return arg;
	}


	/**
	 * Construct U32 object with value equal to MIN_VALUE
	 */
	public U32()
	{
		this.value = MIN_VALUE;
	}


	/**
	 * Construct U32 object with the given value
	 *
	 * @param value The value to assign to the U32 object.
	 *
	 * @throws IllegalArgumentException Thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public U32(long value) throws IllegalArgumentException
	{
		this.value = checkRange(value);
	}


	/**
	 * Copy constructor
	 *
	 * @param other The U32 object to copy
	 *
	 * @throws IllegalArgumentException Thrown if the given U32 object is null.
	 */
	public U32(U32 other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("U32 object is null");
		}

		this.value = other.get();
	}


	/**
	 * Set U32 object with the given value.
	 *
	 * @param value The value to assign to the U32 object.
	 *
	 * @throws IllegalArgumentException Thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public void set(long value) throws IllegalArgumentException
	{
		this.value = checkRange(value);
	}


	/*
	 * Returns the underlying value within the U32 object.
	 */
	public long get()
	{
		return this.value;
	}


	/**
	 * Returns the U32 value as a printable string.
	 */
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		buf.append(this.value);
		return buf.toString();
	}
}
