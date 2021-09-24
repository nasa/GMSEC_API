/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

 /**
 * @file heartbeat_generator.java
 *
 * This file contains an example describing how to instantiate and
 * start/stop a Heartbeat (Message) Generator.
 */

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.HeartbeatGenerator;
import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.gmsecMIST;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class heartbeat_generator
{
	public static final String HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-GENERATOR.HB";
	public static final int    HB_PUBLISH_RATE    = 5;


	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java heartbeat_generator mw-id=<middleware ID>");
			System.exit(-1);
		}

		//o Setup configuration with the supplied command line arguments
		Config config =  new Config(args);

		//o Unless otherwise configured, setup configuration that allows us to
		//  log messages to stderr.
		initializeLogging(config);

		//o Display the version number of the GMSEC API
		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			//o Define the standard fields that will be included with the heartbeat messages
			List<Field> standardFields = defineStandardFields(config);

			//o Instantiate the heartbeat generator
			HeartbeatGenerator hbgen = new HeartbeatGenerator(config, HB_MESSAGE_SUBJECT, HB_PUBLISH_RATE, standardFields);

			//o Start heartbeat generator (use gmsub utility to view messages being published)
			hbgen.start();
			Log.info("Heartbeat Generator is running; use gmsub or other utility to monitor messages.");

			//o Wait for user input to stop the heartbeat generator
			Log.info("Press <enter> to stop the heartbeat generator");
			new Scanner(System.in).nextLine();

			//o Stop heartbeat generator
			hbgen.stop();
			Log.info("Heartbeat Generator has been stopped.");
		}
		catch (GMSEC_Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}
	}


	public static void initializeLogging(Config config)
	{
		// If it was not specified in the command-line arguments, set
		// LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the
		// program report output on the terminal/command line
		try
		{
			String logLevel = config.getValue("loglevel");
			String logFile  = config.getValue("logfile");

			if (logLevel == null)
			{
				config.addValue("loglevel", "info");
			}
			if (logFile == null)
			{
				config.addValue("logfile", "stderr");
			}
		}
		catch(Exception e)
		{
			Log.error(e.toString());
		}
  	}


	public static List<Field> defineStandardFields(Config config)
	{
		ArrayList<Field> standardFields = new ArrayList<Field>();

		standardFields.add(new StringField("COMPONENT", "HEARTBEAT-GENERATOR"));
		standardFields.add(new StringField("MISSION-ID", "MY-MISSION"));
		standardFields.add(new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID"));
		standardFields.add(new StringField("FACILITY", "MY-FACILITY"));

		if (config.getIntegerValue("gmsec-specification-version", gmsecMIST.GMSEC_ISD_CURRENT) == gmsecMIST.GMSEC_ISD_2014_00)
		{
			standardFields.add(new StringField("MSG-ID", "MY-MSG-ID"));
		}
		else if (config.getIntegerValue("gmsec-specification-version", gmsecMIST.GMSEC_ISD_CURRENT) > gmsecMIST.GMSEC_ISD_2016_00)
		{
			standardFields.add(new StringField("DOMAIN1", "MY-DOMAIN-1"));
			standardFields.add(new StringField("DOMAIN2", "MY-DOMAIN-2"));
		}

		return standardFields;
	}
}
