/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file gmreq.cpp
 * 
 * A C++ example demonstration of GMSEC request / reply functionality.
 * The associated example gmrpl will provide the reply.  
 * gmrpl should be run before gmreq.
 * 
 */

#include "../Utility.h"

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::util;


const char* const GMSEC_REQUEST_MESSAGE_SUBJECT = "GMSEC.MSSN.SAT1.REQ.DIR.GMRPL";
const char* const GMSEC_REPLY_MESSAGE_SUBJECT   = "GMSEC.MSSN.SAT1.RESP.DIR.GMREQ.+";

const char* const C2MS_REQUEST_MESSAGE_SUBJECT  = "C2MS.DOM1.DOM2.MSSN.CNS1.SAT1.REQ.DIR.GMRPL";
const char* const C2MS_REPLY_MESSAGE_SUBJECT    = "C2MS.DOM1.DOM2.MSSN.CNS1.SAT1.RESP.DIR.GMREQ.+";


class GMSEC_Request : public Utility
{
public:
	GMSEC_Request(const Config& config);

	~GMSEC_Request();

	void usage(const std::string& programName);

	bool run();

private:
	void request(Message& msg, int msg_timeout_ms, int req_republish_period);
	void request(const std::string& subject, int msg_timeout_ms, int req_republish_period);

	ConnectionManager* connMgr;
};


GMSEC_Request::GMSEC_Request(const Config& c)
	: Utility(c),
	  connMgr(0)
{
	// Initialize utility
	initialize();
}


GMSEC_Request::~GMSEC_Request()
{
	if (connMgr)
	{
		try
		{
			connMgr->cleanup();
		}
		catch (const Exception& e)
		{
			GMSEC_ERROR << e.what();
		}

		delete connMgr;
	}

	ConnectionManager::shutdownAllMiddlewares();
}


void GMSEC_Request::usage(const std::string& programName)
{
	Utility::usage(programName);

	std::cerr << "\n\t\t* subject=<message subject>"
	          << "\n"
	          << "\n\t\t* msg-file=<file with message definition>"
	          << "\n"
	          << "\n\t\t* msg-timeout-ms=<timeout period (milliseconds)>"
	          << "\n"
	          << "\n\t\t* req-republish-timeout-ms=<timeout period (milliseconds)>"
	          << "\n"
	          << std::endl;
}


bool GMSEC_Request::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the ConnectionManager
		connMgr = new ConnectionManager(getConfig());

		//o Connect
		connMgr->initialize();

		//o output connection middleware version
		GMSEC_INFO << connMgr->getLibraryVersion();

		//o Get information from the command line */
		std::string msgFile          = get("MSG-FILE", "");
		std::string cfgFile          = get("CFG-FILE", "");
		GMSEC_I32   msg_timeout_ms   = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
		GMSEC_I32   req_repub_tmo_ms = get("REQ-REPUBLISH-TIMEOUT-MS", GMSEC_REQUEST_REPUBLISH_NEVER);
		std::string subject;

		StdUniquePtr<Message> msg;

		Specification::SchemaLevel schemaLevel = connMgr->getSpecification().getSchemaLevel();

		if (!msgFile.empty())
		{
			msg.reset(readMessageFile(msgFile));

			subject = msg->getSubject();
		}
		else if (!cfgFile.empty())
		{
			std::string msgName = get("MSG-NAME", "");

			if (msgName.empty())
			{
				GMSEC_ERROR << "When using cfg-file option, user must also provide msg-name option";
				return false;
			}

			msg.reset(readConfigFile(cfgFile, msgName));

			subject = msg->getSubject();
		}
		else
		{
			if (connMgr->getSpecification().getVersion() >= GMSEC_ISD_2019_00)
			{
				subject = get("SUBJECT", (schemaLevel == Specification::LEVEL_2 ? GMSEC_REQUEST_MESSAGE_SUBJECT : C2MS_REQUEST_MESSAGE_SUBJECT));

				if ((connMgr->getSpecification().getSchemaLevel() == Specification::LEVEL_1) && (subject.find("C2MS") != std::string::npos))
				{
					subject.replace(0, 4, "C2MS-11B");
				}
			}
			else if (connMgr->getSpecification().getVersion() == GMSEC_ISD_2018_00)
			{
				subject = get("SUBJECT", (schemaLevel == Specification::LEVEL_1 ? GMSEC_REQUEST_MESSAGE_SUBJECT : C2MS_REQUEST_MESSAGE_SUBJECT));
			}
			else
			{
				subject = get("SUBJECT", GMSEC_REQUEST_MESSAGE_SUBJECT);
			}
		}

		//o Check if we're configured for Open-Response
		std::string responseBehavior = getConfig().getValue("gmsec-req-resp", "");

		if (responseBehavior == "open-resp" || responseBehavior == "OPEN-RESP")
		{
			std::string subscribeTo;

			if (connMgr->getSpecification().getVersion() >= GMSEC_ISD_2019_00)
			{
				subscribeTo = (schemaLevel == Specification::LEVEL_2 ? GMSEC_REPLY_MESSAGE_SUBJECT : C2MS_REPLY_MESSAGE_SUBJECT);

				if ((connMgr->getSpecification().getSchemaLevel() == Specification::LEVEL_1) && (subject.find("C2MS") != std::string::npos))
				{
					subscribeTo.replace(0, 4, "C2MS-11B");
				}
			}
			else if (connMgr->getSpecification().getVersion() == GMSEC_ISD_2018_00)
			{
				subscribeTo = (schemaLevel == Specification::LEVEL_1 ? GMSEC_REPLY_MESSAGE_SUBJECT : C2MS_REPLY_MESSAGE_SUBJECT);
			}
			else
			{
				subscribeTo = GMSEC_REPLY_MESSAGE_SUBJECT;
			}

			GMSEC_INFO << "Subscribing to Open-Response Subject: " << subscribeTo.c_str();

			(void) connMgr->subscribe(subscribeTo.c_str());
		}

		//o Output information
		GMSEC_INFO << "Issuing Request Message with Subject: " << subject.c_str();

		if (msg.get())
		{
			// Send custom request message
			request(*(msg.get()), msg_timeout_ms, req_repub_tmo_ms);
		}
		else
		{
			// Send canned request message
			request(subject, msg_timeout_ms, req_repub_tmo_ms);
		}
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


void GMSEC_Request::request(Message& request, int msg_timeout_ms, int req_republish_period)
{
	if (getConfig().getBooleanValue("ENCRYPT", false))
	{
		request.addField("SEC-ENCRYPT", true);
	}

	//o Display XML representation of request message
	GMSEC_INFO << "Requesting:\n" << request.toXML();

	// If the RESPONSE field is not present, or it is and its value is true, then seek
	// a reply.  Otherwise do not.
	if (request.getField("RESPONSE") == NULL || request.getIntegerValue("RESPONSE") == 1)
	{
		//o Send Request Message
		Message* reply = connMgr->request(request, msg_timeout_ms, req_republish_period);

		if (reply)
		{
			//o Got reply; display XML representation of reply message
			GMSEC_INFO << "Received reply:\n" << reply->toXML();

			//o Destroy the reply message
			connMgr->release(reply);
		}
		else
		{
			GMSEC_WARNING << "Reply message not receive after a timeout of " << msg_timeout_ms << " msec.";
		}
	}
	else
	{
		(void) connMgr->request(request, 10, -1);
	}
}


void GMSEC_Request::request(const std::string& subject, int msg_timeout_ms, int req_republish_period)
{
	//o Define standard fields
	DataList<Field*> standardFields;

	StringField mission("MISSION-ID", "MSSN");
	StringField cnst("CONSTELLATION-ID", "CNS1");
	StringField satphys("SAT-ID-PHYSICAL", "SAT1");
	StringField satlog("SAT-ID-LOGICAL", "SAT1");
	StringField facility("FACILITY", "MY-FACILITY");
	StringField comp("COMPONENT", "GMREQ");
	StringField domain1("DOMAIN1", "DOM1");
	StringField domain2("DOMAIN2", "DOM2");

	standardFields.add(&mission);
	standardFields.add(&cnst);
	standardFields.add(&satphys);
	standardFields.add(&satlog);
	standardFields.add(&facility);
	standardFields.add(&comp);

	if (connMgr->getSpecification().getVersion() >= mist::GMSEC_ISD_2018_00)
	{
		standardFields.add(&domain1);
		standardFields.add(&domain2);
	}

	MistMessage::setStandardFields(standardFields);

	//o Create message
	MistMessage request(subject.c_str(), "REQ.DIR", connMgr->getSpecification());

	MistMessage::clearStandardFields();

	//o Add fields to message
	request.addField("DIRECTIVE-KEYWORD", "QUESTION");
	request.addField("DIRECTIVE-STRING", "DOES_THE_REQUEST_REPLY_FUNCTIONALITY_STILL_WORK");
	request.addField("RESPONSE", true);

	if (connMgr->getSpecification().getVersion() == mist::GMSEC_ISD_2014_00)
	{
		request.setValue("MSG-ID", "MY-MESSAGE-ID");
	}
	else if (connMgr->getSpecification().getVersion() >= mist::GMSEC_ISD_2019_00)
	{
		request.addField("REQUEST-ID", (GMSEC_U16) 1);
	}

	if (getConfig().getBooleanValue("ENCRYPT", false))
	{
		request.addField("SEC-ENCRYPT", true);
	}

	//o Display XML representation of request message
	GMSEC_INFO << "Requesting:\n" << request.toXML();

	//o Send Request Message
	Message* reply = connMgr->request(request, msg_timeout_ms, req_republish_period);

	if (reply)
	{
		//o Got reply; display XML representation of reply message
		GMSEC_INFO << "Received reply:\n" << reply->toXML();

		//o Destroy the reply message
		connMgr->release(reply);
	}
	else
	{
		GMSEC_WARNING << "Reply message not receive after a timeout of " << msg_timeout_ms << " msec.";
	}
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	GMSEC_Request gmreq(config);

	if (!gmreq.areOptionsValid(argc))
	{
		gmreq.usage("gmreq");
		return 1;
	}

	gmreq.addToConfigFromFile();

	gmreq.run();
}
