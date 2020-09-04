/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.util;


import gov.nasa.gsfc.gmsecapi.jni.*;


/**
 * @brief TimeUtil class
 */
public class TimeUtil
{
	/**
	 * @brief Formats a GMSEC time string YYYY-DDD-HH-MM-SS.sss
	 */
	public static String FormatTime_s(double time_s)
	{
		return JNITimeUtil.FormatTime_s(time_s);
	}
}
