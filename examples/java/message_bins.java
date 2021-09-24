/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file message_bins.java
 *
 * This file contains an example describing how to aggregate multiple messages
 * into a singular message sent to the middleware server.  This functionality
 * is useful for cutting down on the number of messages sent across the
 * middleware bus, if there is an application in the system which is publishing
 * an exorbitantly large number of messages.
 *
 * It is recommended that you execute the application GMSEC_API/bin/gmsub
 * prior to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but they will be unusable.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import java.math.BigInteger;

public class message_bins
{
	public static final String EXAMPLE_MESSAGE_SUBJECT = "GMSEC.AGGREGATE.PUBLISH";
	public static final String EXAMPLE_BIN_EXCLUDE_SUBJECT = "GMSEC.BIN-EXCLUDE.PUBLISH";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java message_bins logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		//o Enable Message Binning
		config.addValue("GMSEC-USE-MSG-BINS", "true");

		//o Specify the number of messages to be aggregated prior to publishing
		// the aggregate message to the middleware server (This applies to all
		// of the messages which match the subject(s) provided in the
		// GMSEC-MSG-BIN-SUBJECT-N configuration parameters
		// Note: The aggregate message will be sent to the middleware server
		// immediately upon this many messages being published, regardless of
		// the value supplied for GMSEC-MSG-BIN-TIMEOUT.
		config.addValue("GMSEC-MSG-BIN-SIZE", "10");

		//o Specify a timeout (in milliseconds) for the aggregate message to be
		// sent to the middleware server
		// Note: The aggregate message will be sent to the middleware server
		// after this period of time if the message bin does not fill up (per
		// the value provided for GMSEC-MSG-BIN-SIZE) prior to this timeout
		config.addValue("GMSEC-MSG-BIN-TIMEOUT", "5000");

		//o Specify the subjects to aggreate messages for.
		// Note: Subscription wildcard syntax can be used here, and has been
		// supported since GMSEC API version 4.3.
		config.addValue("GMSEC-MSG-BIN-SUBJECT-1", "GMSEC.*.PUBLISH");

		//o Specify any subjects that should be excluded from being aggregated
		// This is useful if a wildcard subscription is provided in one of the
		// GMSEC-MSG-BIN-SUBJECT-N parameters.
		config.addValue("GMSEC-MSG-BIN-EXCLUDE-SUBJECT-1", EXAMPLE_BIN_EXCLUDE_SUBJECT);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.addValue("gmsec-msg-content-validate-all", "false");

		initializeLogging(config);

		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			Log.info("Opening the connection to the middleware server");
			connMgr.initialize();

			Log.info(connMgr.getLibraryVersion());

			{
				Message message = new Message(EXAMPLE_MESSAGE_SUBJECT, Message.MessageKind.PUBLISH);

				for(int i = 0; i < 5; i++)
				{
					populateMessage(message, i+1);

					//o Publish the message to the middleware bus
					connMgr.publish(message);

					//o Display the XML string representation of the Message for
					// the sake of review
					Log.info("Published message: " + message.toXML());
				}
			}

			{
				//o Create a message
				Message message = new Message(EXAMPLE_BIN_EXCLUDE_SUBJECT, Message.MessageKind.PUBLISH);

				populateMessage(message, 1);

				//o Publish the message to the middleware bus
				// Note: When calling publish(), if a message does NOT match
				// one of the subjects to be aggregated, it will be immediately
				// published
				connMgr.publish(message);

				//o Display the XML string representation of the Message for
				// the sake of review
				Log.info("Published message: " + message.toXML());
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

	public static void populateMessage(Message message, int count)
	{
		message.addField("CHAR-FIELD", 'c');
		message.addField("BOOL-FIELD-TRUE", true);
		message.addField("BOOL-FIELD-FALSE", false);
		message.addField("I8-FIELD", (byte) count);
		message.addField("I16-FIELD", (short) count);
		message.addField("I32-FIELD", (int) count);
		message.addField("I64-FIELD", (long) count);
		message.addField("U8-FIELD", new U8(count));
		message.addField("U16-FIELD", new U16(count));
		message.addField("U32-FIELD", new U32(count));
		message.addField("U64-FIELD", new U64(BigInteger.valueOf(count)));
		message.addField("STRING-FIELD", "This is a test");
		message.addField("F32-FIELD", (float) (1 + 1. / count));
		message.addField("F64-FIELD", (double) (1 + 1. / count));
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
