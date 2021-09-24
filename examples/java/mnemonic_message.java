/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file mnemonic_message.java
 *
 * This file contains an example outlining how to create and publish a
 * GMSEC-compliant Mnemonic Data Value message to the middleware bus using the
 * Messaging Interface Standardization Toolkit (MIST) namespace
 * ConnectionManager class function calls available for such operations.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 * bus.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import gov.nasa.gsfc.gmsec.api.field.I16Field;
import gov.nasa.gsfc.gmsec.api.field.I32Field;
import gov.nasa.gsfc.gmsec.api.field.F32Field;
import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.mist.Mnemonic;
import gov.nasa.gsfc.gmsec.api.mist.MnemonicSample;
import gov.nasa.gsfc.gmsec.api.mist.message.MnemonicMessage;
import java.util.ArrayList;

public class mnemonic_message
{
	public static final String GMSEC_SPEC_VERSION   = "201600";
	public static final String MVAL_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.MVAL.MNEMONIC_MESSAGE";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java mnemonic_message logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		initializeLogging(config);

		//o Set the GMSEC message specification version to be used to determine
		// what the structure of messages is for verification and the
		// construction of MistMessages
		config.addValue("GMSEC-SPECIFICATION-VERSION", GMSEC_SPEC_VERSION);

		//o Enable Message validation.  This parameter is "false" by default.
		config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.info("Opening the connection to the middleware server");
			connManager.initialize();

			Log.info(connManager.getLibraryVersion());

			//o Create all of the GMSEC Message header Fields which will
			// be used by all GMSEC Messages
			//
			// Note: Since these Fields contain variable values which are
			// based on the context in which they are used, they cannot be
			// automatically populated using MistMessage.
			ArrayList<Field> definedFields = new ArrayList<>();

			StringField missionField = new StringField("MISSION-ID", "MISSION");
			// Note: SAT-ID-PHYSICAL is an optional header Field, according
			// to the GMSEC ISD.
			StringField satIdField = new StringField("SAT-ID-PHYSICAL", "SPACECRAFT");
			StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
			StringField componentField = new StringField("COMPONENT", "mnemonic_message");

			definedFields.add(missionField);
			definedFields.add(satIdField);
			definedFields.add(facilityField);
			definedFields.add(componentField);

			//o Use setStandardFields to define a set of header fields for
			// all messages which are created or published on the
			// ConnectionManager using the following functions:
			// createLogMessage, publishLog, createHeartbeatMessage,
			// startHeartbeatService, createResourceMessage,
			// publishResourceMessage, or startResourceMessageService
			connManager.setStandardFields(definedFields);

			//o Populate the Mnemonic Sample(s)
			MnemonicSample mSample = new MnemonicSample("MS1", new I32Field("MS1", 15));
			mSample.setEUValue(new F32Field("My EU", 15.0f));
			mSample.setFlags(1);
			mSample.setLimit(MnemonicSample.LimitFlag.RED_HIGH);
			// Implicitly set limit enable/disable with setting of limit
			mSample.setQuality(true);
			mSample.setStalenessStatus(false);
			mSample.setTextValue("15");

			ArrayList<MnemonicSample> mnemonic_samples = new ArrayList<>();
			mnemonic_samples.add(mSample);

			//o Add the Mnemonic values to a Mnemonic object
			Mnemonic mnemonic = new Mnemonic("M1", mnemonic_samples);
			I16Field statusField = new I16Field("status", (short)5);
			mnemonic.setStatus(statusField);
			mnemonic.setUnits("units");

			//o Determine which version of the GMSEC message specification
			// the ConnectionManager was initialized with
			int version = connManager.getSpecification().getVersion();
			String gmsecSpecVersion = "";
			if (version == 201600)
			{
				gmsecSpecVersion = "2016.00";
			}
			else if (version == 201400)
			{
				gmsecSpecVersion = "2014.00";
			}

			//o Build up the Schema ID -- This is used to identify the
			// specific type of MVAL message to construct
			String schemaId = gmsecSpecVersion + ".GMSEC.MSG.MVAL";

			//o Construct an MVAL Message and add the Mnemonic values to it
			MnemonicMessage mvalMessage = new MnemonicMessage(MVAL_MESSAGE_SUBJECT, schemaId, connManager.getSpecification());

			//o Add the mnemonic
			mvalMessage.addMnemonic(mnemonic);

			//o If validating with the 2014 spec, the MSG-ID field is
			// required
			if (version == 201400)
			{
				mvalMessage.setValue("MSG-ID", "MVAL Request MSG-ID would go here");
			}

			//o Add the header fields to the MVAL message
			connManager.addStandardFields(mvalMessage);

			Log.info("Publishing MVAL message:\n" + mvalMessage.toXML());
			connManager.publish(mvalMessage);
		}
		catch (GMSEC_Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}

		return;

	}

	public static void initializeLogging(Config config)
	{
		// If it was not specified in the command-line arguments, set
		// LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the
		// program report output on the terminal/command line
		try
		{
			String logLevel = config.getValue("LOGLEVEL");
			String logFile = config.getValue("LOGFILE");

			if (logLevel == null)
			{
				config.addValue("LOGLEVEL", "INFO");
			}
			if (logFile == null)
			{
				config.addValue("LOGFILE", "STDOUT");
			}
		}
		catch(Exception e)
		{
		  Log.error(e.toString());
		}
  }
}
