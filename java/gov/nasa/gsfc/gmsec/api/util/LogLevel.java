/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogLevel.java
 *
 * @brief Log level definition
 */

package gov.nasa.gsfc.gmsec.api.util;


/** 
 * @enum LogLevel
 * @brief Log level definitions
 */
public enum LogLevel
{
	NONE(0),       ///< No logging enabled
	ERROR(1),      ///< Error level logging
	SECURE(2),     ///< Secure level logging
	WARNING(3),    ///< Warning level logging
	INFO(4),       ///< Info level logging
	VERBOSE(5),    ///< Verbose level logging
	DEBUG(6);      ///< Debug level logging


	private int value;
	private LogLevel(int value) { this.value = value; }

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

	public int getValue() { return value; }

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
