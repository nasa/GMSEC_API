/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request_open_resp.cs
 *
 * This file contains an example demonstrating how to issue a request message
 * and handle a coinciding reply message when using the open-response
 * configuration option. This example program is intended to be run after
 * starting up the 'reply_open_resp' example program.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
 * set to true in order to see how any subscriber can receive reply messages
 * while using the open-response functionality. Note that by setting the
 * configuration option 'GMSEC-REQ-RESP' to 'open-resp' in the requester
 * automatically turns on the 'mw-expose-resp' option.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;


class request_open_resp
{
	static private String OPEN_RESP_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.RESP.REQUEST_OPENRESP";
	static private String OPEN_RESP_REPLY_SUBJECT   = "GMSEC.MISSION.SAT_ID.RESP.REPLY_OPENRESP";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage request_open_resp.exe mw-id=<middleware ID>");
			return -1;
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config = new Config(args);

		//o Ensure that the open-response is enabled
	 	// Note: Other subscribing applications should set the configuration
	 	// option 'mw-expose-resp' to 'true' in order to receive exposed replies
	 	// By setting the configuration option 'GMSEC-REQ-RESP' to 'open-resp'
	 	// here, it automatically enables the 'mw-expose-resp' option.
		config.AddValue("GMSEC-REQ-RESP", "OPEN-RESP");

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.AddValue("gmsec-msg-content-validate-all", "false");

		// If it was not specified in the command-line arguments, set LOGLEVEL
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
			connMgr.Initialize();

			//o Output middleware client library version
			Log.Info(connMgr.GetLibraryVersion());

			//o Subscribe to the bus in preparation to receive the
			// open-response message (Because it will not be routed
			// to the reqeust() call)
			String reply_subject = OPEN_RESP_REPLY_SUBJECT + ".*";
			connMgr.Subscribe(reply_subject);

			//o Output information
			Log.Info("Issuing a request using the subject '" + OPEN_RESP_REQUEST_SUBJECT);

			//o Create message
			using(Message requestMsg = new Message(OPEN_RESP_REQUEST_SUBJECT, Message.MessageKind.REQUEST))
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
				Message replyMsg = connMgr.Request(requestMsg, 1000, (int) GMSECWaitDefs.REQUEST_REPUBLISH_NEVER);

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
