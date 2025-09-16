/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file reply.java
 *
 * @brief This example program provides a basic demonstration of synchronously
 * receiving a Request Directive message from the GMSEC Bus, and then issuing
 * a Response Directive message (should one be requested).
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.ArrayList;
import java.util.List;


public class reply
{
	public static void main(String[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Validate messages before being sent to and after being received from the bus
    	config.addValue("gmsec-msg-content-validate-all", "true");

		// Define subscription topic for receiving request message(s).
		String subscription_topic;
		int level = config.getIntegerValue("gmsec-schema-level", Specification.SchemaLevel.LEVEL_0.getValue());

		switch (Specification.SchemaLevel.valueOf(level))
		{
		case LEVEL_0:
			subscription_topic = "C2MS.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY";
			break;

		case LEVEL_1:
			subscription_topic = "C2MS-PIPE.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY";
			break;

		case LEVEL_2:
		default:
			subscription_topic = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.REPLY";
			break;
		}

		try
		{
			// Create connection instance
			Connection conn = Connection.create(config);

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Set up standard fields within the MessageFactory associated
			// with the connection object.
			setStandardFields(conn.getMessageFactory(), "REPLY");

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Set up our subscription for receiving a request message.
			conn.subscribe(subscription_topic);
			Log.info("Subscribed to topic: " + subscription_topic);

			// Timeout was arbitrarily chosen as Gmsec.WAIT_FOREVER so that
			// we block indefinitely waiting to receive a request message.
			// It can be set to other time values (e.g. 1000 is 1 second) so
			// the application does not block indefinitely waiting for a
			// request message.
			int timeout = Gmsec.WAIT_FOREVER;

			// Block for a Request Directive message to arrive.
			Message reqMsg = conn.receive(timeout);

			// Check if we have received a message.
			if (null != reqMsg)
			{
				// Output in XML the received message.
				Log.info("Received Request Message:\n" + reqMsg.toXML());

				// Check if a response message should be returned.
				if (reqMsg.hasField("RESPONSE") && reqMsg.getBooleanValue("RESPONSE"))
				{
					// Create a Response Directive message using the MessageFactory.
					Message rspMsg = conn.getMessageFactory().createMessage("RESP.DIR");

					// Insert required and optional fields into the Response Directive
					// using values derived from the incoming Request Directive.
					if (reqMsg.hasField("COMPONENT"))
					{
						rspMsg.addField(new StringField("DESTINATION-COMPONENT", reqMsg.getStringValue("COMPONENT"), true));
					}
					if (reqMsg.hasField("REQUEST-ID"))
					{
						rspMsg.addField(reqMsg.getField("REQUEST-ID"));
					}

					// Set the response status for the request.
					rspMsg.setFieldValue("RESPONSE-STATUS", Message.ResponseStatus.SUCCESSFUL_COMPLETION.getValue());

					// Send the Response Directive message.
					conn.reply(reqMsg, rspMsg);

					// Output in XML the message that has been sent.
					Log.info("Sent Response Message:\n" + rspMsg.toXML());

					// Destroy the response message
					Message.destroy(rspMsg);
				}

				// Acknowledge processing the request message
				reqMsg.acknowledge();

				// Destroy the request message
				Message.destroy(reqMsg);
			}
			else
			{
				// This else-block is reached if a reuqest message is not received.
				// When the timeout period is set to Gmsec.WAIT_FOREVER, then this
				// block would never be reached.
				Log.warning("Time out; no Request Directive message received");
			}

			// Disconnect from the GMSEC Bus, and terminate subscriptions.
			conn.disconnect();

			// Destroy the connection instance
			Connection.destroy(conn);
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect toi
			// the GMSEC Bus.
			Log.error("GmsecException: " + e.getMessage());
		}
	}


	// Convenience function for associating standard (i.e. common) fields
	// with the MessageFactory which will be included automatically within
	// all messages created using the factory.
	private static void setStandardFields(MessageFactory factory, String component)
	{
		StringField field1 = new StringField("MISSION-ID", "MY-MISSION", true);
		StringField field2 = new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true);
		StringField field3 = new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true);
		StringField field4 = new StringField("CONSTELLATION-ID", "MY-CNST", true);
		StringField field5 = new StringField("FACILITY", "MY-FACILITY", true);
		StringField field6 = new StringField("DOMAIN1", "MY-DOMAIN-1", true);
		StringField field7 = new StringField("DOMAIN2", "MY-DOMAIN-2", true);
		StringField field8 = new StringField("COMPONENT", component, true);

		List<Field> standardFields = new ArrayList<Field>();
		standardFields.add(field1);
		standardFields.add(field2);
		standardFields.add(field3);
		standardFields.add(field4);
		standardFields.add(field5);
		standardFields.add(field6);
		standardFields.add(field7);
		standardFields.add(field8);

		factory.setStandardFields(standardFields);
	}
}
