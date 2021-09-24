/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogHandler.java
 */

package gov.nasa.gsfc.gmsec.api.util;

import gov.nasa.gsfc.gmsec.api.jni.util.JNILogHandler;


/**
 * This abstract class is for receiving log events.
 * A user created class, extending this class, can be passed into 
 * Log.registerHandler() to have user code executed when a log event occurs.
 * <p>
 * Example LogHandler class:
 * <pre>{@code
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
 * }</pre>
 * <p>
 * Example LogHandler registration:
 * <pre>{@code
 * Log.registerHandler(new MyLogHandler());
 * }</pre>
 *
 * @see Log
 * @see LogEntry
 * @see TimeUtil
 */
public abstract class LogHandler
{
	private JNILogHandler m_jniLogHandler = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param handler A LogHandler object.
	 * @return A JNILogHandler object.
	 */
	public static JNILogHandler getInternal(LogHandler handler)
	{
		return (handler == null ? null : handler.m_jniLogHandler);
	}


	/**
	 * Constructor.
	 */
	protected LogHandler()
	{
		m_jniLogHandler = new JNILogHandler(this);
	}


	/**
	 * This method is called by the API in response to a log message.
	 * 
	 * @param entry The log entry
	 */
	public abstract void onMessage(LogEntry entry);
}
