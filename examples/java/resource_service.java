/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file resource_service.java
 *
 * This file contains an example outlining how to use the Messaging Interface
 * Standardization Toolkit (MIST) namespace ConnectionManager's ResourceMessageService
 * to publish GMSEC-compliant Resource (RSRC) messages to the middleware bus.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 * bus.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.F32Field;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import java.util.Scanner;
import java.util.ArrayList;

public class resource_service
{
	static String RSRC_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.RESOURCE_SERVICE.RSRC";
	static int    RSRC_PUBLISH_RATE    = 5; // in seconds

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java resource_service mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		initializeLogging(config);

		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.info("Opening the connection to the middleware server");
			connManager.initialize();

			Log.info(connManager.getLibraryVersion());

			//o Create all of the GMSEC Message header Fields which will
			// be used by all GMSEC Messages
			ArrayList<Field> headerFields = new ArrayList<Field>();

			int version = connManager.getSpecification().getVersion();

			StringField missionField = new StringField("MISSION-ID", "MY-MISSION");
			StringField facilityField = new StringField("FACILITY", "MY-FACILITY");
			StringField componentField = new StringField("COMPONENT", "RESOURCE-SERVICE");
			StringField domain1Field = new StringField("DOMAIN1", "MY-DOMAIN-1");
			StringField domain2Field = new StringField("DOMAIN2", "MY-DOMAIN-2");
			StringField msgID = new StringField("MSG-ID", "MY-MSG-ID");

			headerFields.add(missionField);
			headerFields.add(facilityField);
			headerFields.add(componentField);

			if (version == 201400)
			{
				headerFields.add(msgID);
			}
			else if (version >=201800)
			{
				headerFields.add(domain1Field);
				headerFields.add(domain2Field);
			}

			//o Use setStandardFields to define a set of header fields for
			// all messages which are created or published on the
			// ConnectionManager using the following functions:
			// createLogMessage, publishLog, createHeartbeatMessage,
			// startHeartbeatService, createResourceMessage,
			// publishResourceMessage, or startResourceMessageService
			connManager.setStandardFields(headerFields);

			//o Create and publish a Resource message using
			// createResourceMessage() and publish()
			//
			// Note: This is useful for applications which may need to add
			// additional Fields to the Resource Messages which are not
			// currently added by the GMSEC API
			Message rsrcMsg = connManager.createResourceMessage(RSRC_MESSAGE_SUBJECT, 1, 10);
			Log.info("Publishing the GMSEC C2CX RSRC message which was created using createResourceMessage():\n" + rsrcMsg.toXML());
			connManager.publish(rsrcMsg);

			//o Kick off the Resource Service -- This will publish resource
			// messages automatically every X seconds, where X is the second
			// parameter provided to the startResourceMessageService() function.
			// If an interval is not provided, the service will default to
			// publishing a message every 60 seconds.
			Log.info("Starting the Resource Message service, a message will be published every " + RSRC_PUBLISH_RATE + " seconds");
			connManager.startResourceMessageService(RSRC_MESSAGE_SUBJECT, RSRC_PUBLISH_RATE, 1, 10);

			//o Wait for user input to end the program
			Log.info("Publishing C2CX Resource Messages indefinitely, press <enter> to exit the program");
			new Scanner(System.in).nextLine();

			//o Stop the Heartbeat Service
			connManager.stopResourceMessageService();

			//o Cleanup
			connManager.cleanup();
		}
		catch (Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}
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
