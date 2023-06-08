#!/usr/bin/env node


/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * request.js
 *
 * This example program provides a basic demonstration of issuing
 * a synchronous Request Directive message to the GMSEC Bus, and
 * then handling the receipt of a Response Directive message.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class request
{
	run()
	{
		try
		{
			// Set up connection configuration options provided by the user
			const config = utility.createConfig(process.argv);

			// Initialize log level for output
			utility.initializeLogLevel(config);

			// Validate messages before being sent to and after being received from the bus
			config.addValue("gmsec-msg-content-validate-all", "true");

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

			// Create a Request Directive message using the MessageFactory,
			// then set both required and optional fields within the message
			const reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			reqMsg.addField(new gmsec.StringField("DESTINATION-COMPONENT", "REPLY", true));
			reqMsg.addField(new gmsec.StringField("DIRECTIVE-STRING", "Turn on the lights"));
			reqMsg.addField(new gmsec.BooleanField("RESPONSE", true));
			reqMsg.addField(new gmsec.U16Field("REQUEST-ID", 0));

			// Output in XML what we intend to send
			gmsec.Log.info("Sending Request Message:\n" + reqMsg.toXML());

			// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
			// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
			// prevent re-sending request message should a timeout occur.
			const timeout   = 5000;
			const republish = 5000;

			// Issue Request Directive.
			// Note, this is a blocking call (i.e. a synchronous operation).
			const rspMsg = conn.request(reqMsg, timeout, republish);

			// We have returned from the previous blocking call; check if we have a message.
			if (null != rspMsg)
			{
				gmsec.Log.info("Received Response Directive message:\n" + rspMsg.toXML());

				// Acknowledge processing the response message
				rspMsg.acknowledge();
			}
			else
			{
				// This else-block is reached if a response message is not received.
				// If the republish period is set to a time period other than
				// GMSEC_REQUEST_REPUBLISH_NEVER, then this block will never be reached.
				gmsec.Log.warning("Time out; no Response Directive message received");
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


// Instantiate the request example, and run it.
const example = new request();
example.run();
