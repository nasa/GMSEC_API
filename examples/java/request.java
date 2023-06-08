/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file request.java
 *
 * @brief This example program provides a basic demonstration of issuing
 * a synchronous Request Directive message to the GMSEC Bus, and then
 * handling the receipt of a Response Directive message.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.ArrayList;
import java.util.List;


public class request
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

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Set up standard fields within the MessageFactory associated
			// with the connection object.
			setStandardFields(conn.getMessageFactory(), "REQUEST");

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Create a Request Directive message using the MessageFactory.
			// Set both required and optional fields within the message.
			Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			reqMsg.addField(new StringField("DESTINATION-COMPONENT", "REPLY", true));   // to whom is the request intended for
			reqMsg.addField("DIRECTIVE-STRING", "Turn on the lights");                  // operation to perform
			reqMsg.addField("RESPONSE", true);                                          // a response message is requested
			reqMsg.addField("REQUEST-ID", new U16(0));

			// Output in XML what we intend to send
			Log.info("Sending Request Message:\n" + reqMsg.toXML());

			// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
			// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
			// prevent re-sending request message should a timeout occur.
			int timeout   = 5000;
			int republish = 5000;

			// Issue Request Directive.
			// Note, this is a blocking call (i.e. a synchronous operation).
			Message rspMsg = conn.request(reqMsg, timeout, republish);

			// We have returned from the previous blocking call; check if we have a message.
			if (null != rspMsg)
			{
				Log.info("Received Response Directive message:\n" + rspMsg.toXML());

				// Acknowledge processing the response message
				rspMsg.acknowledge();

				// Destroy the response message
				Message.destroy(rspMsg);
			}
			else
			{
				// This else-block is reached if a response message is not received.
				// If the republish period is set to a time period other than
				// GMSEC_REQUEST_REPUBLISH_NEVER, then this block will never be reached.
				Log.warning("Time out; no Response Directive message received");
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
