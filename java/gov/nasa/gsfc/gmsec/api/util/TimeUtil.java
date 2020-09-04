/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TimeUtil.java
 *
 * @brief Utility methods for converting time from one format to another.
 */

package gov.nasa.gsfc.gmsec.api.util;

import gov.nasa.gsfc.gmsec.api.jni.*;


/**
 * @class TimeUtil
 *
 * @brief TimeUtil class
 */
public class TimeUtil
{
	/**
	 * @fn static TimeSpec getCurrentTime()
	 *
	 * @brief Returns the current time in seconds since Jan 1, 1970 00:00::00 UTC.
	 */
	public static TimeSpec getCurrentTime()
	{
		return JNITimeUtil.getCurrentTime();
	}


	/**
	 * @fn static String formatTime(TimeSpec time)
	 *
	 * @brief Returns a GMSEC time string YYYY-DDD-HH-MM-SS.sss for the given TimeSpec.
	 *
	 * @param spec - a TimeSpec time
	 */
	public static String formatTime(TimeSpec spec)
	{
		if (spec == null)
		{
			throw new IllegalArgumentException("TimeSpec is null");
		}

		return JNITimeUtil.formatTime(spec);
	}


	/**
	 * @fn static String formatTime(TimeSpec time, int subs)
	 *
	 * @brief Returns a GMSEC time string YYYY-DDD-HH-MM-SS.sss for the given TimeSpec.
	 *
	 * @param spec       - a TimeSpec time
	 * @param subseconds - the number of digits to allow for subseconds [0 .. 9].
	 */
	public static String formatTime(TimeSpec spec, int subseconds)
	{
		if (spec == null)
		{
			throw new IllegalArgumentException("TimeSpec is null");
		}
		if (subseconds < 0 || subseconds > 9)
		{
			throw new IllegalArgumentException("Subsecond value is out of range");
		}

		return JNITimeUtil.formatTime(spec, subseconds);
	}
}
