/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file heartbeat_service.java
 *
 * This file contains an example outlining how to use the Messaging Interface
 * Standardization Toolkit (MIST) namespace ConnectionManager's HeartbeatService
 * to publish GMSEC-compliant Heartbeat (HB) messages to the middleware bus.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 * bus.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.field.I16Field;
import gov.nasa.gsfc.gmsec.api.field.U16Field;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import gov.nasa.gsfc.gmsec.api.field.Field;
import java.util.Scanner;
import java.util.ArrayList;

public class heartbeat_service
{
	public static final String HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-SERVICE.HB";
	public static final int    HB_PUBLISH_RATE    = 5; // in seconds

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java heartbeat_service logging mw-id=<middleware ID>");
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
			ArrayList<Field> headerFields = new ArrayList<>();

			int version = connManager.getSpecification().getVersion();

			StringField missionField = new StringField("MISSION-ID", "MY-MISSION");
			StringField facilityField = new StringField("FACILITY", "MY-FACILITY");
			StringField componentField = new StringField("COMPONENT", "HEARTBEAT-SERVICE");
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
			else if (version >= 201800)
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

			// Note: Fields are immutable, so plan appropriately if you wish
			// to re-use variable names!
			{
				ArrayList<Field> hbStandardFields = new ArrayList<>();

				//o Determine which version of the GMSEC message specification
				// the ConnectionManager was initialized with and add
				// the correctly typed Fields to the Message
				if(version >= 201600)
				{
					hbStandardFields.add(new U16Field("PUB-RATE", new U16(HB_PUBLISH_RATE)));
					hbStandardFields.add(new U16Field("COUNTER", new U16(1)));
				}
				else if (version == 201400)
				{
					hbStandardFields.add(new I16Field("PUB-RATE",(short) HB_PUBLISH_RATE));
					hbStandardFields.add(new I16Field("COUNTER", (short) 1));
				}

				//o Note: COMPONENT-STATUS is an optional field used to
				// denote the operating status of the component, the
				// values are as follows:
				// 0 - Debug
				// 1 - Normal / Green
				// 2 - Warning / Yellow
				// 3 - Orange
				// 4 - Error / Red
				I16Field componentStatusField = new I16Field("COMPONENT-STATUS", (short) 0);

				hbStandardFields.add(componentStatusField);

				//o Create and publish a Heartbeat message using
				// createLogMessage() and publish()
				//
				// Note: This is useful for applications which may need
				// to create proxy heartbeats on behalf of a subsystem,
				// as creating multiple ConnectionManagers can consume
				// more memory than necessary.  In this case, extra
				// logic would need to be added to handle the timing of
				// the publications.
				Message hbMsg = connManager.createHeartbeatMessage(HB_MESSAGE_SUBJECT, hbStandardFields);
				Log.info("Publishing the GMSEC C2CX HB message which was just created using createHeartbeatMessage():\n" + hbMsg.toXML());
				connManager.publish(hbMsg);

				//o Kick off the Heartbeat Service -- This will publish
				// heartbeat messages automatically every X seconds,
				// where Xis the value which was provided for PUB-RATE
				// Note: If PUB-RATE was not provided, it will default
				// to 30 seconds per automatic Heartbeat publication
				Log.info("Starting the Heartbeat service, a message will be published every" + hbStandardFields.get(0).getStringValue() + " seconds");
				connManager.startHeartbeatService(HB_MESSAGE_SUBJECT, hbStandardFields);
			}

			{
				//o Use setHeartbeatServiceField to change the state of the
				// COMPONENT-STATUS Field to indicate that the component has
				// transitioned from a startup/debug state to a running/green
				// state.
				I16Field componentStatusField = new I16Field("COMPONENT-STATUS", (short) 1);
				connManager.setHeartbeatServiceField(componentStatusField);
			}

			//o Wait for user input to end the program
			Log.info("Publishing C2CX Heartbeat Messages indefinitely, press <enter> to exit the program");
			new Scanner(System.in).nextLine();

			//o Stop the Heartbeat Service
			connManager.stopHeartbeatService();

			connManager.cleanup();
		}
		catch(GMSEC_Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}
		return;
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
