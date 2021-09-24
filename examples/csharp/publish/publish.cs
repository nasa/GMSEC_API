/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file publish.cs
 *
 * This file contains an example describing how to create and publish a
 * message to the middleware bus using the lowest level function calls
 * available for such operations.
 *
 * This example program is intended to be run after starting the 'subscribe'
 * or 'subscribe_async' example program.
 */

using GMSEC.API;
using System;
using System.Text;

class publish
{
	static private String EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage publish.exe mw-id=<middleware ID>");
			return -1;
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config = new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.AddValue("gmsec-msg-content-validate-all", "false");

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		InitializeLogging(config);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			//o Create a ConnectionManager object
			// This is the linchpin for all communications between the
			// GMSEC API and the middleware server
			using (ConnectionManager connMgr = new ConnectionManager(config))
			{
				//o Open the connection to the middleware
				Log.Info("Opening the connection to the middleware server");
				connMgr.Initialize();

				//o Output middleware client library version
				Log.Info("Middleware version: " + connMgr.GetLibraryVersion());

				//o Create a message
				// Disclaimer: This message is not based off of a Message
				// Definition outlined by the GMSEC Interface
				// Specification Document (ISD).  It is meant to be an example
				// to demonstrate the various capabilities of the GMSEC Message
				// and Field classes. The GMSEC Team recommends that you map
				// your data into the Messages defined in the GMSEC ISD, as
				// doing so will make your software "GMSEC Compliant" resulting
				// in plug-and-play type functionality with other GMSEC
				// compliant software.
				using(Message message = new Message(EXAMPLE_MESSAGE_SUBJECT, Message.MessageKind.PUBLISH))
				{
					PopulateMessage(message);

					//o Publish the message to the middleware bus
					connMgr.Publish(message);

					//o Display the XML string representation of the Message for
					// the sake of review
					Log.Info("Published message:\n" + message.ToXML());
				}

				//o Disconnect from the middleware and clean up the Connection
				connMgr.Cleanup();
			}
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

	static void PopulateMessage(Message message)
	{
		// Add Fields of varying data types to the Message
		int value = 1;

		// Note: Since C# is a strongly-typed language, it is possible to use
		// type casting to add Field objects to a Message without having to
		// first create a Field object, then add it to the message.
		// Note: If a field of the same name is added to an existing message,
		// the value passed when calling AddField will overwrite the existing
		// value.

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
