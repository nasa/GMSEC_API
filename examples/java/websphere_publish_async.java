/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file websphere_publish_async.cpp
 *
 * This file contains an example which demonstrates how to utilize the
 * WebSphere MQ asynchronous publish functionality, as well as how to check on
 * the status of publish operations (Since the client does not wait to receive
 * the status of each operation in order to achieve asynchronicity) by
 * registering an EventCallback with the ConnectionManager.
 *
 * It is recommended that users execute the program GMSEC_API/bin/gmsub to
 * receive the messages that this example publishes.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerEventCallback;
import java.math.BigInteger;



class AsyncStatusCheckCallback extends ConnectionManagerEventCallback
{
	public boolean eventFired_ = false;
	public void onEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent event)
	{
		// Print the status of publish operations.  This includes counts
		// for successes, warnings, and errors.
		Log.info(status.getReason());

		if(status.isError())
		{
			Log.error("The first occurrence of a WebSphere MQ Asynchronous Put Response warning or failure returned the WebSphere Reason Code: " + status.getCustomCode());
		}
		eventFired_ = true;
	}
}

public class websphere_publish_async
{
	public static final String EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java websphere_publish_async mw-id=<middleware ID>");
			System.exit(-1);
		}

		AsyncStatusCheckCallback cb = new AsyncStatusCheckCallback();

		Config config = new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.addValue("gmsec-msg-content-validate-all", "false");

		initializeLogging(config);

		//o Toggle the WebSphere MQ client libraries-level asynchronous publish
		// functionality on
		config.addValue("MW-ASYNC-PUBLISH", "true");

		//o Toggle the periodic WebSphere MQ asynchronous publish status report
		// functionality on.  The GMSEC API Connection will periodically fire
		// off an Event which will trigger the onEvent function of an
		// EventCallback registered to the event labeled,
		// Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT
		config.addValue("MW-ASYNC-STATUS-CHECK", "true");

		//o Additionally, the "MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL"
		// configuration option can be used to instruct the GMSEC Connection
		// on how frequently (in number of publish operations) it should
		// periodically fire the WSMQ_ASYNC_STATUS_CHECK_EVENT. By default, it
		// will fire once every 100 messages.
		// For the sake of this example, we will use 500 messages.
		config.addValue("MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL", "500");

		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			//o Create the Connection
			ConnectionManager connMgr = new ConnectionManager(config);

			//o Connect
			connMgr.initialize();

			//o Register the event callback with the connection to catch
			// the WebSphere asynchronous publish status events which are
			// eminated from the API
			connMgr.registerEventCallback(Connection.ConnectionEvent.WSMQ_ASYNC_STATUS_CHECK_EVENT, cb);

			//o Output middleware version
			Log.info("Middleware version = " + connMgr.getLibraryVersion());

			Log.info("Publishing messages using the subject: " + EXAMPLE_MESSAGE_SUBJECT);

			//o Create a GMSEC Message object
			Message message = new Message(EXAMPLE_MESSAGE_SUBJECT, Message.MessageKind.PUBLISH);

			//o Publish message as quickly as possible
			// (i.e. No sleep operation between each publish operation)
			int count = 0;
			while(!cb.eventFired_)
			{
				//o Populate the Message with fields, increment a
				// counter so that a publisher can track how many
				// messages were published (if they are interested)
				populateMessage(message, count++);

				//o Publish the message to the middleware bus
				connMgr.publish(message);

				// Note: We do not recommend printing the output of a
				// message when publishing it while using the WebSphere
				// async publish functionality, as it is
				// counter-intuitive to take take up CPU resources
				// performing I/O operations when attempting to achieve
				// high message throughput rates. As such, if you want
				// to analyze the messages published by this program,
				// we recommend you execute GMSEC_API/bin/gmsub to
				// receive the messages.
			}

			Log.info("Event detected, ending example demonstration");

			//o Clean up the ConnectionManager before exiting the program
			connMgr.cleanup();
		}
		catch (GMSEC_Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}

		return;
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
