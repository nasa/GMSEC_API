/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TimeSpec.java
 *
 * @brief GMSEC Time Specification definition
 */

package gov.nasa.gsfc.gmsec.api.util;


/** 
 * @class TimeSpec
 * @brief GMSEC Time Specification definition
 */
public class TimeSpec
{
	private long seconds;
	private long nanoseconds;


	/**
	 * @fn TimeSpec()
	 *
	 * @desc Constructor that sets time to Jan 1 1970 00:00:00 UTC.
	 */
	public TimeSpec()
	{
		seconds     = 0;
		nanoseconds = 0;
	}


	/**
	 * @fn TimeSpec(long seconds, long nanoseconds)
	 *
	 * @desc Constructor that sets time to Jan 1 1970 00:00:00 UTC.
	 *
	 * @param seconds     - seconds since Jan 1 1970 00:00:00 UTC
	 * @param nanoseconds - fractional seconds
	 */
	public TimeSpec(long seconds, long nanoseconds)
	{
		this.seconds     = seconds;
		this.nanoseconds = nanoseconds;

		if (this.nanoseconds < 0L)
		{
			this.seconds     -= 1;
			this.nanoseconds += 1000000000L;
		}
		else if (this.nanoseconds > 1000000000L)
		{
			this.seconds     += 1;
			this.nanoseconds -= 1000000000L;
		}
	}


	/**
	 * @fn TimeSpec(TimeSpec other)
	 *
	 * @desc Copy constructor
	 *
	 * @param other - the TimeSpec object to copy
	 */
	public TimeSpec(TimeSpec other)
	{
		this.seconds     = other.getSeconds();
		this.nanoseconds = other.getNanoseconds();
	}


	/**
	 * @fn long getSeconds()
	 *
	 * @desc Returns the number of seconds since Jan 1 1970 00:00:00 UTC.
	 */
	public long getSeconds()
	{
		return seconds;
	}


	/**
	 * @fn long getNanoseconds()
	 *
	 * @desc Returns the fractional portion of seconds since Jan 1 1970 00:00:00 UTC.
	 * Range is from 0 to 1E9-1.
	 */
	public long getNanoseconds()
	{
		return nanoseconds;
	}
}
