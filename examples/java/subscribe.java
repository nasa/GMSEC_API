/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe.java
 *
 * @brief This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;


public class subscribe
{
	public static void main(String[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Validate messages after they are received from the bus
		config.addValue("gmsec-msg-content-validate-recv", "true");

		// Deduce subscription topic for the example program
		String subscription_topic;
		int level = config.getIntegerValue("gmsec-schema-level", Specification.SchemaLevel.LEVEL_0.getValue());

		switch (Specification.SchemaLevel.valueOf(level))
		{
		case LEVEL_0:
			subscription_topic = "C2MS.>";
			break;

		case LEVEL_1:
			subscription_topic = "C2MS-PIPE.>";
			break;

		case LEVEL_2:
		default:
			subscription_topic = "GMSEC.>";
			break;
		}

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Subscribe to receive message
			conn.subscribe(subscription_topic);
			Log.info("Subscribed to topic: " + subscription_topic);

			// Define timeout period, or use Gmsec.WAIT_FOREVER to block
			// indefinitely to receive a message.
			int timeout = 5000;   // 5 seconds

			// Attempt to receive a message
			Message msg = conn.receive(timeout);

			if (null != msg)
			{
				// Received a message!
				Log.info("Received Message:\n" + msg.toXML());

				// Acknowledge processing the message
				msg.acknowledge();

				// Destroy message
				Message.destroy(msg);
			}
			else
			{
				// Time out!
				Log.warning("Time out; no message was received");
			}

			// Disconnect from the GMSEC Bus, and terminate subscriptions.
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
