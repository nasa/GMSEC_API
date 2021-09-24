/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_ULong
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC ulong field type.
 *
 */
@Deprecated
public class GMSEC_ULong extends GMSEC_U32
{
	/**
	 * @brief construct empty unsigned long
	 */
	public GMSEC_ULong()
	{
	}

	/**
	 * @brief construct ulong with value
	 * @param arg
	 */
	public GMSEC_ULong(long arg)
	{
		value = arg;
	}

	/**
	 * @param l ulong value
	 */
	public GMSEC_ULong(GMSEC_ULong l)
	{
		Set(l.Get());
	}
}
