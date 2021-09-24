/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request_open_resp.java
 *
 * This file contains an example demonstrating how to issue a request message
 * and handle a coinciding reply message when using the open-response
 * configuration option. This example program is intended to be run after
 * starting up the 'reply_open_resp' example program.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
 * set to true in order to see how any subscriber can receive reply messages
 * while using the open-response functionality. Note that by setting the
 * configuration option 'GMSEC-REQ-RESP' to 'open-resp' in the requester
 * automatically turns on the 'mw-expose-resp' option.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import java.lang.Thread;

public class request_open_resp
{
	public static final String OPEN_RESP_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.RESP.REQUEST_OPENRESP";
	public static final String OPEN_RESP_REPLY_SUBJECT   = "GMSEC.MISSION.SAT_ID.RESP.REPLY_OPENRESP";
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java request_open_resp mw-id=<middleware ID>");
			System.exit(-1);
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which is used to
		// pass configuration options into objects such as Connections,
		// ConnectionManagers, Subscribe and Publish function calls, Messages,
		// etc.
		Config config =  new Config(args);

		//o Ensure that the open-response is enabled
	 	// Note: Other subscribing applications should set the configuration
	 	// option 'mw-expose-resp' to 'true' in order to receive exposed replies
	 	// By setting the configuration option 'GMSEC-REQ-RESP' to 'open-resp'
	 	// here, it automatically enables the 'mw-expose-resp' option.
		config.addValue("GMSEC-REQ-RESP", "OPEN-RESP");

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
 		initializeLogging(config);

		//o Output GMSEC API version
 		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Open the connection to the middleware
			connMgr.initialize();

			//o output connection middleware version
			Log.info(connMgr.getLibraryVersion());

			//o Subscribe to the bus in preparation to receive the
			// open-response message (Because it will not be routed
			// to the reqeust() call)
			String reply_subject = OPEN_RESP_REPLY_SUBJECT + ".*";
			connMgr.subscribe(reply_subject);

			//o Output information
			Log.info("Issuing a request using the subject '" + OPEN_RESP_REQUEST_SUBJECT + "''");

			//o Create message
			Message requestMsg = new Message(OPEN_RESP_REQUEST_SUBJECT, Message.MessageKind.REQUEST);

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
			Message replyMsg = connMgr.request(requestMsg, 1000, gmsecAPI.REQUEST_REPUBLISH_NEVER);

			// Example error handling for calling request() with a timeout
			if (replyMsg != null)
			{
				// Display the XML string representation of the reply
				Log.info("Received replyMsg:\n" + replyMsg.toXML());
			}

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
}
