/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_UShort
 *
 * @brief This is a container class for the GMSEC ushort field type.
 *
 */
public class GMSEC_UShort extends GMSEC_U16
{
	/**
	 * @brief construct empty ushort
	 */
	public GMSEC_UShort()
	{
	}

	/**
	 * @brief construct ushort with value
	 * @param arg
	 */
	public GMSEC_UShort(int arg)
	{
		value = arg;
	}

	/**
	 * @param s ushort value
	 */
	public GMSEC_UShort(GMSEC_UShort s)
	{
		Set(s.Get());
	}
}
