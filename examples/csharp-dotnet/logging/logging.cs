/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file logging.cs
 *
 * This file contains an example outlining the use of the GMSEC API logging
 * framework.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;
using System.Threading;

//o Define a baseline LogHandler
// This will be used by the Log macros; The implementation of onMessage
// determines how messages will be logged to output
class BaseHandler: LogHandler
{
	// Use a mutex so messages from different threads don't get mixed up
	private Mutex mutex;
	private String whoAmI;

	public BaseHandler(String wai)
	{
		whoAmI = wai;
		mutex = new Mutex();
	}

	public override void OnMessage(LogEntry entry)
	{
		mutex.WaitOne();

		String tempBuffer = TimeUtil.FormatTime(entry.GetTime());

		Console.WriteLine("[BaseHandler::onMessage] for:" + whoAmI +
			" : " + tempBuffer + " [" + Log.LevelToString(entry.GetLevel()) + "]" +
			entry.GetMessage() + "\n");

		mutex.ReleaseMutex();
	}
}

class ErrorHandler: BaseHandler
{
	public ErrorHandler() : base("GMSEC_ERROR"){}
}

class WarningHandler: BaseHandler
{
	public WarningHandler() : base("GMSEC_WARNING"){}
}

class InfoHandler: BaseHandler
{
	public InfoHandler() : base("GMSEC_INFO"){}
}

class VerboseHandler: BaseHandler
{
	public VerboseHandler() : base("GMSEC_VERBOSE"){}
}

class DebugHandler: BaseHandler
{
	public DebugHandler() : base("GMSEC_DEBUG"){}
}

class AnyHandler: BaseHandler
{
	public AnyHandler() : base("ANY_HANDLER"){}
}

class logging
{
	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage logging.exe mw-id=<middleware ID>");
			return -1;
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config = new Config(args);

		ErrorHandler errorHandler = new ErrorHandler();
		WarningHandler warningHandler = new WarningHandler();
		InfoHandler infoHandler = new InfoHandler();
		VerboseHandler verboseHandler = new VerboseHandler();
		DebugHandler debugHandler = new DebugHandler();
		AnyHandler anyHandler = new AnyHandler();

		Log.RegisterHandler(LoggingLevel.ERROR, errorHandler);
		Log.RegisterHandler(LoggingLevel.WARNING, warningHandler);
		Log.RegisterHandler(LoggingLevel.INFO, infoHandler);
		Log.RegisterHandler(LoggingLevel.VERBOSE, verboseHandler);
		Log.RegisterHandler(LoggingLevel.DEBUG, debugHandler);

		//o Set logging reporting level
		Log.SetReportingLevel(LoggingLevel.VERBOSE);
		Log.Verbose("The log reporting level is now set to: " + Log.GetReportingLevel());

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			//o Create the ConnectionManager
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			Log.Info("Opening the connection to the middleware server");
			connMgr.Initialize();

			//o Output middleware client/wrapper version
			Log.Info(connMgr.GetLibraryVersion());

			//o Publish a message
			PublishTestMessage(connMgr, "GMSEC.TEST.PUBLISH");

			//o Disconnect from the middleware and clean up the Connection
			connMgr.Cleanup();
		}
		catch (Exception e)
		{
			Log.Error(e.ToString());
			return -1;
		}

		//o Unregister log handlers
		Log.RegisterHandler(null);

		//o Set log stream to stderr
		config.AddValue("LOGFILE", "STDERR");

		Log.Info("This message should go to stderr, not stdout.  " +
			 "For example, in bash test by running as:\n" +
			 "./logging mw-id=bolt 2> testfile.txt\n" +
			 "... and then check the contents of testfile.txt");

		//o Reset log stream to stdout
 		config.AddValue("LOGFILE", "STDOUT");

		Log.Error("This is an example error message.");
		Log.Warning("This is an example warning message.");
		Log.Verbose("This is an example \"verbose\" message.");
		Log.Debug("This is an example debug message which should not show.");

			// This last message cannot be shown right now because
	  	// Log::setReportingLevel(logVERBOSE), used above, does not
	  	// allow DEBUG messages to come out.
		Log.Verbose("This is another example \"verbose\" message.");

		Log.SetReportingLevel(LoggingLevel.DEBUG);
		if(Log.GetReportingLevel() == LoggingLevel.DEBUG)
		{
			Log.Info("Changed reporting level to logDEBUG");
		}
		else
		{
			Log.Error("Failed to change reporting level to logDEBUG");
		}

		// The DEBUG message below will be shown successfully, unlike the last
		// debug message.
		Log.Debug( "This is an example debug message which should show.");

		Log.Debug("NONE reporting level, numerically, is " + Log.LevelFromString("NONE"));
		Log.Debug("ERROR reporting level, numerically, is " + Log.LevelFromString("ERROR"));
		Log.Debug("SECURE reporting level, numerically, is " + Log.LevelFromString("SECURE"));
		Log.Debug("WARNING reporting level, numerically, is " + Log.LevelFromString("WARNING"));
		Log.Debug("INFO reporting level, numerically, is " + Log.LevelFromString("INFO"));
		Log.Debug("VERBOSE reporting level, numerically, is " + Log.LevelFromString("VERBOSE"));
		Log.Debug("DEBUG reporting level, numerically, is " + Log.LevelFromString("DEBUG"));

		//o Register general-purpose handler and test
		Log.RegisterHandler(anyHandler);

		Log.Error("NONE reporting level, numerically, is " + Log.LevelFromString("NONE"));
		Log.Error("ERROR reporting level, numerically, is " + Log.LevelFromString("ERROR"));
		Log.Warning("WARNING reporting level, numerically, is " + Log.LevelFromString("WARNING"));
		Log.Info("INFO reporting level, numerically, is " + Log.LevelFromString("INFO"));
		Log.Verbose("VERBOSE reporting level, numerically, is " + Log.LevelFromString("VERBOSE"));
		Log.Debug("DEBUG reporting level, numerically, is " + Log.LevelFromString("DEBUG"));

		//o Unregister log handlers
		Log.RegisterHandler(null);

		return 0;
	}

	static void PublishTestMessage(ConnectionManager connMgr, String subject)
	{
		int i = 123;

		using(Message message = new Message(subject, Message.MessageKind.PUBLISH))
		{
			//o Add fields to the Message
			message.AddField(new BooleanField("F", false));
			message.AddField(new I32Field("I", (int) i));
			message.AddField(new I16Field("K", (short) i));
			message.AddField(new StringField("S", "This is a test"));
			message.AddField(new F32Field("D", (float) (1 + 1.0/i)));
			message.AddField(new BinaryField("BIN-FIELD", System.Text.Encoding.ASCII.GetBytes("JLMNOPQ")));

			//o Publish Message
			connMgr.Publish(message);

			//o Output the Message's XML string representation by invoking Log macro
			Log.Info("Sent:\n" + message.ToXML());
		}
	}
}
