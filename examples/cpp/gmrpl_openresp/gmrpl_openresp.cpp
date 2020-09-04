/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmrpl_openresp.cpp
 * 
 * Another C++ example demonstrating the reply/request abilities of 
 * GMSEC.  This example demonstrates the use of the Open-Response
 * option for Request/Reply messages which requires client (requester)
 * to subscribe to anticipated reply message subject in order to
 * receive the reply.
 *
 * Note:  The Open-Response feature is only available with ActiveMQ,
 *        Bolt and WebSphere.  The feature was first introduced
 *        with GMSEC API 3.6.1.
 * 
 */

#include "../example.h"

#include <string>


using namespace gmsec::api;


class Replier
{
public:
	Replier(Config& config)
		: m_config(config),
		  m_conn(0)
	{
		// Initialize GMSEC logger.
		example::initialize(m_config);
	}


	~Replier()
	{
		teardown();
	}


	bool run()
	{
		bool success = true;

		// Output GMSEC API version
		GMSEC_INFO << Connection::getAPIVersion();

		try
		{
			setup();

			// Wait 30 seconds for a request message
			int timeout = GMSEC_WAIT_FOREVER;

			for (int i = 0; i < 2; ++i)
			{
				GMSEC_INFO << "Waiting for a request message (from gmreq_openresp)...";
				Message* request = m_conn->receive(timeout);

				if (request)
				{
					// Display request message.
					GMSEC_INFO << "Received Request Message:\n" << request->toXML();

					// Create reply message.
					Message reply("GMSEC.SYSTEST.TEST1.RESP.MY-REPLY", Message::REPLY);

					// Add fields required to make reply message GMSEC-compliant.
					addRequiredFields(reply);

					// Issue reply message.
					m_conn->reply(*request, reply);

					// Display reply message.
					GMSEC_INFO << "Issued Reply Message:\n" << reply.toXML();

					// Destroy the request message.
					m_conn->release(request);
				}
				else
				{
					GMSEC_WARNING << "Failed to receive a request message after a timeout of " << timeout << " msec.";
				}
			}
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << e.what();
			success = false;
		}

		return success;
	}

private:
	void setup()
	{
		// Create connection object.
		m_conn = Connection::create(m_config);

		// Establish a connection.
		m_conn->connect();

		// Output the middleware information
		GMSEC_INFO << "Middleware version = " << m_conn->getLibraryVersion();

		const char* subject = "GMSEC.SYSTEST.TEST1.REQ.DIR.>";

		// Subscribe to receive request directive messages.
		info = m_conn->subscribe(subject);

		GMSEC_INFO << "Subscribing to " << subject;
	}


	void teardown()
	{
		if (m_conn)
		{
			GMSEC_INFO << "Unsubscribing from " << info->getSubject();
			m_conn->unsubscribe(info);
			// Disconnect from middleware server.
			m_conn->disconnect();

			// Destroy connection object.
			Connection::destroy(m_conn);
		}
	}


	void addRequiredFields(Message& msg)
	{
		msg.addField("HEADER-VERSION", (GMSEC_F32) 2014);
		msg.addField("MESSAGE-TYPE", "RESP");
		msg.addField("MESSAGE-SUBTYPE", "DIR");
		msg.addField("MISSION-ID", "Systest");
		msg.addField("FACILITY", "GMSEC Lab");
		msg.addField("COMPONENT", "gmrpl-openresp");
	}

	// Class members
	Config      m_config;
	Connection* m_conn;
	SubscriptionInfo* info;
};


int main(int argc, char** argv)
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc, "gmrpl_openresp"))
	{
		example::printUsage("gmrpl_openresp");
		return -1;
	}

	// Configuration option, introduced with GMSEC API 3.6.1, to enable
	// Open-Response behavior.
	config.addValue("GMSEC-REQ-RESP", "OPEN-RESP");

	return (Replier(config).run() ? 0 : -1);
}
