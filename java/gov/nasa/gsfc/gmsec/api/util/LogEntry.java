/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogEntry.java
 *
 * @brief Log entry definition
 */

package gov.nasa.gsfc.gmsec.api.util;


/** 
 * @class LogEntry
 * @brief Log entry definition
 */
public class LogEntry
{
	/**
	 * @desc Provides the level of the LogEntry.
	 */
	public LogLevel level;


	/**
	 * @desc Provides the time that the LogEntry was registered.
	 */
	public TimeSpec time;


	/**
	 * @desc Provides the file name where the LogEntry originated.
	 */
	public String fileName;


	/**
	 * @desc Provides the line number within the file where the LogEntry originated.
	 */
	public int lineNumber;


	/**
	 * @desc Provides the log message.
	 */
	public String message;


	/**
	 * Basic constructor.
	 */
	public LogEntry() {}
}
