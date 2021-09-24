/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe.cpp
 *
 * This file contains a simple example outlining how to synchronously subscribe
 * to and retrieve messages from the middleware bus. Synchronous receipt
 * operations essentially mean that the receive() function is being invoked to
 * retrieve messages.
 *
 * This example program is intended to be run before starting the 'publish'
 * example program.
 */

using GMSEC.API;
using System;

class subscribe
{
	static private String EXAMPLE_SUBSCRIPTION_SUBJECT = "GMSEC.TEST.PUBLISH";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage subscribe.exe mw-id=<middleware ID>");
			return -1;
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config = new Config(args);

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		InitializeLogging(config);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			//o Create a ConnectionManager object
			// This is the linchpin for all communications between the
			// GMSEC API and the middleware server
			using (ConnectionManager connMgr = new ConnectionManager(config))
			{
				//o Open the connection to the middleware
				Log.Info("Opening the connection to the middleware server");
				connMgr.Initialize();

				//o Output middleware client library version
				Log.Info("Middleware version: " + connMgr.GetLibraryVersion());

				//o Set up a subscription to listen for Messages with the topic
				// "GMSEC.TEST.PUBLISH" which are published to the middleware
				// bus
				// Subscription subject wildcard syntax:
				// * - Matches any one token separated by periods in a subject
				// > - Reading left to right, matches everything up to and ONE
				//     or more tokens in a subject
				// + - Reading left to right, matches everything up to and ZERO
				//     or more tokens in a subject
				// For more information on wildcards, please see the GMSEC API
				// User's Guide
				Log.Info("Subscribing to the topic: " + EXAMPLE_SUBSCRIPTION_SUBJECT);
				connMgr.Subscribe(EXAMPLE_SUBSCRIPTION_SUBJECT);

				//o Call receive() to synchronously retrieve a message that has
				// been received by the middleware client libraries
				// Timeout periods:
				// -1 - Wait forever
				//  0 - Return immediately
				// >0 - Time in milliseconds before timing out
				Log.Info("Waiting to receive a Message");
				Message message = connMgr.Receive(Gmsec.GMSEC_WAIT_FOREVER);

				// Example error handling for calling receive() with a timeout
				if (message != null)
				{
					Log.Info("Received message:\n" + message.ToXML());

					// Dispose of received message
					connMgr.Release(message);
				}

				//o Disconnect from the middleware and clean up the Connection
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
