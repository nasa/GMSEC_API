/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file connection_state.cs
 *
 * This file contains an example which demonstrates how to utilize
 * EventCallbacks to track the state of the GMSEC Connection and its connection
 * to the middleware.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;

//o Define an EventCallback to report the status of WebSphere async publications
// Note: The WebSphere MQ client libraries only report the first warning or
// failure code encountered during the series of message publications made
// since the last time that the client libraries retrieved the status of
// asynchronous publish operations.
class ConnectionStateCallback : ConnectionManagerEventCallback
{
	public override void OnEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent connEvent)
	{
		// Perform some action based on the type of event that occurred
		if (connEvent == Connection.ConnectionEvent.CONNECTION_SUCCESSFUL_EVENT)
		{
			Log.Info("Connected to the middleware server");
		}
		else if (connEvent == Connection.ConnectionEvent.CONNECTION_BROKEN_EVENT)
		{
			Log.Info("Connection to the middleware lost or terminated");
		}
		else if (connEvent == Connection.ConnectionEvent.CONNECTION_RECONNECT_EVENT)
		{
			Log.Info("Attempting to reestablish the connection to the middleware");
		}
	}
}

class connection_state
{
	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage connection_state.exe mw-id=<middleware ID>");
			return -1;
		}

		ConnectionStateCallback cb = new ConnectionStateCallback();

		//o Load the command-line input into a GMSEC Config object
		Config config = new Config(args);

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		InitializeLogging(config);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// TODO: Once available, replace this statement with usage of
		// ConnectionManager::getAPIVersion (See RTC 4798)
		Log.Info(Connection.GetAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			connMgr.Initialize();

			//o Register the event callback with the connection to catch
			// connection state changes, including:
			// 1. Connection successfully established to middleware
			// 2. Connection broken from middleware
			// 3. Reconnecting to the middleware
			connMgr.RegisterEventCallback(Connection.ConnectionEvent.CONNECTION_SUCCESSFUL_EVENT, cb);
			connMgr.RegisterEventCallback(Connection.ConnectionEvent.CONNECTION_BROKEN_EVENT, cb);
			connMgr.RegisterEventCallback(Connection.ConnectionEvent.CONNECTION_RECONNECT_EVENT, cb);

			//o Output middleware version
			Log.Info("Middleware version = " + connMgr.GetLibraryVersion());

			// Assuming that the user provided proper reconnection syntax
			// for the corresponding middleware they are using, one could
			// kill the middleware server at this point, then start it back
			// up to demonstrate the EventCallback.onEvent() function
			// triggering

			//o Wait for user input to end the program
			Log.Info("Waiting for Connection events to occur, press <enter> to exit the program");
			Console.ReadLine();

			//o Clean up the ConnectionManager before exiting the program
			connMgr.Cleanup();
		}
		catch (Exception e)
		{
			Log.Error(e.ToString());
			return -1;
		}

		return 0;
	}

	static void InitializeLogging(Config config)
	{
		String logLevel = config.GetValue("LOGLEVEL");
		String logFile = config.GetValue("LOGFILE");

		if (logLevel == null)
		{
			config.AddValue("LOGLEVEL", "INFO");
		}
		if (logFile == null)
		{
			config.AddValue("LOGFILE", "STDERR");
		}
	}

	static void PopulateMessage(Message message, int value)
	{
		// Note: Since C# is a strongly-typed language, it is possible to use
		// type casting to add Field objects to a Message without having to
		// first create a Field object, then add it to the message.

		message.AddField(new CharField("CHAR-FIELD", (sbyte) 'c'));
		message.AddField(new BooleanField("BOOL-FIELD-TRUE", true));
		message.AddField(new BooleanField("BOOL-FIELD-FALSE", false));
		message.AddField(new I8Field("I8-FIELD", (sbyte) value));
		message.AddField(new I16Field("I16-FIELD", (short) value));
		message.AddField(new I32Field("I32-FIELD", (int) value));
		message.AddField(new I64Field("I64-FIELD", (long) value));
		message.AddField(new U8Field("U8-FIELD", (byte) value));
		message.AddField(new U16Field("U16-FIELD", (UInt16) value));
		message.AddField(new U32Field("U32-FIELD", (UInt32) value));
		message.AddField(new U64Field("U64-FIELD", (UInt64) value));
		message.AddField(new StringField("STRING-FIELD", "This is a test"));
		message.AddField(new F32Field("F32-FIELD", (float) (1 + 1.0 / value)));
		message.AddField(new F64Field("F64-FIELD", (double) (1 + 1.0 / value)));
		message.AddField(new BinaryField("BIN-FIELD", System.Text.Encoding.ASCII.GetBytes("JLMNOPQ")));
	}
}
