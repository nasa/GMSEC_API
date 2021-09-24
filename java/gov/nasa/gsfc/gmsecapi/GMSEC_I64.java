/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_I64
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC signed 64-bit integer field type.
 *
 */
@Deprecated
public class GMSEC_I64
{
	public long value;

	/**
	 * @brief construct empty long
	 */
	public GMSEC_I64()
	{
	}

	/**
	 * @brief construct long with value
	 * @param arg
	 */
	public GMSEC_I64(long arg)
	{
		value = arg;
	}

	/**
	 * @param l I64 value
	 */
	public GMSEC_I64(GMSEC_I64 l)
	{
		Set(l.Get());
	}

	/**
	 * @brief set ulong value
	 * @param arg
	 */
	public void Set(long arg)
	{
		value = arg;
	}

	/**
	 * @brief get ulong value
	 * @return
	 */
	public long Get()
	{
		return value;
	}

	/**
	 * @brief get value as printable
	 */
	public String toString()
	{
		return "" + value;
	}
}
