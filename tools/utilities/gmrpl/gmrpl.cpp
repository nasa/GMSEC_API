/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file gmrpl.cpp
 * 
 * A C++ example demonstration of GMSEC request / reply functionality.
 * The associated example gmreq will provide the request.  
 * gmrpl should be run before gmreq.
 * 
 */

#include "../Utility.h"

#include <string>
#include <vector>

#include <ctime>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::util;


class GMSEC_Reply : public Utility
{
public:
	GMSEC_Reply(const Config& config);

	~GMSEC_Reply();

	void usage(const std::string& programName);

	bool run();

private:
	typedef std::vector<std::string> Subjects;

	void sendReply(const Message& request, Message& reply);
	void sendReply(const Message& request, const std::string& subject);

	const char* getFieldValue(const Message& msg, const char* fieldName);

	ConnectionManager* connMgr;
	Subjects           subjects;
};


GMSEC_Reply::GMSEC_Reply(const Config& c)
	: Utility(c),
	  connMgr(0),
	  subjects()
{
	/* Initialize utility */
	initialize();
}


GMSEC_Reply::~GMSEC_Reply()
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


void GMSEC_Reply::usage(const std::string& programName)
{
	Utility::usage(programName);

	std::cerr << "\n\t\t* subject=<message subject>"
	          << "\n"
	          << "\n\t\t* msg-file=<file with message definition>"
	          << "\n"
	          << "\n\t\t* msg-timeout-ms=<timeout period (milliseconds)>"
	          << "\n"
	          << "\n\t\t* prog-timeout-s=<timeout period (seconds)>"
	          << "\n"
	          << "\n\t\t\tNote: msg-timeout-ms must be defined to use prog-timeout-s"
	          << "\n"
	          << std::endl;
}


bool GMSEC_Reply::run()
{
	bool success = true;

	//o output GMSEC API version
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the ConnectionManager
		connMgr = new ConnectionManager(getConfig());

		//o Connect
		connMgr->initialize();

		//o output connection middleware version
		GMSEC_INFO << connMgr->getLibraryVersion();

		//o Get info from command line
		std::string msgFile        = get("MSG-FILE", "");
		std::string cfgFile        = get("CFG-FILE", "");
		GMSEC_I32   msg_timeout_ms = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
		GMSEC_I32   prog_timeout_s = get("PROG-TIMEOUT-S", GMSEC_WAIT_FOREVER);

		unsigned int specVersion = connMgr->getSpecification().getVersion();
		Specification::SchemaLevel schemaLevel = connMgr->getSpecification().getSchemaLevel();

		//o Subscribe
		determineSubjects(specVersion, schemaLevel, subjects);

		//o Subscribe
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();

			(void) connMgr->subscribe(subjects[i].c_str());
		}

		bool   done = false;
		time_t prevTime;
		time_t nextTime;
		double elapsedTime = 0;

		time(&prevTime);

		while (!done)
		{
			if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
			{
				GMSEC_INFO << "Program Timeout Reached!";
				return true;
			}

			//o receive the next message
			Message* message = connMgr->receive(msg_timeout_ms);

			if (prog_timeout_s != GMSEC_WAIT_FOREVER)
			{
				time(&nextTime);
				elapsedTime += difftime(nextTime, prevTime);
				prevTime = nextTime;
			}

			if (!message)
			{
				GMSEC_INFO << "timeout occurred";
				continue;
			}
			else
			{
				//o With open response, we may receive our own reply message; just ignore.
				if (message->getKind() == Message::PUBLISH || message->getKind() == Message::REPLY)
				{
					continue;
				}

				done = (std::string(message->getSubject()) == "GMSEC.TERMINATE" ||
				        std::string(message->getSubject()) == "C2MS.TERMINATE");

				//o Display the XML representation of the received message
				GMSEC_INFO << "Received:\n" << message->toXML();

				//o check if we have a request message, and whether a response is required
				if (message->getKind() == Message::REQUEST &&
				    (message->getField("RESPONSE") == NULL || message->getIntegerValue("RESPONSE") == 1))
				{
					StdUniquePtr<Message> reply;
					std::string           reply_subject;

					if (!msgFile.empty())
					{
						reply.reset(readMessageFile(msgFile));

						reply_subject = reply->getSubject();
					}
					else if (!cfgFile.empty())
					{
						std::string msgName = get("MSG-NAME", "");

						if (msgName.empty())
						{
							GMSEC_ERROR << "When using cfg-file option, user must also provide msg-name option";
							return false;
						}

						reply.reset(readConfigFile(cfgFile, msgName));

						reply_subject = reply->getSubject();
					}
					else
					{
						// Prepare response message subject
						reply_subject = message->getSubject();
						reply_subject.replace(reply_subject.find(".REQ."), 5, ".RESP.");
						size_t comp_pos = reply_subject.find_last_of('.');
						reply_subject = reply_subject.substr(0, comp_pos+1);
						reply_subject += (message->hasField("COMPONENT") ? message->getStringValue("COMPONENT") : "FILL");
					}

					if (reply.get())
					{
						// Send custom reply
						sendReply(*message, *(reply.get()));
					}
					else
					{
						// Send canned reply
						sendReply(*message, reply_subject);
					}
				}

				//o Destroy request message */
				connMgr->release(message);
			}
		}
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


void GMSEC_Reply::sendReply(const Message& request, Message& reply)
{
	if (getConfig().getBooleanValue("ENCRYPT", false))
	{
		reply.addField("SEC-ENCRYPT", true);
	}

	//o Display XML representation of the reply message
	GMSEC_INFO << "Prepared Reply:\n" << reply.toXML();

	//o Send Reply
	connMgr->reply(request, reply);
}


void GMSEC_Reply::sendReply(const Message& request, const std::string& subject)
{
	//o Get fields from request message that we will add to the reply message
	const char* mission   = getFieldValue(request, "MISSION-ID");
	const char* cnst      = getFieldValue(request, "CONSTELLATION-ID");
	const char* satphys   = getFieldValue(request, "SAT-ID-PHYSICAL");
	const char* satlog    = getFieldValue(request, "SAT-ID-LOGICAL");
	const char* facility  = getFieldValue(request, "FACILITY");
	const char* component = getFieldValue(request, "COMPONENT");
	const char* msgID     = NULL;
	const char* requestID = NULL;

	if (connMgr->getSpecification().getVersion() == GMSEC_ISD_2014_00)
	{
		msgID = getFieldValue(request, "MSG-ID");
	}
	else if (connMgr->getSpecification().getVersion() >= GMSEC_ISD_2019_00)
	{
		requestID = getFieldValue(request, "REQUEST-ID");
	}

	//o Set Status Code. See C2MS document for available status codes.
	GMSEC_I16 responseStatus = (GMSEC_I16) mist::ResponseStatus::ACKNOWLEDGEMENT;

	std::ostringstream tmp;
	tmp << subject << "." << responseStatus;

	//o Create reply message
	MistMessage reply(tmp.str().c_str(), "RESP.DIR", connMgr->getSpecification());

    reply.setValue("MISSION-ID", (mission ? mission : "FILL"));
    reply.setValue("CONSTELLATION-ID", (cnst ? cnst : "FILL"));
    reply.setValue("SAT-ID-PHYSICAL", (satphys ? satphys : "FILL"));
    reply.setValue("SAT-ID-LOGICAL", (satlog ? satlog : "FILL"));
    reply.setValue("FACILITY", (facility ? facility : "FILL"));
	reply.setValue("COMPONENT", "GMRPL");
	reply.addField("RESPONSE-STATUS", responseStatus);

	if (connMgr->getSpecification().getVersion() == GMSEC_ISD_2014_00)
	{
		reply.setValue("MSG-ID", (msgID ? msgID : "FILL"));
	}
	else if (connMgr->getSpecification().getVersion() >= GMSEC_ISD_2018_00)
	{
		const char* dom1 = getFieldValue(request, "DOMAIN1");
		const char* dom2 = getFieldValue(request, "DOMAIN2");

		reply.setValue("DOMAIN1", (dom1 ? dom1 : "FILL"));
		reply.setValue("DOMAIN2", (dom2 ? dom2 : "FILL"));

		if (connMgr->getSpecification().getVersion() >= GMSEC_ISD_2019_00)
		{
			reply.setValue("REQUEST-ID", (requestID ? requestID : "0"));

			reply.setValue("DESTINATION-COMPONENT", (component ? component : "FILL"));
		}
	}

	if (getConfig().getBooleanValue("ENCRYPT", false))
	{
		reply.addField("SEC-ENCRYPT", true);
	}

	//o Display XML representation of the reply message
	GMSEC_INFO << "Prepared Reply:\n" << reply.toXML();

	//o Send Reply
	connMgr->reply(request, reply);
}


const char* GMSEC_Reply::getFieldValue(const Message& msg, const char* fieldName)
{
	const char* value;

	try
	{
		value = msg.getStringValue(fieldName);
	}
	catch (...)
	{
		value = NULL;
	}

	return value;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	GMSEC_Reply gmrpl(config);

	if (!gmrpl.areOptionsValid(argc, "gmrpl"))
	{
		gmrpl.usage("gmrpl");
		return 1;
	}

	gmrpl.addToConfigFromFile();

	return gmrpl.run() ? 0 : 1;
}
