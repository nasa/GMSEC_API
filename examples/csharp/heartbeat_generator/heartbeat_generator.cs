/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file heartbeat_generator.cs
 *
 * This file contains an example describing how to instantiate and
 * start/stop a Heartbeat (Message) Generator.
 */

using GMSEC.API;
using System;
using System.Text;

class heartbeat_generator
{
	static private String HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-GENERATOR.HB";
	static private ushort HB_PUBLISH_RATE    = 5;


	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("Usage: heartbeat_generator mw-id=<middleware ID>");
			return -1;
		}

		//o Setup configuration with the supplied command line arguments
		Config config = new Config(args);

		//o Unless otherwise configured, setup configuration that allows us to
		//  log messages to stderr.
		InitializeLogging(config);

		//o Display the version number of the GMSEC API
		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		//o Get our message specification version, if available.
		//  If not availabe, use current specification.
		uint version = (uint) config.GetIntegerValue("gmsec-specification-version", (int) Gmsec.GMSEC_ISD_CURRENT);

		//o Define standard fields
		FieldList standardFields = new FieldList();

		standardFields.Add(new StringField("COMPONENT", "HEARTBEAT-GENERATOR"));
		standardFields.Add(new StringField("MISSION-ID", "MY-MISSION"));
		standardFields.Add(new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID"));
		standardFields.Add(new StringField("FACILITY", "MY-FACILITY"));

		if (version == Gmsec.GMSEC_ISD_2014_00)
		{
			standardFields.Add(new StringField("MSG-ID", "MY-MSG-ID"));
		}
		else if (version >= Gmsec.GMSEC_ISD_2018_00)
		{
			standardFields.Add(new StringField("DOMAIN1", "MY-DOMAIN-1"));
			standardFields.Add(new StringField("DOMAIN2", "MY-DOMAIN-2"));
		}

		try
		{
			//o Instantiate the heartbeat generator
			HeartbeatGenerator hbgen = new HeartbeatGenerator(config, HB_MESSAGE_SUBJECT, HB_PUBLISH_RATE, standardFields);

			//o Start heartbeat generator (use gmsub utility to view messages being published)
			hbgen.Start();
			Log.Info("Heartbeat Generator is running; use gmsub or other utility to monitor messages.");

			//o Wait for user input to stop the heartbeat generator
			Log.Info("Press <enter> to stop the heartbeat generator");
			Console.ReadLine();

			//o Stop heartbeat generator
			hbgen.Stop();
			Log.Info("Heartbeat Generator has been stopped.");

			hbgen = null;
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
