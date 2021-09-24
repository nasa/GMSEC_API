/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file websphere_publish_async.cs
 *
 * This file contains an example which demonstrates how to utilize the
 * WebSphere MQ asynchronous publish functionality, as well as how to check on
 * the status of publish operations (Since the client does not wait to receive
 * the status of each operation in order to achieve asynchronicity) by
 * registering an EventCallback with the ConnectionManager.
 *
 * It is recommended that users execute the program GMSEC_API/bin/gmsub to
 * receive the messages that this example publishes.
 */

using GMSEC.API;
using System;
using System.Collections.Generic;

//o Define an EventCallback to report the status of WebSphere async publications
// Note: The WebSphere MQ client libraries only report the first warning or
// failure code encountered during the series of message publications made
// since the last time that the client libraries retrieved the status of
// asynchronous publish operations.
class AsyncStatusCheckCallback: ConnectionManagerEventCallback
{
	public bool EventFired = false;
	public override void OnEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent connEvent)
	{
		// Print the status of publish operations.  This includes counts
		// for successes, warnings, and errors.
		Log.Info(status.GetReason());

		if (status.IsError())
		{
			Log.Error("The first occurrence of a WebSphere MQ Asynchronous Put Response warning or failure returned the WebSphere Reason Code: " + status.GetCustomCode());
		}

		EventFired = true;
	}
}

class websphere_publish_async
{
	static private String EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage websphere_publish_async.exe mw-id=<middleware ID>");
			return -1;
		}

		//o Load the command-line input into a GMSEC Config object
		Config config = new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.AddValue("gmsec-msg-content-validate-all", "false");

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		InitializeLogging(config);

		//o Toggle the WebSphere MQ client libraries-level asynchronous publish
		// functionality on
		config.AddValue("MW-ASYNC-PUBLISH", "true");

		//o Toggle the periodic WebSphere MQ asynchronous publish status report
		// functionality on.  The GMSEC API Connection will periodically fire
		// off an Event which will trigger the onEvent function of an
		// EventCallback registered to the event labeled,
		// Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT
		config.AddValue("MW-ASYNC-STATUS-CHECK", "true");

		//o Additionally, the "MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL"
		// configuration option can be used to instruct the GMSEC Connection
		// on how frequently (in number of publish operations) it should
		// periodically fire the WSMQ_ASYNC_STATUS_CHECK_EVENT. By default, it
		// will fire once every 100 messages.
		// For the sake of this example, we will use 500 messages.
		config.AddValue("MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL", "500");

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			connMgr.Initialize();

			//o Register the event callback with the connection to catch
			// the WebSphere asynchronous publish status events which are
			// eminated from the API
			AsyncStatusCheckCallback cb = new AsyncStatusCheckCallback();
			connMgr.RegisterEventCallback(Connection.ConnectionEvent.WSMQ_ASYNC_STATUS_CHECK_EVENT, cb);

			//o Output middleware version
			Log.Info("Middleware version: " + connMgr.GetLibraryVersion());

			Log.Info("Publishing messages using the subject: " + EXAMPLE_MESSAGE_SUBJECT);

			//o Create a GMSEC Message object
			Message message = new Message(EXAMPLE_MESSAGE_SUBJECT, Message.MessageKind.PUBLISH);

			int count = 0;
			while (!cb.EventFired)
			{
				//o Populate the Message with fields, increment a
				// counter so that a publisher can track how many
				// messages were published (if they are interested)
				PopulateMessage(message, count++);

				//o Publish the message to the middleware bus
				connMgr.Publish(message);

				// Note: We do not recommend printing the output of a
				// message when publishing it while using the WebSphere
				// async publish functionality, as it is
				// counter-intuitive to take take up CPU resources
				// performing I/O operations when attempting to achieve
				// high message throughput rates. As such, if you want
				// to analyze the messages published by this program,
				// we recommend you execute GMSEC_API/bin/gmsub to
				// receive the messages.
			}

			Log.Info("Event detected, ending example demonstration");

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

	static void PopulateMessage(Message message, int value)
	{
		// Note: Since C# is a strongly-typed language, it is possible to use
		// type casting to add Field objects to a Message without having to
		// first create a Field object, then add it to the message.

		message.AddField(new CharField("CHAR-FIELD", 'c'));
		message.AddField(new BooleanField("BOOL-FIELD-TRUE", true));
		message.AddField(new BooleanField("BOOL-FIELD-FALSE", false));
		message.AddField(new I8Field("I8-FIELD", (sbyte) value));
		message.AddField(new I16Field("I16-FIELD", (short) value));
		message.AddField(new I32Field("I32-FIELD", (int) value));
		message.AddField(new I64Field("I64-FIELD", (long) value));
		message.AddField(new U8Field("U8-FIELD", (byte) value));
		message.AddField(new U16Field("U16-FIELD", (UInt16) value));
		message.AddField(new U32Field("U32-FIELD", (UInt32) value));
		message.AddField(new U64Field("U64-FIELD", (UInt64) value));
		message.AddField(new StringField("STRING-FIELD", "This is a test"));
		message.AddField(new F32Field("F32-FIELD", (float) (1 + 1.0 / value)));
		message.AddField(new F64Field("F64-FIELD", (double) (1 + 1.0 / value)));
		message.AddField(new BinaryField("BIN-FIELD", System.Text.Encoding.ASCII.GetBytes("JLMNOPQ")));
	}
}
