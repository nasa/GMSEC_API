/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_I16
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC signed 18-bit integer field type.
 *
 */
@Deprecated
public class GMSEC_I16
{
	public short value;

	/**
	 * @brief construct empty short
	 */
	public GMSEC_I16()
	{
	}

	/**
	 * @brief construct short with value
	 * @param arg
	 */
	public GMSEC_I16(short arg)
	{
		value = arg;
	}

	/**
	 * @param s I16 value
	 */
	public GMSEC_I16(GMSEC_I16 s)
	{
		Set(s.Get());
	}

	/**
	 * @brief set short value
	 * @param arg
	 */
	public void Set(short arg)
	{
		value = arg;
	}

	/**
	 * @brief get short value
	 * @return
	 */
	public short Get()
	{
		return value;
	}

	/**
	 * @brief get short as printable
	 */
	public String toString()
	{
		return "" + value;
	}
}
