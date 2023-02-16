/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe.cs
 *
 * @brief This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus.
 */

using GMSEC.API5;
using System;
using System.Text;


public class subscribe
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		// Validate messages as they are received from the bus
		config.AddValue("gmsec-msg-content-validate-recv", "true");

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

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Subscribe to receive message
				conn.Subscribe(subscription_topic);
				Log.Info("Subscribed to topic: " + subscription_topic);

				// Define timeout period, or use Gmsec.WAIT_FOREVER to block
				// indefinitely to receive a message.
				int timeout = 5000;   // 5 seconds

				// Attempt to receive a message
				Message msg = conn.Receive(timeout);

				if (null != msg)
				{
					// Received a message!
					Log.Info("Received Message:\n" + msg.ToXML());

					// Acknowledge processing the message
					msg.Acknowledge();

					// Release resources associated with the received message
					Message.Destroy(msg);
				}
				else
				{
					// Time out!
					Log.Warning("Time out; no message was received");
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
