#include "TestCase.h"

#include <vector>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class T041_Callback : public Callback
{
public:
	T041_Callback(Test& test)
		: m_test(test)
	{
		m_statusList.push_back(Message::ResponseStatus::ACKNOWLEDGEMENT);
		m_statusList.push_back(Message::ResponseStatus::WORKING_KEEP_ALIVE);
		m_statusList.push_back(Message::ResponseStatus::WORKING_KEEP_ALIVE);
		m_statusList.push_back(Message::ResponseStatus::WORKING_KEEP_ALIVE);
		m_statusList.push_back(Message::ResponseStatus::SUCCESSFUL_COMPLETION);
	}

	void onMessage(Connection& conn, const Message& message)
	{
		try
		{
			Message reply = conn.getMessageFactory().createMessage("RESP.DIR");
			reply.setSubject( m_test.getSubject("RESP.DIR").c_str() );

			reply.addField("COMPONENT", (message.hasField("DESTINATION-COMPONENT") ? message.getStringValue("DESTINATION-COMPONENT") : "RESPONDER"));
			reply.addField("DESTINATION-COMPONENT", (message.hasField("COMPONENT") ? message.getStringValue("COMPONENT") : "FILL"));

			if (message.hasField("REQUEST-ID"))
			{
				reply.addField(*message.getField("REQUEST-ID"));
			}

			GMSEC_VERBOSE << "Received Request:\n" << message.toXML();

			for (const auto& status : m_statusList)
			{
				reply.addField("RESPONSE-STATUS", static_cast<GMSEC_I16>(status));

				GMSEC_VERBOSE << "Issuing Reply:\n" << reply.toXML();

				conn.reply(message, reply);
			}
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR << "GmsecException in T041_Callback: " << e.what();
		}
	}

	int expectedResponses() const
	{
		return static_cast<int>(m_statusList.size());
	}

private:
	Test& m_test;
	std::vector<Message::ResponseStatus> m_statusList;
};


class T041_ReplyCallback : public ReplyCallback
{
public:
	T041_ReplyCallback()
	{
		repliesReceived = 0;
	}

	void onReply(Connection& conn, const Message& request, const Message& reply)
	{
		try
		{
			GMSEC_INFO << "Received Reply with RESPONSE-STATUS: " << reply.getI16Value("RESPONSE-STATUS");
			++repliesReceived;
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR << "GmsecException in T041_ReplyCallback: " << e.what();
		}
	}

	void onEvent(Connection& conn, const Status& status, Connection::Event connEvent)
	{
		GMSEC_WARNING << "Received unexpected event: " << status.get();
	}

	int numRepliesReceived() const
	{
		return repliesReceived;
	}

private:
	int repliesReceived;
};


int test_MultiResponse(Test& test)
{
	test.setDescription("Multi-Response");

	Config config( test.getConfig() );  // make a copy
	config.addValue("mw-multi-resp", "true");

	T041_Callback       cb(test);
	T041_ReplyCallback rcb;

	StdUniquePtr<Connection> conn;

	try
	{
		conn.reset( new Connection(config) );

		conn->connect();

		set_standard_fields( conn->getMessageFactory() );

		conn->startAutoDispatch();

		Message request = conn->getMessageFactory().createMessage("REQ.DIR");
		request.setSubject( test.getSubject("REQ.DIR").c_str() );
		request.addField("DIRECTIVE-STRING", "Do something!");
		request.addField("DESTINATION-COMPONENT", "RESPONDER");
		request.addField("REQUEST-ID", static_cast<GMSEC_U16>(0));

		// subscribe to receive the response message (hack!)
		conn->subscribe( test.getSubject("RESP.DIR.+").c_str() );

		// subscribe to process the request message
		conn->subscribe( request.getSubject(), &cb );

		GMSEC_VERBOSE << "Issuing Request:\n" << request.toXML();

		conn->request(request, 5000, &rcb, GMSEC_REQUEST_REPUBLISH_NEVER);

		TimeUtil::millisleep(5000);   // allow for the response message(s) to be received

		conn->disconnect();

		test.check("Unexpected response message count", rcb.numRepliesReceived() == cb.expectedResponses());
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
		return 1;
	}
	return 0;
}

TEST_DRIVER(test_MultiResponse)
