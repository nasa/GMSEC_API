/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file gmrpl.cpp
 */

#include "../Utility.h"

#include <string>
#include <vector>

#include <ctime>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class GMSEC_Reply : public Utility
{
public:
	GMSEC_Reply(const Config& config);

	~GMSEC_Reply();

	void usage(const std::string& programName);

	bool run();

private:
	typedef std::vector<std::string> Subjects;

	void sendReply(const Message& request, const Message& reply);
	void genAndSendReply(const Message& request);

	const char* getFieldValue(const Message& msg, const char* fieldName);

	void determineSubjects(unsigned int specVersion, Specification::SchemaLevel schemaLevel, std::vector<std::string>& subjects);

	Connection* conn;
};


GMSEC_Reply::GMSEC_Reply(const Config& c)
	: Utility(c),
	  conn(0)
{
	/* Initialize utility */
	initialize();
}


GMSEC_Reply::~GMSEC_Reply()
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
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		conn = new Connection(getConfig());

		//o output connection middleware version
		GMSEC_INFO << conn->getLibraryVersion();

		//o Connect
		conn->connect();

		//o Get info from command line
		std::string msgFile        = get("MSG-FILE", "");
		std::string cfgFile        = get("CFG-FILE", "");
		GMSEC_I32   msg_timeout_ms = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
		GMSEC_I32   prog_timeout_s = get("PROG-TIMEOUT-S", GMSEC_WAIT_FOREVER);

		unsigned int specVersion = conn->getMessageFactory().getSpecification().getVersion();
		Specification::SchemaLevel schemaLevel = conn->getMessageFactory().getSpecification().getSchemaLevel();

		//o Subscribe
		Subjects subjects;
		this->determineSubjects(specVersion, schemaLevel, subjects);

		//o Subscribe
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();

			(void) conn->subscribe(subjects[i].c_str(), getConfig());
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
			Message* message = conn->receive(msg_timeout_ms);

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
				//o With an overly broad subscription topic, we may receive our own reply message
				//  or other messages; just ignore.
				if (message->getKind() == Message::Kind::PUBLISH || message->getKind() == Message::Kind::REPLY)
				{
					continue;
				}

				//o Display XML/JSON representation of request message
				Utility::displayMessage("Received:\n", *message);

				//o check if we have a request message, and whether a response is required
				if (message->getKind() == Message::Kind::REQUEST &&
				    (message->hasField("RESPONSE") && message->getBooleanValue("RESPONSE") == true))
				{
					StdUniquePtr<Message> reply;

					if (!msgFile.empty())
					{
						reply.reset(readMessageFile(msgFile));
					}
					else if (!cfgFile.empty())
					{
						std::string msgName = get("MSG-NAME", "");

						if (!msgName.empty())
						{
							reply.reset(readConfigFile(cfgFile, msgName));
						}
					}

					if (reply.get())
					{
						// Send custom reply
						sendReply(*message, *(reply.get()));
					}
					else
					{
						// Send canned reply
						genAndSendReply(*message);
					}
				}

				//o Acknowledge processing the request message */
				message->acknowledge();

				//o Destroy request message */
				Message::destroy(message);
			}
		}
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


void GMSEC_Reply::sendReply(const Message& request, const Message& reply)
{
	//o Display XML/JSON representation of reply message
	Utility::displayMessage("Prepared Reply:\n", reply);

	//o Send Reply
	conn->reply(request, reply);
}


void GMSEC_Reply::genAndSendReply(const Message& request)
{
	//o Get fields from request message that we will add to the reply message
	const char* domain1   = getFieldValue(request, "DOMAIN1");
	const char* domain2   = getFieldValue(request, "DOMAIN2");
	const char* mission   = getFieldValue(request, "MISSION-ID");
	const char* cnst      = getFieldValue(request, "CONSTELLATION-ID");
	const char* satphys   = getFieldValue(request, "SAT-ID-PHYSICAL");
	const char* satlog    = getFieldValue(request, "SAT-ID-LOGICAL");
	const char* facility  = getFieldValue(request, "FACILITY");
	const char* component = getFieldValue(request, "COMPONENT");
	const char* requestID = getFieldValue(request, "REQUEST-ID");

	//o Set Status Code. See C2MS document for available status codes.
	GMSEC_I16 responseStatus = static_cast<GMSEC_I16>(Message::ResponseStatus::ACKNOWLEDGEMENT);

	//o Create reply message
	Message reply( conn->getMessageFactory().createMessage("RESP.DIR") );

	reply.setFieldValue("DOMAIN1", (domain1 ? domain1 : "FILL"));
	reply.setFieldValue("DOMAIN2", (domain2 ? domain2 : "FILL"));
	reply.setFieldValue("MISSION-ID", (mission ? mission : "FILL"));
	reply.setFieldValue("CONSTELLATION-ID", (cnst ? cnst : "FILL"));
	reply.setFieldValue("SAT-ID-PHYSICAL", (satphys ? satphys : "FILL"));
	reply.setFieldValue("SAT-ID-LOGICAL", (satlog ? satlog : "FILL"));
	reply.setFieldValue("FACILITY", (facility ? facility : "FILL"));
	reply.setFieldValue("COMPONENT", "GMRPL");
	reply.setFieldValue("REQUEST-ID", (requestID ? requestID : "0"));
	reply.addField("DESTINATION-COMPONENT", (component ? component : "FILL"));
	reply.addField("RESPONSE-STATUS", responseStatus);

	//o Display XML/JSON representation of reply message
	Utility::displayMessage("Prepared Reply:\n", reply);

	//o Send Reply
	conn->reply(request, reply);
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


void GMSEC_Reply::determineSubjects(unsigned int specVersion, Specification::SchemaLevel schemaLevel, std::vector<std::string>& subjects)
{
	std::string value = get("SUBJECT");

	if (!value.empty())
	{
		subjects.push_back(value);
	}
	else
	{
		if (specVersion >= GMSEC_MSG_SPEC_2019_00)
		{
			switch (schemaLevel)
			{
			case Specification::SchemaLevel::LEVEL_0:
				subjects.push_back("C2MS.*.*.*.*.*.REQ.DIR.>");
				break;
			case Specification::SchemaLevel::LEVEL_1:
				subjects.push_back("C2MS-PIPE.*.*.*.*.*.REQ.DIR.>");
				break;
			case Specification::SchemaLevel::LEVEL_2:
			default:
				subjects.push_back("GMSEC.*.*.REQ.DIR.>");
				break;
			}
		}
		else if (specVersion == GMSEC_MSG_SPEC_2018_00)
		{
			switch (schemaLevel)
			{
			case Specification::SchemaLevel::LEVEL_0:
				subjects.push_back("C2MS.*.*.*.*.*.REQ.DIR.>");
				break;
			case Specification::SchemaLevel::LEVEL_1:
			default:
				subjects.push_back("GMSEC.*.*.REQ.DIR.>");
				break;
			}
		}
		else
		{
			subjects.push_back("C2MS.*.*.*.*.*.REQ.DIR.>");
		}
	}
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	try
	{
		GMSEC_Reply gmrpl(config);

		if (!gmrpl.areOptionsValid(argc, "gmrpl"))
		{
			gmrpl.usage("gmrpl");
			return 1;
		}

		gmrpl.addToConfigFromFile();

		return gmrpl.run() ? 0 : 1;
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		return 1;
	}
}
