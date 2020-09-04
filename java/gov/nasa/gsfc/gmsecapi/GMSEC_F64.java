/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_F64
*
* @brief This is a container class for the GMSEC 64-bit floating-point field type.
*
*/
public class GMSEC_F64
{
	public double value;

	/**
	 * @brief Construct empty double
	 */
	public GMSEC_F64()
	{
	}

	/**
	 * @brief construct double with value
	 * @param arg
	 */
	public GMSEC_F64(double arg)
	{
		value = arg;
	}

	/**
	 * @param d F64 value
	 */
	public GMSEC_F64(GMSEC_F64 d)
	{
		Set(d.Get());
	}

	/**
	 * @brief set double
	 * @param arg
	 */
	public void Set(double arg)
	{
		value = arg;
	}

	/**
	 * @brief get double value
	 * @return
	 */
	public double Get()
	{
		return value;
	}

	/**
	 * @brief covert double to printable
	 */
	public String toString()
	{
		return "" + value;
	}
}
