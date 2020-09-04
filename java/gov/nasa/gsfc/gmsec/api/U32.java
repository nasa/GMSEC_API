/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U32.java
 *
 * @brief This is a container class for the GMSEC unsigned 32-bit integer field type.
 */

package gov.nasa.gsfc.gmsec.api;


/**
 * @class U32
 *
 * @brief This is a container class for the GMSEC unsigned 32-bit integer field type.
 */
public class U32
{
	private long value;


	/**
	 * @desc Minimum value (0) allowed for U32 type.
	 */
	public static final long MIN_VALUE = 0L;


	/**
	 * @desc Maximum value (2^32 - 1) allowed for U32 type.
	 */
	public static final long MAX_VALUE = 4294967295L;


	/**
	 * @desc Enforce allowable range for U32 values.
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
	 * @fn U32()
	 *
	 * @brief construct with value equal to MIN_VALUE
	 */
	public U32()
	{
		this.value = MIN_VALUE;
	}


	/**
	 * @fn U32(long value)
	 *
	 * @brief construct with given value
	 *
	 * @param value - the value to assign to the U32 object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public U32(long value)
	{
		this.value = checkRange(value);
	}


	/**
	 * @fn U32(U32 other)
	 *
	 * @brief Copy constructor
	 *
	 * @param other - the U32 object to copy
	 */
	public U32(U32 other)
	{
		this.value = other.get();
	}


	/**
	 * @fn set(long value)
	 *
	 * @brief Set U32 object with given value.
	 *
	 * @param value - the value to assign to the U32 object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public void set(long value)
	{
		this.value = value;
	}


	/*
	 * @fn long get()
	 *
	 * @desc Returns the underlying value within the U32 object.
	 */
	public long get()
	{
		return this.value;
	}


	/**
	 * @fn String toString()
	 *
	 * @desc Returns the U32 value as a printable string.
	 */
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		buf.append(this.value);
		return buf.toString();
	}
}
