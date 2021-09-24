/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file device_message.cs
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

using GMSEC.API;
using System;
using System.Collections.Generic;

class device_message
{
	static private String DEV_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.DEV.DEVICE_MESSAGE.DEV";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage device_message.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		InitializeLogging(config);

		//o Enable Message validation.  This parameter is "false" by default.
		config.AddValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connManager.Initialize();

			Log.Info("Middleware version: " + connManager.GetLibraryVersion());

			//o Create all of the GMSEC Message header Fields which will
			// be used by all GMSEC Messages
			//
			// Note: Since these Fields contain variable values which are
			// based on the context in which they are used, they cannot be
			// automatically populated using MistMessage.
			FieldList definedFields = new FieldList();

			StringField missionField = new StringField("MISSION-ID", "MISSION");
			// Note: SAT-ID-PHYSICAL is an optional header Field, according
			// to the GMSEC ISD.
			StringField satIdField = new StringField("SAT-ID-PHYSICAL", "SPACECRAFT");
			StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
			StringField componentField = new StringField("COMPONENT", "device_message");

			definedFields.Add(missionField);
			definedFields.Add(satIdField);
			definedFields.Add(facilityField);
			definedFields.Add(componentField);

			//o Use setStandardFields to define a set of header fields for
			// all messages which are created or published on the
			// ConnectionManager using the following functions:
			// createLogMessage, publishLog, createHeartbeatMessage,
			// startHeartbeatService, createResourceMessage,
			// publishResourceMessage, or startResourceMessageService
			connManager.SetStandardFields(definedFields);

			I32Field paramVal = new I32Field("DEVICE.1.PARAM.1.VALUE", 79);
			DeviceParam param = new DeviceParam("DEV parameter 1", "parameter 1 timestamp", paramVal);

			Device device1 = new Device("device 1", Device.DeviceStatus.RED);
			device1.SetGroup("group");
			device1.SetRole("role");
			device1.SetModel("model");
			device1.SetSerial("1138");
			device1.SetVersion("1.4.5.2.3.4.5");
			I16Field devInfo = new I16Field("info", 5);
			device1.SetInfo(devInfo);
			I16Field devNum = new I16Field("num", 5);
			device1.SetNumber(devNum);
			device1.AddParam(param);

			//o Construct an DEV Message and add the Device values to it
			using (DeviceMessage devMessage = new DeviceMessage(DEV_MESSAGE_SUBJECT, connManager.GetSpecification()))
			{
				devMessage.AddDevice(device1);

				connManager.AddStandardFields(devMessage);

				connManager.Publish(devMessage);

				Log.Info("Published DEV message:\n" + devMessage.ToXML());
			}

			connManager.Cleanup();
		}
		catch (GmsecException e)
		{
			Log.Error(e.ToString());
			return -1;
		}

		return 0;
	}

	static void InitializeLogging(Config config)
	{
		String logLevel = config.GetValue("LOGLEVEL");
		String logFile = config.GetValue("LOGFILE");

		if (logLevel == null)
		{
			config.AddValue("LOGLEVEL", "INFO");
		}
		if (logFile == null)
		{
			config.AddValue("LOGFILE", "STDERR");
		}
	}
}
