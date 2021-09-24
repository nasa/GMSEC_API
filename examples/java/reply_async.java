/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply_async.java
 *
 * This file contains an example demonstrating how to asynchronously reply to a
 * request message.
 *
 * Asynchronous receipt operations essentially mean that a Callback subclass is
 * implemented, then the corresponding Callback object is registered to a
 * subscription, and that messages are received asynchronously after starting
 * the AutoDispatcher. In this case, replies are created and sent asynchronously
 * when receiving request messages.
 *
 * This example program is intended to be run before starting up either the
 * 'request' or 'request_async' example program.
 */
import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback;
import java.lang.Thread;

public class reply_async
{
	public static final String DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";
	public static boolean replySent = false;
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java reply_async mw-id=<middleware ID>");
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

		//o Output GMSEC API version
		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Open the connection to the middleware
			Log.info("Opening the connection to the middleware server");

			//o Connect
			connMgr.initialize();

			//o output connection middleware version
			Log.info(connMgr.getLibraryVersion());

			//o Subscribe
			connMgr.subscribe(DEFAULT_REQUEST_SUBJECT, new ConnectionManagerCallback()
			{
				public void onMessage(ConnectionManager connMgr, Message message)
				{
					try
					{
						//o Display the XML representation of the received message
						Log.info("[ExampleCallback:onMessage] Received:\n" + message.toXML());

						//o Construct Reply subject.
						String component = "";

						try
						{
							component = message.getStringField("COMPONENT").getValue();
						}
						catch(Exception e)
						{
							Log.warning("COMPONENT field is not available: " + e.getMessage());
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

						//o Create the reply subject.
						// See API Interface Specificaiton Document for
						// more information on Reply Message subjects.
						// Generally speaking, they are composed
						// accordingly:
						// [Spec].[Mission].[Satellite ID].
						// [Message Type].[Component Name].[Status Code]
						String reply_subject = "GMSEC.MISSION.SAT_ID.RESP.REPLY_ASYNC." + status_code;

						Message reply = new Message(reply_subject, Message.MessageKind.REPLY);

						//o Add fields to the reply message
						reply.addField("COMPONENT", component);
						reply.addField("ANSWER", "Sure looks like it!");

						//o Display XML representation of reply message
						Log.info("Prepared Reply:\n" + reply.toXML());

						//o Send Reply
						connMgr.reply(message, reply);

						replySent = true;
					}
					catch (Exception e)
					{
						Log.error("[ExampleCallback::onMessage] " + e.getMessage());
					}
				}
			});

			//o Start the AutoDispatcher to begin async message receipt
			connMgr.startAutoDispatch();

			//o Loop while waiting for the asynchronous response until done
			while(!replySent)
			{
				Thread.sleep(100);
			}

			//o Wait for a few moments to ensure that the asynchronous reply
			// transaction went through with the middleware before closing
			// the connection and exiting the process
			for(int i = 0; i<5; i++)
			{
				Thread.sleep(100);
			}

			//o Clean up
			connMgr.stopAutoDispatch();
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
