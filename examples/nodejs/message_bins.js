#!/usr/bin/env node


/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * message_bins.js
 *
 * This file contains an example describing how to aggregate multiple
 * messages into a singular message sent to the middleware server. This
 * functionality is useful for reducing the number of messages sent across
 * the middleware bus, if there is an application which is publishing an
 * exorbitantly large number of (small) messages.
 *
 * It is recommended to execute the application GMSEC_API/bin/gmsub prior
 * to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but the messages will be unusable.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class message_bins
{
	run()
	{
		try
		{
			// Set up connection configuration options provided by the user
			const config = utility.createConfig(process.argv);

			// Initialize log level for output
			utility.initializeLogLevel(config);

			// Deduce subject and excluded subject for message(s) that will be binned and for those that will not
			var msg_bin_subject;
			var msg_bin_exclude_subject;
			var level = config.getIntegerValue("gmsec-schema-level", gmsec.Specification.SchemaLevel_LEVEL_0);

			switch (level)
			{
			case gmsec.Specification.SchemaLevel_LEVEL_0:
				msg_bin_subject         = "C2MS.*.*.*.*.*.MSG.LOG.*.AAA.>";
				msg_bin_exclude_subject = "C2MS.*.*.*.*.*.MSG.LOG.*.TLM.>";
				break;

			case gmsec.Specification.SchemaLevel_LEVEL_1:
				msg_bin_subject         = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.AAA.>";
				msg_bin_exclude_subject = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.TLM.>";
				break;

			case gmsec.Specification.SchemaLevel_LEVEL_2:
			default:
				msg_bin_subject         = "GMSEC.*.*.MSG.LOG.*.AAA.>";
				msg_bin_exclude_subject = "GMSEC.*.*.MSG.LOG.*.TLM.>";
				break;
			}

			// Validate messages before being sent to the bus
			config.addValue("gmsec-msg-content-validate-send", "true");

			// Configure to enable message binning
			config.addValue("gmsec-use-msg-bins", "true");

			// Specify the number of messages to be aggregated prior to publishing
			// the aggregate message to the middleware server. This applies to all
			// of the messages which match the subject(s) provided in the
			// GMSEC-MSG-BIN-SUBJECT-N configuration parameters
			// Note: The aggregate message will be sent to the middleware server
			// immediately upon this many messages being published, regardless of
			// the value supplied for GMSEC-MSG-BIN-TIMEOUT.
			config.addValue("gmsec-msg-bin-size", "10");

			// Specify a timeout (in milliseconds) for the aggregate message to be
			// sent to the GMSEC Bus.
			// Note: The aggregate message will be sent to the GMSEC Bus after this
			// period of time if the message bin does not fill up (per the value
			// provided for gmsec-msg-bin-size) prior to this timeout
			config.addValue("gmsec-msg-bin-timeout", "5000");

			// Specify the message subject(s) of messages that should be aggregated.
			// Subscription wildcard syntax can be used here.
			// Note: If this option is not specified, then ALL messages will be
			// aggregated (except those that are excluded).
			config.addValue("gmsec-msg-bin-subject-1", msg_bin_subject);

			// Specify any message subject(s) that should be excluded from being
			// aggregated.
			// This is useful if a wildcard subscription is provided in one of the
			// gmsec-msg-bin-subject-N parameters.
			config.addValue("gmsec-msg-bin-exclude-subject-1", msg_bin_exclude_subject);

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
			const logMsg1 = conn.getMessageFactory().createMessage("LOG");
			const logMsg2 = conn.getMessageFactory().createMessage("LOG");

			logMsg1.addField(new gmsec.StringField("SUBCLASS", "AAA"));
			logMsg1.addField(new gmsec.StringField("OCCURRENCE-TYPE", "PLOT"));
			logMsg1.addField(new gmsec.I16Field("SEVERITY", 1));
			logMsg1.addField(new gmsec.StringField("MSG-TEXT", "Message will be binned"));

			logMsg2.addField(new gmsec.StringField("SUBCLASS", "TLM"));
			logMsg2.addField(new gmsec.StringField("OCCURRENCE-TYPE", "DATA"));
			logMsg2.addField(new gmsec.I16Field("SEVERITY", 1));
			logMsg2.addField(new gmsec.StringField("MSG-TEXT", "Message will NOT be binned"));

			for (let i = 0; i < 5; i++) {
				const theTime = gmsec.TimeUtil.formatTime(gmsec.TimeUtil.getCurrentTime());

				logMsg1.addField(new gmsec.StringField("EVENT-TIME", theTime));
				logMsg2.addField(new gmsec.StringField("EVENT-TIME", theTime));

				// Publish the first message; it will be aggregated and published at a later time
				conn.publish(logMsg1);
				gmsec.Log.info("Publishing AAA Log Message " + (i+1) + " (will be aggregated):\n" + logMsg1.toXML());

				// Publish the second message; it will NOT be aggregated and will be published immediately
				conn.publish(logMsg2);
				gmsec.Log.info("Publishing TLM Log Message " + (i+1) + " (will NOT be aggregated):\n" + logMsg2.toXML());

				// Add processing delay
				gmsec.TimeUtil.millisleep(100);
			}

			// Delay to demonstrate message bin is flushed after a period of time (e.g. 5 seconds)
			gmsec.Log.info("Waiting for binned messages to be published...");
			gmsec.TimeUtil.millisleep(6000);

			// Disconnect from the GMSEC Bus, and terminate subscriptions.
			// Note: Any remaining messages in message bins will be published
			// before disconnecting from the GMSEC Bus, thus ensuring no message
			// loss occurs.
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


// Instantiate the message_bins example, and run it.
const example = new message_bins();
example.run();
