/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file connection_state.java
 *
 * This file contains an example which demonstrates how to utilize
 * EventCallbacks to track the state of the GMSEC Connection and its connection
 * to the middleware.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerEventCallback;
import java.util.Scanner;
import java.math.BigInteger;

//o Define an EventCallback to report the status of WebSphere async publications
// Note: The WebSphere MQ client libraries only report the first warning or
// failure code encountered during the series of message publications made
// since the last time that the client libraries retrieved the status of
// asynchronous publish operations.
class ConnectionStateCallback extends ConnectionManagerEventCallback
{
	public void onEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent event)
	{
		// Perform some action based on the type of event that occurred
		if (event == Connection.ConnectionEvent.CONNECTION_SUCCESSFUL_EVENT)
		{
			Log.info("Connected to the middleware server");
		}
		else if (event == Connection.ConnectionEvent.CONNECTION_BROKEN_EVENT)
		{
			Log.info("Connection to the middleware lost or terminated");
		}
		else if (event == Connection.ConnectionEvent.CONNECTION_RECONNECT_EVENT)
		{
			Log.info("Attempting to reestablish the connection to the middleware");
		}
	}
}

public class connection_state
{
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java connection_state mw-id=<middleware ID>");
			System.exit(-1);
		}

		ConnectionStateCallback cb = new ConnectionStateCallback();

		//o Load the command-line input into a GMSEC Config object
		Config config = new Config(args);

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		initializeLogging(config);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// TODO: Once available, replace this statement with usage of
		// ConnectionManager::getAPIVersion (See RTC 4798)
		Log.info(Connection.getAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			connMgr.initialize();

			//o Register the event callback with the connection to catch
			// connection state changes, including:
			// 1. Connection successfully established to middleware
			// 2. Connection broken from middleware
			// 3. Reconnecting to the middleware
			connMgr.registerEventCallback(Connection.ConnectionEvent.CONNECTION_SUCCESSFUL_EVENT, cb);
			connMgr.registerEventCallback(Connection.ConnectionEvent.CONNECTION_BROKEN_EVENT, cb);
			connMgr.registerEventCallback(Connection.ConnectionEvent.CONNECTION_RECONNECT_EVENT, cb);

			//o Output middleware version
			Log.info("Middleware version = " + connMgr.getLibraryVersion());

			// Assuming that the user provided proper reconnection syntax
			// for the corresponding middleware they are using, one could
			// kill the middleware server at this point, then start it back
			// up to demonstrate the EventCallback.onEvent() function
			// triggering

			//o Wait for user input to end the program
			Log.info("Waiting for Connection events to occur, press <enter> to exit the program");
			new Scanner(System.in).nextLine();

			//o Clean up the ConnectionManager before exiting the program
			connMgr.cleanup();
		}
		catch (Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}
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

	public static void populateMessage(Message message, int value)
	{
		message.addField("CHAR-FIELD", 'c');
		message.addField("BOOL-FIELD-TRUE", true);
		message.addField("BOOL-FIELD-FALSE", false);
		message.addField("I8-FIELD", (byte) value);
		message.addField("I16-FIELD", (short) value);
		message.addField("I32-FIELD", (int) value);
		message.addField("I64-FIELD", (long) value);
		message.addField("U8-FIELD", new U8(value % 255));
		message.addField("U16-FIELD", new U16(value));
		message.addField("U32-FIELD", new U32(value));
		message.addField("U64-FIELD", new U64(BigInteger.valueOf(value)));
		message.addField("STRING-FIELD", "This is a test");
		message.addField("F32-FIELD", (float) (1 + 1. / value));
		message.addField("F64-FIELD", (double) (1 + 1. / value));
		message.addField("BIN-FIELD", "JLMNOPQ".getBytes());
	}
}
