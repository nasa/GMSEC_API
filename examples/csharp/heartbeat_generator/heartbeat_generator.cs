/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file heartbeat_generator.cs
 *
 * @brief This example program provides a basic demonstration of using
 * the HeartbeatGenerator to publish heartbeat messages onto the GMSEC
 * Bus.
 */

using GMSEC.API5;
using System;
using System.Text;


public class heartbeat_generator
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		// Validate messages before being sent to the bus
		config.AddValue("gmsec-msg-content-validate-send", "true");

		// Define the heartbeat publish rate
		ushort publishRate = 2;  // in seconds

		try
		{
			// Create HeartbeatGenerator instance.
			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(config, publishRate, GetStandardFields()))
			{
				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("HeartbeatGenerator publish rate is: " + publishRate + " seconds");

				// Start the heartbeat generator
				hbgen.Start();
				Log.Info("HeartbeatGenerator is running; use gmsub or other utility to monitor messages");

				Log.Info("Press <enter> to stop the HeartbeatGenerator");
				Console.ReadLine();

				// Stop the HeartbeatGenerator
				hbgen.Stop();
				Log.Info("HeartbeatGenerator has been stopped");
			}
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus.
			Log.Error("GmsecException: " + e.ToString());
			return -1;
		}

		return 0;
	}


	private static void InitializeLogLevel(Config config)
	{
		LogLevel level = Log.FromString( config.GetValue("loglevel", "info") );
		Log.SetReportingLevel(level);
	}


	private static FieldList GetStandardFields()
	{
		FieldList standardFields = new FieldList()
		{
			new StringField("MISSION-ID", "MY-MISSION", true),
			new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true),
			new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true),
			new StringField("CONSTELLATION-ID", "MY-CNST", true),
			new StringField("FACILITY", "MY-FACILITY", true),
			new StringField("DOMAIN1", "MY-DOMAIN-1", true),
			new StringField("DOMAIN2", "MY-DOMAIN-2", true),
			new StringField("COMPONENT", "HB-GEN", true)
		};

		return standardFields;
	}
}
