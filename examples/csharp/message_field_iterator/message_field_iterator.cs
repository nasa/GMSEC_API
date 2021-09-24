/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file message_field_iterator.cs
 *
 * This file contains a simple example demonstrating the use of the
 * MessageFieldIterator class to iterate over the Fields of a Message.
 *
 * This example program is intended to be run prior to executing any example
 * program which publishes a GMSEC message (e.g. The publish example).
 */

using GMSEC.API;
using System;

class FieldIterationCallback: ConnectionManagerCallback
{
	public override void OnMessage(ConnectionManager connection, Message message)
	{
		try
		{
			Log.Info("Received a message with subject: " + message.GetSubject());

			Log.Info("Field Name (Field Type): Field Value");
			MessageFieldIterator iter = message.GetFieldIterator();
			while (iter.HasNext())
			{
				Field field = iter.Next();

				//o Extract the Field Name, Type, and Value (As
				// a string, to print)
				//
				// Note: 'getter' functions are also defined for
				// Integer, Unsigned Integer, and Double values.
				Log.Info(field.GetName() + " (" + TypeToString(field.GetFieldType()) + "): " + field.GetStringValue());

				//o Field objects can also be converted to
				// specific Field types prior to retrieval of
				// the value contained in the Field.  This is
				// useful for ensuring that data types do not
				// lose any level of precision, but requires
				// a more intricate implementation.
			}
		}
		catch(GmsecException e)
		{
			Log.Error(e.ToString());
		}
	}

	public String TypeToString(Field.FieldType type)
	{
		String ret_string = "";

		switch (type)
		{
			case Field.FieldType.BOOL_TYPE:
			ret_string = "BOOL";
			break;

			case Field.FieldType.BIN_TYPE:
			ret_string = "BIN";
			break;

			case Field.FieldType.CHAR_TYPE:
			ret_string = "CHAR";
			break;

			case Field.FieldType.I8_TYPE:
			ret_string = "I8";
			break;

			case Field.FieldType.I16_TYPE:
			ret_string = "I16";
			break;

			case Field.FieldType.I32_TYPE:
			ret_string = "I32";
			break;

			case Field.FieldType.I64_TYPE:
			ret_string = "I64";
			break;

			case Field.FieldType.F32_TYPE:
			ret_string = "F32";
			break;

			case Field.FieldType.F64_TYPE:
			ret_string = "F64";
			break;

			case Field.FieldType.STRING_TYPE:
			ret_string = "STRING";
			break;

			case Field.FieldType.U8_TYPE:
			ret_string = "U8";
			break;

			case Field.FieldType.U16_TYPE:
			ret_string = "U16";
			break;

			case Field.FieldType.U32_TYPE:
			ret_string = "U32";
			break;

			case Field.FieldType.U64_TYPE:
			ret_string = "U64";
			break;

			default:
			String oss = "Unsupported Field TYPE: " + type;
			throw new GmsecException(StatusClass.FIELD_ERROR, StatusCode.UNKNOWN_FIELD_TYPE, oss, 0);
		}

		return ret_string;
	}
}

class message_field_iterator
{
	static private String DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.>";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage message_field_iterator.exe mw-id=<middleware ID>");
			return -1;
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config = new Config(args);

		// If it was not specified in the command-line arguments, set LOGLEVEL
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
			ConnectionManager connMgr = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connMgr.Initialize();

			Log.Info("Middleware version: " + connMgr.GetLibraryVersion());

			Log.Info("Subscribing to the topic: " + DEFAULT_SUBSCRIPTION_SUBJECT);
			FieldIterationCallback cb = new FieldIterationCallback();
			connMgr.Subscribe(DEFAULT_SUBSCRIPTION_SUBJECT, cb);

			//o Start the AutoDispatcher to begin asynchronously processing
			// messages
			connMgr.StartAutoDispatch();

			//o Wait for user input to end the program
			Log.Info("Listening for Messages indefinitely, press <enter> to exit the program");
			Console.ReadLine();

			//o Clean up
			connMgr.StopAutoDispatch();
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
}
