/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_I32
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC signed 32-bit integer field type.
 *
 */
@Deprecated
public class GMSEC_I32
{
	public int value;

	/**
	 * @brief construct empty long
	 */
	public GMSEC_I32()
	{
	}

	/**
	 * @brief construct long with value
	 * @param arg
	 */
	public GMSEC_I32(int arg)
	{
		value = arg;
	}

	/**
	 * @param l I32 value
	 */
	public GMSEC_I32(GMSEC_I32 l)
	{
		Set(l.Get());
	}

	/**
	 * @brief set ulong value
	 * @param arg
	 */
	public void Set(int arg)
	{
		value = arg;
	}

	/**
	 * @brief get ulong value
	 * @return
	 */
	public int Get()
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
