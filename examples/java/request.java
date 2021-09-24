/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request.java
 *
 * This file contains an example demonstrating how to issue a request message
 * and handle a coinciding reply message. This example program is intended to
 * be run after starting up either the 'reply' or 'reply_async' example program.
 */

 import gov.nasa.gsfc.gmsec.api.*;
 import gov.nasa.gsfc.gmsec.api.util.Log;
 import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;

public class request
{
	public static final String DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java request mw-id=<middleware ID>");
			System.exit(-1);
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config = new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.addValue("gmsec-msg-content-validate-all", "false");

		// If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		initializeLogging(config);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
		Log.info(Connection.getAPIVersion());

		try
		{
			//o Create the ConnectionManager
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Open the connection to the middleware
			connMgr.initialize();

			//o Output information
			Log.info(connMgr.getLibraryVersion());

			//o Output information
			Log.info("Issuing a request using the DEFAULT_REQUEST_SUBJECT '" + DEFAULT_REQUEST_SUBJECT + "'");

			//o Create message
			Message requestMsg = new Message(DEFAULT_REQUEST_SUBJECT, Message.MessageKind.REQUEST);

			//o Add fields to message
			requestMsg.addField("QUESTION", "Is there anyone out there?");
			requestMsg.addField("COMPONENT", "request");

			//o Display XML representation of request message
			Log.info("Sending request message:\n" + requestMsg.toXML());

			//o Send Request Message
			// Timeout periods:
			// -1 - Wait forever
			//  0 - Return immediately
			// >0 - Time in milliseconds before timing out
			Message replyMsg = connMgr.request(requestMsg, -1, -1);

			// Example error handling for calling request() with a timeout
			if (replyMsg != null)
			{
				// Display the XML string representation of the reply
				Log.info("Received replyMsg:\n" + replyMsg.toXML());
			}

			//o Disconnect from the middleware and clean up the Connection
			connMgr.cleanup();
		}
		catch(Exception e)
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
}
