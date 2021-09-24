/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply_open_resp.cs
 *
 * This file contains an example demonstrating how to utilize the open
 * response functionality to expose reply messages to normal subscribers.
 *
 * This example program is intended to be run before executing the
 * 'request_open_resp' example program.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
 * set to true in order to see how any subscriber can receive reply messages
 * while using the open-response functionality.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;
using System.Threading;

class reply_open_resp
{
	static private String OPEN_RESP_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.RESP.REQUEST_OPENRESP";
	static private String OPEN_RESP_REPLY_SUBJECT   = "GMSEC.MISSION.SAT_ID.RESP.REPLY_OPENRESP";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage reply_open_resp.exe mw-id=<middleware ID>");
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

		// If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		InitializeLogging(config);

		//o Ensure that the open-response is enabled
		config.AddValue("GMSEC-REQ-RESP", "OPEN-RESP");

		//o Output GMSEC API version
		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Open the connection to the middleware
			Log.Info("Opening the connection to the middleware server");
			connMgr.Initialize();

			//o Output middleware client/wrapper version
			Log.Info(connMgr.GetLibraryVersion());

			connMgr.Subscribe(OPEN_RESP_REQUEST_SUBJECT);

			//o Call receive() to synchronously retrieve a message that has
			// been received by the middleware client libraries
			// Timeout periods:
			// -1 - Wait forever
			//  0 - Return immediately
			// >0 - Time in milliseconds before timing out
			Message requestMsg = connMgr.Receive(-1);

			// Example error handling for calling receive() with a timeout
			if (requestMsg != null)
			{
				//o Display the XML representation of the received message
				Log.Info("Received a message\n" + requestMsg.ToXML());

				//o Double-check the Message type to ensure that it is a request
				if(requestMsg.GetKind() == Message.MessageKind.REQUEST)
				{
					//o Get the name of the component who issued the request
					String component = "";

					//o Construct a Reply message
					try
					{
						StringField compField = requestMsg.GetStringField("COMPONENT");
						component = compField.GetValue();
					}
					catch (Exception e)
					{
						Log.Warning(e.ToString());
					}

					//o Set Status Code to indicate Successful Completion.
					// The GMSEC Interface Specification Document defines 6
					// unique STATUS-CODE values:
					// 1 - Acknowledgement
					// 2 - Working/keep alive
					// 3 - Successful completion
					// 4 - Failed completion
					// 5 - Invalid request
					// 6 - Final message
					// If an asynchronous requestor is awaiting a reply, the
					// ReplyCallback in use will remain active for multiple
					// messages, so long as the messages it receives contain
					// a STATUS-CODE of either 1 or 2.
					String status_code = "3";

					//o Set the reply subject.
					// See API Interface Specificaiton Document for
					// more information on Reply Message subjects.
					// Generally speaking, they are composed
					// accordingly:
					// [Spec].[Mission].[Satellite ID].
					// [Message Type].[Component Name].[Status Code]
					String reply_subject = OPEN_RESP_REPLY_SUBJECT + "." + status_code;

					//o Create reply message
					using(Message replyMsg = new Message(reply_subject, Message.MessageKind.REPLY))
					{
						//o Add fields to the reply message
						replyMsg.AddField(new StringField("ANSWER", "Yup, I'm here!"));
						replyMsg.AddField(new StringField("COMPONENT", component));

						//o Display XML representation of the reply message
						Log.Info("Prepared Reply:\n" + replyMsg.ToXML());

						//o Send Reply
						connMgr.Reply(requestMsg, replyMsg);
					}

					//o Wait for a moment to ensure that the reply is sent out
					for(int i = 0; i < 10; i++)
					{
						Thread.Sleep(100);
					}
				}
				//o Destroy request message to release its memory
				connMgr.Release(ref requestMsg);
			}
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
