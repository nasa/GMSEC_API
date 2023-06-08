#!/usr/bin/env node


/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * publish.js
 *
 * This example program provides a basic demonstration of publishing
 * a (heartbeat) message to the GMSEC Bus.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class publish
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
			config.addValue("gmsec-msg-content-validate-send", "true");

			// Create a Connection instance
			const conn = new gmsec.Connection(config);

			// Establish connection to the GMSEC Bus
			conn.connect();

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("Middleware version = " + conn.getLibraryVersion());

			// Set up standard fields within the MessageFactory associated
			// with the connection object
			utility.setStandardFields(conn.getMessageFactory(), this.constructor.name);

			// Create a Heartbeat message using the MessageFactory
			const hbMsg = conn.getMessageFactory().createMessage("HB");

			// Publish the message
			conn.publish(hbMsg);

			// Output in XML what we have published
			gmsec.Log.info("Publishing Message:\n" + hbMsg.toXML());

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


// Instantiate the publish example, and run it.
const example = new publish();
example.run();
