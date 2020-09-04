/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.util;

/** 
 * @class LogEntry
 * @brief Log entry definition
 */
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
