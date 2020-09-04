/*
 * Copyright 2007-2018 United States Government as represented by the
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

#include <memory>

using namespace gmsec::api;
using namespace gmsec::api::util;


class GMSEC_Request : public Utility
{
public:
	GMSEC_Request(const Config& config);

	~GMSEC_Request();

	void usage(const std::string& programName);

	bool run();

private:
	Connection* connection;

	void request(Message& msg, int msg_timeout_ms, int req_republish_period);
	void request(const std::string& subject, int msg_timeout_ms, int req_republish_period);
};


GMSEC_Request::GMSEC_Request(const Config& c)
	: Utility(c),
	  connection(0)
{
	// Initialize utility
	initialize();
}


GMSEC_Request::~GMSEC_Request()
{
	if (connection)
	{
		try
		{
			connection->disconnect();
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << e.what();
		}

		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
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
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(getConfig());

		//o Connect
		connection->connect();

		//o output connection middleware version
		GMSEC_INFO << connection->getLibraryVersion();

		//o Get information from the command line */
		std::string msgFile          = get("MSG-FILE", "");
		std::string cfgFile          = get("CFG-FILE", "");
		int         msg_timeout_ms   = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
		int         req_repub_tmo_ms = get("REQ-REPUBLISH-TIMEOUT-MS", GMSEC_REQUEST_REPUBLISH_NEVER);
		std::string subject;

		std::auto_ptr<Message> msg;

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
			subject = get("SUBJECT", "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL");
		}

		//o Output information
		GMSEC_INFO << "Using subject '" << subject.c_str() << "'";

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
	catch (Exception& e)
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
		Message* reply = connection->request(request, msg_timeout_ms, req_republish_period);

		if (reply)
		{
			//o Got reply; display XML representation of reply message
			GMSEC_INFO << "Received reply:\n" << reply->toXML();

			//o Destroy the reply message
			connection->release(reply);
		}
		else
		{
			GMSEC_WARNING << "Reply message not receive after a timeout of " << msg_timeout_ms << " msec.";
		}
	}
	else
	{
		(void) connection->request(request, 10, -1);
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
	Message* reply = connection->request(request, msg_timeout_ms, req_republish_period);

	if (reply)
	{
		//o Got reply; display XML representation of reply message
		GMSEC_INFO << "Received reply:\n" << reply->toXML();

		//o Destroy the reply message
		connection->release(reply);
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
