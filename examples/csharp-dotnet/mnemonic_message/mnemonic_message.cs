/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file mnemonic_message.cs
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

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using GMSEC.API.MIST.MESSAGE;
using System;
using System.Collections.Generic;

class mnemonic_message
{
	static private String GMSEC_SPEC_VERSION   = "201600";
	static private String MVAL_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.MVAL.MNEMONIC_MESSAGE";
	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage mnemonic_message.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		InitializeLogging(config);

		//o Set the GMSEC message specification version to be used to determine
		// what the structure of messages is for verification and the
		// construction of MistMessages
		config.AddValue("GMSEC-SPECIFICATION-VERSION", GMSEC_SPEC_VERSION);

		//o Enable Message validation.  This parameter is "false" by default.
		config.AddValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connManager = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connManager.Initialize();

			Log.Info(connManager.GetLibraryVersion());

			//o Create all of the GMSEC Message header Fields which will
			// be used by all GMSEC Messages
			//
			// Note: Since these Fields contain variable values which are
			// based on the context in which they are used, they cannot be
			// automatically populated using MistMessage.
			List<Field> definedFields = new List<Field>();

			StringField missionField = new StringField("MISSION-ID", "MISSION");
			// Note: SAT-ID-PHYSICAL is an optional header Field, according
			// to the GMSEC ISD.
			StringField satIdField = new StringField("SAT-ID-PHYSICAL", "SPACECRAFT");
			StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
			StringField componentField = new StringField("COMPONENT", "device_message");

			definedFields.Add(missionField);
			definedFields.Add(satIdField);
			definedFields.Add(facilityField);
			definedFields.Add(componentField);

			//o Use setStandardFields to define a set of header fields for
			// all messages which are created or published on the
			// ConnectionManager using the following functions:
			// createLogMessage, publishLog, createHeartbeatMessage,
			// startHeartbeatService, createResourceMessage,
			// publishResourceMessage, or startResourceMessageService
			connManager.SetStandardFields(definedFields);

			//o Populate the Mnemonic Sample(s)
			MnemonicSample mSample = new MnemonicSample("MS1", new I32Field("MS1", 15));
			mSample.SetEUValue(new F32Field("My EU", (float) 15.0));
			mSample.SetFlags(1);
			mSample.SetLimit(MnemonicSample.LimitFlag.RED_HIGH);
			// Implicitly set limit enable/disable with setting of limit
			mSample.SetQuality(true);
			mSample.SetStalenessStatus(false);
			mSample.SetTextValue("15");

			List<MnemonicSample> mnemonic_samples = new List<MnemonicSample>();
			mnemonic_samples.Add(mSample);

			//o Add the Mnemonic values to a Mnemonic object
			Mnemonic mnemonic = new Mnemonic("M1", mnemonic_samples);
			I16Field statusField = new I16Field("status", 5);
			mnemonic.SetStatus(statusField);
			mnemonic.SetUnits("units");

			//o Determine which version of the GMSEC message specification
			// the ConnectionManager was initialized with
			uint version = connManager.GetSpecification().GetVersion();
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
			using(GMSEC.API.MIST.MESSAGE.MnemonicMessage mvalMessage = new GMSEC.API.MIST.MESSAGE.MnemonicMessage(MVAL_MESSAGE_SUBJECT, schemaId, connManager.GetSpecification()))
			{
				mvalMessage.AddMnemonic(mnemonic);

				//o If validating with the 2014 spec, the MSG-ID field is
				// required
				if (version == 201400)
				{
					mvalMessage.SetValue("MSG-ID", "MVAL Request MSG-ID would go here");
				}

				//o Add the header fields to the MVAL message
				connManager.AddStandardFields(mvalMessage);

				Log.Info("Publishing MVAL message:\n" + mvalMessage.ToXML());
				connManager.Publish(mvalMessage);
			}

			connManager.Cleanup();
		}
		catch (GMSEC_Exception e)
		{
			Log.Error(e.ToString());
			return -1;
		}

		return 0;
	}

	static void InitializeLogging(Config config)
	{
		String logLevel = config.GetValue("LOGLEVEL");
		String logFile = config.GetValue("LOGFILE");

		if (logLevel == null)
		{
			config.AddValue("LOGLEVEL", "INFO");
		}
		if (logFile == null)
		{
			config.AddValue("LOGFILE", "STDERR");
		}
	}
}
