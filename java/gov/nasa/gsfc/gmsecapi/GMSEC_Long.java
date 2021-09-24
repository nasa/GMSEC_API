/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_Long
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC long field type.
 *
 */
@Deprecated
public class GMSEC_Long extends GMSEC_I32
{
	/**
	 * @brief construct empty long
	 */
	public GMSEC_Long()
	{
	}

	/**
	 * @brief construct long with value
	 * @param arg
	 */
	public GMSEC_Long(int arg)
	{
		value = arg;
	}

	/**
	 * @param l long value
	 */
	public GMSEC_Long(GMSEC_Long l)
	{
		Set(l.Get());
	}
}
