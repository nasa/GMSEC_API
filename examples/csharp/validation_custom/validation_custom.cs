/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_custom.cs
 *
 * This file contains an example demonstrating how to implement additional
 * Message validation logic in addition to that which the GMSEC API provides.
 */

using GMSEC.API;
using System;
using System.Collections.Generic;


class CustomMessageValidator : MessageValidator
{
	public override Status ValidateMessage(Message msg)
	{
		Status status = new Status();

		try
		{
			//o Get message type and subtype
			string type    = "";
			string subtype = "";

			if (msg.HasField("MESSAGE-TYPE"))
			{
				type = msg.GetStringValue("MESSAGE-TYPE");
			}
			if (msg.HasField("C2CX-SUBTYPE"))
			{
				subtype = msg.GetStringValue("C2CX-SUBTYPE");
			}
			else if (msg.HasField("MESSAGE-SUBTYPE"))
			{
				subtype = msg.GetStringValue("MESSAGE-SUBTYPE");
			}

			//o Ensure we have a Heartbeat message and it contains the PUB-RATE field
			if (type == "MSG" && subtype == "HB" && msg.HasField("PUB-RATE"))
			{
				Int64 pubRate = msg.GetIntegerValue("PUB-RATE");

				//o Ensure PUB-RATE field value is between 10 and 60 (inclusive)
				if (pubRate < 10 || pubRate > 60)
				{
					status.Set(StatusClass.MSG_ERROR, StatusCode.VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value");
				}
			}
			else
			{
				status.Set(StatusClass.MSG_ERROR, StatusCode.INVALID_MSG, "Non-Heartbeat message received");
			}
		}
		catch (GmsecException e)
		{
			status = new Status(e.GetErrorClass(), e.GetErrorCode(), e.GetErrorMessage(), e.GetCustomCode());
		}

		return status;
	}
}


class validation_custom
{
	static private string HB_MSG_SUBJECT     = "GMSEC.MISSION.SATELLITE.MSG.HB.VALIDATION-CUSTOM";
	static private string ALT_HB_MSG_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.VALIDATION-CUSTOM.HB";


	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage validation_custom.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		InitializeLogging(config);

		//o Enable Message validation.  This parameter is "false" by default.
		config.AddValue("gmsec-msg-content-validate-send", "true");
		config.AddValue("gmsec-validation-level", "3");

		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			connMgr.Initialize();

			Log.Info("Middleware version: " + connMgr.GetLibraryVersion());

			//o Register custom message validator
			CustomMessageValidator cmv = new CustomMessageValidator();
			connMgr.RegisterMessageValidator(cmv);

			//o Create and publish a simple Product File Message
			uint specVersion = connMgr.GetSpecification().GetVersion();
			SetupStandardFields(specVersion);

			//o Create Heartbeat Message
			//o Note: Message subject and schema ID vary depending on the specification in use
			string subject  = (specVersion > Gmsec.GMSEC_ISD_2018_00 ? HB_MSG_SUBJECT : ALT_HB_MSG_SUBJECT);
			string schemaID = (specVersion > Gmsec.GMSEC_ISD_2018_00 ? "MSG.HB" : "MSG.C2CX.HB");

			MistMessage msg = new MistMessage(subject, schemaID, connMgr.GetSpecification());

			//o Add PUB-RATE field with illegal value
			msg.SetValue("PUB-RATE", "5");

			//o For very old specifications, we need to add a MSG-ID field
			if (specVersion <= Gmsec.GMSEC_ISD_2014_00)
			{
				msg.AddField("MSG-ID", "12345");
			}

			//o Attempt to publish malformed message
			try
			{
				Log.Info("Attempting to publish malformed message...");
				connMgr.Publish(msg);

				Log.Warning("Was expecting an error");
			}
			catch (GmsecException e)
			{
				//o We expect to encounter error with the PUB-RATE field
				Log.Info("This is an expected error:\n" + e.ToString());
			}

			//o Fix PUB-RATE field with legal value
			msg.SetValue("PUB-RATE", "15");

			//o Publish a good message
			try
			{
				Log.Info("Attempting to publish good message...");
				connMgr.Publish(msg);

				Log.Info("Message published!");
			}
			catch (GmsecException e)
			{
				Log.Warning("Unexpected error:\n" + e.ToString());
			}

			//o Disconnect from the middleware and clean up the Connection
			connMgr.Cleanup();

			//o Clear standard/common fields used with all messages
			ClearStandardFields();
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

	static void SetupStandardFields(uint specVersion)
	{
		FieldList definedFields = new FieldList();

		StringField mission       = new StringField("MISSION-ID", "MISSION");
		StringField constellation = new StringField("CONSTELLATION-ID", "CONSTELLATION");
		StringField satIdPhys     = new StringField("SAT-ID-PHYSICAL", "SATELLITE");
		StringField satIdLog      = new StringField("SAT-ID-LOGICAL", "SATELLITE");
		StringField facility      = new StringField("FACILITY", "GMSEC-LAB");
		StringField component     = new StringField("COMPONENT", "VALIDATE-CUSTOM");

		definedFields.Add(mission);
		definedFields.Add(constellation);
		definedFields.Add(satIdPhys);
		definedFields.Add(satIdLog);
		definedFields.Add(facility);
		definedFields.Add(component);

		if (specVersion >= Gmsec.GMSEC_ISD_2018_00)
		{
			StringField domain1 = new StringField("DOMAIN1", "DOMAIN1");
			StringField domain2 = new StringField("DOMAIN2", "DOMAIN2");

			definedFields.Add(domain1);
			definedFields.Add(domain2);
		}

		MistMessage.SetStandardFields(definedFields);
	}

	static void ClearStandardFields()
	{
		MistMessage.ClearStandardFields();
	}
}
