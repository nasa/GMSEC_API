/*
 * Copyright 2007-2018 United States Government as represented by the
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
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;
using System.Collections.Generic;

class connection_state
{
	static String RSRC_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.RESOURCE_SERVICE.RSRC";
	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage resource_service.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		InitializeLogging(config);

		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connManager.Initialize();

			Log.Info(connManager.GetLibraryVersion());

			//o Create all of the GMSEC Message header Fields which will
			// be used by all GMSEC Messages
			List<Field> headerFields = new List<Field>();

			F32Field versionField = new F32Field("HEADER-VERSION", 2010.0f);
			StringField missionField = new StringField("MISSION-ID", "GMSEC");
			StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
			StringField componentField = new StringField("COMPONENT", "heartbeat_service");

			headerFields.Add(versionField);
			headerFields.Add(missionField);
			headerFields.Add(facilityField);
			headerFields.Add(componentField);

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
			int interval_s = 30;
			Log.Info("Starting the Resource Message service, a message will be published every " + interval_s + " seconds");
			connManager.StartResourceMessageService(RSRC_MESSAGE_SUBJECT, interval_s, 1, 10);

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
