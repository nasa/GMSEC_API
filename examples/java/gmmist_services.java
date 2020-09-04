/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmmist_validation.java
 *
 * An example Java program using MIST validation and objects.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.mist.*;
import gov.nasa.gsfc.gmsec.api.field.*;
import gov.nasa.gsfc.gmsec.api.util.Log;

import java.util.List;
import java.util.ArrayList;


public class gmmist_services implements Example
{
	private Config     config;
	private ConnectionManager connManager;


	public gmmist_services(Config config) throws GMSEC_Exception, ExampleException
	{
		this.config = config;

		Util.initialize(config);

		if (!Util.hasConnectionType(config))
		{
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}


	private void printUsage()
	{
		System.err.println("\nusage: java gmpub connectionType=<middleware> " +
		          "[ <parameter>=<value> ]\n" +
		          "\n\tNote that the parameter 'connectiontype' is required. "  +
		          "\n\tThe rest of other parameters are optional.\n" +
		          "\n\tserver=<server name> " +
		          "(required if middleware is not bolt/MB locally)" +
		          "\n\nFor more information, see API User's Guide\n\n");
	}


	public boolean run() throws GMSEC_Exception
	{
		// Output GMSEC API version
		Log.info(Connection.getAPIVersion());

		try
		{
			connManager = new ConnectionManager(config, true, gmsecMIST.GMSEC_ISD_CURRENT);

			connManager.initialize();

			//o setStandardFields()
			Log.info("Setting fields which will be applied globally to message from this ConnectionManager");
			Log.info("These fields are usually certain values from the GMSEC Header which apply broadly");

			List<Field> definedFields = new ArrayList<Field>();
			definedFields.add(new F32Field("HEADER-VERSION", 2010.0f));
			definedFields.add(new StringField("MISSION-ID", "GMSEC"));
			definedFields.add(new StringField("FACILITY", "GMSEC Lab"));
			definedFields.add(new StringField("COMPONENT", "gmmist-demo"));

			connManager.setStandardFields(definedFields);


			//o createLogMessage()
			List<Field> logDefinedFields = new ArrayList<Field>();

			Log.info("No need to add MESSAGE-TYPE and MESSAGE-SUBTYPE for a log, we do that for you!");
			Log.info("Adding general log fields");

			logDefinedFields.add(new StringField("SUBCLASS", "TAC"));
			logDefinedFields.add(new StringField("OCCURRENCE-TYPE", "NORM"));

			connManager.setLoggingDefaults("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);


			I16Field fieldLogSeverity = new I16Field("SEVERITY", (short) 1);
			Log.info("Sending log message regarding imminent start of heartbeat service");
			connManager.publishLog("About to start heartbeat service", fieldLogSeverity);


			List<Field> hbDefinedFields = new ArrayList<Field>();

			Log.info("No need to add MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE for a heartbeat, we do that for you!");
			Log.info("Adding general heartbeat fields");

			hbDefinedFields.add( new I16Field("PUB-RATE", (short) 1) );
			hbDefinedFields.add( new I16Field("COUNTER", (short) 0) );
			hbDefinedFields.add( new StringField("MSG-ID", "My heartbeat identifier") );

			Log.info("Starting heartbeat service");
			connManager.startHeartbeatService("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.HB", hbDefinedFields);
			Log.info("Heartbeat service has been started. Publish rate is defined by PUB-RATE, defaulting to 30 seconds if not defined. "
			           + "Component status will be reported as nominal unless otherwise instructed. The counter will automatically increment.");
		

			connManager.publishLog("Heartbest service has been started!", fieldLogSeverity);


			Log.info("Time to start up the resource service. This will automatically publish resource information");
			Log.info("Java version requires the publish interval, the time between samples, and the averaging period of sampling");
			connManager.startResourceMessageService("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.EXAMPLE.RSRC.EXAMPLE2",
					60, 1, 10);

			Log.info("Waiting for 60 seconds to allow heartbeats and resources to publish.");
			Log.info("The user may wish to use gmsub to observe message traffic.");
			try{
				Thread.sleep(60000);
			}catch(Exception e){
				//No worries, keep going
			}
			
			Log.info("Changing the example component status to RED");
			I16Field compStatus = new I16Field("COMPONENT-STATUS", (short) 4);
			connManager.changeComponentStatus(compStatus);

			Log.info("Wait another 60 seconds to observe the change");
			try{
				Thread.sleep(60000);
			}catch(Exception e){
				//No worries, keep going
			}

			connManager.publishLog("End of demonstration, cleaning up ConnectionManager", fieldLogSeverity);
			Log.info("End of demonstration, cleaning up ConnectionManager");


			connManager.stopHeartbeatService();
			Log.info("Stopping heartbeat service. This will happen automatically when the ConnectionManager cleans up resources, "
			           + "should the user not perform so manually.");

		}
		catch (Exception e)
		{
			Log.info(e.toString());
			return false;
		}

		return true;
	}




	public boolean cleanup() throws GMSEC_Exception
	{
		if (connManager != null)
		{
			connManager.cleanup();
		}

		return true;
	}


	public static void main(String[] args)
	{
		Example example = null;

		try {
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmmist_services(config);
			example.run();
		}
		catch (IllegalArgumentException e) {
			System.err.println(e.toString());
		}
		catch (GMSEC_Exception e) {
			System.err.println(e.toString());
		}
		catch (ExampleException e) {
			System.err.println(e.toString());
		}
		finally {
			if (example != null)
				Util.cleanup(example);
		}
	}
}

