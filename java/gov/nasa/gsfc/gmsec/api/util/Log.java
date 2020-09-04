/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Log.java
 *
 * @brief Basic logging class.
 */

package gov.nasa.gsfc.gmsec.api.util;

import gov.nasa.gsfc.gmsec.api.jni.*;


/**
 * @class Log
 *
 * @brief Basic logging class. This class supports basic logging to any output
 *        stream. This class is thread-safe.
 */
public class Log
{
	/**
	 * @fn void setReportingLevel(LogLevel level)
	 *
	 * @brief Sets maximum logging level.
	 *
	 * @param level - Possible values LogLevel.NONE, LogLevel.ERROR,
	 *                LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *                LogLevel.VERBOSE, and LogLevel.DEBUG.
	 */
	public static void setReportingLevel(LogLevel level)
	{
		JNILog.setReportingLevel(level);
	}


	/**
	 * @fn LogLevel getReportingLevel()
	 *
	 * @brief Returns the maximum logging level.
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
	 * @fn void registerHandler(LogHandler handler)
	 *
	 * @brief Registers a LogHandler that will be called for all logging levels.
	 *
	 * @param handler - An instance of a class whose implementation is LogHandler.
	 *
	 * @throws An IllegalArgumentException is thrown if the given LogHandler is null.
	 */
	public static void registerHandler(LogHandler handler) throws IllegalArgumentException
	{
		if (handler == null)
		{
			throw new IllegalArgumentException("LogHandler is null");
		}

		JNILog.registerHandler(handler);
	}


	/**
	 * @fn registerHandler(LogLevel level, LogHandler handler)
	 *
	 * @brief Registers a LogHandler that will be called for a specific
	 *        logging level.
	 *
	 * @param level - Possible values LogLevel.NONE, LogLevel.ERROR,
	 *                LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *                LogLevel.VERBOSE, and LogLevel.DEBUG.
	 *
	 * @param handler - An instance of a class whose implementation is LogHandler.
	 *
	 * @throws An IllegalArgumentException is thrown if the given LogHandler is null.
	 */
	public static void registerHandler(LogLevel level, LogHandler handler) throws IllegalArgumentException
	{
		if (handler == null)
		{
			throw new IllegalArgumentException("LogHandler is null");
		}

		JNILog.registerHandler(level, handler);
	}


	/**
	 * @fn String levelToString(LogLevel level)
	 *
	 * @brief Converts the log level number to a string value.
	 *
	 * @param level - Possible values LogLevel.NONE, LogLevel.ERROR,
	 *                LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *                LogLevel.VERBOSE, and LogLevel.DEBUG
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
	 * @fn LogLevel levelFromString(String level)
	 *
	 * @brief Converts the string value to a log level number.
	 *
	 * @param level - String with possible contents of "NONE," "ERROR,"
	 *                "SECURE," "WARNING," "INFO," "VERBOSE," "DEBUG," or
	 *                "INVALID."
	 *
	 * @return Possible values LogLevel.NONE, LogLevel.ERROR,
	 *          LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *          LogLevel.VERBOSE, and LogLevel.DEBUG
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
	 * @fn void error(String message)
	 *
	 * @brief Logs an error message.
	 *
	 * @param message - String to log.
	 */
	public static void error(String message)
	{
		if (message != null)
		{
			JNILog.logError(message);
		}
	}


	/**
	 * @fn void secure(String message)
	 *
	 * @brief Logs a secure message.
	 *
	 * @param message - String to log.
	 */
	public static void secure(String message)
	{
		if (message != null)
		{
			JNILog.logSecure(message);
		}
	}


	/**
	 * @fn void warning(String message)
	 *
	 * @brief Logs a warning message.
	 *
	 * @param message - String to log.
	 */
	public static void warning(String message)
	{
		if (message != null)
		{
			JNILog.logWarning(message);
		}
	}


	/**
	 * @fn void info(String message)
	 *
	 * @brief Logs an info message.
	 *
	 * @param message - String to log.
	 */
	public static void info(String message)
	{
		if (message != null)
		{
			JNILog.logInfo(message);
		}
	}


	/**
	 * @fn void verbose(String message)
	 *
	 * @brief Logs a verbose message.
	 *
	 * @param message - String to log.
	 */
	public static void verbose(String message)
	{
		if (message != null)
		{
			JNILog.logVerbose(message);
		}
	}


	/**
	 * @fn void debug(String message)
	 *
	 * @brief Logs a debug message.
	 *
	 * @param message - String to log.
	 */
	public static void debug(String message)
	{
		if (message != null)
		{
			JNILog.logDebug(message);
		}
	}
}
