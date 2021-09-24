/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe_async.java
 *
 * This file contains a simple example outlining how to asynchronously subscribe
 * to and retrieve messages from the middleware bus. Asynchronous receipt
 * operations essentially mean that a Callback subclass is implemented, that the
 * corresponding Callback object is registered to a subscription, and that
 * messages are received asynchronously after starting the AutoDispatcher.
 *
 * This example program is intended to be run before starting the 'publish'
 * example program.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback;
import java.lang.Thread;

public class subscribe_async
{
	public static final String DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.TEST.PUBLISH";
	public static boolean receivedMessage = false;

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java subscribe_async mw-id=<middleware ID>");
			System.exit(-1);
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which
		// is used to pass configuration options into objects such
		// as Connection objects, ConnectionManager objects, Subscribe
		// and Publish calls, Message objects, etc.
		Config config = new Config(args);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
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

			//o Output middleware client library version
			Log.info(connMgr.getLibraryVersion());

			//o Set up a subscription to listen for Messages with the topic
			// "GMSEC.TEST.PUBLISH" which are published to the middleware
			// bus and register the callback object to the subscription
			// Subscription subject wildcard syntax:
			// * - Matches any one token separated by periods in a subject
			// > - Reading left to right, matches everything up to and ONE
			//     or more tokens in a subject
			// + - Reading left to right, matches everything up to and ZERO
			//     or more tokens in a subject
			// For more information on wildcards, please see the GMSEC API
			// User's Guide
			Log.info("Subscribing to the topic: " + DEFAULT_SUBSCRIPTION_SUBJECT);
			connMgr.subscribe(DEFAULT_SUBSCRIPTION_SUBJECT, new ConnectionManagerCallback()
			{
				public void onMessage(ConnectionManager connection, Message message)
				{
					Log.info("[ExampleCallback::onMessage] Received:\n" + message.toXML());
					receivedMessage = true;
				}
			});

			//o Start the AutoDispatcher to begin asynchronously processing
			// messages
			connMgr.startAutoDispatch();

			//o Because the handling of messages is occurring asynchronously
			// in another thread, we will need to 'busy wait' here until a
			// message is received
			while(!receivedMessage)
			{
				Thread.sleep(100);
			}

			//o Clean up
			connMgr.stopAutoDispatch();
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
