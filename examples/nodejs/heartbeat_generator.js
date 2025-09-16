#!/usr/bin/env node


/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * heartbeat_generator.js
 *
 * This example program provides a basic demonstration of using
 * the HeartbeatGenerator to publish heartbeat messages onto the GMSEC
 * Bus.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");
const readline = require("readline");

const utility = require("./utility.js");


class heartbeat_generator
{
	run()
	{
		try
		{
			// Set up connection configuration options provided by the user
			const config = utility.createConfig(process.argv);

			// Initialize log level for output
			utility.initializeLogLevel(config);

			// Validate messages before being sent to the bus
			config.addValue("gmsec-msg-content-validate-send", "true");

			// Define the heartbeat publish rate
			const publishRate = 2;

			// Create a HeartbeatGenerator instance
			const hbgen = new gmsec.HeartbeatGenerator(config, publishRate, utility.getStandardFields(this.constructor.name));

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("HeartbeatGenerator publish rate is " + publishRate.toString() + " seconds");

			// Start the heartbeat generator
			hbgen.start();

			gmsec.Log.info("HeartbeatGenerator is running; use gmsub or other utility to monitor messages");

			gmsec.Log.info("Press <enter> to stop the HeartbeatGenerator");
			const rl = readline.createInterface({input: process.stdin});
			rl.question("", (input) => {

				// Stop the HeartbeatGenerator
				hbgen.stop();
				gmsec.Log.info("HeartbeatGenerator has been stopped");

				rl.close();
			});
		}
		catch (e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus.
			gmsec.Log.error("Exception: " + e.message);
		}
	}
}


// Instantiate the heartbeat_generator example, and run it.
const example = new heartbeat_generator();
example.run();
