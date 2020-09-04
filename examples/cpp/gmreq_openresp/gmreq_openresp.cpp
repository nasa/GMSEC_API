/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmreq_openresp.cpp
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


class Requester : public ReplyCallback
{
public:
	Requester(Config& config)
		: m_config(config),
		  m_conn(0),
		  m_request(0)
	{
		// Initialize GMSEC logger.
		example::initialize(m_config);
	}


	virtual ~Requester()
	{
	}


	bool run()
	{
		bool success = true;

		// Output the GMSEC API version
		GMSEC_INFO << Connection::getAPIVersion();

		try
		{
			setup();

			const GMSEC_I32 timeout = 5000;

			// Display request message.
			GMSEC_INFO << "Issuing Request Message:\n" << m_request->toXML();

			// Using Request(Message* request, GMSEC_I32 timeout, Message* reply)
			//
			Message* reply = m_conn->request(*m_request, timeout);

			if (reply)
			{
				// Display reply message.
				GMSEC_INFO << "Received Reply Message:\n" << reply->toXML();

				m_conn->release(reply);
			}
			else
			{
				GMSEC_WARNING << "Failed to receive reply message after a timeout of " << timeout << " msec.";
			}


			// Using Request(Message* request, GMSEC_I32 timeout, ReplyCallback* cb)
			//
			m_conn->request(*m_request, timeout, this);

			// Display request message.
			GMSEC_INFO << "Issued Request Message:\n" << m_request->toXML();

			// Allow some time for the ReplyCallback to receive the reply.
			example::millisleep(timeout);

			// Reply messages are exposed.
			// Using receive() to receive replies.
			//
			// Note: Two replies are expected, one for each of the requests performed
			//       above.
			//
			for (int i = 0; i < 2; ++i)
			{
				reply = m_conn->receive(timeout);

				if (reply)
				{
					// Display reply message.
					GMSEC_INFO << "Received exposed Reply Message using receive():\n" << reply->toXML();

					m_conn->release(reply);
				}
				else
				{
					GMSEC_WARNING << "Failed to receive exposed reply message after a timeout of " << timeout << " msec.";
				}
			}

			teardown();
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << e.what();
			success = false;
		}

		return success;
	}


	virtual void CALL_TYPE onReply(Connection& conn, const Message& request, const Message& reply)
	{
		GMSEC_INFO << "[Requester::onReply] Received Reply:\n" << reply.toXML();
	}


	virtual void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
	{
		if (status.isError())
		{
			GMSEC_WARNING << "[Requester::onEvent] " << status.get() << ", event=" << event;
		}
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

		const char* subject = "GMSEC.SYSTEST.TEST1.RESP.>";

		// Subscribe to (anticipated) response/reply subject.
		info = m_conn->subscribe(subject);

		GMSEC_INFO << "Subscribing to " << subject;

		// Create request directive message.
		m_request = new Message("GMSEC.SYSTEST.TEST1.REQ.DIR.MY-REQUEST", Message::REQUEST);

		// Add fields required to make request message GMSEC-compliant.
		addRequiredFields(*m_request);
	}


	void teardown()
	{
		delete m_request;

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
		msg.addField("MESSAGE-TYPE", "REQ");
		msg.addField("MESSAGE-SUBTYPE", "DIR");
		msg.addField("MISSION-ID", "Systest");
		msg.addField("FACILITY", "GMSEC Lab");
		msg.addField("COMPONENT", "gmreq-openresp");
	}

	// Class members
	Config      m_config;
	Connection* m_conn;
	Message*    m_request;
	SubscriptionInfo* info;
};


int main(int argc, char** argv)
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc, "gmreq_openresp"))
	{
		example::printUsage("gmreq_openresp");
		return -1;
	}

	// Configuration option, introduced with GMSEC API 3.6.1, to enable
	// Open-Response behavior.
	config.addValue("GMSEC-REQ-RESP", "OPEN-RESP");

	return (Requester(config).run() ? 0 : -1);
}
