/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_String
 *
 * @brief This is a container class for the GMSEC string field type.
 *
 */
public class GMSEC_String
{
	public String value;

	/**
	 * @brief construct empty string
	 */
	public GMSEC_String()
	{
	}

	/**
	 * @brief construct string with value
	 * @param arg
	 */
	public GMSEC_String(String arg)
	{
		value = arg;
	}

	/**
	 * @param s string value
	 */
	public GMSEC_String(GMSEC_String s)
	{
		Set(s.Get());
	}

	/**
	 * @brief set string value
	 * @param arg
	 */
	public void Set(String arg)
	{
		value = arg;
	}

	/**
	 * @brief get string value
	 * @return
	 */
	public String Get()
	{
		return value;
	}

	/**
	 * @brief get value as printable
	 */
	public String toString()
	{
		return value;
	}
}
