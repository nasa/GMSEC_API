/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file msg_field_container.java
 *
 *
 * This file contains an example demonstrating the creation of GMSEC Messages
 * using the two available container types for Field objects: Binary Tree Map
 * (STL map) or Hash Map (unordered map).
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;

public class msg_field_container
{
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java msg_field_container <config-file>");
			System.out.println("Please see the GMSEC_API/example/java directory for an example xml configuration file.");
			System.exit(-1);
		}

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		initializeLogging();

		try{
			//o Demonstration of setting the configuration options via
			// configuration file
			createMsgUsingConfigFile(args[0]);

			//o Demonstration of hard-coding the configuration options
			// using a Config object
			createMsgUsingConfigObject();
		}
		catch (Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}
	}

	public static void createMsgUsingConfigFile(String filename) throws GMSEC_Exception
	{
		//o Load and parse configuration file
		ConfigFile cfgFile = new ConfigFile();
		cfgFile.load(filename);

		//o Retrieve config from file
		Config cfg = cfgFile.lookupConfig("config");

		//o Loop through three different examples outlined in the configuration
		// file provided with this example
		for (int i = 0; i < 3; ++i)
		{
			//o Form message name (e.g. msg1) to reference.
			String msgName = "msg" + (i + 1);

			//o Parse the message from the configuration file and load it
			// into a GMSEC Message
			Message msg = cfgFile.lookupMessage(msgName);

			String description = "";
			switch(i+1){
				case 1: description = "Expecting TREE map (at size limit of 8 fields):"; break;
				case 2: description = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):"; break;
				case 3: description = "Expecting HASH map (size limit is ignored):"; break;
			}
			displayMessage(msg, description);
		}
	}

	public static void createMsgUsingConfigObject() throws GMSEC_Exception
	{
		Config cfg1 = new Config(), cfg2 = new Config(), cfg3 = new Config(), empty = new Config();


		cfg1.addValue("gmsec-msgfld-store-type", "tree");
		//o If the Message object is using a binary tree map to store fields,
		// the 'gmsec-msgfield-store-size' configuration option indicates how
		// many fields should be allowed to be stored before the Message
		// object converts to using a hash map (Default is 50).
		cfg1.addValue("gmsec-msgfld-store-size", "10");

		cfg2.addValue("gmsec-msgfld-store-type", "hash");

		cfg3.addValue("gmsec-msgfld-store-type", "tree");    // container type
		cfg3.addValue("gmsec-msgfld-store-size", "0");       // never rollover

		Message msg1 = buildMessage(cfg1,   9);   // uses bin-tree container map (under size limit)
		Message msg2 = buildMessage(cfg1,  10);   // uses bin-tree container map (at size limit)
		Message msg3 = buildMessage(cfg1,  11);   // uses hash container map (exceeds size limit)
		Message msg4 = buildMessage(cfg2,   5);   // default to using hash container map (size limit ignored)
		Message msg5 = buildMessage(cfg3,  55);   // always uses bin-tree container map
		Message msg6 = buildMessage(empty, 50);   // uses bin-tree container map (default size limit not exceeded)
		Message msg7 = buildMessage(empty, 51);   // uses hash container map (exceeds default size limit)

		displayMessage(msg1, "Message using TREE container map (under limit):");
		displayMessage(msg2, "Message using TREE container map (at max limit):");
		displayMessage(msg3, "Message was using TREE container map, but rolls over to HASH map (limit exceeded):");
		displayMessage(msg4, "Message using HASH container map (limit ignored):");
		displayMessage(msg5, "Message using TREE container map (limit ignored):");
		displayMessage(msg6, "Message using TREE container map (at limit):");
		displayMessage(msg7, "Message using HASH container map (limit exceeded):");
	}

	public static Message buildMessage(Config config, int numFields)
	{
		Message msg = new Message("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", Message.MessageKind.PUBLISH, config);

		if (numFields > 0)
		{
			// Add Fields
			for (int i = 0; i < numFields; ++i)
			{
				String oss = "FIELD-" + (i < 10 ? "00" : (i < 100 ? "0" : "")) + i;

				msg.addField(oss, new U16(i));
			}
		}

		return msg;
	}

	public static void displayMessage(Message msg, String description)
	{
		String xml = msg.toXML();
		Log.info(description + "\n" + xml);
	}

	public static void initializeLogging()
	{
		// Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
		// object, it affects the logging level and output destination for all
		// logging macros in the entire API, regardless of whether that Config
		// object is kept around or not. In this case, we are creating a local
		// Config object just for the sake of initializing logging.
		Config config = new Config();

		try
		{
			String logLevel = config.getValue("LOGLEVEL");
			String logFile = config.getValue("LOGFILE");

			if (logLevel == null)
			{
				config.addValue("LOGLEVEL", "INFO");
			}
			if (logFile == null)
			{
				config.addValue("LOGFILE", "STDOUT");
			}
		}
		catch(Exception e)
		{
			Log.error(e.toString());
		}
  }
}
