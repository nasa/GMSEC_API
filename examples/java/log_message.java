/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file log_message.java
 *
 * This file contains an example outlining how to create and publish a
 * GMSEC-compliant LOG message to the middleware bus using the Messaging
 * Interface Standardization Toolkit (MIST) namespace ConnectionManager class
 * function calls available for such operations.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 * bus.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.util.TimeUtil;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.field.I16Field;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.mist.message.MistMessage;
import java.util.ArrayList;

public class log_message
{
	public static final String GMSEC_SPEC_VERSION  = "201600";
	public static final String LOG_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.LOG";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java log_message logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		initializeLogging(config);

		//o Set the GMSEC message specification version to be used to determine
		// what the structure of messages is for verification and the
		// construction of MistMessages
		config.addValue("GMSEC-SPECIFICATION-VERSION", GMSEC_SPEC_VERSION);

		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.info("Opening the connection to the middleware server");
			connManager.initialize();

			Log.info(connManager.getLibraryVersion());

			//o Begin the steps necessary to create a GMSEC-compliant LOG
			// message using the ConnectionManager

			//o Create all of the GMSEC Message header Fields which will
			// be used by all GMSEC Messages
			//
			// Note: Since these Fields contain variable values which are
			// based on the context in which they are used, they cannot be
			// automatically populated using MistMessage.
			ArrayList<Field> definedFields = new ArrayList<>();

			StringField missionField = new StringField("MISSION-ID", "MISSION");
			// Note: SAT-ID-PHYSICAL is an optional header Field, according
			// to the GMSEC ISD.
			StringField satIdField = new StringField("SAT-ID-PHYSICAL", "SPACECRAFT");
			StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
			StringField componentField = new StringField("COMPONENT", "log_message");

			definedFields.add(missionField);
			definedFields.add(satIdField);
			definedFields.add(facilityField);
			definedFields.add(componentField);

			//o Use setStandardFields to define a set of header fields for
			// all messages which are created or published on the
			// ConnectionManager using the following functions:
			// createLogMessage, publishLog, createHeartbeatMessage,
			// startHeartbeatService, createResourceMessage,
			// publishResourceMessage, or startResourceMessageService
			connManager.setStandardFields(definedFields);

			//o Use MistMessage to construct a GMSEC LOG message based off
			// of the latest XML templates provided with the GMSEC API.
			// This will automatically populate the Message with all of the
			// Fields which have specific values defined for them in the XML
			// template files.  For more information on which Fields have
			// values defined for them, please review the XML template files
			// located in GMSEC_API/templates.
			//
			// Note: The second parameter is an identifier for the type of
			// message to construct.
			MistMessage logMsg = new MistMessage(LOG_MESSAGE_SUBJECT, "MSG.LOG", connManager.getSpecification());

			//o Add the LOG-specific fields to the LOG message
			//
			// Note: Since these Fields contain variable values which are
			// based on the context in which they are used, they cannot be
			// automatically populated using MistMessage.
			String eventTime = TimeUtil.formatTime(TimeUtil.getCurrentTime());

			logMsg.addField("SEVERITY", (short) 1);
			logMsg.setValue("MSG-TEXT", "Creating an example GMSEC LOG Message");
			logMsg.setValue("OCCURRENCE-TYPE", "SYS");
			logMsg.setValue("SUBCLASS", "AST");
			logMsg.setValue("EVENT-TIME", eventTime);

			//o Add the standard fields to the LOG message
			connManager.addStandardFields(logMsg);

			connManager.publish(logMsg);

			Log.info("Published LOG message:\n" + logMsg.toXML());

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
