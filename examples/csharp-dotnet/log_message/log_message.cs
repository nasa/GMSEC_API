/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file log_message.cs
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

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using GMSEC.API.MIST.MESSAGE;
using System;
using System.Collections.Generic;

class log_message
{
	static private String GMSEC_SPEC_VERSION  = "201600";
	static private String LOG_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.LOG";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage log_message.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		InitializeLogging(config);

		//o Set the GMSEC message specification version to be used to determine
		// what the structure of messages is for verification and the
		// construction of MistMessages
		config.AddValue("GMSEC-SPECIFICATION-VERSION", GMSEC_SPEC_VERSION);

		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connManager.Initialize();

			Log.Info(connManager.GetLibraryVersion());


			//o Begin the steps necessary to create a GMSEC-compliant LOG
			// message using the ConnectionManager

			//o Create all of the GMSEC Message header Fields which will
			// be used by all GMSEC Messages
			//
			// Note: Since these Fields contain variable values which are
			// based on the context in which they are used, they cannot be
			// automatically populated using MistMessage.
			List<Field> definedFields = new List<Field>();

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

			//o Determine which version of the GMSEC message specification
			// the ConnectionManager was initialized with
			uint version = connManager.GetSpecification().GetVersion();
			String gmsecSpecVersion = "";
			if (version == 201600)
			{
				gmsecSpecVersion = "2016.00";
			}
			else if (version == 201400)
			{
				gmsecSpecVersion = "2014.00";
			}

			String msgId = gmsecSpecVersion + ".GMSEC.MSG.LOG";

			//o Use MistMessage to construct a GMSEC LOG message based off
			// of the latest XML templates provided with the GMSEC API.
			// This will automatically populate the Message with all of the
			// Fields which have specific values defined for them in the XML
			// template files.  For more information on which Fields have
			// values defined for them, please review the XML template files
			// located in GMSEC_API/templates.
			//
			// Note: The second parameter is an identifier for the type of
			// GMSEC-compliant message to construct.  This parameter varies
			// depending on the version of the GMSEC ISD that the messages
			// are based off of.  This example shows how to handle a varying
			// case.
			using(MistMessage logMsg = new MistMessage(LOG_MESSAGE_SUBJECT, msgId, connManager.GetSpecification()))
			{
				//o Add the LOG-specific fields to the LOG message
				//
				// Note: Since these Fields contain variable values which are
				// based on the context in which they are used, they cannot be
				// automatically populated using MistMessage.
				String eventTime = TimeUtil.FormatTime(TimeUtil.GetCurrentTime());

				logMsg.AddField(new I16Field("SEVERITY", (short) 1));
				logMsg.SetValue("MSG-TEXT", "Creating an example GMSEC LOG Message");
				logMsg.SetValue("OCCURRENCE-TYPE", "SYS");
				logMsg.SetValue("SUBCLASS", "AST");
				logMsg.SetValue("EVENT-TIME", eventTime);

				//o Add the standard fields to the LOG message
				connManager.AddStandardFields(logMsg);

				connManager.Publish(logMsg);

				Log.Info("Published LOG message:\n" + logMsg.ToXML());
			}

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
