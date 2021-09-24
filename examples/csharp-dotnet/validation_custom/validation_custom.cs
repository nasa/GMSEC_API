/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_custom.cs
 *
 * This file contains an example demonstrating how to implement additional
 * Message validation logic in addition to that which the GMSEC API provides.
 *
 * Note: This example focuses on adding additional validation upon the receipt
 * of a message.  It almost goes without saying that additional logic can be
 * added to a program prior to invoking the publish() function without having
 * to do anything special.
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using GMSEC.API.MIST.MESSAGE;
using System;
using System.Collections.Generic;

class ValidationCallback: ConnectionManagerCallback
{
	public override void OnMessage(ConnectionManager connMgr, Message message)
	{
		try
		{
			//o Run the message through the GMSEC API-supplied validation
			connMgr.GetSpecification().ValidateMessage(message);

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
			if (IsProdMsg(message))
			{
				GMSEC.API.MIST.MESSAGE.ProductFileMessage prodMessage = new GMSEC.API.MIST.MESSAGE.ProductFileMessage(message.ToXML());

				//o Extract the Product File URI location(s) from the
				// message using a ProductFileIterator
				ProductFileIterator prodIter = prodMessage.GetProductFileIterator();

				while(prodIter.HasNext())
				{
					ProductFile prodFile = prodIter.Next();

					//o Check to ensure that the URI contains "//hostname/dir"
					String prodUri = prodFile.GetURI();
					if(prodUri.IndexOf("//hostname/dir") == -1)
					{
						String errorMsg = "Received an invalid PROD Message (bad URI):\n" + message.ToXML();
						throw new GMSEC_Exception(StatusClass.MIST_ERROR, StatusCode.MESSAGE_FAILED_VALIDATION, errorMsg);
					}
				}

				Log.Info("Received a valid message:\n" + message.ToXML());
			}
		}
		catch (Exception e)
		{
			Log.Error(e.ToString());
		}
	}

	public bool IsProdMsg(Message message)
	{
		bool result =
			message.GetStringValue("MESSAGE-TYPE") == "MSG"
			&& message.GetStringValue("MESSAGE-SUBTYPE") == "PROD";

		return result;
	}
}

class validation_custom
{
	static private String PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE";

	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage validation_custom.exe mw-id=<middleware ID>");
			return -1;
		}

		Config config = new Config(args);

		InitializeLogging(config);

		//o Enable Message validation.  This parameter is "false" by default.
		config.AddValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

		Log.Info(ConnectionManager.GetAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			Log.Info("Opening the connection to the middleware server");
			connMgr.Initialize();

			Log.Info(connMgr.GetLibraryVersion());

			//o Set up the ValidationCallback and subscribe
			ValidationCallback vc = new ValidationCallback();
			connMgr.Subscribe(PROD_MESSAGE_SUBJECT, vc);

			//o Start the AutoDispatcher
			connMgr.StartAutoDispatch();

			//o Create and publish a simple Product File Message
			SetupStandardFields(connMgr);

			GMSEC.API.MIST.MESSAGE.ProductFileMessage productMessage = CreateProductFileMessage(connMgr, "//hostname/dir/filename");

			connMgr.Publish(productMessage);

			productMessage = CreateProductFileMessage(connMgr, "//badhost/dir/filename");

			//o Publish the message to the middleware bus
			connMgr.Publish(productMessage);

			connMgr.StopAutoDispatch();

			//o Disconnect from the middleware and clean up the Connection
			connMgr.Cleanup();
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

	static void SetupStandardFields(ConnectionManager connMgr)
	{
		List<Field> definedFields = new List<Field>();

		StringField missionField = new StringField("MISSION-ID", "MISSION");
		StringField satIdField = new StringField("SAT-ID-PHYSICAL", "SPACECRAFT");
		StringField facilityField = new StringField("FACILITY", "GMSEC Lab");
		StringField componentField = new StringField("COMPONENT", "device_message");

		definedFields.Add(missionField);
		definedFields.Add(satIdField);
		definedFields.Add(facilityField);
		definedFields.Add(componentField);

		if (connMgr.GetSpecification().GetVersion() >= GMSEC_ISD.GMSEC_ISD_2018_00)
		{
			StringField domain1 = new StringField("DOMAIN1", "MY-DOMAIN-1");
			StringField domain2 = new StringField("DOMAIN2", "MY-DOMAIN-2");

			definedFields.Add(domain1);
			definedFields.Add(domain2);
		}

		connMgr.SetStandardFields(definedFields);
	}

	static GMSEC.API.MIST.MESSAGE.ProductFileMessage CreateProductFileMessage(ConnectionManager connMgr, String filePath)
	{
		ProductFile externalFile = new ProductFile("External File", "External File Description", "1.0.0", "TXT", filePath);

		GMSEC.API.MIST.MESSAGE.ProductFileMessage productMessage;

		if (connMgr.GetSpecification().GetVersion() <= GMSEC_ISD.GMSEC_ISD_2016_00)
		{
			productMessage = new GMSEC.API.MIST.MESSAGE.ProductFileMessage(PROD_MESSAGE_SUBJECT, ResponseStatus.SUCCESSFUL_COMPLETION, "MSG.PROD.AUTO", connMgr.GetSpecification());
		}
		else
		{
			productMessage = new GMSEC.API.MIST.MESSAGE.ProductFileMessage(PROD_MESSAGE_SUBJECT, ResponseStatus.SUCCESSFUL_COMPLETION, "MSG.PROD", connMgr.GetSpecification());

			productMessage.AddField(new StringField("PROD-TYPE", "AUTO"));
			productMessage.AddField(new StringField("PROD-SUBTYPE", "DM"));
		}

		productMessage.AddProductFile(externalFile);

		connMgr.AddStandardFields(productMessage);

		return productMessage;
	}
}
