/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U64.java
 *
 * @brief This is a container class for the GMSEC unsigned 64-bit integer field type.
 */

package gov.nasa.gsfc.gmsec.api;

import java.math.BigInteger;


/**
 * @class U64
 *
 * @brief This is a container class for the GMSEC unsigned 64-bit integer field type.
 */
public class U64
{
	private BigInteger value;


	/**
	 * @desc Minimum value (0) allowed for U64 type.
	 */
	public static final BigInteger MIN_VALUE = BigInteger.ZERO;


	/**
	 * @desc Maximum value (2^64 - 1) allowed for U64 type.
	 */
	public static final BigInteger MAX_VALUE = new BigInteger("18446744073709551615");


	/**
	 * @desc Enforce allowable range for U64 values.
	 */
	private BigInteger checkRange(BigInteger arg) throws IllegalArgumentException
	{
		if (arg == null)
		{
			throw new IllegalArgumentException("BigInteger value is null");
		}
		if (arg.compareTo(MIN_VALUE) == -1)
		{
			throw new IllegalArgumentException("Value is less than U64.MIN_VALUE");
		}
		if (arg.compareTo(MAX_VALUE) == 1)
		{
			throw new IllegalArgumentException("Value is greater than U64.MAX_VALUE");
		}
		return arg;
	}


	/**
	 * @fn U64()
	 *
	 * @brief construct with value equal to MIN_VALUE
	 */
	public U64()
	{
		this.value = MIN_VALUE;
	}


	/**
	 * @fn U64(BigInteger value)
	 *
	 * @brief construct with given value
	 *
	 * @param value - the value to assign to the U64 object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public U64(BigInteger value) throws IllegalArgumentException
	{
		this.value = checkRange(value);
	}


	/**
	 * @fn U64(String value)
	 *
	 * @brief construct with given decimal string representation of a BigInteger.
	 *
	 * @param value - the decimal string representation of a BigInteger.
	 *
	 * @throws An IllegalArgumentException is thrown if the given string value is not in the range of [MIN_VALUE, MAX_VALUE].
	 * @throws A NumberFormatException is thrown if the value in the given string is not a valid representation of a number.
	 */
	public U64(String value) throws IllegalArgumentException
	{
		this.value = checkRange(new BigInteger(value));
	}


	/**
	 * @fn U64(U64 other)
	 *
	 * @brief Copy constructor
	 *
	 * @param other - the U64 object to copy
	 *
	 * @throws An IllegalArgumentException is thrown if the given U64 object is null.
	 */
	public U64(U64 other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("U64 object is null");
		}

		this.value = other.get();
	}


	/**
	 * @fn set(BigInteger value)
	 *
	 * @brief Set U64 object with given value.
	 *
	 * @param value - the value to assign to the U64 object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public void set(BigInteger value) throws IllegalArgumentException
	{
		this.value = checkRange(value);
	}


	/*
	 * @fn BigInteger get()
	 *
	 * @desc Returns the underlying value within the U64 object.
	 */
	public BigInteger get()
	{
		return this.value;
	}


	/**
	 * @fn String toString()
	 *
	 * @desc Returns the U64 value as a printable string.
	 */
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		buf.append(this.value.toString());
		return buf.toString();
	}
}
