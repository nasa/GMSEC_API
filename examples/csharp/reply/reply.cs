/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file reply.cs
 *
 * This file contains an example demonstrating how to issue a reply to a
 * request message.
 *
 * This example program is intended to be run before starting up the
 * 'request' or 'request_async' example program.
 */

using GMSEC.API;
using System;

class reply
{
	static private String DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage reply.exe mw-id=<middleware ID>");
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

		//o Output GMSEC API version
		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			//o Create the ConnectionManager
			using (ConnectionManager connMgr = new ConnectionManager(config))
			{
				//o Open the connection to the middleware
				Log.Info("Opening the connection to the middleware server");
				connMgr.Initialize();

				//o Output middleware client library version
				Log.Info("Middleware version: " + connMgr.GetLibraryVersion());

				connMgr.Subscribe(DEFAULT_REQUEST_SUBJECT);

				//o Call receive() to synchronously retrieve a message that has
				// been received by the middleware client libraries
				// Timeout periods:
				// -1 - Wait forever
				//  0 - Return immediately
				// >0 - Time in milliseconds before timing out
				Message requestMsg = connMgr.Receive(Gmsec.GMSEC_WAIT_FOREVER);

				// Example error handling for calling receive() with a timeout
				if (requestMsg != null)
				{
					//o Display the XML representation of the received message
					Log.Info("Received a message\n" + requestMsg.ToXML());

					//o Double-check the Message type to ensure that it is a request
					if (requestMsg.GetKind() == Message.MessageKind.REQUEST)
					{
						//o Get the name of the component who issued the request
						String component = "";

						//o Construct a Reply message
						try
						{
							StringField compField = requestMsg.GetStringField("COMPONENT");
							component = compField.GetValue();
						}
						catch(GmsecException e)
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
						String reply_subject = "GMSEC.MISSION.SAT_ID.RESP.REPLY." + status_code;

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
					}

					//o Destroy request message to release its memory
					connMgr.Release(requestMsg);
				}
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
