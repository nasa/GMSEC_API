/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_custom.java
 *
 * This file contains an example demonstrating how to implement additional
 * Message validation logic in addition to that which the GMSEC API provides.
 *
 * Note: This example focuses on adding additional validation upon the receipt
 * of a message.  It almost goes without saying that additional logic can be
 * added to a program prior to invoking the publish() function without having
 * to do anything special.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback;
import gov.nasa.gsfc.gmsec.api.field.StringField;
import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.mist.ProductFile;
import gov.nasa.gsfc.gmsec.api.mist.ProductFileIterator;
import gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage;
import gov.nasa.gsfc.gmsec.api.mist.gmsecMIST;
import java.util.ArrayList;

class ValidationCallback extends ConnectionManagerCallback
{
	public void onMessage(ConnectionManager connMgr, Message message)
	{
		try
		{
			//o Run the message through the GMSEC API-supplied validation
			connMgr.getSpecification().validateMessage(message);

			//o In this example scenario, we are expecting to receive a
			// GMSEC PROD message containing a URI to a location on the disk
			// where a product file has been placed for retrieval.  In this
			// case, we want to validate that the location on the disk is
			// in an area which we would expect (i.e. Something that the
			// team has agreed upon prior to operational deployment).
			//
			// By validating this URI, we ensure that no malicious users
			// have infiltrated the system and somehow modified the messages
			// to cause us to retrieve a file from an unknown location.

			//o Start by checking to ensure that this is a PROD message
			if (isProdMsg(message))
			{
				ProductFileMessage prodMessage = new ProductFileMessage(connMgr.getSpecification(), message.toXML());

				//o Extract the Product File URI location(s) from the
				// message using a ProductFileIterator
				ProductFileIterator prodIter = prodMessage.getProductFileIterator();

				while(prodIter.hasNext())
				{
					ProductFile prodFile = prodIter.next();

					String prodUri = prodFile.getURI();

					//o Check to ensure that the URI contains "//hostname/dir"
					if (!prodUri.contains("//hostname/dir"))
					{
						String errMsg = "Received an invalid PROD Message (bad URI):\n" + message.toXML();
						throw new GMSEC_Exception(StatusClassification.MIST_ERROR, StatusCode.MESSAGE_FAILED_VALIDATION, errMsg);
					}
				}

				Log.info("Received a valid message:\n" + message.toXML());
			}
		}
		catch(Exception e)
		{
			Log.error(e.getMessage());
		}
	}

	public static boolean isProdMsg(Message message) throws GMSEC_Exception
	{
		boolean result =
			message.getStringValue("MESSAGE-TYPE").equals("MSG") &&
			message.getStringValue("MESSAGE-SUBTYPE").equals("PROD");

		return result;
	}
}

public class validation_custom
{
	public static final String PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java validation_custom logging mw-id=<middleware ID>");
			System.exit(-1);
		}

		Config config = new Config(args);

		initializeLogging(config);

		//o Enable Message validation.  This parameter is "false" by default.
		config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			Log.info("Opening the connection to the middleware server");
			connMgr.initialize();

			Log.info(connMgr.getLibraryVersion());

			//o Set up the ValidationCallback and subscribe
			ValidationCallback vc = new ValidationCallback();
			connMgr.subscribe(PROD_MESSAGE_SUBJECT, vc);

			//o Start the AutoDispatcher
			connMgr.startAutoDispatch();

			//o Create and publish a simple Product File Message
			setupStandardFields(connMgr);

			ProductFileMessage productMessage = createProductFileMessage(connMgr, "//hostname/dir/filename");

			//o Publish the message to the middleware bus
			connMgr.publish(productMessage);

			productMessage = createProductFileMessage(connMgr, "//badhost/dir/filename");

			connMgr.publish(productMessage);

			//o Disconnect from the middleware and clean up the Connection
			connMgr.cleanup();
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

	public static void setupStandardFields(ConnectionManager connMgr) throws GMSEC_Exception
	{
		ArrayList<Field> definedFields = new ArrayList<Field>();

		StringField missionField = new StringField("MISSION-ID", "MISSION");
		StringField satIdField = new StringField("SAT-ID-PHYSICAL", "SPACECRAFT");
		StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
		StringField componentField = new StringField("COMPONENT", "validation_custom");

		definedFields.add(missionField);
		definedFields.add(satIdField);
		definedFields.add(facilityField);
		definedFields.add(componentField);

		if (connMgr.getSpecification().getVersion() >= gmsecMIST.GMSEC_ISD_2018_00)
		{
			StringField domain1 = new StringField("DOMAIN1", "MY-DOMAIN-1");
			StringField domain2 = new StringField("DOMAIN2", "MY-DOMAIN-2");

			definedFields.add(domain1);
			definedFields.add(domain2);
		}

		connMgr.setStandardFields(definedFields);
	}

	public static ProductFileMessage createProductFileMessage(ConnectionManager connMgr, String filePath)
	{
		ProductFile externalFile = new ProductFile("External File", "External File Description", "1.0.0", "TXT", filePath);

		ProductFileMessage productMessage;

		if (connMgr.getSpecification().getVersion() <= gmsecMIST.GMSEC_ISD_2016_00)
		{
			productMessage = new ProductFileMessage(PROD_MESSAGE_SUBJECT, gmsecMIST.ResponseStatus.SUCCESSFUL_COMPLETION, "MSG.PROD.AUTO", connMgr.getSpecification());
		}
		else
		{
			productMessage = new ProductFileMessage(PROD_MESSAGE_SUBJECT, gmsecMIST.ResponseStatus.SUCCESSFUL_COMPLETION, "MSG.PROD", connMgr.getSpecification());

			productMessage.addField("PROD-TYPE", "AUTO");
			productMessage.addField("PROD-SUBTYPE", "DM");
		}

		productMessage.addProductFile(externalFile);

		connMgr.addStandardFields(productMessage);

		return productMessage;
	}
}
