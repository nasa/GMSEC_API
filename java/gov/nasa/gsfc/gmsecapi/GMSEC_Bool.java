/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_Bool
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC boolean field type.
 *
 */
@Deprecated
public class GMSEC_Bool
{
	public boolean value;

	/**
	 * @brief create boolean and default to false
	 */
	public GMSEC_Bool()
	{
		value = false;
	}

	/**
	 * @brief create boolean with value
	 * @param arg
	 */
	public GMSEC_Bool(boolean arg)
	{
		value = arg;
	}

	/**
	 * @brief set boolean value
	 * @param arg
	 */
	public void Set(boolean arg)
	{
		value = arg;
	}

	/**
	 * @brief get boolean value
	 * @return
	 */
	public boolean Get()
	{
		return value;
	}

	/**
	 * @brief convert boolean value to string
	 */
	public String toString()
	{
		if (value)
			return "true";
		else
			return "false";
	}

}
