/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe_async.cs
 *
 * @brief This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus using an asynchronous callback.
 */

using GMSEC.API5;
using System;
using System.Text;


// Define custom callback to receive messages
//
class MyCallback : Callback
{
	private bool gotMsg;

	public MyCallback()
	{
		gotMsg = false;
	}

	public override void OnMessage(Connection conn, Message msg)
	{
		gotMsg = true;

		Log.Info("MyCallback.OnMessage -- \n" + msg.ToXML());

		// Acknowledge processing the message
		msg.Acknowledge();
	}

	public bool GotMessage()
	{
		return gotMsg;
	}
}


// Define custom callback to receive events
//
// Although not required, an event callback can assist with determining
// whether we have a connection to the GMSEC Bus, or in the case of this
// example program, whether the Auto-Dispatcher thread has terminated
// due to some error.
//
class MyEventCallback : EventCallback
{
	private bool error;

	public MyEventCallback()
	{
		error = false;
	}

	public override void OnEvent(Connection conn, Status status, Connection.Event theEvent)
	{
		// Check event type and/or error status of the Status object.

		if (theEvent == Connection.Event.DISPATCHER_ERROR_EVENT)
		{
			// Auto-dispatcher error has occurred; we are done!
			Log.Error("MyEventCallback.OnEvent -- " + status.GetReason());
			error = true;
		}
		else if (status.HasError())
		{
			// Soft-error has occurred
			Log.Warning("MyEventCallback.OnEvent -- " + status.GetReason());
		}
		else
		{
			// Nominal event has occurred
			Log.Info("MyEventCallback.OnEvent -- " + status.GetReason());
		}
	}

	public bool HaveError()
	{
		return error;
	}
}


public class subscribe_async
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options from the command line.
		Config config = new Config(args);

		// Validate incoming messages
		config.AddValue("gmsec-msg-content-validate-recv", "true");

		// Initialize log level for output
		InitializeLogLevel(config);

		// Declared custom callback and custom event callback
		MyCallback      mcb = new MyCallback();
		MyEventCallback ecb = new MyEventCallback();

		// Deduce subscription topic for the example program
		string subscription_topic;
		int level = config.GetIntegerValue("gmsec-schema-level", (int) Specification.SchemaLevel.LEVEL_0);

		switch ((Specification.SchemaLevel) level)
		{
		case Specification.SchemaLevel.LEVEL_0:
			subscription_topic = "C2MS.>";
			break;

		case Specification.SchemaLevel.LEVEL_1:
			subscription_topic = "C2MS-PIPE.>";
			break;

		case Specification.SchemaLevel.LEVEL_2:
		default:
			subscription_topic = "GMSEC.>";
			break;
		}

		try
		{
			// Create connection instance.
			using (Connection conn = new Connection(config))
			{
				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Register custom Event Callback
				conn.RegisterEventCallback(Connection.Event.ALL_EVENTS, ecb);

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Subscribe to receive message via a callback
				conn.Subscribe(subscription_topic, mcb);
				Log.Info("Subscribe to topic: " + subscription_topic);

				// Start the auto-dispatcher so we can receive messages via the callback
				conn.StartAutoDispatch();

				// Normally an application would have something important to do;
				// here we just idle until a message arrives. Should an error
				// occur, we bail.
				Log.Info("Waiting to receive a message...");
				while (!mcb.GotMessage() && !ecb.HaveError())
				{
					TimeUtil.Millisleep(1000);
				}

				// Disconnect from the GMSEC Bus, and terminate subscriptions.
				conn.Disconnect();
			}
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus.
			Log.Error("GmsecException: " + e.ToString());
			return -1;
		}

		return 0;
	}


	private static void InitializeLogLevel(Config config)
	{
		LogLevel level = Log.FromString( config.GetValue("loglevel", "info") );
		Log.SetReportingLevel(level);
	}
}
