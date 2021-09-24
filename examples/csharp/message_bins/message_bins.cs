/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file message_bins.cs
 *
 * This file contains an example describing how to aggregate multiple messages
 * into a singular message sent to the middleware server.  This functionality
 * is useful for cutting down on the number of messages sent across the
 * middleware bus, if there is an application in the system which is publishing
 * an exorbitantly large number of messages.
 *
 * It is recommended that you execute the application GMSEC_API/bin/gmsub
 * prior to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but they will be unusable.
 */

using GMSEC.API;
using System;

class message_bins
{
	static private String EXAMPLE_MESSAGE_SUBJECT = "GMSEC.AGGREGATE.PUBLISH";
	static private String EXAMPLE_BIN_EXCLUDE_SUBJECT = "GMSEC.BIN-EXCLUDE.PUBLISH";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage message_bins.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		//o Enable Message Binning
		config.AddValue("GMSEC-USE-MSG-BINS", "true");

		//o Specify the number of messages to be aggregated prior to publishing
		// the aggregate message to the middleware server (This applies to all
		// of the messages which match the subject(s) provided in the
		// GMSEC-MSG-BIN-SUBJECT-N configuration parameters
		// Note: The aggregate message will be sent to the middleware server
		// immediately upon this many messages being published, regardless of
		// the value supplied for GMSEC-MSG-BIN-TIMEOUT.
		config.AddValue("GMSEC-MSG-BIN-SIZE", "10");

		//o Specify a timeout (in milliseconds) for the aggregate message to be
		// sent to the middleware server
		// Note: The aggregate message will be sent to the middleware server
		// after this period of time if the message bin does not fill up (per
		// the value provided for GMSEC-MSG-BIN-SIZE) prior to this timeout
		config.AddValue("GMSEC-MSG-BIN-TIMEOUT", "5000");

		//o Specify the subjects to aggreate messages for.
		// Note: Subscription wildcard syntax can be used here, and has been
		// supported since GMSEC API version 4.3.
		config.AddValue("GMSEC-MSG-BIN-SUBJECT-1", "GMSEC.*.PUBLISH");

		//o Specify any subjects that should be excluded from being aggregated
		// This is useful if a wildcard subscription is provided in one of the
		// GMSEC-MSG-BIN-SUBJECT-N parameters.
		config.AddValue("GMSEC-MSG-BIN-EXCLUDE-SUBJECT-1", EXAMPLE_BIN_EXCLUDE_SUBJECT);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.AddValue("gmsec-msg-content-validate-all", "false");

		InitializeLogging(config);

		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connMgr.Initialize();

			Log.Info("Middleware version: " + connMgr.GetLibraryVersion());

			//o Create a message
			using(Message message = new Message(EXAMPLE_MESSAGE_SUBJECT, Message.MessageKind.PUBLISH))
			{
				for (int i = 0; i < 5; i++)
				{
					PopulateMessage(message, i+1);

					//o Publish the message to the middleware bus
					connMgr.Publish(message);

					//o Display the XML string representation of the Message for
					// the sake of review
					Log.Info("Published message: " + message.ToXML());
				}
			}

			//o Create a message
			using(Message message = new Message(EXAMPLE_BIN_EXCLUDE_SUBJECT, Message.MessageKind.PUBLISH))
			{
				PopulateMessage(message, 1);

				//o Publish the message to the middleware bus
				// Note: When calling publish(), if a message does NOT match
				// one of the subjects to be aggregated, it will be immediately
				// published
				connMgr.Publish(message);

				//o Display the XML string representation of the Message for
				// the sake of review
				Log.Info("Published message: " + message.ToXML());
			}

			//o Disconnect from the middleware and clean up the Connection
			connMgr.Cleanup();
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

	static void PopulateMessage(Message message, int count)
	{
		// Note: Since C# is a strongly-typed language, it is possible to use
		// type casting to add Field objects to a Message without having to
		// first create a Field object, then add it to the message.
		// Note: If a field of the same name is added to an existing message,
		// the value passed when calling AddField will overwrite the existing
		// value.

		message.AddField(new CharField("CHAR-FIELD", 'c'));
		message.AddField(new BooleanField("BOOL-FIELD-TRUE", true));
		message.AddField(new BooleanField("BOOL-FIELD-FALSE", false));
		message.AddField(new I8Field("I8-FIELD", (sbyte) count));
		message.AddField(new I16Field("I16-FIELD", (short) count));
		message.AddField(new I32Field("I32-FIELD", (int) count));
		message.AddField(new I64Field("I64-FIELD", (long) count));
		message.AddField(new U8Field("U8-FIELD", (byte) count));
		message.AddField(new U16Field("U16-FIELD", (UInt16) count));
		message.AddField(new U32Field("U32-FIELD", (UInt32) count));
		message.AddField(new U64Field("U64-FIELD", (UInt64) count));
		message.AddField(new StringField("STRING-FIELD", "This is a test"));
		message.AddField(new F32Field("F32-FIELD", (float) (1 + 1.0 / count)));
		message.AddField(new F64Field("F64-FIELD", (double) (1 + 1.0 / count)));
		message.AddField(new BinaryField("BIN-FIELD", System.Text.Encoding.ASCII.GetBytes("JLMNOPQ")));
	}
}
