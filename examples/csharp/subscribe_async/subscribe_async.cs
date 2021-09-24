/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe_async.cs
 *
 * This file contains a simple example outlining how to asynchronously subscribe
 * to and retrieve messages from the middleware bus. Asynchronous receipt
 * operations essentially mean that a Callback subclass is implemented, that the
 * corresponding Callback object is registered to a subscription, and that
 * messages are received asynchronously after starting the AutoDispatcher.
 *
 * This example program is intended to be run before starting the 'publish'
 * example program.
 */

using GMSEC.API;
using System;
using System.Threading;


//o Create a subclass of Callback and implement the onMessage function
// The onMessage function will determine what action(s) the program takes
// when receiving messages asynchronously
class ExampleCallback: ConnectionManagerCallback
{
	public bool ReceivedMessage = false;

	public override void OnMessage(ConnectionManager connection, Message message)
	{
		Log.Info("[ExampleCallback::onMessage] Received:\n" + message.ToXML());
		ReceivedMessage = true;
	}
}

class subscribe_async
{
	static private String DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.TEST.PUBLISH";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage subscribe_async.exe mw-id=<middleware ID>");
			return -1;
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config = new Config(args);

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
				// bus and register the callback object to the subscription
				// Subscription subject wildcard syntax:
				// * - Matches any one token separated by periods in a subject
				// > - Reading left to right, matches everything up to and ONE
				//     or more tokens in a subject
				// + - Reading left to right, matches everything up to and ZERO
				//     or more tokens in a subject
				// For more information on wildcards, please see the GMSEC API
				// User's Guide
				Log.Info("Subscribing to the topic: " + DEFAULT_SUBSCRIPTION_SUBJECT);
				ExampleCallback cb = new ExampleCallback();
				connMgr.Subscribe(DEFAULT_SUBSCRIPTION_SUBJECT, cb);

				//o Start the AutoDispatcher to begin asynchronously processing
				// messages
				connMgr.StartAutoDispatch();

				//o Because the handling of messages is occurring asynchronously
				// in another thread, we will need to 'busy wait' here until a
				// message is received
				while (cb.ReceivedMessage == false)
				{
					TimeUtil.Millisleep(100);
				}

				//o Clean up
				connMgr.StopAutoDispatch();
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
