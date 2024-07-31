/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file subscribe_subject_filter.cs
 * 
 * This file contains a simple example outlining how to subscribe
 * to and retrieve messages from the middleware bus while using subject
 * exclusion filtering to reduce the scope of a wildcarded subject subscription.
 *
 * This example program is intended to be run before starting the
 * 'publish_subject_filter' example program.
 */

using GMSEC.API5;
using System;
using System.Text;


public class subscribe_subject_filter
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		// Deduce subscription topics for the example program
		string ALL_MESSAGES_TOPIC;
		string HB_MESSAGES_TOPIC;
		int level = config.GetIntegerValue("gmsec-schema-level", (int) Specification.SchemaLevel.LEVEL_0);

		switch ((Specification.SchemaLevel) level)
		{
		case Specification.SchemaLevel.LEVEL_0:
			ALL_MESSAGES_TOPIC = "C2MS.>";
			HB_MESSAGES_TOPIC  = "C2MS.*.*.*.*.*.MSG.HB.+";
			break;

		case Specification.SchemaLevel.LEVEL_1:
			ALL_MESSAGES_TOPIC = "C2MS-PIPE.>";
			HB_MESSAGES_TOPIC  = "C2MS-PIPE.*.*.*.*.*.MSG.HB.+";
			break;

		case Specification.SchemaLevel.LEVEL_2:
		default:
			ALL_MESSAGES_TOPIC = "GMSEC.>";
			HB_MESSAGES_TOPIC  = "GMSEC.*.*.MSG.HB.+";
			break;
		}

		try
		{
			// Create connection instance.
			using (Connection conn = new Connection(config))
			{
				// Establish connection to the GMSEC Bus.
				conn.Connect();
		
				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Subscribe to receive all messages using the GMSEC specification
				Log.Info("Subscribe to: " + ALL_MESSAGES_TOPIC);
				conn.Subscribe(ALL_MESSAGES_TOPIC);

				// Add specific message topic to the Connection's exclusion filter
				Log.Info("Filtering out: " + HB_MESSAGES_TOPIC);
				conn.ExcludeSubject(HB_MESSAGES_TOPIC);


				// Receive first message (LOG) sent by publisher
				Log.Info("Waiting to receive a message...");
				Message rcvdMsg = conn.Receive();

				Log.Info("Received LOG message:\n" + rcvdMsg.ToXML());

				// Acknowledge processing the message
				rcvdMsg.Acknowledge();

				// Release resources associated with the received message
				Message.Destroy(rcvdMsg);


				// Although a second message (HB) is sent by the publisher, it will not
				// be delivered to this Connection object
				Log.Info("Waiting to receive other message(s)...");
				rcvdMsg = conn.Receive(5000);

				if (rcvdMsg == null)
				{
					Log.Info("As expected, a timeout occurred (i.e. no HB message received)");
				}
				else
				{
					Log.Warning("Received unexpected message:\n" + rcvdMsg.ToXML());

					// Acknowledge processing the message
					rcvdMsg.Acknowledge();

					// Release resources associated with the received message
					Message.Destroy(rcvdMsg);
				}

				// Disconnect from the GMSEC Bus.
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
