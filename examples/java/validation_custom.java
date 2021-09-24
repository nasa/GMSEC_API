/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_custom.java
 *
 * This file contains an example demonstrating how to implement additional
 * Message validation logic in addition to that which the GMSEC API provides.
 */

import gov.nasa.gsfc.gmsec.api.*;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.StringField;

import gov.nasa.gsfc.gmsec.api.mist.gmsecMIST;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.MessageValidator;

import gov.nasa.gsfc.gmsec.api.mist.message.MistMessage;

import gov.nasa.gsfc.gmsec.api.util.Log;

import java.util.ArrayList;


class CustomMessageValidator extends MessageValidator
{
	public Status validateMessage(Message msg)
	{
		Status status = new Status();

		try
		{
			//o Get message type and subtype
			String type    = "";
			String subtype = "";

			if (msg.hasField("MESSAGE-TYPE"))
			{
				type = msg.getStringValue("MESSAGE-TYPE");
			}
			if (msg.hasField("C2CX-SUBTYPE"))
			{
				subtype = msg.getStringValue("C2CX-SUBTYPE");
			}
			else if (msg.hasField("MESSAGE-SUBTYPE"))
			{
				subtype = msg.getStringValue("MESSAGE-SUBTYPE");
			}

			//o Ensure we have a Heartbeat message and it contains the PUB-RATE field
			if (type.equals("MSG") && subtype.equals("HB") && msg.hasField("PUB-RATE"))
			{
				long pubRate = msg.getIntegerValue("PUB-RATE");

				//o Ensure PUB-RATE field value is between 10 and 60 (inclusive)
				if (pubRate < 10 || pubRate > 60)
				{
					status.set(StatusClassification.MSG_ERROR, StatusCode.VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value");
				}
			}
			else
			{
				status.set(StatusClassification.MSG_ERROR, StatusCode.INVALID_MSG, "Non-Heartbeat message received");
			}
		}
		catch (GMSEC_Exception e)
		{
			status = new Status(e.getErrorClassification(), e.getErrorCode(), e.getErrorMessage(), e.getCustomCode());
		}

		return status;
	}
}


public class validation_custom
{
	private static final String HB_MSG_SUBJECT     = "GMSEC.MISSION.SATELLITE.MSG.HB.VALIDATION-CUSTOM";
	private static final String ALT_HB_MSG_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.VALIDATION-CUSTOM.HB";


	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java validation_custom logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		initializeLogging(config);

		//o Enable Message validation via connection configuration
		config.addValue("gmsec-msg-content-validate-send", "true");
		config.addValue("gmsec-validation-level", "3");

		Log.info("API version: " + ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			connMgr.initialize();

			Log.info("Middleware version: " + connMgr.getLibraryVersion());

			//o Register custom message validator
			CustomMessageValidator cmv = new CustomMessageValidator();
			connMgr.registerMessageValidator(cmv);

			//o Set up standard/common fields used with all messages
			int specVersion = connMgr.getSpecification().getVersion();
			setupStandardFields(specVersion);

			//o Create Heartbeat Message
			//o Note: Message subject and schema ID vary depending on the specification in use
			final String subject  = (specVersion > gmsecMIST.GMSEC_ISD_2018_00 ? HB_MSG_SUBJECT : ALT_HB_MSG_SUBJECT);
			final String schemaID = (specVersion > gmsecMIST.GMSEC_ISD_2018_00 ? "MSG.HB" : "MSG.C2CX.HB");

			MistMessage msg = new MistMessage(subject, schemaID, connMgr.getSpecification());

			//o Add PUB-RATE field with illegal value
			msg.setValue("PUB-RATE", "5");

			//o For very old specifications, we need to add a MSG-ID field
			if (specVersion <= gmsecMIST.GMSEC_ISD_2014_00)
			{
				msg.addField("MSG-ID", "12345");
			}

			//o Attempt to publish malformed message
			try
			{
				Log.info("Attempting to publish malformed message...");
				connMgr.publish(msg);

				Log.warning("Was expecting an error");
			}
			catch (GMSEC_Exception e)
			{
				//o We expect to encounter error with the PUB-RATE field
				Log.info("This is an expected error:\n" + e.getMessage());
			}

			//o Fix PUB-RATE field with legal value
			msg.setValue("PUB-RATE", "15");

			//o Publish a good message
			try
			{
				Log.info("Attempting to publish good message...");
				connMgr.publish(msg);

				Log.info("Message published!");
			}
			catch (GMSEC_Exception e)
			{
				Log.warning("Unexpected error:\n" + e.getMessage());
			}

			//o Disconnect from the middleware and clean up the Connection
			connMgr.cleanup();

			//o Clear standard/common fields used with all messages
			clearStandardFields();
		}
		catch (GMSEC_Exception e)
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
		String logLevel = config.getValue("LOGLEVEL");
		String logFile  = config.getValue("LOGFILE");

		if (logLevel == null)
		{
			config.addValue("LOGLEVEL", "INFO");
		}
		if (logFile == null)
		{
			config.addValue("LOGFILE", "STDOUT");
		}
	}

	private static void setupStandardFields(int specVersion)
		throws GMSEC_Exception
	{
		ArrayList<Field> definedFields = new ArrayList<Field>();

		StringField mission       = new StringField("MISSION-ID", "MISSION");
		StringField constellation = new StringField("CONSTELLATION-ID", "CONSTELLATION");
		StringField satIdPhys     = new StringField("SAT-ID-PHYSICAL", "SATELLITE");
		StringField satIdLog      = new StringField("SAT-ID-LOGICAL", "SATELLITE");
		StringField facility      = new StringField("FACILITY", "GMSEC-LAB");
		StringField component     = new StringField("COMPONENT", "VALIDATION-CUSTOM");
		StringField domain1       = new StringField("DOMAIN1", "DOMAIN1");
		StringField domain2       = new StringField("DOMAIN2", "DOMAIN2");

		definedFields.add(mission);
		definedFields.add(constellation);
		definedFields.add(satIdPhys);
		definedFields.add(satIdLog);
		definedFields.add(facility);
		definedFields.add(component);

		if (specVersion >= gmsecMIST.GMSEC_ISD_2018_00)
		{
			definedFields.add(domain1);
			definedFields.add(domain2);
		}

		MistMessage.setStandardFields(definedFields);
	}

	private static void clearStandardFields()
	{
		MistMessage.clearStandardFields();
	}
}
