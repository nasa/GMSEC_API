/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_Short
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC short field type.
 *
 */
@Deprecated
public class GMSEC_Short extends GMSEC_I16
{
	/**
	 * @brief construct empty short
	 */
	public GMSEC_Short()
	{
	}

	/**
	 * @brief construct short with value
	 * @param arg
	 */
	public GMSEC_Short(short arg)
	{
		value = arg;
	}

	/**
	 * @param s short value
	 */
	public GMSEC_Short(GMSEC_Short s)
	{
		Set(s.Get());
	}
}
