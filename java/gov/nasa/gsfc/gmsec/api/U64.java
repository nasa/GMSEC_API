/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file U64.java
 */

package gov.nasa.gsfc.gmsec.api;

import java.math.BigInteger;


/**
 * This is a container class for the GMSEC unsigned 64-bit integer field type.
 */
public class U64
{
	private BigInteger value;


	/**
	 * Minimum value (0) allowed for U64 type.
	 */
	public static final BigInteger MIN_VALUE = BigInteger.ZERO;


	/**
	 * Maximum value (2^64 - 1) allowed for U64 type.
	 */
	public static final BigInteger MAX_VALUE = new BigInteger("18446744073709551615");


	/**
	 * Enforce allowable range for U64 values.
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
	 * Construct U64 object with value equal to MIN_VALUE
	 */
	public U64()
	{
		this.value = MIN_VALUE;
	}


	/**
	 * Construct U64 object with the given value
	 *
	 * @param value The value to assign to the U64 object.
	 *
	 * @throws IllegalArgumentException Thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public U64(BigInteger value) throws IllegalArgumentException
	{
		this.value = checkRange(value);
	}


	/**
	 * Construct U64 object with the given decimal string representation of a BigInteger.
	 *
	 * @param value The decimal string representation of a BigInteger.
	 *
	 * @throws IllegalArgumentException Thrown if the given string value is not in the range of [MIN_VALUE, MAX_VALUE].
	 * @throws NumberFormatException Thrown if the value in the given string is not a valid representation of a number.
	 */
	public U64(String value) throws IllegalArgumentException
	{
		this.value = checkRange(new BigInteger(value));
	}


	/**
	 * Copy constructor
	 *
	 * @param other The U64 object to copy
	 *
	 * @throws IllegalArgumentException Thrown if the given U64 object is null.
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
	 * Set U64 object with the given value.
	 *
	 * @param value The value to assign to the U64 object.
	 *
	 * @throws IllegalArgumentException Thrown if the given value is not in the range of [MIN_VALUE, MAX_VALUE].
	 */
	public void set(BigInteger value) throws IllegalArgumentException
	{
		this.value = checkRange(value);
	}


	/*
	 * Returns the underlying value within the U64 object.
	 */
	public BigInteger get()
	{
		return this.value;
	}


	/**
	 * Returns the U64 value as a printable string.
	 */
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		buf.append(this.value.toString());
		return buf.toString();
	}
}
