/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request.cs
 *
 * This file contains an example demonstrating how to issue a request message
 * and handle a coinciding reply message. This example program is intended to
 * be run after starting up either the 'reply' or 'reply_async' example program.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;

class request
{
	static private String DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage request.exe mw-id=<middleware ID>");
			return -1;
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config = new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.AddValue("gmsec-msg-content-validate-all", "false");

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		InitializeLogging(config);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			//o Create the ConnectionManager
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Open the connection to the middleware
			Log.Info("Opening the connection to the middleware server");
			connMgr.Initialize();

			//o Output middleware client library version
			Log.Info(connMgr.GetLibraryVersion());

			//o Output information
			Log.Info("Issuing a request using the DEFAULT_REQUEST_SUBJECT '" + DEFAULT_REQUEST_SUBJECT);

			//o Create message
			using(Message requestMsg = new Message(DEFAULT_REQUEST_SUBJECT, Message.MessageKind.REQUEST))
			{

				//o Add fields to message
				requestMsg.AddField(new StringField("QUESTION", "Is there anyone out there?"));
				requestMsg.AddField(new StringField("COMPONENT", "request"));

				//o Display XML representation of request message
				Log.Info("Sending request message:\n" + requestMsg.ToXML());

				//o Send Request Message
				// Timeout periods:
				// -1 - Wait forever
				//  0 - Return immediately
				// >0 - Time in milliseconds before timing out
				Message replyMsg = connMgr.Request(requestMsg, -1, -1);

				// Example error handling for calling request() with a timeout
				if (replyMsg != null)
				{
					// Display the XML string representation of the reply
					Log.Info("Received replyMsg:\n" + replyMsg.ToXML());

					//o Destroy the replyMsg message
					connMgr.Release(ref replyMsg);
				}
			}

			//o Disconnect from the middleware and clean up the Connection
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
}
