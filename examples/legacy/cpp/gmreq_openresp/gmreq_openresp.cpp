/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec_cpp.h>

#include <string>


using gmsec::Connection;
using gmsec::ConnectionFactory;
using gmsec::Config;
using gmsec::Field;
using gmsec::Message;
using gmsec::ReplyCallback;
using gmsec::Status;


class Requester : public ReplyCallback
{
public:
	Requester(Config& config)
		: m_conn(0), m_request(0)
	{
		setup(config);
	}


	virtual ~Requester()
	{
		teardown();
	}


	void issueRequest()
	{
		Status   status;
		Message* reply = 0;

		if (m_request)
		{
			const GMSEC_I32 timeout = 1000;
			const char* xml = 0;

			// Using Request(Message* request, GMSEC_I32 timeout, Message* reply)
			//
			status = m_conn->Request(m_request, timeout, reply);
			example::check("Request failed", status);

			// Display request message.
			(void) m_request->ToXML(xml);
			LOG_INFO << "Issued Request Message:\n" << xml;

			if (!status.IsError() && reply)
			{
				// Display reply message.
				(void) reply->ToXML(xml);
				LOG_INFO << "Received Reply Message:\n" << xml;

				(void) m_conn->DestroyMessage(reply);
				reply = 0;
			}


			// Using Request(Message* request, GMSEC_I32 timeout, ReplyCallback* cb)
			//
			status = m_conn->Request(m_request, timeout, this);
			example::check("Request with callback failed", status);

			// Display request message.
			(void) m_request->ToXML(xml);
			LOG_INFO << "Issued Request Message:\n" << xml;

			// Allow some time for the ReplyCallback to receive the reply.
			example::example_millisleep(timeout);


			// Reply messages are exposed.
			// Using GetNextMsg() to receive replies.
			//
			// Note: Two replies are expected, one for each of the requests performed
			//       above.
			//
			for (int i = 0; i < 2; ++i)
			{
				status = m_conn->GetNextMsg(reply, timeout);
				example::check("GetNextMsg failed", status);

				if (!status.IsError() && reply)
				{
					// Display reply message.
					const char* xml = 0;
					(void) reply->ToXML(xml);
					LOG_INFO << "Received Reply Message from GetNextMsg:\n" << xml;

					(void) m_conn->DestroyMessage(reply);
					reply = 0;
				}
			}
		}
	}


	virtual void CALL_TYPE OnReply(Connection* conn, Message* request, Message* reply)
	{
		if (reply)
		{
			const char* xml = 0;
			(void) reply->ToXML(xml);
			LOG_INFO << "Received Reply Message from within OnReply:\n" << xml;
		}
	}


	virtual void CALL_TYPE OnError(Connection* conn, Message* request, Status* status, const char* text)
	{
		if (!text)
		{
			text = "(void)";
		}

		if (status->IsError())
		{
			LOG_WARNING << "[Requester::OnError] " << status->Get() << " text=" << text;
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

		// Subscribe to (anticipated) response/reply subject.
		status = m_conn->Subscribe("GMSEC.SYSTEST.TEST1.RESP.>");
		example::check("Unable to setup subscription for reply messages", status);

		// Create request directive message.
		status = m_conn->CreateMessage("GMSEC.SYSTEST.TEST1.REQ.DIR.MY-REQUEST", GMSEC_MSG_REQUEST, m_request);
		example::check("Unable to create request message", status);

		// Add fields required to make request message GMSEC-compliant.
		addRequiredFields(m_request);
	}


	void teardown()
	{
		if (m_conn)
		{
			if (m_request)
			{
				// Destroy request directive message.
				(void) m_conn->DestroyMessage(m_request);
				m_request = 0;
			}

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
		field.Set("REQ");
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
		field.Set("gmreq-openresp");
		msg->AddField(field);

		field.SetName("MSG-ID");
		field.Set("GMREQ-MSG");
		msg->AddField(field);

	}

	// Class members
	Connection* m_conn;
	Message*    m_request;
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
	config.AddValue("GMSEC-REQ-RESP", "OPEN-RESP");

	try
	{
		Requester requester(config);
		requester.issueRequest();
	}
	catch (example::gmsec_exception& e)
	{
		LOG_ERROR << e.what();
		return -1;
	}
}
