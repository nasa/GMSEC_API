/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file publish.java
 *
 * @brief This example program provides a basic demonstration of publishing
 * a (heartbeat) message to the GMSEC Bus.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.ArrayList;
import java.util.List;


public class publish
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
			setStandardFields(conn.getMessageFactory(), "PUBLISH");

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Create a Heartbeat message using the MessageFactory.
			Message hbMsg = conn.getMessageFactory().createMessage("HB");

			// Publish the message
			conn.publish(hbMsg);

			// Output in XML what we have published
			Log.info("Publishing Message:\n" + hbMsg.toXML());

			// Disconnect from the GMSEC Bus.
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
