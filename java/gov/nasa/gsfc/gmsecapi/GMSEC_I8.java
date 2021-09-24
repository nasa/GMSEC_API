/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_I8
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC signed 8-bit integer field type.
 *
 */
@Deprecated
public class GMSEC_I8
{
	public byte value;

	/**
	 * @brief create empty char
	 */
	public GMSEC_I8()
	{
	}

	/**
	 * @brief creat char with value
	 * @param arg
	 */
	public GMSEC_I8(byte arg)
	{
		value = arg;
	}

	/**
	 * @param c I8 value
	 */
	public GMSEC_I8(GMSEC_I8 c)
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
		StringBuffer buf = new StringBuffer();
		buf.append((int) this.value);
		return buf.toString();
	}
}
