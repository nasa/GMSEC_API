/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file message_bins.cs
 *
 * @brief This file contains an example describing how to aggregate multiple
 * messages into a singular message sent to the middleware server. This
 * functionality is useful for reducing the number of messages sent across
 * the * middleware bus, if there is an application which is publishing an
 * exorbitantly large number of (small) messages.
 *
 * It is recommended to execute the application GMSEC_API/bin/gmsub prior
 * to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but they will be unusable.
 */

using GMSEC.API5;
using System;
using System.Text;


public class message_bins
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		// Deduce subject and excluded subject for message(s) that will be binned and for those that will not
		string msg_bin_subject;
		string msg_bin_exclude_subject;
		int level = config.GetIntegerValue("gmsec-schema-level", (int) Specification.SchemaLevel.LEVEL_0);

		switch ((Specification.SchemaLevel) level)
		{
		case Specification.SchemaLevel.LEVEL_0:
			msg_bin_subject         = "C2MS.*.*.*.*.*.MSG.LOG.*.AAA.>";
			msg_bin_exclude_subject = "C2MS.*.*.*.*.*.MSG.LOG.*.TLM.>";
			break;

		case Specification.SchemaLevel.LEVEL_1:
			msg_bin_subject         = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.AAA.>";
			msg_bin_exclude_subject = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.TLM.>";
			break;

		case Specification.SchemaLevel.LEVEL_2:
		default:
			msg_bin_subject         = "GMSEC.*.*.MSG.LOG.*.AAA.>";
			msg_bin_exclude_subject = "GMSEC.*.*.MSG.LOG.*.TLM.>";
			break;
		}

		// Validate messages before being sent to the bus
		config.AddValue("gmsec-msg-content-validate-send", "true");

		// Configure to enable message binning
		config.AddValue("gmsec-use-msg-bins", "true");

		// Specify the number of messages to be aggregated prior to publishing
		// the aggregate message to the middleware server. This applies to all
		// of the messages which match the subject(s) provided in the
		// GMSEC-MSG-BIN-SUBJECT-N configuration parameters
		// Note: The aggregate message will be sent to the middleware server
		// immediately upon this many messages being published, regardless of
		// the value supplied for GMSEC-MSG-BIN-TIMEOUT.
		config.AddValue("gmsec-msg-bin-size", "10");

		// Specify a timeout (in milliseconds) for the aggregate message to be
		// sent to the GMSEC Bus.
		// Note: The aggregate message will be sent to the GMSEC Bus after this
		// period of time if the message bin does not fill up (per the value
		// provided for gmsec-msg-bin-size) prior to this timeout
		config.AddValue("gmsec-msg-bin-timeout", "5000");

		// Specify the message subject(s) of messages that should be aggregated.
		// Subscription wildcard syntax can be used here.
		// Note: If this option is not specified, then ALL messages will be
		// aggregated (except those that are excluded).
		config.AddValue("gmsec-msg-bin-subject-1", msg_bin_subject);

		// Specify any message subject(s) that should be excluded from being
		// aggregated.
		// This is useful if a wildcard subscription is provided in one of the
		// gmsec-msg-bin-subject-N parameters.
		config.AddValue("gmsec-msg-bin-exclude-subject-1", msg_bin_exclude_subject);

		try
		{
			// Create connection instance.
			using (Connection conn = new Connection(config))
			{
				// Set up standard fields within the MessageFactory associated
				// with the connection object.
				SetStandardFields(conn.GetMessageFactory(), "MESSAGE-BINS");

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Create a Log messages using the MessageFactory.
				// Set both required and optional fields within the message.
				Message logMsg1 = conn.GetMessageFactory().CreateMessage("LOG");
				Message logMsg2 = conn.GetMessageFactory().CreateMessage("LOG");

				logMsg1.AddField("SUBCLASS", "AAA");
				logMsg1.AddField("OCCURRENCE-TYPE", "PLOT");
				logMsg1.AddField("SEVERITY", (short) 1);
				logMsg1.AddField("MSG-TEXT", "Message will be binned");

				logMsg2.AddField("SUBCLASS", "TLM");
				logMsg2.AddField("OCCURRENCE-TYPE", "DATA");
				logMsg2.AddField("SEVERITY", (short) 1);
				logMsg2.AddField("MSG-TEXT", "Message will NOT be binned");

				for (int i = 0; i < 5; ++i)
				{
					String theTime = TimeUtil.FormatTime( TimeUtil.GetCurrentTime() );

					logMsg1.AddField("EVENT-TIME", theTime);
					logMsg2.AddField("EVENT-TIME", theTime);

					// Publish the first message; it will be aggregated and published at a later time
					conn.Publish(logMsg1);
					Log.Info("Publishing AAA Log Message " + (i+1) + " (will be aggregated):\n" + logMsg1.ToXML());

					// Publish the second message; it will NOT be aggregated and will be published immediately
					conn.Publish(logMsg2);
					Log.Info("Publishing TLM Log Message " + (i+1) + " (will NOT be aggregated):\n" + logMsg2.ToXML());

					// Add processing delay
					TimeUtil.Millisleep(100);
				}

				// Delay to demonstrate message bin is flushed after a period of time (e.g. 5 seconds)
				Log.Info("Waiting for binned messages to be published...");
				TimeUtil.Millisleep(6000);

				// Disconnect from the GMSEC Bus, and terminate subscriptions.
				// Note: Any remaining messages in message bins will be published
				// before disconnecting from the GMSEC Bus, thus ensuring no message
				// loss occurs.
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


	// Convenience function for associating standard (i.e. common) fields
	// with the MessageFactory which will be included automatically within
	// all messages created using the factory.
	private static void SetStandardFields(MessageFactory factory, string component)
	{
		FieldList standardFields = new FieldList()
		{
			new StringField("MISSION-ID", "MY-MISSION", true),
			new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true),
			new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true),
			new StringField("CONSTELLATION-ID", "MY-CNST", true),
			new StringField("FACILITY", "MY-FACILITY", true),
			new StringField("DOMAIN1", "MY-DOMAIN-1", true),
			new StringField("DOMAIN2", "MY-DOMAIN-2", true),
			new StringField("COMPONENT", component, true)
		};

		factory.SetStandardFields(standardFields);
	}
}
