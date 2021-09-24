/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_U32
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC unsigned 32-bit integer field type.
 *
 */
@Deprecated
public class GMSEC_U32
{
	public long value;

	/**
	 * @brief construct empty long
	 */
	public GMSEC_U32()
	{
	}

	/**
	 * @brief construct long with value
	 * @param arg
	 */
	public GMSEC_U32(long arg)
	{
		value = arg;
	}

	/**
	 * @param l u32 value
	 */
	public GMSEC_U32(GMSEC_U32 l)
	{
		Set(l.Get());
	}

	/**
	 * @brief set long value
	 * @param arg
	 */
	public void Set(long arg)
	{
		value = arg;
	}

	/**
	 * @brief get long value
	 * @return
	 */
	public long Get()
	{
		return value;
	}

	/**
	 * @brief get long as printable
	 */
	public String toString()
	{
		return "" + value;
	}
}
