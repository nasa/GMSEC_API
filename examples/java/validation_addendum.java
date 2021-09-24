/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_addendum.java
 *
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.mist.message.MistMessage;
import gov.nasa.gsfc.gmsec.api.util.TimeUtil;
import java.util.ArrayList;

public class validation_addendum
{
	//o Note: The name 'EXAMPLE' in this subject represents the name of the message
	// specification which is being used.  If using message definitions outlined in
	// the GMSEC Interface Specification Document, this would instead be, "GMSEC."
	public static final String EXAMPLE_MESSAGE_SUBJECT = "EXAMPLE.MISSION.SPACECRAFT.MSG.LOG";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java validation_addendum logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		initializeLogging(config);

		//o Enable Message validation.  This parameter is "false" by default.
		config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

		//o Tell the API that there is an additional layer of message schema to
		// validate (The 'EXAMPLE' message definitions).  This value is set to
		// 0 (Only 'GMSEC' specification validation) by default.
		//
		// Note: These levels for validation are determined by the "LEVEL-X"
		// attributes defined in the .DIRECTORY.xml file contained in the XML
		// templates directory.  In thise case, Level-0 means GMSEC and Level-1
		// means EXAMPLE.
		//
		// Note: The GMSEC team envisions using message specifications in a
		// layered hierarchical fashion.  For example, the "GMSEC" message
		// specification would be 'layer 0', followed by an organization-level
		// message specification at 'layer 1' which builds upon the message
		// specification outlined in the GMSEC ISD.  This would be followed by
		// a mission-level message specification at 'layer 2' and so on.
		config.addValue("GMSEC-SCHEMA-LEVEL", "1");

		//o Tell the API where to find all of the schema files.
		//
		// Note: This example only demonstrates a simple usage case of this
		// functionality.  When using this functionality, if the intent is
		// to use any of the GMSEC message definitions, then ALL of the XML
		// template files must be contained in the same directory.
		// e.g. GMSEC_API/templates/2016.00 (Or the location defined in
		// GMSEC-SCHEMA-PATH)
		config.addValue("GMSEC-SCHEMA-PATH", "templates");

		//o Since this example relies on the 2016.00 version of the templates,
		//  we indicate such within the configuration object.
		config.addValue("GMSEC-SPECIFICATION-VERSION", "201600");

		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			Log.info("Opening the connection to the middleware server");
			connMgr.initialize();

			Log.info(connMgr.getLibraryVersion());

			ArrayList<Field> definedFields = new ArrayList<>();

			StringField missionField = new StringField("MISSION-ID", "MISSION");
			StringField satIdField = new StringField("SAT-ID-PHYSICAL", "SPACECRAFT");
			StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
			StringField componentField = new StringField("COMPONENT", "log_message");

			definedFields.add(missionField);
			definedFields.add(satIdField);
			definedFields.add(facilityField);
			definedFields.add(componentField);

			connMgr.setStandardFields(definedFields);

			//o Create a Message using a subject defined in the XML template
			// outlining our example addendum message definitions
			MistMessage message = new MistMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.getSpecification());

			//o Add all of the necessary Fields to our message
			message.addField("NUM-OF-EVENTS", new U16(2));
			message.setValue("EVENT.1", addTimeToString("AOS occurred at: "));
			message.setValue("EVENT.2", addTimeToString("Telemetry downlink began at: "));

			connMgr.addStandardFields(message);

			//o Publish the message to the middleware bus
			connMgr.publish(message);

			//o Display the XML string representation of the Message for
			// the sake of review
			Log.info("Published message:\n" + message.toXML());

			//o Setup a new message without some of the Required Fields and
			// attempt to publish it (i.e. Trigger a validation failure)
			MistMessage badMessage = new MistMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.getSpecification());

			try
			{
				connMgr.publish(badMessage);
			}
			catch (Exception e)
			{
				Log.error("This error is expected:\n" + e.getMessage());
			}

			//o Disconnect from the middleware and clean up the Connection
			connMgr.cleanup();
		}
		catch (GMSEC_Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}

		return;
	}

	public static String addTimeToString(String preface)
	{
		String time = TimeUtil.formatTime(TimeUtil.getCurrentTime());
		String result = preface + time;
		return result;
	}

	public static void initializeLogging(Config config)
	{
		// If it was not specified in the command-line arguments, set
		// LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the
		// program report output on the terminal/command line
		try
		{
			String logLevel = config.getValue("LOGLEVEL");
			String logFile = config.getValue("LOGFILE");

			if (logLevel == null)
			{
				config.addValue("LOGLEVEL", "INFO");
			}
			if (logFile == null)
			{
				config.addValue("LOGFILE", "STDOUT");
			}
		}
		catch(Exception e)
		{
		  Log.error(e.toString());
		}
  }
}
