#!/usr/bin/env node


/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * resource_generator.js
 *
 * This example program provides a basic demonstration of using
 * the ResourceGenerator to publish resource messages onto the GMSEC
 * Bus.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");
const readline = require("readline");

const utility = require("./utility.js");


class resource_generator
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

			// Define the resource message publish rate and how often resources are collected
			const publishRate     = 5;   // in seconds
			const sampleInterval  = 1;   // in seconds
			const averageInterval = 10;  // in seconds

			// Create a ResourceGenerator instance
			const rsrcgen = new gmsec.ResourceGenerator(config, publishRate, sampleInterval, averageInterval, utility.getStandardFields(this.constructor.name));

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("ResourceGenerator publish rate is " + publishRate.toString() + " seconds");

			// Start the resource generator
			rsrcgen.start();

			gmsec.Log.info("ResourceGenerator is running; use gmsub or other utility to monitor messages");

			gmsec.Log.info("Press <enter> to stop the ResourceGenerator");
			const rl = readline.createInterface({input: process.stdin});
			rl.question("", (input) => {

				// Stop the ResourceGenerator
				rsrcgen.stop();
				gmsec.Log.info("ResourceGenerator has been stopped");

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


// Instantiate the resource_generator example, and run it.
const example = new resource_generator();
example.run();
