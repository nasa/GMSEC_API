/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file resource_service.cs
 *
 * This file contains an example outlining how to use the Messaging Interface
 * Standardization Toolkit (MIST) namespace ConnectionManager's ResourceMessageService
 * to publish GMSEC-compliant Resource (RSRC) messages to the middleware bus.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 * bus.
 */

using GMSEC.API;
using System;
using System.Collections.Generic;

class connection_state
{
	static String RSRC_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.RESOURCE-SERVICE.RSRC";
	static int    RSRC_PUBLISH_RATE    = 5; // in seconds

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage resource_service.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		InitializeLogging(config);

		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connManager.Initialize();

			Log.Info("Middleware version: " + connManager.GetLibraryVersion());

			//o Create all of the GMSEC Message header Fields which will
			// be used by all GMSEC Messages
			FieldList headerFields = new FieldList();

			uint version = connManager.GetSpecification().GetVersion();

			StringField missionField = new StringField("MISSION-ID", "MY-MISSION");
			StringField facilityField = new StringField("FACILITY", "MY-FACILITY");
			StringField componentField = new StringField("COMPONENT", "RESOURCE-SERVICE");
			StringField domain1Field = new StringField("DOMAIN1", "MY-DOMAIN-1");
			StringField domain2Field = new StringField("DOMAIN2", "MY-DOMAIN-2");
			StringField msgID = new StringField("MSG-ID", "MY-MSG-ID");

			headerFields.Add(missionField);
			headerFields.Add(facilityField);
			headerFields.Add(componentField);

			if (version == 201400)
			{
				headerFields.Add(msgID);
			}
			else if (version >= 201800)
			{
				headerFields.Add(domain1Field);
				headerFields.Add(domain2Field);
			}

			//o Use setStandardFields to define a set of header fields for
			// all messages which are created or published on the
			// ConnectionManager using the following functions:
			// createLogMessage, publishLog, createHeartbeatMessage,
			// startHeartbeatService, createResourceMessage,
			// publishResourceMessage, or startResourceMessageService
			connManager.SetStandardFields(headerFields);

			//o Create and publish a Resource message using
			// createResourceMessage() and publish()
			//
			// Note: This is useful for applications which may need to add
			// additional Fields to the Resource Messages which are not
			// currently added by the GMSEC API
			Message rsrcMsg = connManager.CreateResourceMessage(RSRC_MESSAGE_SUBJECT, 1, 10);
			Log.Info("Publishing the GMSEC C2CX RSRC message which was created using createResourceMessage():\n" + rsrcMsg.ToXML());
			connManager.Publish(rsrcMsg);

			//o Kick off the Resource Service -- This will publish resource
			// messages automatically every X seconds, where X is the second
			// parameter provided to the startResourceMessageService() function.
			// If an interval is not provided, the service will default to
			// publishing a message every 60 seconds.
			Log.Info("Starting the Resource Message service, a message will be published every " + RSRC_PUBLISH_RATE + " seconds");
			connManager.StartResourceMessageService(RSRC_MESSAGE_SUBJECT, RSRC_PUBLISH_RATE, 1, 10);

			//o Wait for user input to end the program
			Log.Info("Publishing C2CX Resource Messages indefinitely, press <enter> to exit the program");
			Console.ReadLine();

			//o Stop the Heartbeat Service
			connManager.StopResourceMessageService();

			//o Cleanup
			connManager.Cleanup();
		}
		catch (Exception e)
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
