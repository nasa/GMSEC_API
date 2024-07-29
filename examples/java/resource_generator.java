/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file resource_generator.java
 *
 * @brief This example program provides a basic demonstration of using
 * the ResourceGenerator to publish resource messages onto the GMSEC
 * Bus.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class resource_generator
{
	public static void main(String[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Validate messages before being sent to the bus
		config.addValue("gmsec-msg-content-validate-send", "true");

		// Define the resource publish rate and how often resource are collected
		int publishRate     = 5;  // in seconds
		int sampleInterval  = 1;  // in seconds
		int averageInterval = 10; // in seconds

		try
		{
			// Acquire standard fields to be included with the heartbeat messages
			List<Field> standardFields = getStandardFields();

			// Create ResourceGenerator instance.
			ResourceGenerator rsrcgen = ResourceGenerator.create(config, publishRate, sampleInterval, averageInterval, standardFields);

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("ResourceGenerator publish rate is: " + publishRate + " seconds");

			// Start the resource generator
			rsrcgen.start();

			Log.info("ResourceGenerator is running; use gmsub or other utility to monitor messages");

			Log.info("Press <enter> to stop the ResourceGenerator");
			new Scanner(System.in).nextLine();

			// Stop the ResourceGenerator
			rsrcgen.stop();
			Log.info("ResourceGenerator has been stopped");

			// Destroy the ResourceGenerator instance
			ResourceGenerator.destroy(rsrcgen);
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus.
			Log.error("GmsecException: " + e.getMessage());
		}
	}


	private static List<Field> getStandardFields()
	{
		List<Field> standardFields = new ArrayList<Field>();

		standardFields.add(new StringField("MISSION-ID", "MY-MISSION", true));
		standardFields.add(new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true));
		standardFields.add(new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true));
		standardFields.add(new StringField("CONSTELLATION-ID", "MY-CNST", true));
		standardFields.add(new StringField("FACILITY", "MY-FACILITY", true));
		standardFields.add(new StringField("DOMAIN1", "MY-DOMAIN-1", true));
		standardFields.add(new StringField("DOMAIN2", "MY-DOMAIN-2", true));
		standardFields.add(new StringField("COMPONENT", "RSRC-GEN", true));

		return standardFields;
	}
}
