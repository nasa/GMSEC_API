/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U8.java
 *
 * @brief This is a container class for the GMSEC unsigned 8-bit integer field type.
 */

package gov.nasa.gsfc.gmsec.api;


/**
 * @class U8
 *
 * @brief This is a container class for the GMSEC unsigned 8-bit integer field type.
 */
public class U8
{
	private int value;


	/**
	 * @desc Minimum value (0) allowed for U8 type.
	 */
	public static final int MIN_VALUE = 0;


	/**
	 * @desc Maximum value (2^8 - 1) allowed for U8 type.
	 */
	public static final int MAX_VALUE = 255;


	/**
	 * @desc Enforce allowable range for U8 values.
	 */
	private int checkRange(int arg) throws IllegalArgumentException
	{
		if (arg < MIN_VALUE)
		{
			throw new IllegalArgumentException("Value is less than U8.MIN_VALUE");
		}
		if (arg > MAX_VALUE)
		{
			throw new IllegalArgumentException("Value is greater than U8.MAX_VALUE");
		}
		return arg;
	}


	/**
	 * @fn U8()
	 *
	 * @brief construct with value equal to MIN_VALUE
	 */
	public U8()
	{
		this.value = MIN_VALUE;
	}


	/**
	 * @fn U8(int value)
	 *
	 * @brief construct with given value
	 *
	 * @param value - the value to assign to the U8 object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public U8(int value)
	{
		this.value = checkRange(value);
	}


	/**
	 * @fn U8(U8 other)
	 *
	 * @brief Copy constructor
	 *
	 * @param other - the U8 object to copy
	 */
	public U8(U8 other)
	{
		this.value = other.get();
	}


	/**
	 * @fn set(int value)
	 *
	 * @brief Set U8 object with given value.
	 *
	 * @param value - the value to assign to the U8 object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public void set(int value)
	{
		this.value = value;
	}


	/*
	 * @fn int get()
	 *
	 * @desc Returns the underlying value within the U8 object.
	 */
	public int get()
	{
		return this.value;
	}


	/**
	 * @fn String toString()
	 *
	 * @desc Returns the U8 value as a printable string.
	 */
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		buf.append(this.value);
		return buf.toString();
	}
}
