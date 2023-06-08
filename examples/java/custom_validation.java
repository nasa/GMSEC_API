/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file custom_validation.java
 *
 * @brief This example program demonstrates how to implement additional
 * message validation logic in addition to that which the GMSEC API
 * provides.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.ArrayList;
import java.util.List;


// Define a custom message validator
class CustomMessageValidator extends MessageValidator
{
	public Status validateMessage(Message msg)
	{
		Status status = new Status();

		// Check if we have a Heartbeat message, and if so, then check if
		// the PUB-RATE has an acceptable value.
		try
		{
			if (msg.getStringValue("MESSAGE-SUBTYPE").equals("HB"))
			{
				if (msg.hasField("PUB-RATE"))
				{
					int pubRate = msg.getI32Value("PUB-RATE");

					if (pubRate < 10 || pubRate > 60)
					{
						status.set(StatusClassification.MSG_ERROR, StatusCode.VALUE_OUT_OF_RANGE,
						           "PUB-RATE field does not have a valid value");
					}
				}
				else
				{
					status.set(StatusClassification.MSG_ERROR, StatusCode.MISSING_REQUIRED_FIELD,
					           "PUB-RATE field not found");
				}
			}
		}
		catch (GmsecException e)
		{
			status = new Status(e);
		}

		return status;
	}
}


public class custom_validation
{
	public static void main(String[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Validate messages before being sent to the bus
		config.addValue("gmsec-msg-content-validate-send", "true");

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Set up standard fields within the MessageFactory associated
			// with the connection object.
			setStandardFields(conn.getMessageFactory(), "CUSTOM-VALIDATION");

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Register custom message validator
			CustomMessageValidator cmv = new CustomMessageValidator();
			conn.getMessageFactory().registerMessageValidator(cmv);

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Create a Heartbeat message using the MessageFactory.
			// Set both required and optional fields within the message.
			Message hbMsg = conn.getMessageFactory().createMessage("HB");

			// Publish the message without PUB-RATE (expecting custom validation error)
			try {
				Log.info("Publishing Message:\n" + hbMsg.toXML());
				conn.publish(hbMsg);

				Log.warning("An error was expected!");
			}
			catch (GmsecException e) {
				Log.info("This error is expected: " + e.getMessage());
			}

			// Publish the message with invalid PUB-RATE value (expecting custom validation error)
			try {
				hbMsg.setFieldValue("PUB-RATE", 5);

				Log.info("Publishing Message:\n" + hbMsg.toXML());
				conn.publish(hbMsg);

				Log.warning("An error was expected!");
			}
			catch (GmsecException e) {
				Log.info("This error is expected: " + e.getMessage());
			}

			// Publish the message with valid PUB-RATE value (expecting no validation error)
			try {
				hbMsg.setFieldValue("PUB-RATE", 10);

				Log.info("Publishing Message:\n" + hbMsg.toXML());
				conn.publish(hbMsg);

				Log.info("Message is valid!");
			}
			catch (GmsecException e) {
				Log.error("Unexpected error: " + e.getMessage());
			}

			// Disconnect from the GMSEC Bus
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
