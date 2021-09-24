/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TimeSpec.java
 */

package gov.nasa.gsfc.gmsec.api.util;


/** 
 * GMSEC Time Specification definition
 */
public class TimeSpec
{
	private long seconds;
	private long nanoseconds;


	/**
	 * Constructor that sets time to Jan 1 1970 00:00:00 UTC.
	 */
	public TimeSpec()
	{
		seconds     = 0;
		nanoseconds = 0;
	}


	/**
	 * Constructor that sets the time to the given time period.
	 *
	 * @param seconds     Seconds since Jan 1 1970 00:00:00 UTC
	 * @param nanoseconds Fractional seconds
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
	 * Copy constructor
	 *
	 * @param other The TimeSpec object to copy
	 */
	public TimeSpec(TimeSpec other)
	{
		this.seconds     = other.getSeconds();
		this.nanoseconds = other.getNanoseconds();
	}


	/**
	 * Returns the number of seconds since Jan 1 1970 00:00:00 UTC.
	 *
	 * @return The number of seconds.
	 */
	public long getSeconds()
	{
		return seconds;
	}


	/**
	 * Returns the fractional portion of seconds since Jan 1 1970 00:00:00 UTC.
	 * Range is from 0 to 1E9-1.
	 *
	 * @return The number of nanoseconds.
	 */
	public long getNanoseconds()
	{
		return nanoseconds;
	}
}
