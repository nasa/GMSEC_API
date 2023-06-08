#!/usr/bin/env node


/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * subscribe.js
 *
 * This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class subscribe
{
	run()
	{
		try
		{
			// Set up connection configuration options provided by the user
			const config = utility.createConfig(process.argv);

			// Initialize log level for output
			utility.initializeLogLevel(config);

			// Validate messages before being sent to the bus
			config.addValue("gmsec-msg-content-validate-recv", "true");

			// Deduce subscription topic for the example program
			var subscription_topic;
			const level = config.getIntegerValue("gmsec-schema-level", gmsec.Specification.SchemaLevel_LEVEL_0);

			switch (level)
			{
				case gmsec.Specification.SchemaLevel_LEVEL_0:
					subscription_topic = "C2MS.>";
					break;

				case gmsec.Specification.SchemaLevel_LEVEL_1:
					subscription_topic = "C2MS-PIPE.>";
					break;

				case gmsec.Specification.SchemaLevel_LEVEL_2:
				default:
					subscription_topic = "GMSEC.>";
					break;
				}

			// Create connection instance
			const conn = new gmsec.Connection(config);

			// Establish connection to the GMSEC Bus
			conn.connect();

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("Middleware version = " + conn.getLibraryVersion());

			// Subscribe to receive message
			conn.setupSubscription(subscription_topic);
			gmsec.Log.info("Subscribed to topic: " + subscription_topic);

			// Define timeout period, or use Gmsec.WAIT_FOREVER to block
			// indefinitely to receive a message
			const timeout = 5000;   // 5 seconds

			// Attempt to receive a message
			const msg = conn.receive(timeout);

			if (null != msg)
			{
				// Received a message!
				gmsec.Log.info("Received Message:\n" + msg.toXML());

				// Acknowledge processing the message
				msg.acknowledge();
			}
			else
			{
				// Time out!
				gmsec.Log.warning("Time out; no message was received");
			}

			// Disconnect from the GMSEC Bus
			conn.disconnect();
		}
		catch (e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus
			gmsec.Log.error("Exception: " + e.message);
		}
	}
}


// Instantiate the subscribe example, and run it.
const example = new subscribe();
example.run();
