/*
 * Copyright 2007-2023 United States Government as represented by the
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
 */

#include "../Utility.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class GMSEC_Request : public Utility
{
public:
	GMSEC_Request(const Config& config);

	~GMSEC_Request();

	void usage(const std::string& programName);

	bool run();

private:
	void request(Message& msg, int msg_timeout_ms, int req_republish_period);
	void request(const std::string& subject, const std::string& resp_topic, int msg_timeout_ms, int req_republish_period);

	Connection* conn;
};


GMSEC_Request::GMSEC_Request(const Config& c)
	: Utility(c),
	  conn(0)
{
	// Initialize utility
	initialize();
}


GMSEC_Request::~GMSEC_Request()
{
	if (conn)
	{
		try
		{
			conn->disconnect();
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR << e.what();
		}

		delete conn;
	}

	Connection::shutdownAllMiddlewares();
}


void GMSEC_Request::usage(const std::string& programName)
{
	Utility::usage(programName);

	std::cerr << "\n\t\t* subject=<request message subject>"
	          << "\n"
	          << "\n\t\t* resp-topic=<custom topic for response message subscription; required if subject is used>"
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
	GMSEC_INFO << Connection::getAPIVersion();

	//o Get information from the command line */
	std::string subject          = get("SUBJECT", "");
	std::string resp_topic       = get("RESP-TOPIC", "");
	std::string msgFile          = get("MSG-FILE", "");
	std::string cfgFile          = get("CFG-FILE", "");
	GMSEC_I32   msg_timeout_ms   = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
	GMSEC_I32   req_repub_tmo_ms = get("REQ-REPUBLISH-TIMEOUT-MS", GMSEC_REQUEST_REPUBLISH_NEVER);
	Config      config           = getConfig();  // make a copy

	StdUniquePtr<Message> msg;

	if (!subject.empty() && resp_topic.empty())
	{
		GMSEC_WARNING << "A custom response topic is required if a custom message subject is provided";

		usage("gmreq");

		return false;
	}

	if (!resp_topic.empty())
	{
		config.addValue("MW-SUBSCRIBE-FOR-RESP", "false");
	}

	if (!msgFile.empty())
	{
		msg.reset(readMessageFile(msgFile));
	}
	else if (!cfgFile.empty())
	{
		std::string msgName = get("MSG-NAME", "");

		if (msgName.empty())
		{
			subject = get("SUBJECT", "");
		}
		else
		{
			msg.reset(readConfigFile(cfgFile, msgName));
		}
	}

	if (msg.get() && !subject.empty())
	{
		msg->setSubject(subject.c_str());
	}

	try
	{
		//o Create the Connection
		conn = new Connection(config);

		//o Connect
		conn->connect();

		//o output connection middleware version
		GMSEC_INFO << conn->getLibraryVersion();

		if (msg.get())
		{
			// Send custom request message
			request(*(msg.get()), msg_timeout_ms, req_repub_tmo_ms);
		}
		else
		{
			// Send canned request message
			request(subject, resp_topic, msg_timeout_ms, req_repub_tmo_ms);
		}
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


void GMSEC_Request::request(Message& request, int msg_timeout_ms, int req_republish_period)
{
	//o Display XML/JSON representation of request message
	Utility::displayMessage("Requesting:\n", request);

	// If the RESPONSE field is not present, or it is and its value is true, then seek
	// a reply.  Otherwise do not.
	if (request.hasField("RESPONSE") && request.getBooleanValue("RESPONSE") == true)
	{
		//o Send Request Message
		Message* reply = conn->request(request, msg_timeout_ms, req_republish_period);

		if (reply)
		{
			//o Got reply; display XML/JSON representation of reply message
			Utility::displayMessage("Received reply:\n", *reply);

			//o Acknlowedge processing the reply message
			reply->acknowledge();

			//o Destroy the reply message
			Message::destroy(reply);
		}
		else
		{
			GMSEC_WARNING << "Reply message not receive after a timeout of " << msg_timeout_ms << " msec.";
		}
	}
	else
	{
		(void) conn->request(request, 10, -1);
	}
}


void GMSEC_Request::request(const std::string& subject,
                            const std::string& resp_topic,
                            int msg_timeout_ms,
                            int req_republish_period)
{
	// TODO: If a custom subject is provided, then ideally these fields
	// should be derived from it, perhaps going as far as to use
	// FILL for values that cannot be extracted from the subject.
	Utility::setStandardFields( conn->getMessageFactory(), "GMREQ" );

	//o Create message
	Message request( conn->getMessageFactory().createMessage("REQ.DIR") );

	//o Add fields to message
	request.addField("DESTINATION-COMPONENT", "GMRPL");
	request.addField("DIRECTIVE-KEYWORD", "QUESTION");
	request.addField("DIRECTIVE-STRING", "DOES_THE_REQUEST_REPLY_FUNCTIONALITY_STILL_WORK");
	request.addField("RESPONSE", true);
	request.addField("REQUEST-ID", (GMSEC_U16) 1);

	if (!subject.empty())
	{
		request.setSubject(subject.c_str());
	}

	if (!resp_topic.empty())
	{
		GMSEC_INFO << "Subscribing for response: " << resp_topic.c_str();
		conn->subscribe(resp_topic.c_str());
	}

	//o Display XML/JSON representation of request message
	Utility::displayMessage("Requesting:\n", request);

	//o Send Request Message
	Message* reply = conn->request(request, msg_timeout_ms, req_republish_period);

	if (reply)
	{
		//o Display XML/JSON representation of reply message
		Utility::displayMessage("Received reply:\n", *reply);

		//o Acknlowedge processing the reply message
		reply->acknowledge();

		//o Destroy the reply message
		Message::destroy(reply);
	}
	else
	{
		GMSEC_WARNING << "Reply message not receive after a timeout of " << msg_timeout_ms << " msec.";
	}
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	try
	{
		GMSEC_Request gmreq(config);

		if (!gmreq.areOptionsValid(argc))
		{
			gmreq.usage("gmreq");
			return 1;
		}

		gmreq.addToConfigFromFile();

		gmreq.run();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		return 1;
	}
}
