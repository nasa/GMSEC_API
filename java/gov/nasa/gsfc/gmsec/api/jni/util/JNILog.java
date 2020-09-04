/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.util.LogLevel;
import gov.nasa.gsfc.gmsec.api.util.LogHandler;

import java.util.HashMap;
import java.util.Map;


public class JNILog
{
	private static Map<LogLevel, LogHandler> logHandlers = new HashMap<LogLevel, LogHandler>();


	private static void storeLogHandler(LogLevel level, LogHandler handler)
	{
		logHandlers.put(level, handler);
	}


	public static void setReportingLevel(LogLevel level)
	{
		gmsecJNI.Log_SetReportingLevel(level.getValue());
	}


	public static LogLevel getReportingLevel()
	{
		int level = gmsecJNI.Log_GetReportingLevel();

		return LogLevel.values()[level];
	}


	public static void registerHandler(LogHandler handler)
	{
		long handlerPtr = JNILogHandler.getCPtr(LogHandler.getInternal(handler));

		gmsecJNI.Log_RegisterHandler(handlerPtr);

		// We need to save a reference to the log handler (or null if such is the case)
		// for all log levels.  This will assist in preventing the JVM garbage collector
		// from disposing of anonymous log handler objects.
		for (int l = LogLevel.ERROR.getValue() ; l <= LogLevel.DEBUG.getValue(); ++l)
		{
			storeLogHandler(LogLevel.getUsingValue(l), handler);
		}
	}


	public static void registerHandler(LogLevel level, LogHandler handler)
	{
		long handlerPtr = JNILogHandler.getCPtr(LogHandler.getInternal(handler));

		gmsecJNI.Log_RegisterHandler(level.getValue(), handlerPtr);

		// We need to save a reference to the log handler (or null if such is the case)
		// for the given log level.  This will assist in preventing the JVM garbage collector
		// from disposing of anonymous log handler objects.
		storeLogHandler(level, handler);
	}


	public static String logLevelToString(LogLevel level)
	{
		return gmsecJNI.Log_LogLevelToString(level.getValue());
	}


	public static LogLevel logLevelFromString(String level)
	{
		int nivel = gmsecJNI.Log_LogLevelFromString(level);

		return LogLevel.values()[nivel];
	}


	public static void logError(String message)
	{
		gmsecJNI.Log_LogError(message);
	}


	public static void logSecure(String message)
	{
		gmsecJNI.Log_LogSecure(message);
	}


	public static void logWarning(String message)
	{
		gmsecJNI.Log_LogWarning(message);
	}


	public static void logInfo(String message)
	{
		gmsecJNI.Log_LogInfo(message);
	}


	public static void logVerbose(String message)
	{
		gmsecJNI.Log_LogVerbose(message);
	}


	public static void logDebug(String message)
	{
		gmsecJNI.Log_LogDebug(message);
	}
}
