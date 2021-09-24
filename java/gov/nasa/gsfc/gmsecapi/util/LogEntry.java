/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.util;

/** 
 * @class LogEntry
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 * @brief Log entry definition
 */
@Deprecated
public class LogEntry
{
	/**
	 * Provides the level of the LogEntry.
	 */
	public int level;


	/**
	 * Provides the time that the LogEntry was registered.
	 */
	public double time;


	/**
	 * Provides the log message.
	 */
	public String message;


	/**
	 * Basic constructor.
	 */
	public LogEntry()
	{
	}
}
