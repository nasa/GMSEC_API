/*
 * Copyright 2007-2019 United States Government as represented by the
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
using namespace gmsec::api::util;


const char* const DEFAULT_REQUEST_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.GMRPL";
const char* const DEFAULT_REPLY_MESSAGE_SUBJECT   = "GMSEC.MY-MISSION.MY-SAT-ID.RESP.DIR.GMRPL.+";


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
	          << "\n\t\t* cfg-file=<configuration file>"
	          << "\n"
	          << "\n\t\t\tNote: msg-name must be defined when using cfg-file"
	          << "\n"
	          << "\n\t\t* msg-name=<name of message within configuration file>"
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
		int         msg_timeout_ms   = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
		int         req_repub_tmo_ms = get("REQ-REPUBLISH-TIMEOUT-MS", 1000);
		std::string subject;

		StdUniquePtr<Message> msg;

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
			subject = get("SUBJECT", DEFAULT_REQUEST_MESSAGE_SUBJECT);
		}

		//o Check if we're configured for Open-Response
		std::string responseBehavior = getConfig().getValue("gmsec-req-resp", "");

		if (responseBehavior == "open-resp" || responseBehavior == "OPEN-RESP")
		{
			GMSEC_INFO << "Subscribing to Open-Response Subject: " << DEFAULT_REPLY_MESSAGE_SUBJECT;

			(void) connMgr->subscribe(DEFAULT_REPLY_MESSAGE_SUBJECT);
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
	//o Create message
	Message request(subject.c_str(), Message::REQUEST);

	//o Add fields to message
	request.addField("QUESTION", "Does the request/reply functionality still work?");
	request.addField("COMPONENT", "GMREQ");
	request.addField("RESPONSE", true);

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

	gmreq.addToConfigFromFile();

	if (gmreq.isOptionInvalid(argc))
	{
		gmreq.usage("gmreq");
		return 1;
	}

	gmreq.run();
}
