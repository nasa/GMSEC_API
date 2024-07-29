#!/usr/bin/env node


/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * subscribe_subject_filter.js
 *
 * This file contains a simple example outlining how to subscribe
 * to and retrieve messages from the middleware bus while using subject
 * exclusion filtering to reduce the scope of a wildcarded subject subscription.
 * 
 * This example program is intended to be run before starting the
 * 'publish_subject_filter' example program.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class subscribe_subject_filter
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
			var all_messages_topic;
			var hb_messages_topic;
			const level = config.getIntegerValue("gmsec-schema-level", gmsec.Specification.SchemaLevel_LEVEL_0);

			switch (level)
			{
				case gmsec.Specification.SchemaLevel_LEVEL_0:
					all_messages_topic = "C2MS.>";
					hb_messages_topic  = "C2MS.*.*.*.*.*.MSG.HB.+";
					break;

				case gmsec.Specification.SchemaLevel_LEVEL_1:
					all_messages_topic = "C2MS-PIPE.>";
					hb_messages_topic  = "C2MS-PIPE.*.*.*.*.*.MSG.HB.+";
					break;

				case gmsec.Specification.SchemaLevel_LEVEL_2:
				default:
					all_messages_topic = "GMSEC.>";
					hb_messages_topic  = "GMSEC.*.*.MSG.HB.+";
					break;
				}

			// Create connection instance
			const conn = new gmsec.Connection(config);

			// Establish connection to the GMSEC Bus
			conn.connect();

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("Middleware version = " + conn.getLibraryVersion());

			// Subscribe to receive all messages
			conn.setupSubscription(all_messages_topic);
			gmsec.Log.info("Subscribed to topic: " + all_messages_topic);

			// Add specific message topic to the Connection's exclusion filter
			conn.excludeSubject(hb_messages_topic);
			gmsec.Log.info("Filtering topic: " + hb_messages_topic);

			// Define timeout period, or use Gmsec.WAIT_FOREVER to block
			// indefinitely to receive a message
			let msg = conn.receive(gmsec.GMSEC_WAIT_FOREVER);

			gmsec.Log.info("Received LOG message:\n" + msg.toXML());

			// Acknowledge processing the message
			msg.acknowledge();

			// Although a second message (HB) is sent by the publisher, it will not
			// be delivered to this Connection object
			gmsec.Log.info("Waiting to receive other message(s)...");
			msg = conn.receive(5000);

			if (null == msg)
			{
				gmsec.Log.info("As expected, a timeout occurred (i.e. no HB message received)");
			}
			else
			{
				gmsec.Log.warning("Received unexpected message:\n" + msg.toXML());

				// Acknowledge processing the message
				msg.acknowledge();
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


// Instantiate the subscribe_subject_filter example, and run it.
const example = new subscribe_subject_filter();
example.run();
