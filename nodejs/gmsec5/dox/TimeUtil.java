/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TimeUtil.java
 */

package gmsec;


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
	public static millisleep(ms);


	/**
	 * Returns the current time in seconds since Jan 1, 1970 00:00::00 UTC.
	 *
	 * @return A TimeSpec object.
	 */
	public static getCurrentTime();


	/**
	 * Returns the current time in (real) seconds since Jan 1, 1970.
	 *
	 * @return A double value representing current time in seconds.
	 */
	public static getCurrentTime_s();


	/**
	 * Returns the current time in (real) seconds since Jan 1, 1970.
	 *
	 * @param timeSpec A TimeSpec object which will be populated.
	 *
	 * @return A double value representing current time in seconds.
	 *
	 * @throws SWIG_Exception Thrown if a non-TimeSpec object is provided.
	 */
	public static getCurrentTime_s(timeSpec);


	/**
	 * Converts the given time string into a TimeSpec.
	 *
	 * @param timeString A time string in the format of YYYY-DDD-HH-MM-SS[.sss]
	 *
	 * @return A TimeSpec object.
	 *
	 * @throws SWIG_Exception Thrown if the given time string is null.
	 * @throws GmsecException Thrown if the given time string is not valid.
	 */
	public static convertTimeString(timeString);


	/**
	 * Converts the given time string into a TimeSpec.
	 *
	 * @param timeString A time string in the format of YYYY-DDD-HH-MM-SS[.sss]
	 *
	 * @return A double value representing current time in seconds.
	 *
	 * @throws SWIG_Exception Thrown if the given time string is null.
	 * @throws GmsecException Thrown if the given time string is not valid.
	 */
	public static convertTimeString_s(timeString);


	/**
	 * Returns a GMSEC time string YYYY-DDD-HH-MM-SS.sss for the given TimeSpec.
	 *
	 * @param timeSpec A TimeSpec time.
	 *
	 * @return A time string.
	 *
	 * @throws SWIG_Exception Thrown if the given TimeSpec is null.
	 * @throws SWIG_Exception Thrown if a non-TimeSpec object is provided.
	 *
	 * @see TimeUtil.getCurrentTime()
	 */
	public static formatTime(timeSpec);


	/**
	 * Returns a GMSEC time string YYYY-DDD-HH-MM-SS[....sss] for the given TimeSpec.
	 *
	 * @param timeSpec   A TimeSpec time.
	 * @param subseconds The number of digits to allow for subseconds [0 .. 9].
	 *
	 * @return A time string.
	 *
	 * @throws SWIG_Exception Thrown if the given TimeSpec is null.
	 * @throws SWIG_Exception Thrown if a non-TimeSpec object is provided.
	 * @throws GmsecException Thrown if the subseconds are not in the range of 0 thru 9.
	 *
	 * @see TimeUtil.getCurrentTime()
	 */
	public static formatTime(timeSpec, subseconds);
}
