/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file subscribe_subject_filter.java
 * 
 * This file contains a simple example outlining how to subscribe
 * to and retrieve messages from the middleware bus while using subject
 * exclusion filtering to reduce the scope of a wildcarded subject subscription.
 *
 * This example program is intended to be run before starting the
 * 'publish_subject_filter' example program.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;


public class subscribe_subject_filter
{
	public static void main(String[] args)
	{
		// Set up connection configuration options
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Deduce subscription topics for the example program
		String ALL_MESSAGES_TOPIC;
		String HB_MESSAGES_TOPIC;
		int level = config.getIntegerValue("gmsec-schema-level", Specification.SchemaLevel.LEVEL_0.getValue());

		switch (Specification.SchemaLevel.valueOf(level))
		{
		case LEVEL_0:
			ALL_MESSAGES_TOPIC = "C2MS.>";
			HB_MESSAGES_TOPIC  = "C2MS.*.*.*.*.*.MSG.HB.+";
			break;

		case LEVEL_1:
			ALL_MESSAGES_TOPIC = "C2MS-PIPE.>";
			HB_MESSAGES_TOPIC  = "C2MS-PIPE.*.*.*.*.*.MSG.HB.+";
			break;

		case LEVEL_2:
		default:
			ALL_MESSAGES_TOPIC = "GMSEC.>";
			HB_MESSAGES_TOPIC  = "GMSEC.*.*.MSG.HB.+";
			break;
		}

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Establish connection to the GMSEC Bus.
			conn.connect();
		
			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Subscribe to receive all messages using the GMSEC specification
			Log.info("Subscribe to: " + ALL_MESSAGES_TOPIC);
			conn.subscribe(ALL_MESSAGES_TOPIC);

			// Add specific message topic to the Connection's exclusion filter
			Log.info("Filtering out: " + HB_MESSAGES_TOPIC);
			conn.excludeSubject(HB_MESSAGES_TOPIC);


			// Receive first message (LOG) sent by publisher
			Log.info("Waiting to receive a message...");
			Message rcvdMsg = conn.receive();

			Log.info("Received LOG message:\n" + rcvdMsg.toXML());

			// Acknowledge processing the message
			rcvdMsg.acknowledge();

			// Destroy the message
			Message.destroy(rcvdMsg);


			// Although a second message (HB) is sent by the publisher, it will not
			// be delivered to this Connection object
			Log.info("Waiting to receive other message(s)...");
			rcvdMsg = conn.receive(5000);

			if (rcvdMsg == null)
			{
				Log.info("As expected, a timeout occurred (i.e. no HB message received)");
			}
			else
			{
				Log.warning("Received unexpected message:\n" + rcvdMsg.toXML());

				// Acknowledge processing the message
				rcvdMsg.acknowledge();

				// Destroy the message
				Message.destroy(rcvdMsg);
			}

			// Disconnect from the GMSEC Bus.
			conn.disconnect();

			// Destroy the connection instance
			Connection.destroy(conn);
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus.
			Log.error("GmsecException: " + e.getMessage());
		}
	}
}
