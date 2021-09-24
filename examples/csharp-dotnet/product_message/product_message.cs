/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file product_message.cs
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

class product_message
{
	static private String PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage product_message.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		InitializeLogging(config);

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

			//o Create a ProductFile object with the product name,
			// description, version, file format, and the URI
			ProductFile externalFile = new ProductFile("External File", "External File Description", "1.0.0", "TXT", "//hostname/dir/filename");

			int fSize = 8;
			byte[] filePayload = new byte[8];
			for(byte idx = 0; idx < fSize; idx++)
			{
				filePayload[idx] = idx;
			}

			//o Create a ProductFile object with the product name,
			// description, version, format, binary array, and file size
			ProductFile binaryFile = new ProductFile("File as Binary", "Binary File Description", "1.0.0", "BIN", filePayload);

			//o Create a Product File Message with the subject,
			// RESPONSE-STATUS Field value, Message type (publish, request,
			// or reply), PROD-TYPE Field value, PROD-SUBTYPE Field value,
			// and pass it the Specification object from the Connection
			// Manager
			using(GMSEC.API.MIST.MESSAGE.ProductFileMessage productMessage = new GMSEC.API.MIST.MESSAGE.ProductFileMessage(PROD_MESSAGE_SUBJECT, ResponseStatus.SUCCESSFUL_COMPLETION, Message.MessageKind.PUBLISH, "AUTO", "DM", connManager.GetSpecification()))
			{
				productMessage.AddProductFile(externalFile);
				productMessage.AddProductFile(binaryFile);

				connManager.AddStandardFields(productMessage);

				connManager.Publish(productMessage);

				Log.Info("Published DEV message:\n" + productMessage.ToXML());
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
