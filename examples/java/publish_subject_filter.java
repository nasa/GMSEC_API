/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file publish_subject_filter.java
 * 
 * This file contains an example publishing a few messages with different
 * subjects in order to demonstrate the subject filtering functionality of the
 * API. 
 *
 * This example program is intended to be run after starting the
 * 'subscribe_subject_filter' example program.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.StringField;
import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;

import java.util.ArrayList;
import java.util.List;


public class publish_subject_filter
{
	public static void main(String[] args)
	{
		// Set up connection configuration options.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Set up standard fields within the MessageFactory associated
			// with the connection object.
			setStandardFields(conn.getMessageFactory(), "PUBLISH-SUBJECT-FILTER");

			// Establish connection to the GMSEC Bus.
			conn.connect();
		
			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Create a valid Log message using the MessageFactory.
			Message logMsg = conn.getMessageFactory().createMessage("LOG");

			// Add required fields
			logMsg.addField("SUBCLASS", "INFO");
			logMsg.addField("OCCURRENCE-TYPE", "TEST");
			logMsg.addField("SEVERITY", (short) 1);
			logMsg.addField("MSG-TEXT", "Hello subscriber!");

			// And the current (event) time
			logMsg.addField("EVENT-TIME", TimeUtil.formatTime( TimeUtil.getCurrentTime() ));

			// Create a valid Heartbeat message using the MessageFactory.
			Message hbMsg = conn.getMessageFactory().createMessage("HB");

			Log.info("Publishing two messages -- Log message will be received by the subscriber,"
			         + " the Heartbeat message will be filtered out");

			// Publish the Log message
			conn.publish(logMsg);
			Log.info("Published LOG message:\n" + logMsg.toXML());

			// Delay a bit
			TimeUtil.millisleep(2000);

			// Publish the Heartbeat message
			conn.publish(hbMsg);
			Log.info("Published HB message:\n" + hbMsg.toXML());

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
