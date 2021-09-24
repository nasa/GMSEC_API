/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





package gov.nasa.gsfc.gmsecapi.util;

import gov.nasa.gsfc.gmsecapi.jni.*;


/**
 * @class Log
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 * @brief Basic logging class. This class supports basic logging to any output
 *        stream. This class is thread-safe.
 */
@Deprecated
public class Log
{
	/** @fn SetReportingLevel(int level)
	 * @brief Sets maximum logging level.
	 *
	 * @param level - Possible values LogLevel.NONE, LogLevel.ERROR,
	 *                LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *                LogLevel.VERBOSE, and LogLevel.DEBUG.
	 *
	 */
	public static void SetReportingLevel(int level)
	{
		JNILog.SetReportingLevel(level);
	}
	
	/** @fn GetReportingLevel()
	 * @brief Returns the maximum logging level.
	 *
	 * @return Possible values LogLevel.NONE, LogLevel.ERROR,
	 *         LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *         LogLevel.VERBOSE, and LogLevel.DEBUG.
	 *
	 */
	public static int GetReportingLevel()
	{
		return JNILog.GetReportingLevel();
	}
	
	/** @fn RegisterHandler(LogHandler handler)
	 * @brief Registers a LogHandler that will be called for all 
	 *        logging levels.
	 *
	 * @param handler - An instance of a class whose
	 *                  implementation is LogHandler.
	 *
	 */
	public static void RegisterHandler(LogHandler handler)
	{
		if (handler != null) {
			JNILog.RegisterHandler(handler);
		}
	}

	/** @fn RegisterHandler(int level, LogHandler handler)
	 * @brief Registers a LogHandler that will be called for a specific
	 *        logging level.
	 *
	 * @param level - Possible values LogLevel.NONE, LogLevel.ERROR,
	 *                LogLevel.SECURE, LogLevel.WARNING, LogLevel.INFO,
	 *                LogLevel.VERBOSE, and LogLevel.DEBUG.
	 *
	 * @param handler - An instance of a class whose implementation
	 *                  is LogHandler.
	 *
	 */
	public static void RegisterHandler(int level, LogHandler handler)
	{
		if (handler != null) {
			JNILog.RegisterHandler(level, handler);
		}
	}
	
	/** @fn LevelToString(int level)
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
	public static String LevelToString(int level)
	{
		return JNILog.LogLevelToString(level);
	}
	
	/** @fn LevelFromString(String level)
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
	public static int LevelFromString(String level)
	{
		if (level == null) {
			return LogLevel.NONE;
		}
		return JNILog.LogLevelFromString(level);
	}
	
	/** @fn Error(String message)
	 * @brief Logs an error message.
	 *
	 * @param message - String to log.
	 *
	 */
	public static void Error(String message)
	{
		if (message != null) {
			JNILog.LogError(message);
		}
	}

	/** @fn Warning(String message)
	 * @brief Logs a warning message.
	 *
	 * @param message - String to log.
	 *
	 */
	public static void Warning(String message)
	{
		if (message != null) {
			JNILog.LogWarning(message);
		}
	}

	/** @fn Info(String message)
	 * @brief Logs an info message.
	 *
	 * @param message - String to log.
	 *
	 */
	public static void Info(String message)
	{
		if (message != null) {
			JNILog.LogInfo(message);
		}
	}

	/** @fn Verbose(String message)
	 * @brief Logs a verbose message.
	 *
	 * @param message - String to log.
	 *
	 */
	public static void Verbose(String message)
	{
		if (message != null) {
			JNILog.LogVerbose(message);
		}
	}

	/** @fn Debug(String message)
	 * @brief Logs a debug message.
	 *
	 * @param message - String to log.
	 *
	 */
	public static void Debug(String message)
	{
		if (message != null) {
			JNILog.LogDebug(message);
		}
	}
}
