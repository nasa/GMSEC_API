/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_U16
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC unsigned 16-bit integer field type.
 *
 */
@Deprecated
public class GMSEC_U16
{
	public int value;

	/**
	 * @brief construct empty ushort
	 */
	public GMSEC_U16()
	{
	}

	/**
	 * @brief construct ushort with value
	 * @param arg
	 */
	public GMSEC_U16(int arg)
	{
		value = arg;
	}

	/**
	 * @param s u16 value
	 */
	public GMSEC_U16(GMSEC_U16 s)
	{
		Set(s.Get());
	}

	/**
	 * @brief set ushort value
	 * @param arg
	 */
	public void Set(int arg)
	{
		value = arg;
	}

	/**
	 * @brief get ushort value
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
