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


public class gmmist_validation implements Example
{
	private Config     config;
	private ConnectionManager connManager;


	public gmmist_validation(Config config) throws GMSEC_Exception, ExampleException
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

			Message msg = connManager.createLogMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);

			msg.addField("SEVERITY", (short) 1);
			msg.addField("MSG-TEXT", "Self-published log message");


			//o Publish valid log message
			Log.info("Attempting to manually publish a valid log message...");
			connManager.publish(msg);

			Log.info("Demonstrating the fast log functionality...");
			Log.info("First we set the defaults for all log messages called with 'publishLog'");
			connManager.setLoggingDefaults("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);
			I16Field severityField = new I16Field("SEVERITY", (short) 1);
			Log.info("...Then all we have to do is call 'publishLog' with a log string and a severity");
			connManager.publishLog("Self-published log message", severityField);
			connManager.publishLog("Another log, with one function call!", severityField);


			//o Attempt to publish invalid log message
			msg.addField("SEVERITY", "SEVERITY Field in a LOG should be an I16");

			try {
				Log.info("Attempting to manually publish an invalid log message...");
				connManager.publish(msg);
				Log.warning("Ignored improper severity and published anyway");
			}
			catch (Exception e) {
				Log.info("Invalid log message was NOT published; reason: " + e.toString());
			}


			//o Create Product message
			Log.info("Creating PROD message...");
			Message prodMsg = new Message("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM", Message.MessageKind.PUBLISH);

			Log.info("Setting fields related to PROD message header...");
			connManager.addStandardFields(prodMsg);
			prodMsg.addField("MESSAGE-TYPE", "MSG");
			prodMsg.addField("MESSAGE-SUBTYPE", "PROD");
			prodMsg.addField("CONTENT-VERSION", (float) 2013.0);
			prodMsg.addField("RESPONSE-STATUS", (short) 6);
			prodMsg.addField("PROD-TYPE", "AUTO");
			prodMsg.addField("PROD-SUBTYPE", "DM");


			//o Publish valid PROD message
			Log.info("Publishing PROD message...");
			connManager.publish(prodMsg);


			//o Alter PROD message to make it invalid
			prodMsg.addField("PROD-SUBTYPE", (float) 45);


			//o Attempt to publish invalid PROD message
			try {
				Log.info("Attempting to publish invalid PROD message...");
				connManager.publish(prodMsg);
				Log.warning("Ignored improper PROD-SUBTYPE and published anyway");
			}
			catch (Exception e) {
				Log.info("Invalid PROD message was NOT published; reason: " + e.toString());
			}

			Log.info("But wait, publishing a product is even easier!");

			ProductFileMessage productFileMessage = new ProductFileMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM", 
					gmsecMIST.ResponseStatus.FINAL_MESSAGE, "AUTO", "DM", gmsecMIST.GMSEC_ISD_CURRENT);

			ProductFile myFile = new ProductFile("My Product", "My Product Description", "Version", 
				"TXT", "A path to somewhere");

			productFileMessage.addProductFile(myFile);
			connManager.addStandardFields(productFileMessage);

			try {
				Log.info("Attempting to publish PROD message with ProductFileMessage");
				connManager.publish(productFileMessage);
			}
			catch (Exception e) {
				Log.info("PROD message was NOT published; reason: " + e.toString());
			}

			Log.info("Mnemonic Messages are also easier!");

			List<MnemonicSample> mnemonicSamples = new ArrayList<MnemonicSample>();
			MnemonicSample ms1 = new MnemonicSample("Timestamp 1", new I32Field("Timestamp 1", 15));
			ms1.setEUValue(new F32Field("My EU", 15.0f));
			ms1.setFlags(1);
			ms1.setLimit(MnemonicSample.LimitFlag.RED_HIGH);
			//implicitly set limit enable/disable with setting of limit
			ms1.setQuality(true);
			ms1.setStalenessStatus(false);
			ms1.setTextValue("15");
			
			mnemonicSamples.add(ms1);
			mnemonicSamples.add(new MnemonicSample("Timestamp 2", new I32Field("Timestamp 2", 16)));  
			mnemonicSamples.add(new MnemonicSample("Timestamp 3", new I32Field("Timestamp 3", 17)));  
			mnemonicSamples.add(new MnemonicSample("Timestamp 4", new I32Field("Timestamp 4", 18)));

			Mnemonic m1 = new Mnemonic("MyMnemonic", mnemonicSamples);
			m1.setStatus(new I16Field("My status", (short) 5));
			m1.setUnits("MW");

			MnemonicMessage mMessage = new MnemonicMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.MVAL.EXAMPLE-PROG", gmsecMIST.GMSEC_ISD_CURRENT);
			mMessage.addMnemonic(m1);
			connManager.addStandardFields(mMessage);

			if(gmsecMIST.GMSEC_ISD_CURRENT == gmsecMIST.GMSEC_ISD_2014_00)
			{
				//MSG-ID was required in GMSEC messages before the 2016 specification.
				mMessage.addField("MSG-ID", "MYMSG");
			}

			Log.info("MnemonicMessage formats my GMSEC data automatically!");
			Log.info(mMessage.toXML());
			connManager.publish(mMessage);

			Log.info("Device Messages have their own subclass, as well");

			Device device1 = new Device("Device 1", Device.DeviceStatus.RED);
			DeviceParam param = new DeviceParam("DEV Param 1", "Param 1 Timestamp", new I32Field("Param1Val", 79));
			device1.addParam(param);

			Device device2 = new Device("Device 2", Device.DeviceStatus.NOMINAL_GREEN);
			DeviceParam param1 = new DeviceParam("DEV Param 1", "Param 1 Timestamp", new I32Field("Param1Val", 79));
			DeviceParam param2 = new DeviceParam("DEV Param 2", "Param 2 Timestamp", new I32Field("Param2Val", 79));
			device2.addParam(param1);
			device2.addParam(param2);

			DeviceMessage deviceMessage = new DeviceMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.EXAMPLE-PROG.DEV.EXAMPLE-PROG-2", gmsecMIST.GMSEC_ISD_CURRENT);
			deviceMessage.addDevice(device1);
			deviceMessage.addDevice(device2);

			Log.info("DeviceMessage formats my GMSEC data automatically!");
			Log.info(deviceMessage.toXML());

			connManager.addStandardFields(deviceMessage);
			connManager.publish(deviceMessage);

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

			example = new gmmist_validation(config);
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

