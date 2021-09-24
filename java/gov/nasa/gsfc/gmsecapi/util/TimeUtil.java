/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.util;


import gov.nasa.gsfc.gmsecapi.jni.*;


/**
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 * @brief TimeUtil class
 */
@Deprecated
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
