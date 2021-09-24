/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_F32
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC 32-bit floating-point field type.
 *
 */
@Deprecated
public class GMSEC_F32
{
	public float value;

	/**
	 * @brief construct empty float
	 */
	public GMSEC_F32()
	{
	}

	/**
	 * @brief construct float with value
	 * @param arg
	 */
	public GMSEC_F32(float arg)
	{
		value = arg;
	}

	public GMSEC_F32(GMSEC_F32 f)
	{
		Set(f.Get());
	}

	/**
	 * @brief set F32 value
	 * @param arg
	 */
	public void Set(float arg)
	{
		value = arg;
	}

	/**
	 * @brief get F32 value
	 * @return
	 */
	public float Get()
	{
		return value;
	}

	/**
	 * @brief get value as printable
	 */
	public String toString()
	{
		return "" + value;
	}
}
