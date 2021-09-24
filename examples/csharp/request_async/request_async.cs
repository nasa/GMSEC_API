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
using System;

class ExampleReplyCallback : ConnectionManagerReplyCallback
{
	public bool ReceivedReply = false;

	public override void OnReply(ConnectionManager connMgr, Message request, Message reply)
	{
		Log.Info("[ExampleReplyCallback::OnReply]\n" + reply.ToXML());

		ReceivedReply = true;
	}

	public override void OnEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent connEvent)
	{
		if (status.IsError())
		{
			Log.Error("[ExampleReplyCallback::OnEvent] " + status.Get() + ", event=" + connEvent); 
		}
	}
}


class request_async
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
		Log.Info("API version:" + ConnectionManager.GetAPIVersion());

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

				//o Output information
				Log.Info("Issuing a request using the DEFAULT_REQUEST_SUBJECT '" + DEFAULT_REQUEST_SUBJECT);

				//o Create message
				using (Message requestMsg = new Message(DEFAULT_REQUEST_SUBJECT, Message.MessageKind.REQUEST))
				{
					//o Add fields to message
					requestMsg.AddField(new StringField("QUESTION", "Is there anyone out there?"));
					requestMsg.AddField(new StringField("COMPONENT", "request_async"));

					//o Display XML representation of request message
					Log.Info("Sending request message:\n" + requestMsg.ToXML());

					//o Send Request Message
					ExampleReplyCallback cb = new ExampleReplyCallback();
					connMgr.Request(requestMsg, 1000, cb, 1000);

					// Loop while waiting for the asynchronous response until done
					for (int i = 0; i < 20; ++i)
					{
						if (cb.ReceivedReply == true)
						{
							Log.Info("Response Received (in ReplyCallback)");
							break;
						}
						TimeUtil.Millisleep(500);
					}

					if (!cb.ReceivedReply)
					{
						Log.Warning("Giving up!");
					}
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
