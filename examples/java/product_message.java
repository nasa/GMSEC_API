/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file product_message.java
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
import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.mist.ProductFile;
import gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage;
import gov.nasa.gsfc.gmsec.api.mist.gmsecMIST;
import java.util.ArrayList;

public class product_message
{
	public static final String PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java product_message logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		initializeLogging(config);

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
			StringField componentField = new StringField("COMPONENT", "product_message");

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

			ProductFileMessage productMessage = new ProductFileMessage(PROD_MESSAGE_SUBJECT, gmsecMIST.ResponseStatus.SUCCESSFUL_COMPLETION, Message.MessageKind.PUBLISH,"AUTO", "DM", connManager.getSpecification());

			productMessage.addProductFile(externalFile);
			productMessage.addProductFile(binaryFile);

			connManager.addStandardFields(productMessage);

			connManager.publish(productMessage);

			Log.info("Published DEV message:\n" + productMessage.toXML());

			connManager.cleanup();
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
