/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request_async.cpp
 *
 * This file contains an example demonstrating how to asynchronously handle the
 * receipt of reply message(s) after issuing a request.
 * Asynchronous request operations essentially mean that a Callback subclass is
 * implemented, then a corresponding Callback object is supplied when invoking
 * request. Reply messages that are then received asynchronously and handled by
 * the Callback object.
 *
 * This example program is intended to be run after either starting up the
 * 'reply' or 'reply_async' example program.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;
using System.Threading;

class ExampleReplyCallback: ConnectionManagerReplyCallback
{
	public bool ReceivedReply = false;

	public override void OnReply(ConnectionManager connection, Message request, Message reply)
	{
		//o Display XML representation of reply message
		Log.Info("[ExampleReplyCallback::onReply]\n" + reply.ToXML());

		ReceivedReply = true;
	}

	public override void OnEvent(ConnectionManager connection, Status status, Connection.ConnectionEvent connEvent)
	{
		if (status.IsError())
		{
			Log.Error("[ExampleReplyCallback::onError] " + status.Get() + ", event=" + connEvent);
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
			Console.WriteLine("usage request_async.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.AddValue("gmsec-msg-content-validate-all", "false");

		// If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		InitializeLogging(config);

		//o Output GMSEC API version
		Log.Info(Connection.GetAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			connMgr.Initialize();

			//o Output middleware client library version
			Log.Info(connMgr.GetLibraryVersion());

			//o Create request message
			using(Message requestMsg = new Message(DEFAULT_REQUEST_SUBJECT, Message.MessageKind.REQUEST))
			{
				//o Add fields to request message
				requestMsg.AddField(new StringField("QUESTION", "Does the request/reply functionality still work?"));
				requestMsg.AddField(new StringField("COMPONENT", "request_async"));

				//o Display XML representation of request message
				Log.Info("Requesting:\n" + requestMsg.ToXML());

				ExampleReplyCallback cb = new ExampleReplyCallback();
				connMgr.Request(requestMsg, -1, cb, -1);

				Log.Info("Waiting for response...");

				// Loop while waiting for the asynchronous response until done
				while (cb.ReceivedReply == false)
				{
					Thread.Sleep(100);
				}

				if (cb.ReceivedReply)
				{
					Log.Info("Response Received!");
				}
				else
				{
					Log.Warning("No response received");
				}
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
