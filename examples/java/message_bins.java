/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file message_bins.java
 *
 * @brief This file contains an example describing how to aggregate multiple
 * messages into a singular message sent to the middleware server. This
 * functionality is useful for reducing the number of messages sent across
 * the * middleware bus, if there is an application which is publishing an
 * exorbitantly large number of (small) messages.
 *
 * It is recommended to execute the application GMSEC_API/bin/gmsub prior
 * to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but they will be unusable.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;

import java.util.ArrayList;
import java.util.List;


public class message_bins
{
	public static void main(String[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Deduce subject and excluded subject for message(s) that will be binned and for those that will not
		String msg_bin_subject;
		String msg_bin_exclude_subject;
		int level = config.getIntegerValue("gmsec-schema-level", Specification.SchemaLevel.LEVEL_0.getValue());

		switch (Specification.SchemaLevel.valueOf(level))
		{
		case LEVEL_0:
			msg_bin_subject         = "C2MS.*.*.*.*.*.MSG.LOG.*.AAA.>";
			msg_bin_exclude_subject = "C2MS.*.*.*.*.*.MSG.LOG.*.TLM.>";
			break;

		case LEVEL_1:
			msg_bin_subject         = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.AAA.>";
			msg_bin_exclude_subject = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.TLM.>";
			break;

		case LEVEL_2:
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

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Set up standard fields within the MessageFactory associated
			// with the connection object.
			setStandardFields(conn.getMessageFactory(), "MESSAGE-BINS");

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Create a Log messages using the MessageFactory.
			// Set both required and optional fields within the message.
			Message logMsg1 = conn.getMessageFactory().createMessage("LOG");
			Message logMsg2 = conn.getMessageFactory().createMessage("LOG");

			logMsg1.addField("SUBCLASS", "AAA");
			logMsg1.addField("OCCURRENCE-TYPE", "PLOT");
			logMsg1.addField("SEVERITY", (short) 1);
			logMsg1.addField("MSG-TEXT", "Message will be binned");

			logMsg2.addField("SUBCLASS", "TLM");
			logMsg2.addField("OCCURRENCE-TYPE", "DATA");
			logMsg2.addField("SEVERITY", (short) 1);
			logMsg2.addField("MSG-TEXT", "Message will NOT be binned");

			for (int i = 0; i < 5; ++i)
			{
				String theTime = TimeUtil.formatTime( TimeUtil.getCurrentTime() );

				logMsg1.addField("EVENT-TIME", theTime);
				logMsg2.addField("EVENT-TIME", theTime);

				// Publish the first message; it will be aggregated and published at a later time
				conn.publish(logMsg1);
				Log.info("Publishing AAA Log Message " + (i+1) + " (will be aggregated):\n" + logMsg1.toXML());

				// Publish the second message; it will NOT be aggregated and will be published immediately
				conn.publish(logMsg2);
				Log.info("Publishing TLM Log Message " + (i+1) + " (will NOT be aggregated):\n" + logMsg2.toXML());

				// Add processing delay
				TimeUtil.millisleep(100);
			}

			// Delay to demonstrate message bin is flushed after a period of time (e.g. 5 seconds)
			Log.info("Waiting for binned messages to be published...");
			TimeUtil.millisleep(6000);

			// Disconnect from the GMSEC Bus, and terminate subscriptions.
			// Note: Any remaining messages in message bins will be published
			// before disconnecting from the GMSEC Bus, thus ensuring no message
			// loss occurs.
			conn.disconnect();

			// Destroy the connection instance
			Connection.destroy(conn);

			// Destroy the Log messages (careful... wait until messages are published!)
			Message.destroy(logMsg1);
			Message.destroy(logMsg2);
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus.
			Log.error("GmsecException: " + e.getMessage());
		}
	}


	// Convenience function for associating standard (i.e. common) fields
	// with the MessageFactory which will be included automatically within
	// all messages created using the factory.
	private static void setStandardFields(MessageFactory factory, String component)
	{
		List<Field> standardFields = new ArrayList<Field>();

		standardFields.add(new StringField("MISSION-ID", "MY-MISSION", true));
		standardFields.add(new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true));
		standardFields.add(new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true));
		standardFields.add(new StringField("CONSTELLATION-ID", "MY-CNST", true));
		standardFields.add(new StringField("FACILITY", "MY-FACILITY", true));
		standardFields.add(new StringField("DOMAIN1", "MY-DOMAIN-1", true));
		standardFields.add(new StringField("DOMAIN2", "MY-DOMAIN-2", true));
		standardFields.add(new StringField("COMPONENT", component, true));

		factory.setStandardFields(standardFields);
	}
}
