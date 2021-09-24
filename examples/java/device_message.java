/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file device_message.java
 *
 * This file contains an example outlining how to create and publish a
 * GMSEC-compliant Mnemonic Data Value message to the middleware bus using the
 * Messaging Interface Standardization Toolkit (MIST) namespace
 * ConnectionManager class function calls available for such operations.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 * bus.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.field.I16Field;
import gov.nasa.gsfc.gmsec.api.field.I32Field;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage;
import gov.nasa.gsfc.gmsec.api.mist.Device;
import gov.nasa.gsfc.gmsec.api.mist.DeviceParam;
import java.util.ArrayList;

public class device_message
{
	public static final String DEV_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.DEV.DEVICE_MESSAGE.DEV";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java device_message logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		initializeLogging(config);

		//o Enable Message validation.  This parameter is "false" by default.
		config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.info("Opening the connection to the middleware server");
			connManager.initialize();

			Log.info(connManager.getLibraryVersion());

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
			StringField componentField = new StringField("COMPONENT", "device_message");

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

			I32Field paramVal = new I32Field("DEVICE.1.PARAM.1.VALUE", 79);
			DeviceParam param = new DeviceParam("DEV parameter 1", "parameter 1 timestamp", paramVal);

			Device device1 = new Device("device 1", Device.DeviceStatus.RED);
			device1.setGroup("group");
			device1.setRole("role");
			device1.setModel("model");
			device1.setSerial("1138");
			device1.setVersion("1.4.5.2.3.4.5");
			I16Field devInfo = new I16Field("info", (short)5);
			device1.setInfo(devInfo);
			I16Field devNum = new I16Field("num", (short)5);
			device1.setNumber(devNum);
			device1.addParam(param);

			//o Construct an DEV Message and add the Device values to it
			DeviceMessage devMessage = new DeviceMessage(DEV_MESSAGE_SUBJECT, connManager.getSpecification());
			devMessage.addDevice(device1);

			connManager.addStandardFields(devMessage);

			connManager.publish(devMessage);

			Log.info("Published DEV message:\n" + devMessage.toXML());

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
