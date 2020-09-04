/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file validation_custom.cpp
 * 
 * This file contains an example demonstrating how to implement additional
 * Message validation logic in addition to that which the GMSEC API provides.
 *
 * Note: This example focuses on adding additional validation upon the receipt
 * of a message.  It almost goes without saying that additional logic can be
 * added to a program prior to invoking the publish() function without having
 * to do anything special.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>
#include <sstream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;

const char* PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE";

//o Helper functions
void initializeLogging(Config& config);
void setupStandardFields(ConnectionManager& connMgr);
mist::message::ProductFileMessage createProductFileMessage(const ConnectionManager& connMgr, const char* filePath);
bool isProdMsg(Message message);

//o Create a callback and define message content validation logic which will
// be used in combination with the GMSEC API validation.
class ValidationCallback: public ConnectionManagerCallback
{
public:
	virtual void CALL_TYPE onMessage(ConnectionManager& connMgr, const Message& message)
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
				mist::message::ProductFileMessage prodMessage(message.toXML());

				//o Extract the Product File URI location(s) from the
				// message using a ProductFileIterator
				ProductFileIterator& prodIter = prodMessage.getProductFileIterator();

				while(prodIter.hasNext())
				{
					ProductFile prodFile = prodIter.next();

					//o Check to ensure that the URI contains "//hostname/dir"
					std::string prodUri = prodFile.getURI();
					if (prodUri.find("//hostname/dir") == std::string::npos)
					{
						std::stringstream errorMsg;
						errorMsg << "Received an invalid PROD Message (bad URI):\n" << message.toXML();
						throw Exception(MIST_ERROR, MESSAGE_FAILED_VALIDATION, errorMsg.str().c_str());
					}
				}

				GMSEC_INFO << "Received a valid message:\n" << message.toXML();
			}
		}
		catch (Exception e)
		{
			GMSEC_ERROR << e.what();
		}
	}
};


int main (int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	Config config(argc, argv);

	initializeLogging(config);

	//o Enable Message validation.  This parameter is "false" by default.
	config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

	// TODO: Once available, replace this statement with usage of
	// ConnectionManager::getAPIVersion (See RTC 4798)
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		ConnectionManager connMgr(config);

		GMSEC_INFO << "Opening the connection to the middleware server";
		connMgr.initialize();

		GMSEC_INFO << connMgr.getLibraryVersion();

		//o Set up the ValidationCallback and subscribe
		ValidationCallback vc;
		connMgr.subscribe(PROD_MESSAGE_SUBJECT, &vc);

		//o Start the AutoDispatcher
		connMgr.startAutoDispatch();

		//o Create and publish a simple Product File Message
		setupStandardFields(connMgr);

		mist::message::ProductFileMessage productMessage = createProductFileMessage(connMgr, "//hostname/dir/filename");

		//o Publish the message to the middleware bus
		connMgr.publish(productMessage);

		productMessage = createProductFileMessage(connMgr, "//badhost/dir/filename");

		connMgr.publish(productMessage);

		//o Disconnect from the middleware and clean up the Connection
		connMgr.cleanup();
	}
	catch (Exception e)
	{
		GMSEC_ERROR << e.what();
		return -1;
	}

	return 0;
}


void initializeLogging(Config& config)
{
	const char* logLevel  = config.getValue("LOGLEVEL");
	const char* logFile   = config.getValue("LOGFILE");

	if (!logLevel)
	{
		config.addValue("LOGLEVEL", "INFO");
	}
	if (!logFile)
	{
		config.addValue("LOGFILE", "STDERR");
	}
}


void setupStandardFields(ConnectionManager& connMgr)
{
	DataList<Field*> definedFields;

	StringField missionField("MISSION-ID", "MISSION");
	StringField satIdField("SAT-ID-PHYSICAL", "SPACECRAFT");
	StringField facilityField("FACILITY", "GMSEC Lab");
	StringField componentField("COMPONENT", "validation_custom");

	definedFields.push_back(&missionField);
	definedFields.push_back(&satIdField);
	definedFields.push_back(&facilityField);
	definedFields.push_back(&componentField);

	connMgr.setStandardFields(definedFields);
}


mist::message::ProductFileMessage createProductFileMessage(const ConnectionManager& connMgr, const char* filePath)
{
	ProductFile externalFile("External File", "External File Description", "1.0.0", "TXT", filePath);

	mist::message::ProductFileMessage productMessage(PROD_MESSAGE_SUBJECT, ResponseStatus::SUCCESSFUL_COMPLETION, Message::PUBLISH, "AUTO", "DM", connMgr.getSpecification());
	productMessage.addProductFile(externalFile);

	connMgr.addStandardFields(productMessage);

	return productMessage;
}


bool isProdMsg(const Message message)
{
	bool result =
		strcmp(message.getStringValue("MESSAGE-TYPE"), "MSG") == 0
		&& strcmp(message.getStringValue("MESSAGE-SUBTYPE"), "PROD") == 0;

	return result;
}

