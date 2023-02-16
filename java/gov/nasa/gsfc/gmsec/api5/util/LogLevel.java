/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogLevel.java
 */

package gov.nasa.gsfc.gmsec.api5.util;


/** 
 * Log level definitions
 */
public enum LogLevel
{
	/** No logging enabled */
	NONE(0),

	/** Error level logging */
	ERROR(1),

	/** Secure level logging */
	SECURE(2),

	/** Warning level logging */
	WARNING(3),

	/** Info level logging */
	INFO(4),

	/** Verbose level logging */
	VERBOSE(5),

	/** Debug level logging */
	DEBUG(6);


	private int value;
	private LogLevel(int value) { this.value = value; }

	/**
	 * Displays the log level held by the LogLevel object as a string.
	 */
	public String toString()
	{
		switch (value)
		{
			case 0 : return "NONE";
			case 1 : return "ERROR";
			case 2 : return "SECURE";
			case 3 : return "WARNING";
			case 4 : return "INFO";
			case 5 : return "VERBOSE";
			case 6 : return "DEBUG";
		}
		return "NONE";
	}

	/**
	 * Converts the enumerated LogLevel to an integer.
	 * @return An integer value. 
	 */
	public int getValue() { return value; }

	/**
	 * Converts the integer value to a LogLevel enumerated value.
	 * @param value An integer value representing a LogLevel enumerated value.
	 * @return A LogLevel enumerated value.
	 */
	public static LogLevel getUsingValue(int value)
	{
		LogLevel level = NONE;

		for (LogLevel l : LogLevel.values())
		{
			if (l.value == value)
			{
				level = l;
				break;
			}
		}

		return level;
	}
}
