/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec_cpp.h>

#include <string>


using gmsec::Connection;
using gmsec::ConnectionFactory;
using gmsec::Config;
using gmsec::Field;
using gmsec::Message;
using gmsec::Status;


class Replier
{
public:
	Replier(Config& config)
		: m_conn(0)
	{
		setup(config);
	}


	~Replier()
	{
		teardown();
	}


	void handleRequest()
	{
		Status   status;
		Message* request = 0;
		Message* reply = 0;

		// Wait (indefinitely) for a request message
		status = m_conn->GetNextMsg(request, -1);
		example::check("Unable to receive request using GetNextMsg", status);

		if (request)
		{
			const char* xml = 0;

			// Display request message.
			(void) request->ToXML(xml);
			LOG_INFO << "Received Request Message:\n" << xml;

			// Create reply message.
			status = m_conn->CreateMessage("GMSEC.SYSTEST.TEST1.RESP.MY-REPLY", GMSEC_MSG_REPLY, reply);
			example::check("Unable to create reply message", status);

			// Add fields required to make reply message GMSEC-compliant.
			addRequiredFields(reply);

			// Issue reply message.
			status = m_conn->Reply(request, reply);
			example::check("Unable to issue reply message", status);

			// Display reply message.
			(void) reply->ToXML(xml);
			LOG_INFO << "Issued Reply Message:\n" << xml;

			// Destroy the request message.
			(void) m_conn->DestroyMessage(request);

			// Destroy the reply message.
			(void) m_conn->DestroyMessage(reply);
		}
	}

private:
	void setup(Config& config)
	{
		Status status;

		// Initialize GMSEC logger.
		example::initialize(config);

		// Create connection object.
		status = ConnectionFactory::Create(&config, m_conn);
		example::check("Unable to create connection", status);

		// Establish a connection.
		status = m_conn->Connect();
		example::check("Unable to connect to mw server", status);

		// Subscribe to receive request directive messages.
		status = m_conn->Subscribe("GMSEC.SYSTEST.TEST1.REQ.DIR.>");
		example::check("Unable to setup subscription for request messages", status);
	}


	void teardown()
	{
		if (m_conn)
		{
			// Disconnect from middleware server.
			(void) m_conn->Disconnect();

			// Destroy connection object.
			(void) ConnectionFactory::Destroy(m_conn);

			m_conn = 0;
		}
	}


	void addRequiredFields(Message* msg)
	{
		if (!msg) return;

		Field field;

		field.SetName("HEADER-VERSION");
		field.Set((GMSEC_F32) 2014);
		msg->AddField(field);

		field.SetName("MESSAGE-TYPE");
		field.Set("RESP");
		msg->AddField(field);

		field.SetName("MESSAGE-SUBTYPE");
		field.Set("DIR");
		msg->AddField(field);

		field.SetName("MISSION-ID");
		field.Set("Systest");
		msg->AddField(field);

		field.SetName("FACILITY");
		field.Set("GMSEC Lab");
		msg->AddField(field);

		field.SetName("COMPONENT");
		field.Set("gmrpl-openresp");
		msg->AddField(field);
	}

	// Class members
	Connection* m_conn;
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
	config.AddValue("GMSEC-REQ-RESP", "OPEN-RESP");

	try
	{
		Replier replier(config);

		while (true)
		{
			replier.handleRequest();
		}
	}
	catch (example::gmsec_exception& e)
	{
		LOG_ERROR << e.what();
		return -1;
	}
}
