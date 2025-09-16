/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file request_async.cs
 *
 * @brief This example program provides a basic demonstration of issuing
 * an asynchronous Request Directive message to the GMSEC Bus, and then
 * handling the receipt of a Response Directive message(s) via a
 * ReplyCallback.
 */

using GMSEC.API5;
using System;
using System.Text;


class MyReplyCallback : ReplyCallback
{
	private bool responseComplete;

	public MyReplyCallback()
	{
		responseComplete = false;
	}

	public override void OnReply(Connection conn, Message reqMsg, Message repMsg)
	{
		Log.Info("MyReplyCallback OnReply:\n" + repMsg.ToXML());

		if (repMsg.HasField("RESPONSE-STATUS"))
		{
			try {
				int status = repMsg.GetI32Value("RESPONSE-STATUS");

				switch ((Message.ResponseStatus) status)
				{
				case Message.ResponseStatus.ACKNOWLEDGEMENT:
				case Message.ResponseStatus.WORKING_KEEP_ALIVE:
					responseComplete = false;
					break;

				case Message.ResponseStatus.SUCCESSFUL_COMPLETION:
				case Message.ResponseStatus.FAILED_COMPLETION:
				case Message.ResponseStatus.INVALID_REQUEST:
				case Message.ResponseStatus.FINAL_MESSAGE:
					responseComplete = true;
					break;
				}
			}
			catch (GmsecException e) {
				Log.Error("Unexpected error: " + e.ToString());
			}
		}
		else
		{
			responseComplete = true;
		}

		// Acknowledge processing the response message
		repMsg.Acknowledge();
	}

	public override void OnEvent(Connection conn, Status status, Connection.Event theEvent)
	{
		if (status.HasError())
		{
			Log.Error("MyReplyCallback OnEvent: " + status.Get() + ", event: " + theEvent);
		}
		else
		{
			Log.Info("MyReplyCallback OnEvent: " + status.Get() + ", event: " + theEvent);
		}
	}

	public bool IsResponseComplete()
	{
		return responseComplete;
	}
}


public class request_async
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

		// IMPORTANT! This config option must be set to receive multiple response messages
		config.AddValue("mw-multi-resp", "true");

		// Define instance of MyReplyCallback
		MyReplyCallback rcb = new MyReplyCallback();

		try
		{
			// Create connection instance.
			using (Connection conn = new Connection(config))
			{
				// Set up standard fields within the MessageFactory associated
				// with the connection object.
				SetStandardFields(conn.GetMessageFactory(), "REQUEST-ASYNC");

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Create a Request Directive message using the MessageFactory.
				// Set both required and optional fields within the message.
				Message reqMsg = conn.GetMessageFactory().CreateMessage("REQ.DIR");
				reqMsg.AddField(new StringField("DESTINATION-COMPONENT", "REPLY-MULTI", true));   // to whom is the request intended for
				reqMsg.AddField("DIRECTIVE-STRING", "Turn on the lights");                        // operation to perform
				reqMsg.AddField("RESPONSE", true);                                                // a response message is requested
				reqMsg.AddField("REQUEST-ID", (ushort) 0);

				// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
				// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
				// prevent re-sending request message should a timeout occur.
				int timeout   = 5000;
				int republish = 5000;

				// Issue Request Directive.
				// Note, this is a non-blocking call (i.e. an asynchronous operation).
				conn.Request(reqMsg, timeout, rcb, republish);

				// Output in XML what we have sent
				Log.Info("Sending Request Directive Message:\n" + reqMsg.ToXML());
				Log.Info("Use reply_multi example program to issue response messages");

				// Normally an application would continue with other operation, however for
				// demonstration purposes, we will loop until it has been verified that a
				// response message has been received.
				while (!rcb.IsResponseComplete())
				{
					Log.Info("Waiting to receive response message...");

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
