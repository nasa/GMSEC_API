/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_addendum.cpp
 *
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using GMSEC.API.MIST.MESSAGE;
using System;
using System.Collections.Generic;

class validation_addendum
{
	//o Note: The name 'EXAMPLE' in this subject represents the name of the message
	// specification which is being used.  If using message definitions outlined in
	// the GMSEC Interface Specification Document, this would instead be, "GMSEC."
	static private String EXAMPLE_MESSAGE_SUBJECT = "EXAMPLE.MISSION.SPACECRAFT.MSG.LOG";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage validation_addendum.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		//o Since this example program uses an invalid message, we ensure the
		//  validation check is disabled.
		config.AddValue("gmsec-msg-content-validate-all", "false");

		InitializeLogging(config);

		//o Enable Message validation.  This parameter is "false" by default.
		config.AddValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

		//o Tell the API that there is an additional layer of message schema to
		// validate (The 'EXAMPLE' message definitions).  This value is set to
		// 0 (Only 'GMSEC' specification validation) by default.
		//
		// Note: These levels for validation are determined by the "LEVEL-X"
		// attributes defined in the .DIRECTORY.xml file contained in the XML
		// templates directory.  In thise case, Level-0 means GMSEC and Level-1
		// means EXAMPLE.
		//
		// Note: The GMSEC team envisions using message specifications in a
		// layered hierarchical fashion.  For example, the "GMSEC" message
		// specification would be 'layer 0', followed by an organization-level
		// message specification at 'layer 1' which builds upon the message
		// specification outlined in the GMSEC ISD.  This would be followed by
		// a mission-level message specification at 'layer 2' and so on.
		config.AddValue("GMSEC-SCHEMA-LEVEL", "1");

		//o Tell the API where to find all of the schema files.
		//
		// Note: This example only demonstrates a simple usage case of this
		// functionality.  When using this functionality, if the intent is
		// to use any of the GMSEC message definitions, then ALL of the XML
		// template files must be contained in the same directory.
		// e.g. GMSEC_API/templates/2016.00 (Or the location defined in
		// GMSEC-SCHEMA-PATH)
		config.AddValue("GMSEC-SCHEMA-PATH", "templates");

		//o Since this example relies on the 2016.00 version of the templates,
		//  we indicate such within the configuration object.
		config.AddValue("GMSEC-SPECIFICATION-VERSION", "201600");

		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connMgr.Initialize();

			Log.Info(connMgr.GetLibraryVersion());

			List<Field> definedFields = new List<Field>();

			StringField missionField = new StringField("MISSION-ID", "MISSION");
			StringField satIdField = new StringField("SAT-ID-PHYSICAL", "SPACECRAFT");
			StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
			StringField componentField = new StringField("COMPONENT", "device_message");

			definedFields.Add(missionField);
			definedFields.Add(satIdField);
			definedFields.Add(facilityField);
			definedFields.Add(componentField);

			connMgr.SetStandardFields(definedFields);

			//o Create a Message using a subject defined in the XML template
			// outlining our example addendum message definitions
			using(MistMessage message = new MistMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.GetSpecification()))
			{

				//o Add all of the necessary Fields to our message
				message.AddField(new U16Field("NUM-OF-EVENTS", (UInt16) 2));
				message.SetValue("EVENT.1", AddTimeToString("AOS occurred at: "));
				message.SetValue("EVENT.2", AddTimeToString("Telemetry downlink began at: "));

				connMgr.AddStandardFields(message);

				//o Publish the message to the middleware bus
				connMgr.Publish(message);

				//o Display the XML string representation of the Message for
				// the sake of review
				Log.Info("Published message: " + message.ToXML());
			}

			//o Setup a new message without some of the Required Fields and
			// attempt to publish it (i.e. Trigger a validation failure)
			using(MistMessage badMessage = new MistMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.GetSpecification()))
			{
				try
				{
					connMgr.Publish(badMessage);
				}
				catch(Exception e)
				{
					Log.Error("This error is expected:\n" + e.ToString());
				}
			}

			//o Disconnect from the middleware and clean up the Connection
			connMgr.Cleanup();
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

	static String AddTimeToString(String preface)
	{
		String theTime = TimeUtil.FormatTime(TimeUtil.GetCurrentTime());
		String result = preface + theTime;
		return result;
	}
}
