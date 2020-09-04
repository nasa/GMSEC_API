/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_Float
 *
 * @brief This is a container class for the GMSEC float field type.
 *
 */
public class GMSEC_Float extends GMSEC_F32
{
	/**
	 * @brief construct empty float
	 */
	public GMSEC_Float()
	{
	}

	/**
	 * @brief construct float with value
	 * @param arg
	 */
	public GMSEC_Float(float arg)
	{
		value = arg;
	}

	/**
	 * @param f float value
	 */
	public GMSEC_Float(GMSEC_Float f)
	{
		Set(f.Get());
	}
}
