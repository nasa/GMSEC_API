/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file custom_validation.cs
 *
 * @brief This example program demonstrates how to implement additional
 * message validation logic in addition to that which the GMSEC API
 * provides.
 */

using GMSEC.API5;
using System;
using System.Text;


// Define a custom message validator
class CustomMessageValidator : MessageValidator
{
	public override Status ValidateMessage(Message msg)
	{
		Status status = new Status();

		// Check if we have a Heartbeat message, and if so, then check if
		// the PUB-RATE has an acceptable value.
		try
		{
			if (msg.GetStringValue("MESSAGE-SUBTYPE") == "HB")
			{
				if (msg.HasField("PUB-RATE"))
				{
					int pubRate = msg.GetI32Value("PUB-RATE");

					if (pubRate < 10 || pubRate > 60)
					{
						status.Set((int) StatusClass.MSG_ERROR, (int) StatusCode.VALUE_OUT_OF_RANGE,
						           "PUB-RATE field does not have a valid value");
					}
				}
				else
				{
					status.Set((int) StatusClass.MSG_ERROR, (int) StatusCode.MISSING_REQUIRED_FIELD,
					           "PUB-RATE field not found");
				}
			}
		}
		catch (GmsecException e)
		{
			status = new Status(e);
		}

		return status;
	}
}


public class custom_validation
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		// Validate messages before being sent to the bus
		config.AddValue("gmsec-msg-content-validate-send", "true");

		try
		{
			// Create connection instance.
			using (Connection conn = new Connection(config))
			{
				// Set up standard fields within the MessageFactory associated
				// with the connection object.
				SetStandardFields(conn.GetMessageFactory(), "CUSTOM-VALIDATION");

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Register custom message validator
				CustomMessageValidator cmv = new CustomMessageValidator();
				conn.GetMessageFactory().RegisterMessageValidator(cmv);

				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Create a Heartbeat message using the MessageFactory.
				// Set both required and optional fields within the message.
				Message hbMsg = conn.GetMessageFactory().CreateMessage("HB");

				// Publish the message without PUB-RATE (expecting custom validation error)
				try {
					Log.Info("Attempting to publish message with no PUB-RATE field:\n" + hbMsg.ToXML());
					conn.Publish(hbMsg);

					Log.Warning("An error was expected!");
				}
				catch (GmsecException e) {
					Log.Info("This error is expected: " + e.ToString());
				}

				// Publish the message with invalid PUB-RATE value (expecting custom validation error)
				try {
					hbMsg.SetFieldValue("PUB-RATE", 5);

					Log.Info("Attempting to publish message with invalid PUB-RATE value:\n" + hbMsg.ToXML());
					conn.Publish(hbMsg);

					Log.Warning("An error was expected!");
				}
				catch (GmsecException e) {
					Log.Info("This error is expected: " + e.ToString());
				}

				// Publish the message with valid PUB-RATE value (expecting no validation error)
				try {
					hbMsg.SetFieldValue("PUB-RATE", 10);

					Log.Info("Attempting to publish message with valid PUB-RATE:\n" + hbMsg.ToXML());
					conn.Publish(hbMsg);

					Log.Info("Message is valid!");
				}
				catch (GmsecException e) {
					Log.Error("Unexpected error: " + e.ToString());
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
