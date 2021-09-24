/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file logging.java
 *
 * This file contains an example outlining the use of the GMSEC API logging
 * framework.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.util.LogLevel;
import gov.nasa.gsfc.gmsec.api.util.LogHandler;
import gov.nasa.gsfc.gmsec.api.util.LogEntry;
import gov.nasa.gsfc.gmsec.api.util.TimeUtil;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback;
import java.util.concurrent.Semaphore;

//o Define a baseline LogHandler
// This will be used by the Log macros; The implementation of onMessage
// determines how messages will be logged to output
class BaseHandler extends LogHandler
{
	// Use a mutex so messages from different threads don't get mixed up
	private Semaphore mutex;
	private String whoAmI = "";

	public BaseHandler(String wai)
	{
		mutex = new Semaphore(1);
		whoAmI = wai;
	}

	public void onMessage(LogEntry entry)
	{
		try
		{
			mutex.acquire();

			String time = TimeUtil.formatTime(entry.time);

			System.out.println("[BaseHandler::onMessage] for: " + whoAmI +
				 " : " + time + " [" + Log.levelToString(entry.level) + "]" +
				 " [" + entry.fileName + ":" + entry.lineNumber + "] " +
				 entry.message + "\n\n");

			mutex.release();
		}
		catch(Exception e)
		{
			Log.error(e.getMessage());
		}
	}
}

class ErrorHandler extends BaseHandler
{
	public ErrorHandler()
	{
		super("GMSEC_ERROR");
	}
}

class WarningHandler extends BaseHandler
{
	public WarningHandler()
	{
		super("GMSEC_WARNING");
	}
}

class InfoHandler extends BaseHandler
{
	public InfoHandler()
	{
		super("GMSEC_INFO");
	}
}

class VerboseHandler extends BaseHandler
{
	public VerboseHandler()
	{
		super("GMSEC_VERBOSE");
	}
}

class DebugHandler extends BaseHandler
{
	public DebugHandler()
	{
		super("GMSEC_DEBUG");
	}
}

class AnyHandler extends BaseHandler
{
	public AnyHandler()
	{
		super("ANY_HANDLER");
	}
}

public class logging
{
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		//o Create and register log handlers
		ErrorHandler errorHandler = new ErrorHandler();
		WarningHandler warningHandler = new WarningHandler();
		InfoHandler infoHandler = new InfoHandler();
		VerboseHandler verboseHandler = new VerboseHandler();
		DebugHandler debugHandler = new DebugHandler();
		AnyHandler anyHandler = new AnyHandler();

		Log.registerHandler(LogLevel.ERROR, errorHandler);
		Log.registerHandler(LogLevel.WARNING, warningHandler);
		Log.registerHandler(LogLevel.INFO, infoHandler);
		Log.registerHandler(LogLevel.VERBOSE, verboseHandler);
		Log.registerHandler(LogLevel.DEBUG, debugHandler);

		//o Set logging reporting level
		Log.setReportingLevel(LogLevel.VERBOSE);
		Log.verbose("The log reporting level is now set to: " + Log.getReportingLevel());

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			//o Create the ConnectionManager
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			connMgr.initialize();

			//o Output middleware client library version
			Log.info(connMgr.getLibraryVersion());

			//o Publish a message
			publishTestMessage(connMgr, "GMSEC.TEST.PUBLISH");

			//o Disconnect from the middleware and clean up the Connection
			connMgr.cleanup();
		}
		catch (Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}

		//o Unregister log handlers
		Log.registerHandler(null);

		//o Set log stream to stderr
		config.addValue("LOGFILE", "STDERR");
		Log.info("This message should go to stderr, not stdout.  " +
						 "For example, in bash test by running as:\n" +
						 "java logging mw-id=bolt 2> testfile.txt\n" +
						 "... and then check the contents of testfile.txt");

		//o Reset log stream to stdout
		config.addValue("LOGFILE", "STDOUT");

		Log.error("This is an example error message.");
		Log.warning("This is an example warning message.");
		Log.verbose("This is an example \"verbose\" message.");
		Log.debug("This is an example debug message which should not show.");

			// This last message cannot be shown right now because
	  	// Log::setReportingLevel(logVERBOSE), used above, does not
	  	// allow DEBUG messages to come out.
		Log.verbose("This is another example \"verbose\" message.");

		// Set logging reporting level to now allow DEBUG messages to be shown
		Log.setReportingLevel(LogLevel.DEBUG);
		if(Log.getReportingLevel() == LogLevel.DEBUG)
		{
			Log.info("Changed reporting level to logDEBUG");
		}
		else
		{
			Log.error("Failed to change reporting level to logDEBUG");
		}
		// The DEBUG message below will be shown successfully, unlike the last
		// debug message.
		Log.debug( "This is an example debug message which should show.");

		Log.debug("NONE reporting level, numerically, is " + Log.levelFromString("NONE"));
		Log.debug("ERROR reporting level, numerically, is " + Log.levelFromString("ERROR"));
		Log.debug("SECURE reporting level, numerically, is " + Log.levelFromString("SECURE"));
		Log.debug("WARNING reporting level, numerically, is " + Log.levelFromString("WARNING"));
		Log.debug("INFO reporting level, numerically, is " + Log.levelFromString("INFO"));
		Log.debug("VERBOSE reporting level, numerically, is " + Log.levelFromString("VERBOSE"));
		Log.debug("DEBUG reporting level, numerically, is " + Log.levelFromString("DEBUG"));

		//o Register general-purpose handler and test
		Log.registerHandler(anyHandler);

		Log.error("NONE reporting level, numerically, is " + Log.levelFromString("NONE"));
		Log.error("ERROR reporting level, numerically, is " + Log.levelFromString("ERROR"));
		Log.warning("WARNING reporting level, numerically, is " + Log.levelFromString("WARNING"));
		Log.info("INFO reporting level, numerically, is " + Log.levelFromString("INFO"));
		Log.verbose("VERBOSE reporting level, numerically, is " + Log.levelFromString("VERBOSE"));
		Log.debug("DEBUG reporting level, numerically, is " + Log.levelFromString("DEBUG"));

		//o Unregister log handlers
		Log.registerHandler(null);

		return;
	}

	public static void publishTestMessage(ConnectionManager connMgr, String subject)
	{
		int i = 123;
		try
		{
			//o Create a Message object
			Message message = new Message(subject, Message.MessageKind.PUBLISH);

			//o Add fields to the Message
			message.addField("F", false);
			message.addField("I", (int) i); //I32
			message.addField("K", (short) i); //I16
			message.addField("S", "This is a test");
			message.addField("D", (float) (1 + 1./i)); //F32
			message.addField("X", "JLMNOPQ".getBytes()); //Bin

			//o Publish Message
			connMgr.publish(message);

			//o Output the Message's XML string representation by invoking Log macro
			Log.info("Sent:\n" + message.toXML());
		}
		catch(Exception e)
		{
			Log.error(e.getMessage());
		}
	}
}
