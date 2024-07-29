#!/usr/bin/env node


/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * publish_subject_filter.js
 *
 * This file contains an example publishing a few messages with different
 * subjects in order to demonstrate the subject filtering functionality of the
 * API. 
 *
 * This example program is intended to be run after starting the
 * 'subscribe_subject_filter' example program.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class publish_subject_filter
{
	run()
	{
		var conn = null;

		try
		{
			// Set up connection configuration options provided by the user
			const config = utility.createConfig(process.argv);

			// Initialize log level for output
			utility.initializeLogLevel(config);

			// Validate messages before being sent to the bus
			config.addValue("gmsec-msg-content-validate-send", "true");

			// Create a Connection instance
			conn = gmsec.Connection.create(config);

			// Establish connection to the GMSEC Bus
			conn.connect();

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("Middleware version = " + conn.getLibraryVersion());

			// Set up standard fields within the MessageFactory associated
			// with the connection object
			utility.setStandardFields(conn.getMessageFactory(), this.constructor.name);

			// Create a Log message using the MessageFactory
			const logMsg = conn.getMessageFactory().createMessage("LOG");

			// Add required fields
			logMsg.addField(new gmsec.StringField("SUBCLASS", "INFO"));
			logMsg.addField(new gmsec.StringField("OCCURRENCE-TYPE", "TEST"));
			logMsg.addField(new gmsec.I16Field("SEVERITY", 1));
			logMsg.addField(new gmsec.StringField("MSG-TEXT", "Hello subscriber!"));

			// And the current (event) time
			logMsg.addField(new gmsec.StringField("EVENT-TIME", gmsec.TimeUtil.formatTime(gmsec.TimeUtil.getCurrentTime())));

			// Create a Heartbeat message using the MessageFactory
			const hbMsg = conn.getMessageFactory().createMessage("HB");

			gmsec.Log.info("Publishing two messages -- Log message will be received by the subscriber,"
			               + " the Heartbeat message will be filtered out");

			// Publish the Log message
			conn.publish(logMsg);
			gmsec.Log.info("Published Log Message:\n" + logMsg.toXML());

			// Delay a bit
			gmsec.TimeUtil.millisleep(2000);

			// Publish the Heartbeat message
			conn.publish(hbMsg);
			gmsec.Log.info("Published HB Message:\n" + hbMsg.toXML());

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
		finally
		{
			// Destroy the connection
			gmsec.Connection.destroy(conn);
		}
	}
}


// Instantiate the publish_subject_filter example, and run it.
const example = new publish_subject_filter();
example.run();
