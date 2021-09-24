/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogEntry.java
 */

package gov.nasa.gsfc.gmsec.api.util;


/** 
 * Log entry definition
 */
public class LogEntry
{
	/**
	 * Provides the level of the LogEntry.
	 */
	public LogLevel level;


	/**
	 * Provides the time that the LogEntry was registered.
	 */
	public TimeSpec time;


	/**
	 * Provides the file name where the LogEntry originated.
	 */
	public String fileName;


	/**
	 * Provides the line number within the file where the LogEntry originated.
	 */
	public int lineNumber;


	/**
	 * Provides the log message.
	 */
	public String message;


	/**
	 * Basic constructor.
	 */
	public LogEntry() {}
}
