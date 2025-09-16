/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config_from_file.cs
 *
 * @brief This example program provides a basic demonstration of creating
 * a Config object using the XML contents of a file that represent a
 * connection (or message) configuration.
 *
 * A configuration file should have contents using a format similar to:
 *
 * <CONFIG NAME="Bolt">
 *     <PARAMETER NAME="mw-id">bolt</PARAMETER>
 *     <PARAMETER NAME="server">my-server</PARAMETER>
 * </CONFIG>
 *
 * <CONFIG NAME="ActiveMQ">
 *     <PARAMETER NAME="mw-id">activemq39</PARAMETER>
 *     <PARAMETER NAME="server">tcp://my-server:61616</PARAMETER>
 * </CONFIG>
 *
 * See config.xml for a simple example.
 */


using GMSEC.API5;
using System;
using System.Text;


public class config_from_file
{
	public static int Main(string[] args)
	{
		if (args.Length != 1)
		{
			Log.Error("Usage: Provide an example XML config file of interest.");
			return 1;
		}

		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		try
		{
			// Load/create Config from a file.
			Config config1 = Config.GetFromFile( args[0], "Bolt" );
			Config config2 = Config.GetFromFile( args[0], "ActiveMQ" );
			Config config3 = Config.GetFromFile( args[0], null );

			Log.Info("Config 1 is:\n" + config1.ToXML());
			Log.Info("Config 2 is:\n" + config2.ToXML());
			Log.Info("Config 3 is:\n" + config3.ToXML());
		}
		catch (GmsecException e)
		{
			Log.Error("Exception: " + e.ToString());
			return 1;
		}

		return 0;
	}


	private static void InitializeLogLevel(Config config)
	{
		LogLevel level = Log.FromString( config.GetValue("loglevel", "info") );
		Log.SetReportingLevel(level);
	}
}
