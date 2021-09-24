/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request_async.java
 *
 * This file contains an example demonstrating how to asynchronously handle the
 * receipt of reply message(s) after issuing a request.
 * Asynchronous request operations essentially mean that a Callback subclass is
 * implemented, then a corresponding Callback object is supplied when invoking
 * request. Reply messages that are then received asynchronously and handled by
 * the Callback object.
 *
 * This example program is intended to be run after either starting up the
 * 'reply' or 'reply_async' example program.
 */
import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerReplyCallback;
import java.lang.Thread;

public class request_async
{
	public static final String DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";
	public static boolean receivedReply = false;
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java request_async mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.addValue("gmsec-msg-content-validate-all", "false");

		// If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		initializeLogging(config);

		//o Output GMSEC API version
		Log.info(Connection.getAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			connMgr.initialize();

			//o output connection middleware version
			Log.info(connMgr.getLibraryVersion());

			//o Create request message
			Message requestMsg = new Message(DEFAULT_REQUEST_SUBJECT, Message.MessageKind.REQUEST);

			//o Add fields to request message
			requestMsg.addField("QUESTION", "Does the request/reply functionality work?");
			requestMsg.addField("COMPONENT", "request_async");

			//o Display XML representation of request message
			Log.info("Requesting:\n" + requestMsg.toXML());

			connMgr.request(requestMsg, -1, new ConnectionManagerReplyCallback()
			{
				public void onReply(ConnectionManager connection, Message request, Message reply)
				{
					Log.info("[ExampleReplyCallback::onReply]\n" + reply.toXML());
					receivedReply = true;
				}
				public void onEvent(ConnectionManager connection, Status status, Connection.ConnectionEvent event)
				{
					Log.error("[ExampleReplyCallback::onError] " + status.get() + ", event=" + event);
				}
			}, -1);

			Log.info("Waiting for response...");

			// Loop while waiting for the asynchronous response until done
			while (!receivedReply)
			{
				Thread.sleep(100);
			}

			if (receivedReply)
			{
				Log.info("Response Received!");
			}
			else
			{
				Log.warning("No response received");
			}
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
