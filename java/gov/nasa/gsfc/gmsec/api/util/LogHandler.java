/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogHandler.java
 *
 * @brief An interface for receiving log events.
 */

package gov.nasa.gsfc.gmsec.api.util;

import gov.nasa.gsfc.gmsec.api.jni.JNILogHandler;


/**
 * @class LogHandler
 *
 * @brief This abstract class is for receiving log events.
 * A user created class, extending this class, can be passed into 
 * Log.registerHandler() to have user code executed when a log event occurs.
 *
 * Example LogHandler class:
 * @code
 * class MyLogHandler extends LogHandler
 * {
 *     public void onMessage(LogEntry entry)
 *     {
 *        String time = TimeUtil.formatTime(entry.time);
 *
 *        StringBuilder sb = new StringBuilder();
 *
 *        sb.append("[").append(time).append("] ");
 *        sb.append("[").append(Log.levelToString(entry.level)).append("] ");
 *        sb.append(": ").append(entry.message).append("\n");
 *
 *        System.out.println(sb.toString());
 *     }
 * }
 * @endcode
 *
 * Example LogHandler registration:
 * @code
 * Log.registerHandler(new MyLogHandler());
 * @endcode
 *
 * @sa Log
 * @sa LogEntry
 * @sa TimeUtil
 */
public abstract class LogHandler
{
	private JNILogHandler m_jniLogHandler = null;


	public static JNILogHandler getInternal(LogHandler handler)
	{
		return (handler == null ? null : handler.m_jniLogHandler);
	}


	protected LogHandler()
	{
		m_jniLogHandler = new JNILogHandler(this);
	}


	/**
	 * @fn void onMessage(LogEntry entry)
	 *
	 * @brief This function is called by the API in response to a log message.
	 * 
	 * @param entry - the log entry
	 */
	public abstract void onMessage(LogEntry entry);
}
