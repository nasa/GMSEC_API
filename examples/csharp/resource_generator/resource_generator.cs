/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file resource_generator.cs
 *
 * @brief This example program provides a basic demonstration of using
 * the ResourceGenerator to publish resource messages onto the GMSEC
 * Bus.
 */

using GMSEC.API5;
using System;
using System.Text;


public class resource_generator
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

		// Define the resource publish rate and how often resource are collected
		ushort publishRate     = 5;  // in seconds
		ushort sampleInterval  = 1;  // in seconds
		ushort averageInterval = 10; // in seconds

		try
		{
			// Create ResourceGenerator instance.
			using (ResourceGenerator rsrcgen = new ResourceGenerator(config, publishRate, sampleInterval, averageInterval, GetStandardFields()))
			{
				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("ResourceGenerator publish rate is: " + publishRate + " seconds");

				// Start the resource generator
				rsrcgen.Start();
				Log.Info("ResourceGenerator is running; use gmsub or other utility to monitor messages");

				Log.Info("Press <enter> to stop the ResourceGenerator");
				Console.ReadLine();

				// Stop the ResourceGenerator
				rsrcgen.Stop();
				Log.Info("ResourceGenerator has been stopped");
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
			new StringField("COMPONENT", "RSRC-GEN", true)
		};

		return standardFields;
	}
}
