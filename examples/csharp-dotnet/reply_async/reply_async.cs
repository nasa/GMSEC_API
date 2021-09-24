/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply_async.cs
 *
 * This file contains an example demonstrating how to asynchronously reply to a
 * request message.
 *
 * Asynchronous receipt operations essentially mean that a Callback subclass is
 * implemented, then the corresponding Callback object is registered to a
 * subscription, and that messages are received asynchronously after starting
 * the AutoDispatcher. In this case, replies are created and sent asynchronously
 * when receiving request messages.
 *
 * This example program is intended to be run before starting up either the
 * 'request' or 'request_async' example program.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;
using System.Threading;

class ExampleCallback: ConnectionManagerCallback
{
	public bool ReplySent = false;
	public override void OnMessage(ConnectionManager connMgr, Message message)
	{
		try
		{
			//o Display the XML representation of the received message
			Log.Info("[ExampleCallback:onMessage] Received:\n" + message.ToXML());

			String component = "";

			//o Construct Reply subject.
			try
			{
				component = message.GetStringField("COMPONENT").GetValue();
			}
			catch (Exception e)
			{
				Log.Warning("COMPONENT field is not available: " + e.ToString());
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

			//o Create the reply subject.
			// See API Interface Specificaiton Document for
			// more information on Reply Message subjects.
			// Generally speaking, they are composed
			// accordingly:
			// [Spec].[Mission].[Satellite ID].
			// [Message Type].[Component Name].[Status Code]
			String reply_subject = "GMSEC.MISSION.SAT_ID.RESP.REPLY_ASYNC." + status_code;

			using(Message reply = new Message(reply_subject, Message.MessageKind.REPLY))
			{
				//o Add fields to the reply message
				reply.AddField(new StringField("COMPONENT", component));
				reply.AddField(new StringField("ANSWER", "Sure looks like it!"));

				//o Display XML representation of reply message
				Log.Info("Prepared Reply:\n" + reply.ToXML());

				//o Send Reply
				connMgr.Reply(message,reply);

				ReplySent = true;
			}
		}
		catch (Exception e)
		{
			Log.Error("[ExampleCallback::onMessage] " + e.ToString());
		}
	}
}

class reply_async
{
	static private String DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage reply_async.exe mw-id=<middleware ID>");
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

		//o Output GMSEC API version
		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			connMgr.Initialize();

			//o Output middleware client library version
			Log.Info(connMgr.GetLibraryVersion());

			//o Subscribe
			ExampleCallback cb = new ExampleCallback();
			connMgr.Subscribe(DEFAULT_REQUEST_SUBJECT, cb);

			//o Start the AutoDispatcher to begin async message receipt
			connMgr.StartAutoDispatch();

			//o Loop while waiting for the asynchronous response until done
			while (cb.ReplySent == false)
			{
				Thread.Sleep(100);
			}

			//o Wait for a few moments to ensure that the asynchronous reply
			// transaction went through with the middleware before closing
			// the connection and exiting the process
			for(int i = 0; i < 5; i++)
			{
				Thread.Sleep(100);
			}

			//o Clean up
			connMgr.StopAutoDispatch();
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
