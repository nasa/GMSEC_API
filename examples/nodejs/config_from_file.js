#!/usr/bin/env node


/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * config_from_file.js
 *
 * This example program provides a basic demonstration of creating
 * a Config object using the XML contents of a file that represent
 * a connection (or message) configuration.
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

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class config_from_file
{
	run()
	{
		try
		{
			// Set up configuration options (if any) provided by the user
			const config = utility.createConfig(process.argv, false);

			// Initialize log level for output
			utility.initializeLogLevel(config);

            // Load/create Config from a file.
			const config1 = gmsec.Config.getFromFile("config.xml", "Bolt");      // acquire specific configuration
			const config2 = gmsec.Config.getFromFile("config.xml", "ActiveMQ");  // acquire specific configuration
			const config3 = gmsec.Config.getFromFile("config.xml", "");          // acquire first configuration

			gmsec.Log.info("Config 1 is:\n" + config1.toXML());
			gmsec.Log.info("Config 2 is:\n" + config2.toXML());
			gmsec.Log.info("Config 3 is:\n" + config3.toXML());
		}
		catch (e)
		{
			gmsec.Log.error("Exception: " + e.message);
		}
	}
}


// Instantiate the config_from_file example, and run it.
const example = new config_from_file();
example.run();
