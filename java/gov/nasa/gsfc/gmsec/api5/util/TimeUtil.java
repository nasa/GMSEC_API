/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TimeUtil.java
 */

package gov.nasa.gsfc.gmsec.api5.util;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.jni.util.JNITimeUtil;


/**
 * A Time Utility class
 */
public class TimeUtil
{
	// To prevent users from instantiating this class.
	private TimeUtil() {}


	/**
	 * Suspends execution of the process for the given amount of milliseconds.
	 *
	 * @param ms The number of milliseconds to delay/sleep (minimum is at least 1).
	 */
	public static void millisleep(int ms)
	{
		JNITimeUtil.millisleep(ms);
	}


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
	 * Returns the current time in (real) seconds since Jan 1, 1970.
	 *
	 * @return A double value representing current time in seconds.
	 */
	public static double getCurrentTime_s()
	{
		return JNITimeUtil.getCurrentTime_s();
	}


	/**
	 * Converts the given time string into a TimeSpec.
	 *
	 * @param timeString A time string in the format of YYYY-DDD-HH-MM-SS[.sss]
	 *
	 * @return A TimeSpec object.
	 *
	 * @throws IllegalArgumentException Thrown if the given time string is null
	 * @throws GmsecException Thrown if the given time string is not valid
	 */
	public static TimeSpec convertTimeString(String timeString)
		throws IllegalArgumentException, GmsecException
	{
		if (timeString == null)
		{
			throw new IllegalArgumentException("Time String is null");
		}

		return JNITimeUtil.convertTimeString(timeString);
	}


	/**
	 * Returns a GMSEC time string YYYY-DDD-HH-MM-SS.sss for the given TimeSpec.
	 *
	 * @param spec A TimeSpec time
	 *
	 * @return A time string.
	 *
	 * @throws IllegalArgumentException Thrown if the given TimeSpec is null
	 *
	 * @see TimeUtil#getCurrentTime()
	 */
	public static String formatTime(TimeSpec spec)
		throws IllegalArgumentException
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
	 * @throws IllegalArgumentException Thrown if the given TimeSpec is null, or if the subseconds are not in the range of 0 thru 9
	 *
	 * @see TimeUtil#getCurrentTime()
	 */
	public static String formatTime(TimeSpec spec, int subseconds)
		throws IllegalArgumentException
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
