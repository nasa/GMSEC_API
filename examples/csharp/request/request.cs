/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file request.cs
 *
 * @brief This example program provides a basic demonstration of issuing
 * a synchronous Request Directive message to the GMSEC Bus, and then
 * handling the receipt of a Response Directive message.
 */

using GMSEC.API5;
using System;
using System.Text;


public class request
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		// Validate messages before being sent to and as they are received from the bus
		config.AddValue("gmsec-msg-content-validate-all", "true");

		try
		{
			// Create connection instance.
			using (Connection conn = new Connection(config))
			{
				// Set up standard fields within the MessageFactory associated
				// with the connection object.
				SetStandardFields(conn.GetMessageFactory(), "REQUEST");

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Create a Request Directive message using the MessageFactory.
				// Set both required and optional fields within the message.
				Message reqMsg = conn.GetMessageFactory().CreateMessage("REQ.DIR");
				reqMsg.AddField(new StringField("DESTINATION-COMPONENT", "REPLY", true));   // to whom is the request intended for
				reqMsg.AddField("DIRECTIVE-STRING", "Turn on the lights");                  // operation to perform
				reqMsg.AddField("RESPONSE", true);                                          // a response message is requested
				reqMsg.AddField("REQUEST-ID", (ushort) 0);

				// Output in XML what we intend to send
				Log.Info("Sending Request Message:\n" + reqMsg.ToXML());

				// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
				// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
				// prevent re-sending request message should a timeout occur.
				int timeout   = 5000;
				int republish = 5000;

				// Issue Request Directive.
				// Note, this is a blocking call (i.e. a synchronous operation).
				Message rspMsg = conn.Request(reqMsg, timeout, republish);

				// We have returned from the previous blocking call; check if we have a message.
				if (null != rspMsg)
				{
					Log.Info("Received Response Directive message:\n" + rspMsg.ToXML());

					// Acknowledge processing the response message
					rspMsg.Acknowledge();

					// Release resources associated with the received message
					Message.Destroy(rspMsg);
				}
				else
				{
					// This else-block is reached if a response message is not received.
					// If the republish period is set to a time period other than
					// GMSEC_REQUEST_REPUBLISH_NEVER, then this block will never be reached.
					Log.Warning("Time out; no Response Directive message received");
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
