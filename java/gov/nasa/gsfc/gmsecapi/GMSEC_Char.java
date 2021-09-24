/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_Char
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC character field type.
 *
 */
@Deprecated
public class GMSEC_Char
{
	public byte value;

	/**
	 * @brief create empty char
	 */
	public GMSEC_Char()
	{
	}

	/**
	 * @brief creat char with value
	 * @param arg
	 */
	public GMSEC_Char(byte arg)
	{
		value = arg;
	}

	/**
	 * @param c Char value
	 */
	public GMSEC_Char(GMSEC_Char c)
	{
		Set(c.Get());
	}

	/**
	 * @brief set char value
	 * @param arg
	 */
	public void Set(byte arg)
	{
		value = arg;
	}

	/**
	 * @brief get char value
	 * @return
	 */
	public byte Get()
	{
		return value;
	}

	/**
	 * @brief convert char to string
	 */
	public String toString()
	{
		String s = new String(new byte[] {value});
		return s;
	}
}
