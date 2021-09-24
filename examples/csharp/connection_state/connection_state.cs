/*
 * Copyright 2007-2021 United States Government as represented by the
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
using System;


class ConnectionStateCallback : ConnectionManagerEventCallback
{
	public override void OnEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent connEvent)
	{
		switch (connEvent)
		{
		case Connection.ConnectionEvent.CONNECTION_SUCCESSFUL_EVENT:
			Log.Info("[OnEvent]: Connected to the middleware server");
			break;

		case Connection.ConnectionEvent.CONNECTION_BROKEN_EVENT:
			Log.Info("[OnEvent]: Connection to the middleware lost or terminated");
			break;

		case Connection.ConnectionEvent.CONNECTION_RECONNECT_EVENT:
			Log.Info("[OnEvent]: Attempting to reestablish the connection to the middleware");
			break;

		default:
			Log.Info("[OnEvent]: " + status.Get());
			break;
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
		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			//o Create the Connection
			using ( ConnectionManager connMgr = new ConnectionManager(config))
			{
				//o Register the event callback with the connection to catch
				// connection state changes, including:
				// 1. Connection successfully established to middleware
				// 2. Connection broken from middleware
				// 3. Reconnecting to the middleware
				connMgr.RegisterEventCallback(Connection.ConnectionEvent.CONNECTION_SUCCESSFUL_EVENT, cb);
				connMgr.RegisterEventCallback(Connection.ConnectionEvent.CONNECTION_BROKEN_EVENT, cb);
				connMgr.RegisterEventCallback(Connection.ConnectionEvent.CONNECTION_RECONNECT_EVENT, cb);

				//o Connect
				connMgr.Initialize();

				//o Output middleware version
				Log.Info("Middleware version: " + connMgr.GetLibraryVersion());

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
		}
		catch (GmsecException e)
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
}
