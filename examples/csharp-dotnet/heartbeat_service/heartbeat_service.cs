/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file heartbeat_service.cs
 *
 * This file contains an example outlining how to use the Messaging Interface
 * Standardization Toolkit (MIST) namespace ConnectionManager's HeartbeatService
 * to publish GMSEC-compliant Heartbeat (HB) messages to the middleware bus.
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

class heartbeat_service
{
	static private String HB_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.C2CX.HB";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage heartbeat_service.exe mw-id=<middleware ID>");
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

			StringField missionField = new StringField("MISSION-ID", "GMSEC");
			StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
			StringField componentField = new StringField("COMPONENT", "heartbeat_service");

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

			// Note: Fields are immutable, so plan appropriately if you wish
			// to re-use variable names!
			{
				//o Create all of the GMSEC Message header Fields which
				// will be used by all GMSEC HB Messages
				List<Field> hbStandardFields = new List<Field>();

				//o Determine which version of the GMSEC message specification
				// the ConnectionManager was initialized with and add
				// the correctly typed Fields to the Message
				uint version = connManager.GetSpecification().GetVersion();
				if (version == 201600)
				{
					hbStandardFields.Add(new U16Field("PUB-RATE", (UInt16) 30));
					hbStandardFields.Add(new U16Field("COUNTER", (UInt16) 1));
				}
				else if (version == 201400)
				{
					hbStandardFields.Add(new I16Field("PUB-RATE", (short) 30));
					hbStandardFields.Add(new I16Field("COUNTER", (short) 1));
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

				hbStandardFields.Add(componentStatusField);

				//o Create and publish a Heartbeat message using
				// createLogMessage() and publish()
				//
				// Note: This is useful for applications which may need
				// to create proxy heartbeats on behalf of a subsystem,
				// as creating multiple ConnectionManagers can consume
				// more memory than necessary.  In this case, extra
				// logic would need to be added to handle the timing of
				// the publications.
				Message hbMsg = connManager.CreateHeartbeatMessage(HB_MESSAGE_SUBJECT, hbStandardFields);
				Log.Info("Publishing the GMSEC C2CX HB message which was just created using createHeartbeatMessage():\n" + hbMsg.ToXML());
				connManager.Publish(hbMsg);

				//o Kick off the Heartbeat Service -- This will publish
				// heartbeat messages automatically every X seconds,
				// where Xis the value which was provided for PUB-RATE
				// Note: If PUB-RATE was not provided, it will default
				// to 30 seconds per automatic Heartbeat publication
				Log.Info("Starting the Heartbeat service, a message will be published every " + hbStandardFields[0].GetStringValue() + " seconds");
				connManager.StartHeartbeatService(HB_MESSAGE_SUBJECT, hbStandardFields);
			}

			{
				//o Use setHeartbeatServiceField to change the state of the
				// COMPONENT-STATUS Field to indicate that the component has
				// transitioned from a startup/debug state to a running/green
				// state.
				I16Field componentStatusField = new I16Field("COMPONENT-STATUS", (short) 1);
				connManager.SetHeartbeatServiceField(componentStatusField);
			}

			//o Wait for user input to end the program
			Log.Info("Publishing C2CX Heartbeat Messages indefinitely, press <enter> to exit the program");
			Console.ReadLine();

			//o Stop the Heartbeat Service
			connManager.StopHeartbeatService();

			connManager.Cleanup();
		}
		catch (GMSEC_Exception e)
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
