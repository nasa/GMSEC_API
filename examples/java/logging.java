/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


 
/** 
 * @file logging.java
 *
 * A Java demonstration example of GMSEC logging.
 *
 */

import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.util.LogEntry;
import gov.nasa.gsfc.gmsec.api.util.LogHandler;
import gov.nasa.gsfc.gmsec.api.util.LogLevel;
import gov.nasa.gsfc.gmsec.api.util.TimeUtil;


class GenericHandler
{
	public static void onMessageAux(String source, LogEntry entry)
	{
		StringBuilder sb = new StringBuilder();

		sb.append(source).append(" : ");
		sb.append(TimeUtil.formatTime(entry.time)).append(" [").append(entry.level.toString()).append("] ");
		sb.append("[").append(entry.fileName).append(":").append(entry.lineNumber).append("] ");
		sb.append(entry.message).append("\n");

		String output = sb.toString().replace("\n", "\n\t");

		System.out.println(sb.toString());
	}


	public static class ErrorHandler extends LogHandler
	{
		public void onMessage(LogEntry entry)
		{
			onMessageAux(getClass().getName(), entry);
		}
	}


	public static class WarningHandler extends LogHandler
	{
		public void onMessage(LogEntry entry)
		{
			onMessageAux(getClass().getName(), entry);
		}
	}

	public static class InfoHandler extends LogHandler
	{
		public void onMessage(LogEntry entry)
		{
			onMessageAux(getClass().getName(), entry);
		}
	}


	public static class VerboseHandler extends LogHandler
	{
		public void onMessage(LogEntry entry)
		{
			onMessageAux(getClass().getName(), entry);
		}
	}


	public static class DebugHandler extends LogHandler
	{
		public void onMessage(LogEntry entry)
		{
			onMessageAux(getClass().getName(), entry);
		}
	}

	public static class AnyHandler extends LogHandler
	{
		public void onMessage(LogEntry entry)
		{
			onMessageAux(getClass().getName(), entry);
		}
	}
}


public class logging
{
	public static void main(String args[])
	{
		GenericHandler.ErrorHandler   errorHandler = new GenericHandler.ErrorHandler();
		GenericHandler.WarningHandler warningHandler = new GenericHandler.WarningHandler();    
		GenericHandler.InfoHandler    infoHandler = new GenericHandler.InfoHandler();
		GenericHandler.VerboseHandler verboseHandler = new GenericHandler.VerboseHandler();    
		GenericHandler.DebugHandler   debugHandler = new GenericHandler.DebugHandler();    
		GenericHandler.AnyHandler     anyHandler = new GenericHandler.AnyHandler();    

		Log.registerHandler(LogLevel.ERROR, errorHandler);    
		Log.registerHandler(LogLevel.WARNING, warningHandler);    
		Log.registerHandler(LogLevel.INFO, infoHandler);    
		Log.registerHandler(LogLevel.VERBOSE, verboseHandler);    
		Log.registerHandler(LogLevel.DEBUG, debugHandler);    

		System.out.println("Setting log level to " + Log.levelFromString("VERBOSE"));
		Log.setReportingLevel(LogLevel.VERBOSE);

		Log.info("The reporting level is set to " + Log.getReportingLevel());

		Log.error("This is an example error message.");
		Log.warning("This is an example warning message.");
		Log.verbose("This is an example verbose message.");
		// This last message cannot be shown right now because
		// Log.setReportingLevel(LogLevel.VERBOSE), used above, does not
		// allow DEBUG messages to come out.
		Log.debug("This is an example debug message which should not show.");

		// Set logging reporting level to now allow DEBUG messages to be shown
		Log.setReportingLevel(LogLevel.DEBUG);

		if (Log.getReportingLevel() == LogLevel.DEBUG)
		{
			Log.info("Changed reporting level to LogLevel.DEBUG");
		}
		else
		{
			Log.error("Failed to change reporting level to LogLevel.DEBUG");
		}
		Log.debug("This is an example debug message which should show.");

		Log.debug("NONE reporting level, numerically, is " + Log.levelFromString("NONE"));
		Log.debug("ERROR reporting level, numerically, is " + Log.levelFromString("ERROR"));
		Log.debug("SECURE reporting level, numerically, is " + Log.levelFromString("SECURE"));
		Log.debug("WARNING reporting level, numerically, is " + Log.levelFromString("WARNING"));
		Log.debug("INFO reporting level, numerically, is " + Log.levelFromString("INFO"));
		Log.debug("VERBOSE reporting level, numerically, is " + Log.levelFromString("VERBOSE"));
		Log.debug("DEBUG reporting level, numerically, is " + Log.levelFromString("DEBUG"));

		// Register general-purpose handler and test
		Log.registerHandler(anyHandler);

		Log.error("NONE reporting level, numerically, is " + Log.levelFromString("NONE"));
		Log.error("ERROR reporting level, numerically, is " + Log.levelFromString("ERROR"));
		Log.warning("WARNING reporting level, numerically, is " + Log.levelFromString("WARNING"));
		Log.info("INFO reporting level, numerically, is " + Log.levelFromString("INFO"));
		Log.verbose("VERBOSE reporting level, numerically, is " + Log.levelFromString("VERBOSE"));
		Log.debug("DEBUG reporting level, numerically, is " + Log.levelFromString("DEBUG"));
	}
}
