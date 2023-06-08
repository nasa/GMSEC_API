/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TimeSpec.java
 */

package gmsec;


/** 
 * GMSEC Time %Specification definition
 */
public class TimeSpec
{
	/**
	 * Constructor that sets time to Jan 1 1970 00:00:00 UTC.
	 */
	public TimeSpec();


	/**
	 * Constructor that sets the time to the given time period.
	 *
	 * @param seconds     Seconds since Jan 1 1970 00:00:00 UTC
	 * @param nanoseconds Fractional seconds
	 */
	public TimeSpec(seconds, nanoseconds);


	/**
	 * Copy constructor
	 *
	 * @param other The TimeSpec object to copy
	 *
	 * @throws SWIG_Exception Thrown if the given TimeSpec object is null.
	 */
	public TimeSpec(other);


	/**
	 * Returns the number of seconds since Jan 1 1970 00:00:00 UTC.
	 *
	 * @return The number of seconds.
	 */
	public getSeconds();


	/**
	 * Returns the fractional portion of seconds since Jan 1 1970 00:00:00 UTC.
	 * Range is from 0 to 1E9-1.
	 *
	 * @return The number of nanoseconds.
	 */
	public getNanoseconds();
}
