/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file msg_field_container.cs
 *
 *
 * This file contains an example demonstrating the creation of GMSEC Messages
 * using the two available container types for Field objects: Binary Tree Map
 * (STL map) or Hash Map (unordered map).
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;

class connection_state
{
	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage msg_field_container.exe <config-file>");
			Console.WriteLine("\nPlease see the GMSEC_API/example/cs/message_field_container directory for an example xml configuration file.");
			return -1;
		}

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		InitializeLogging();

		try
		{
			//o Demonstration of setting the configuration options via
			// configuration file
			CreateMsgUsingConfigFile(args[0]);

			//o Demonstration of hard-coding the configuration options
			// using a Config object
			CreateMsgUsingConfigObject();
		}
		catch (Exception e)
		{
			Log.Error(e.ToString());
			return -1;
		}

		return 0;
	}

	static void CreateMsgUsingConfigFile(String filename)
	{
		//o Load and parse configuration file
		ConfigFile cfgFile = new ConfigFile();
		cfgFile.Load(filename);

		//o Retrieve config from file
		Config cfg = cfgFile.LookupConfig("config");

		//o Loop through three different examples outlined in the configuration
		// file provided with this example
		for (int i = 0; i < 3; ++i)
		{
			//o Form message name (e.g. msg1) to reference.
			String msgName = "msg" + (i+1);

			//o Parse the message from the configuration file and load it
			// into a GMSEC Message
			Message msg = cfgFile.LookupMessage(msgName);

			//o Display the message
			String description = "";
			switch (i+1)
			{
				case 1: description = "Expecting TREE map (at size limit of 8 fields):"; break;
				case 2: description = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):"; break;
				case 3: description = "Expecting HASH map (size limit is ignored):"; break;
			}

			DisplayMessage(msg, description);
		}
	}

	static void CreateMsgUsingConfigObject()
	{
		Config cfg1 = new Config(), cfg2 = new Config(), cfg3 = new Config(), empty = new Config();

		cfg1.AddValue("gmsec-msgfld-store-type", "tree");
		//o If the Message object is using a binary tree map to store fields,
		// the 'gmsec-msgfield-store-size' configuration option indicates how
		// many fields should be allowed to be stored before the Message
		// object converts to using a hash map (Default is 50).
		cfg1.AddValue("gmsec-msgfld-store-size", "10");

		cfg2.AddValue("gmsec-msgfld-store-type", "hash");

		cfg3.AddValue("gmsec-msgfld-store-type", "tree");    // container type
		cfg3.AddValue("gmsec-msgfld-store-size", "0");       // never rollover

		Message msg1 = BuildMessage(cfg1,   9);   // uses bin-tree container map (under size limit)
		Message msg2 = BuildMessage(cfg1,  10);   // uses bin-tree container map (at size limit)
		Message msg3 = BuildMessage(cfg1,  11);   // uses hash container map (exceeds size limit)
		Message msg4 = BuildMessage(cfg2,   5);   // default to using hash container map (size limit ignored)
		Message msg5 = BuildMessage(cfg3,  55);   // always uses bin-tree container map
		Message msg6 = BuildMessage(empty, 50);   // uses bin-tree container map (default size limit not exceeded)
		Message msg7 = BuildMessage(empty, 51);   // uses hash container map (exceeds default size limit)

		DisplayMessage(msg1, "Message using TREE container map (under limit):");
		DisplayMessage(msg2, "Message using TREE container map (at max limit):");
		DisplayMessage(msg3, "Message was using TREE container map, but rolls over to HASH map (limit exceeded):");
		DisplayMessage(msg4, "Message using HASH container map (limit ignored):");
		DisplayMessage(msg5, "Message using TREE container map (limit ignored):");
		DisplayMessage(msg6, "Message using TREE container map (at limit):");
		DisplayMessage(msg7, "Message using HASH container map (limit exceeded):");
	}

	static Message BuildMessage(Config config, int numFields)
	{
		Message msg = new Message("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", Message.MessageKind.PUBLISH, config);

		if (numFields > 0)
		{
			// Add Fields
			for (int i = 0; i < numFields; ++i)
			{
				String oss = "FIELD-" + (i < 10 ? "00" : (i < 100 ? "0" : "")) + i;

				msg.AddField(new U64Field(oss, (UInt64) i));
			}
		}

		return msg;
	}

	static void DisplayMessage(Message msg, String description)
	{
		String xml = msg.ToXML();
		Log.Info(description + "\n" + xml);
	}

	static void InitializeLogging()
	{
		// Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
		// object, it affects the logging level and output destination for all
		// logging macros in the entire API, regardless of whether that Config
		// object is kept around or not. In this case, we are creating a local
		// Config object just for the sake of initializing logging.
		Config config = new Config();

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
