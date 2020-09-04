/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.util;

/**
 * @interface LogHandler
 *
 * @brief This interface is for received error callbacks. 
 * A user created class, implemented with this interface, can be passed into 
 * %RegisterHandler() to have user code executed when the identified error
 * occurs.
 *
 * @sa Log
 * 
 * Example LogHandler class:
 * @code
 * class ErrorLogHandler implements LogHandler
 * {
 *	public void OnMessage(LogEntry entry) {
 *	}
 * }
 * @endcode
 *
 * Example LogHandler registration:
 * @code
 * Log.RegisterHandler(level, new ErrorLogHandler());
 * @endcode
 *
 */
public interface LogHandler
{
	/**
	 * @brief This function is called by the API in response to a log message.
	 * 
	 * @param entry - the log entry
	 * 
	 */
	public void OnMessage(LogEntry entry);
}
