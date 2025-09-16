/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmlog.cpp
 */

#include "../Utility.h"

#include <string>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class GMSEC_Logger : public Utility
{
public:
	GMSEC_Logger(const Config& config);

	~GMSEC_Logger();

	void usage(const std::string& programName);

	bool run();

private:
	void publish(Message& msg);
	void publish(GMSEC_U32 interval_ms);

	StdUniquePtr<Connection> conn;

	std::string m_subject;
	std::string m_subclass;
	std::string m_occurType;
	std::string m_msgText;
	GMSEC_I16   m_severity;
};


GMSEC_Logger::GMSEC_Logger(const Config& c)
	: Utility(c),
	  conn(0)
{
	initialize();
}


GMSEC_Logger::~GMSEC_Logger()
{
	if (conn.get())
	{
		try
		{
			conn->disconnect();
			conn.reset();
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR << e.what();
		}
	}

	Connection::shutdownAllMiddlewares();
}


void GMSEC_Logger::usage(const std::string& programName)
{
	Utility::usage(programName);

	std::cerr << "\n\t\t* subject=<message subject>"
	          << "\n"
	          << "\n\t\t* subclass=<sub-classification>"
	          << "\n"
	          << "\n\t\t* occur-type=<occurrence type>"
	          << "\n"
	          << "\n\t\t* severity=<severity>"
	          << "\n"
	          << "\n\t\t* msg-text=<message text>"
	          << "\n"
	          << "\n\t\t* msg-file=<file with message definition>"
	          << "\n"
	          << "\n\t\t* iterations=<number of messages to publish, or -1 to publish indefinitely>"
	          << "\n"
	          << "\n\t\t* interval-ms=<publish interval (milliseconds)>"
	          << "\n"
	          << std::endl;
}


bool GMSEC_Logger::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		conn.reset( new Connection(getConfig()) );

		//o Connect
		conn->connect();

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		//o Get information from the command line
		m_subclass  = get("SUBCLASS", "GMSEC");
		m_occurType = get("OCCUR-TYPE", "API");
		m_severity  = get("SEVERITY", 1);
		m_msgText   = get("MSG-TEXT", "GMSEC Rocks!");

		std::string msgFile     = get("MSG-FILE", "");
		std::string cfgFile     = get("CFG-FILE", "");
		GMSEC_I32   iterations  = get("ITERATIONS", 1);
		GMSEC_U32   interval_ms = get("INTERVAL-MS", 1000);

		StdUniquePtr<Message> msg;

		if (!msgFile.empty())
		{
			msg.reset(readMessageFile(msgFile));

			m_subject = msg->getSubject();
		}
		else if (!cfgFile.empty())
		{
			std::string msgName = get("MSG-NAME", "");

			if (msgName.empty())
			{
				m_subject = get("SUBJECT", "");
			}
			else
			{
				msg.reset(readConfigFile(cfgFile, msgName));

				m_subject = msg->getSubject();
			}
		}
		else
		{
			m_subject = get("SUBJECT", "");
		}

		//o Output information
		if (iterations < -1 || iterations == 0)
		{
			GMSEC_INFO << "No messages will be published";
		}
		else if (iterations == -1)
		{
			GMSEC_INFO << "Publishing indefinitely";
		}
		else if (iterations > 0)
		{
			GMSEC_INFO << "Publishing " << iterations << " message(s)";
		}

		if (iterations == -1 || iterations >= 1)
		{
			GMSEC_INFO << "Publish interval " << interval_ms << " [ms]";
		}

		//o Publish message(s)
		for (GMSEC_I32 i = 0; i < iterations || iterations == -1; ++i)
		{
			if (msg.get())
			{
				// Publish custom message
				publish(*msg.get());
			}
			else
			{
				// Publish canned message
				publish(interval_ms);
			}

			if (i < (iterations - 1) || iterations == -1)
			{
				TimeUtil::millisleep(interval_ms);
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


void GMSEC_Logger::publish(Message& message)
{
	//o Publish Message
	conn->publish(message, getConfig());

	//o Display XML/JSON representation of message
	Utility::displayMessage("Published:\n", message);
}


void GMSEC_Logger::publish(GMSEC_U32 interval_ms)
{
	// TODO: If a custom subject is provided, then ideally these fields
	// should be derived from it, perhaps going as far as to use
	// FILL for values that cannot be extracted from the subject.
	Utility::setStandardFields( conn->getMessageFactory(), "GMLOG" );

	const char* schemaID = "LOG";
	Message message( conn->getMessageFactory().createMessage(schemaID) );

	if (!m_subject.empty())
	{
		message.setSubject(m_subject.c_str());
	}

	char theTime[GMSEC_TIME_BUFSIZE + 1];
	TimeUtil::formatTime( TimeUtil::getCurrentTime(), theTime );

	//o Add required fields
	message.addField("SUBCLASS", m_subclass.c_str());
	message.addField("OCCURRENCE-TYPE", m_occurType.c_str());
	message.addField("SEVERITY", m_severity);
	message.addField("MSG-TEXT", m_msgText.c_str());
	message.addField("EVENT-TIME", theTime);

	//o Publish Message
	conn->publish(message, getConfig());

	//o Display XML/JSON representation of message
	Utility::displayMessage("Published:\n", message);
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	try
	{
		GMSEC_Logger gmlog(config);

		if (!gmlog.areOptionsValid(argc))
		{
			gmlog.usage("gmlog");
			return 1;
		}

		gmlog.addToConfigFromFile();

		gmlog.run();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		return 1;
	}
}
