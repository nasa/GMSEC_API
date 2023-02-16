/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config_from_file.java
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


import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.util.Log;


public class config_from_file
{
	public static void main(String[] args)
	{
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		try
		{
			// Load/create Config from a file.
			Config config1 = Config.getFromFile( "config.xml", "Bolt" );
			Config config2 = Config.getFromFile( "config.xml", "ActiveMQ" );
			Config config3 = Config.getFromFile( "config.xml", null );

			Log.info("Config 1 is:\n" + config1.toXML());
			Log.info("Config 2 is:\n" + config2.toXML());
			Log.info("Config 3 is:\n" + config3.toXML());
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			Log.error("Exception: " + e.getMessage());
		}
	}
}
