/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

 /**
 * @file publish.java
 *
 * This file contains an example describing how to create and publish a
 * message to the middleware bus using the lowest level function calls
 * available for such operations.
 *
 * This example program is intended to be run after starting the 'subscribe'
 * or 'subscribe_async' example program.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.util.LogHandler;
import gov.nasa.gsfc.gmsec.api.util.LogEntry;
import gov.nasa.gsfc.gmsec.api.util.TimeUtil;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import java.math.BigInteger;

public class publish
{
	public static final String EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH";
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java publish mw-id=<middleware ID>");
			System.exit(-1);
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config =  new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.addValue("gmsec-msg-content-validate-all", "false");

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		initializeLogging(config);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			//o Create a ConnectionManager object
			// This is the linchpin for all communications between the
			// GMSEC API and the middleware server
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Open the connection to the middleware
			Log.info("Opening the connection to the middleware server");
			connMgr.initialize();

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
			Message message = new Message(EXAMPLE_MESSAGE_SUBJECT, Message.MessageKind.PUBLISH);
			populateMessage(message);

			//o Publish the message to the middleware bus
			connMgr.publish(message);

			//o Display the XML string representation of the Message for
			// the sake of review
			Log.info("Published message: " + message.toXML());

			//o Disconnect from the middleware and clean up the Connection
			connMgr.cleanup();
		}
		catch (Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}

		return;
	}

	public static void initializeLogging(Config config)
	{
		// If it was not specified in the command-line arguments, set
		// LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the
		// program report output on the terminal/command line
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

	public static void populateMessage(Message message)
	{
		// Add Fields of varying data types to the Message
		int value = 1;

		message.addField("CHAR-FIELD", 'c');
		message.addField("BOOL-FIELD-TRUE", true);
		message.addField("BOOL-FIELD-FALSE", false);
		message.addField("I8-FIELD", (byte) value);
		message.addField("I16-FIELD", (short) value);
		message.addField("I32-FIELD", (int) value);
		message.addField("I64-FIELD", (long) value);
		message.addField("U8-FIELD", new U8(value));
		message.addField("U16-FIELD", new U16(value));
		message.addField("U32-FIELD", new U32(value));
		message.addField("U64-FIELD", new U64(BigInteger.valueOf(value)));
		message.addField("STRING-FIELD", "This is a test");
		message.addField("F32-FIELD", (float) (1 + 1. / value));
		message.addField("F64-FIELD", (double) (1 + 1. / value));
		message.addField("BIN-FIELD", "JLMNOPQ".getBytes());
	}
}
