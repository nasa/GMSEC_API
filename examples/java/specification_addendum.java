/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file specification_addendum.java
 * 
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.StringField;
import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;

import java.util.ArrayList;
import java.util.List;


public class specification_addendum
{
	public static void main(String[] args)
	{
		// Set up connection configuration options from the command line.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);


		// Enable Message validation. This setting is "false" by default.
		config.addValue("GMSEC-MSG-CONTENT-VALIDATE-SEND", "true");


		// Tell the API that there is an additional layer of message schema to
		// validate (The 'EXAMPLE' message definitions). By default, this value
		// is set to 0 (only 'C2MS' specification validation).
		//
		// Note: These levels for validation are determined by the "LEVEL-X"
		// attributes defined in the DIRECTORY.xml file contained in the XML
		// templates directory. In the case of this example, Level-0 implies
		// C2MS, and Level-1 is our example extension.
		//
		// Note: The GMSEC team envisions using message specifications in a
		// layered hierarchical fashion. For example, the "C2MS" message
		// specification would be 'layer 0', followed by an organization-level
		// message specification at 'layer 1' which builds upon the message
		// specification outlined in C2MS.  This would be followed by a mission
		// level message specification at 'layer 2' (e.g. GMSEC) and so on.
		config.addValue("GMSEC-SCHEMA-LEVEL", "1");


		// Tell the API where to find all of the schema files.
		//
		// Note: This example only demonstrates a simple usage case of this
		// functionality. When using this functionality, if the intent is
		// to use any of the GMSEC message definitions, then ALL of the XML
		// template files must be contained in the same directory.
		// e.g. GMSEC_API/templates/2019.00 (or the location defined in
		// GMSEC-SCHEMA-PATH)
		config.addValue("GMSEC-SCHEMA-PATH", "templates");


		// Since this example relies on the 2019.00 version of the templates,
		// we indicate such within the configuration object.
		config.addValue("GMSEC-SPECIFICATION-VERSION", "201900");


		try
		{
			// Create connection instance using custom configuration.
			Connection conn = Connection.create(config);

			// Set up standard fields within the MessageFactory associated
			// with the connection object.
			setStandardFields(conn.getMessageFactory(), "SPECIFICATION-ADDENDUM");

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Output information regarding the API version and connection.
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());


			// Create a LOG Message using a subject defined in the XML template
			// outlining our example addendum message definitions.
			Message logMsg = conn.getMessageFactory().createMessage("LOG");

			// Add all of the required fields to the LOG message.
			logMsg.addField("NUM-OF-EVENTS", new U16(2));
			logMsg.addField("EVENT.1.DESC", addTimeToString("AOS occurred at: "));
			logMsg.addField("EVENT.2.DESC", addTimeToString("Telemetry downlink began at: "));

			// Publish the LOG message to the GMSEC Bus.
			conn.publish(logMsg);

			// Display the XML string representation of the LOG message just published.
			Log.info("Published custom LOG Message:\n" + logMsg.toXML());


			// Setup a new LOG message without the NUM-OF-EVENTS required field and
			// attempt to publish it (i.e. trigger a validation failure)
			Message badLogMsg = conn.getMessageFactory().createMessage("LOG");

			try
			{
				conn.publish(badLogMsg);
				Log.warning("Surprisingly was able to publish an invalid message:\n" + badLogMsg.toXML());
			}
			catch (GmsecException e)
			{
				Log.info("This validation error is expected for the incomplete LOG message:\n" + e.getMessage());
			}

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


	// Convenience method for associating standard (i.e. common) fields
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


	private static String addTimeToString(String preface)
	{
		return preface + TimeUtil.formatTime(TimeUtil.getCurrentTime());
	}
}
