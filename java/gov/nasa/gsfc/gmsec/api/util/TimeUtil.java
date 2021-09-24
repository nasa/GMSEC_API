/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TimeUtil.java
 */

package gov.nasa.gsfc.gmsec.api.util;

import gov.nasa.gsfc.gmsec.api.jni.util.JNITimeUtil;


/**
 * A Time Utility class
 */
public class TimeUtil
{
	// To prevent users from instantiating this class.
	private TimeUtil() {}


	/**
	 * Returns the current time in seconds since Jan 1, 1970 00:00::00 UTC.
	 *
	 * @return A TimeSpec object.
	 */
	public static TimeSpec getCurrentTime()
	{
		return JNITimeUtil.getCurrentTime();
	}


	/**
	 * Returns a GMSEC time string YYYY-DDD-HH-MM-SS.sss for the given TimeSpec.
	 *
	 * @param spec A TimeSpec time
	 *
	 * @return A time string.
	 *
	 * @see TimeUtil#getCurrentTime()
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
	 * Returns a GMSEC time string YYYY-DDD-HH-MM-SS[....sss] for the given TimeSpec.
	 *
	 * @param spec       A TimeSpec time
	 * @param subseconds The number of digits to allow for subseconds [0 .. 9].
	 *
	 * @return A time string.
	 *
	 * @see TimeUtil#getCurrentTime()
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
