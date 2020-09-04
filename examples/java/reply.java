/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply.java
 *
 * This file contains an example demonstrating how to issue a reply to a
 * request message.
 *
 * This example program is intended to be run before starting up the
 * 'request' or 'request_async' example program.
 */

 import gov.nasa.gsfc.gmsec.api.*;
 import gov.nasa.gsfc.gmsec.api.util.Log;
 import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
 import gov.nasa.gsfc.gmsec.api.field.StringField;

public class reply
{
	public static final String DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java reply mw-id=<middleware ID>");
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

		//o Output GMSEC API version
		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Open the connection to the middleware
			Log.info("Opening the connection to the middleware server");
			connMgr.initialize();

			//o Output middleware client/wrapper version
			Log.info(connMgr.getLibraryVersion());

			connMgr.subscribe(DEFAULT_REQUEST_SUBJECT);

			//o Call receive() to synchronously retrieve a message that has
			// been received by the middleware client libraries
			// Timeout periods:
			// -1 - Wait forever
			//  0 - Return immediately
			// >0 - Time in milliseconds before timing out
			Message requestMsg = connMgr.receive(-1);

			// Example error handling for calling receive() with a timeout
			if (requestMsg != null)
			{
				//o Display the XML representation of the received message
				Log.info("Received a message\n" + requestMsg.toXML());

				//o Double-check the Message type to ensure that it is a request
				if (requestMsg.getKind() == Message.MessageKind.REQUEST)
				{
					//o Get the name of the component who issued the request
					String component = "";

					//o Construct a Reply message
					try
					{
						StringField compFeild = requestMsg.getStringField("COMPONENT");
						component = compFeild.getValue();
					}
					catch(Exception e)
					{
						Log.warning(e.getMessage());
					}

					//o Set Status Code to indicate Successful Completion.
					// The GMSEC Interface Specification Document defines 6
					// unique STATUS-CODE values:
					// 1 - Acknowledgement
					// 2 - Working/keep alive
					// 3 - Successful completion
					// 4 - Failed completion
					// 5 - Invalid request
					// 6 - Final message
					// If an asynchronous requestor is awaiting a reply, the
					// ReplyCallback in use will remain active for multiple
					// messages, so long as the messages it receives contain
					// a STATUS-CODE of either 1 or 2.
					String status_code = "3";

					//o Set the reply subject.
					// See API Interface Specificaiton Document for
					// more information on Reply Message subjects.
					// Generally speaking, they are composed
					// accordingly:
					// [Spec].[Mission].[Satellite ID].
					// [Message Type].[Component Name].[Status Code]
					String reply_subject = "GMSEC.MISSION.SAT_ID.RESP.REPLY." + status_code;

					//o Create reply message
					Message replyMsg = new Message(reply_subject, Message.MessageKind.REPLY);

					//o Add fields to the reply message
					replyMsg.addField("ANSWER", "Yup, I'm here!");
					replyMsg.addField("COMPONENT", component);

					//o Display XML representation of the reply message
					Log.info("Prepared Reply:\n" + replyMsg.toXML());

					//o Send Reply
					connMgr.reply(requestMsg, replyMsg);
				}
			}
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
