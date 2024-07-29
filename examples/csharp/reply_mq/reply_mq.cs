/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file reply_mq.cs
 *
 * @brief This example program provides a basic demonstration of synchronously
 * receiving a Request Directive message from the GMSEC Bus using an Apache
 * Artemis Message Queue, and then issuing a Response Directive message (should
 * one be requested).
 *
 * This example is intended to be used with the request example program.
 */

using GMSEC.API5;
using System;
using System.Text;


public class reply
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

		// Check user has specified an mw-id of artemis
		string mw = config.GetValue("mw-id", "");
		if (mw != "artemis")
		{
			Log.Error("This example program only works with Apache Artemis; use mw-id=artemis and run again");
			return -1;
		}

		// Define subscription topic for receiving request message(s).
		// The topic includes the syntax for a Fully Qualified Queue Name (FQQN).
		string subscription_topic;
		int level = config.GetIntegerValue("gmsec-schema-level", (int) Specification.SchemaLevel.LEVEL_0);

		switch ((Specification.SchemaLevel) level)
		{
		case Specification.SchemaLevel.LEVEL_0:
			subscription_topic = "C2MS.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY::MY-MSG-QUEUE";
			break;

		case Specification.SchemaLevel.LEVEL_1:
			subscription_topic = "C2MS-PIPE.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY::MY-MSG-QUEUE";
			break;

		case Specification.SchemaLevel.LEVEL_2:
		default:
			subscription_topic = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.REPLY::MY-MSG-QUEUE";
			break;
		}

		try
		{
			// Create connection instance
			using (Connection conn = new Connection(config))
			{
				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Set up standard fields within the MessageFactory associated
				// with the connection object.
				SetStandardFields(conn.GetMessageFactory(), "REPLY");

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Set up our subscription for receiving a request message.
				conn.Subscribe(subscription_topic);
				Log.Info("Subscribed to topic: " + subscription_topic);

				// Timeout was arbitrarily chosen as Gmsec.GMSEC_WAIT_FOREVER so that
				// we block indefinitely waiting to receive a request message.
				// It can be set to other time values (e.g. 1000 is 1 second) so
				// the application does not block indefinitely waiting for a
				// request message.
				int timeout = Gmsec.GMSEC_WAIT_FOREVER;

				Log.Info("Responder is waiting to receive message(s)...");
				Log.Info("Consider running a second instance of the example program");
				Log.Info("Use the 'request' example program to send a message");

				// Block for a Request Directive message to arrive.
				Message reqMsg = conn.Receive(timeout);

				// Check if we have received a message.
				if (null != reqMsg)
				{
					// Output in XML the received message.
					Log.Info("Received Request Message:\n" + reqMsg.ToXML());

					// Check if a response message should be returned.
					if (reqMsg.HasField("RESPONSE") && reqMsg.GetBooleanValue("RESPONSE"))
					{
						// Create a Response Directive message using the MessageFactory.
						Message rspMsg = conn.GetMessageFactory().CreateMessage("RESP.DIR");

						// Insert required and optional fields into the Response Directive
						// using values derived from the incoming Request Directive.
						if (reqMsg.HasField("COMPONENT"))
						{
							rspMsg.AddField(new StringField("DESTINATION-COMPONENT", reqMsg.GetStringValue("COMPONENT"), true));
						}
						if (reqMsg.HasField("REQUEST-ID"))
						{
							rspMsg.AddField(reqMsg.GetField("REQUEST-ID"));
						}

						// Set the response status for the request.
						rspMsg.SetFieldValue("RESPONSE-STATUS", (int) Message.ResponseStatus.SUCCESSFUL_COMPLETION);

						// Send the Response Directive message.
						conn.Reply(reqMsg, rspMsg);

						// Output in XML the message that has been sent.
						Log.Info("Sent Response Message:\n" + rspMsg.ToXML());
					}

					// Acknowledge processing the request message so that it is not delivered to other clients.
					reqMsg.Acknowledge();

					// Release resources associated with the received message.
					Message.Destroy(reqMsg);
				}
				else
				{
					// This else-block is reached if a request message is not received.
					// When the timeout period is set to Gmsec.WAIT_FOREVER, then this
					// block would never be reached.
					Log.Warning("Time out; no Request Directive message received");
				}

				// Disconnect from the GMSEC Bus, and terminate subscriptions.
				conn.Disconnect();
			}
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect toi
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
