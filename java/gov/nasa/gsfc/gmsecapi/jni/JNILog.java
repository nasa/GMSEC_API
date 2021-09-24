/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.jni;

import gov.nasa.gsfc.gmsecapi.util.LogHandler;

/** @class JNILog
 *
 * @brief 
 */
public class JNILog
{
	private long swigCPtr;
	protected boolean swigCMemOwn;

	protected JNILog(long cPtr, boolean cMemoryOwn)
	{
		swigCMemOwn = cMemoryOwn;
		swigCPtr = cPtr;
	}


	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Log(swigCPtr, this);
			swigCMemOwn = false;
		}
		swigCPtr = 0;
	}

	protected static long getCPtr(JNILog obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}

	/**
	 * @brief Create an empty Log
	 */
	public JNILog()
	{
		this(gmsecJNI.new_Log(), true);
	}


	public static void SetReportingLevel(int level)
	{
		gmsecJNI.Log_SetReportingLevel(level);
	}
	
	public static int GetReportingLevel()
	{
		return gmsecJNI.Log_GetReportingLevel();
	}
	
	public static void RegisterHandler(LogHandler handler)
	{
		gmsecJNI.Log_RegisterHandler(handler);
	}

	public static void RegisterHandler(int level, LogHandler handler)
	{
		gmsecJNI.Log_RegisterHandler(level, handler);
	}
	
	public static String LogLevelToString(int level)
	{
		return gmsecJNI.Log_LogLevelToString(level);
	}
	
	public static int LogLevelFromString(String level)
	{
		return gmsecJNI.Log_LogLevelFromString(level);
	}
	
	public static void LogError(String message)
	{
		gmsecJNI.Log_LogError(message);
	}

	public static void LogWarning(String message)
	{
		gmsecJNI.Log_LogWarning(message);
	}

	public static void LogInfo(String message)
	{
		gmsecJNI.Log_LogInfo(message);
	}

	public static void LogVerbose(String message)
	{
		gmsecJNI.Log_LogVerbose(message);
	}

	public static void LogDebug(String message)
	{
		gmsecJNI.Log_LogDebug(message);
	}
	
}
