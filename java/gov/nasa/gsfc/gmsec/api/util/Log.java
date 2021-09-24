/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Log.java
 */

package gov.nasa.gsfc.gmsec.api.util;

import gov.nasa.gsfc.gmsec.api.jni.util.JNILog;


/**
 * Basic logging class. This class supports basic logging to any output
 * stream. This class is thread-safe.
 */
public class Log
{
	/**
	 * Sets maximum logging level.
	 *
	 * @param level Possible values LogLevel.NONE, LogLevel.ERROR,
	 *              LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *              LogLevel.VERBOSE, and LogLevel.DEBUG.
	 */
	public static void setReportingLevel(LogLevel level)
	{
		JNILog.setReportingLevel(level);
	}


	/**
	 * Returns the maximum logging level.
	 *
	 * @return Possible values LogLevel.NONE, LogLevel.ERROR,
	 *         LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *         LogLevel.VERBOSE, and LogLevel.DEBUG.
	 */
	public static LogLevel getReportingLevel()
	{
		return JNILog.getReportingLevel();
	}


	/**
	 * Registers a LogHandler that will be called for all logging levels.
	 *
	 * @param handler An instance of a class which extends LogHandler, or null to
	 * restore the default log handler.
	 */
	public static void registerHandler(LogHandler handler)
	{
		JNILog.registerHandler(handler);
	}


	/**
	 * Registers a LogHandler that will be called for a specific logging level.
	 *
	 * @param level Possible values LogLevel.NONE, LogLevel.ERROR,
	 *              LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *              LogLevel.VERBOSE, and LogLevel.DEBUG.
	 * @param handler An instance of a class which extends LogHandler, or null to
	 * restore the default log handler.
	 */
	public static void registerHandler(LogLevel level, LogHandler handler)
	{
		JNILog.registerHandler(level, handler);
	}


	/**
	 * Converts the log level number to a string value.
	 *
	 * @param level Possible values LogLevel.NONE, LogLevel.ERROR,
	 *              LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *              LogLevel.VERBOSE, and LogLevel.DEBUG
	 *
	 * @return String with possible contents of "NONE," "ERROR,"
	 *         "SECURE," "WARNING," "INFO," "VERBOSE," "DEBUG," or
	 *         "INVALID."
	 */
	public static String levelToString(LogLevel level)
	{
		return JNILog.logLevelToString(level);
	}


	/**
	 * Converts the string value to a log level number.
	 *
	 * @param level String with possible contents of "NONE," "ERROR,"
	 *              "SECURE," "WARNING," "INFO," "VERBOSE," "DEBUG," or
	 *              "INVALID."
	 *
	 * @return Possible values LogLevel.NONE, LogLevel.ERROR,
	 *         LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *         LogLevel.VERBOSE, and LogLevel.DEBUG
	 */
	public static LogLevel levelFromString(String level)
	{
		if (level == null)
		{
			return LogLevel.NONE;
		}
		return JNILog.logLevelFromString(level);
	}


	/**
	 * Logs an error message.
	 *
	 * @param message String to log.
	 */
	public static void error(String message)
	{
		if (message != null)
		{
			JNILog.logError(message);
		}
	}


	/**
	 * Logs a secure message.
	 *
	 * @param message String to log.
	 */
	public static void secure(String message)
	{
		if (message != null)
		{
			JNILog.logSecure(message);
		}
	}


	/**
	 * Logs a warning message.
	 *
	 * @param message String to log.
	 */
	public static void warning(String message)
	{
		if (message != null)
		{
			JNILog.logWarning(message);
		}
	}


	/**
	 * Logs an info message.
	 *
	 * @param message String to log.
	 */
	public static void info(String message)
	{
		if (message != null)
		{
			JNILog.logInfo(message);
		}
	}


	/**
	 * Logs a verbose message.
	 *
	 * @param message String to log.
	 */
	public static void verbose(String message)
	{
		if (message != null)
		{
			JNILog.logVerbose(message);
		}
	}


	/**
	 * Logs a debug message.
	 *
	 * @param message String to log.
	 */
	public static void debug(String message)
	{
		if (message != null)
		{
			JNILog.logDebug(message);
		}
	}
}
