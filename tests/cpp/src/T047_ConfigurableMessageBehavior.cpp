#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class MyReplyCallback : public ReplyCallback
{
public:
	MyReplyCallback() : m_gotReply(false), m_gotTimeout(false) {}

	void onReply(Connection& conn, const Message& request, const Message& reply) 
	{
		GMSEC_INFO << "Received:\n" << reply.toXML();
		m_gotReply = true; 
	}

	void onEvent(Connection& conn, const Status& status, gmsec::api5::Connection::Event event)
	{
		m_gotTimeout = (event == Connection::Event::REQUEST_TIMEOUT_EVENT);
	}

	bool gotReply() const { return m_gotReply; }

	bool gotTimeout() const { return m_gotTimeout; }

	void reset() { m_gotReply = false; m_gotTimeout = false; }

private:
	bool m_gotReply;
	bool m_gotTimeout;
};


void test_showMsgConfig(Test& test)
{
	GMSEC_INFO << "Test show message config";

	Message msg;

	msg.setConfig(Config("GMSEC-SHOW-MSG-CONFIG=TRUE", DataType::KEY_VALUE_DATA));

	std::string xml = msg.toXML();

	test.check("Config missing from Message XML", xml.find("<PARAMETER NAME=\"GMSEC-SHOW-MSG-CONFIG\">TRUE</PARAMETER>") != std::string::npos);
}


void test_configReplySubject(Test& test)
{
	GMSEC_INFO << "Test auto-subscribe via message config";

	MyReplyCallback mrb;

	GMSEC_I32 timeout = 10000;

	Connection conn(test.getConfig());

	StringField field1("MISSION-ID", "MY-MISSION", true);
	StringField field2("SAT-ID-PHYSICAL", "MY-SAT-ID", true);
	StringField field3("SAT-ID-LOGICAL", "MY-SAT-ID", true);
	StringField field4("CONSTELLATION-ID", "MY-CNST", true);
	StringField field5("FACILITY", "MY-FACILITY", true);
	StringField field6("DOMAIN1", "MY-DOMAIN-1", true);
	StringField field7("DOMAIN2", "MY-DOMAIN-2", true);
	StringField field8("COMPONENT", "REQUESTOR", true);

	List<Field*> standardFields;
	standardFields.push_back(&field1);
	standardFields.push_back(&field2);
	standardFields.push_back(&field3);
	standardFields.push_back(&field4);
	standardFields.push_back(&field5);
	standardFields.push_back(&field6);
	standardFields.push_back(&field7);
	standardFields.push_back(&field8);

	conn.getMessageFactory().setStandardFields(standardFields);

	conn.connect();
	Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
	reqMsg.setFieldValue("DESTINATION-COMPONENT", "RESPONDER");
	reqMsg.setFieldValue("DIRECTIVE-STRING", "this is a TEST");
	reqMsg.setFieldValue("DIRECTIVE-KEYWORD", "TEST");
	reqMsg.setFieldValue("RESPONSE", "true");
	reqMsg.setFieldValue("REQUEST-ID", "0");

	reqMsg.setConfig(Config("GMSEC-SHOW-MSG-CONFIG=TRUE GMSEC-REPLY-SUBJECT=C2MS.MY-DOMAIN-1.MY-DOMAIN-2.MY-MISSION.MY-CNST.MY-SAT-ID.RESP.SERV.REQUESTOR.3", DataType::KEY_VALUE_DATA));

	std::string xml = reqMsg.toXML();

	test.check("Config missing from Message XML", xml.find("<PARAMETER NAME=\"GMSEC-REPLY-SUBJECT\">C2MS.MY-DOMAIN-1.MY-DOMAIN-2.MY-MISSION.MY-CNST.MY-SAT-ID.RESP.SERV.REQUESTOR.3</PARAMETER>") != std::string::npos);

	conn.subscribe(reqMsg.getSubject());
	
	GMSEC_INFO << "subscribed to \"" << reqMsg.getSubject() << "\"";

	GMSEC_INFO << "Request:\n" << xml.c_str();
	conn.request(reqMsg, timeout*2, &mrb);

	Message* rcvMsg = conn.receive(timeout);

	test.check("Failed to receive request message", rcvMsg);

	if (rcvMsg && rcvMsg->hasField("RESPONSE") && rcvMsg->getBooleanValue("RESPONSE"))
	{
		GMSEC_INFO << "Received:\n" << rcvMsg->toXML();
		Message rspMsg = conn.getMessageFactory().createMessage("RESP.SERV");
		if (rcvMsg->hasField("COMPONENT"))
		{
			rspMsg.addField(StringField("DESTINATION-COMPONENT", rcvMsg->getStringValue("COMPONENT"), true));
		}
		if (rcvMsg->hasField("REQUEST-ID"))
		{
			rspMsg.addField(*rcvMsg->getField("REQUEST-ID"));
		}

		// Set the response status for the request.
		rspMsg.setFieldValue("RESPONSE-STATUS", static_cast<GMSEC_I32>(Message::ResponseStatus::SUCCESSFUL_COMPLETION));

		// Send the Response Directive message.
		GMSEC_INFO << "Reply:\n" << rspMsg.toXML();
		conn.reply(*rcvMsg, rspMsg);
	}

	Message::destroy(rcvMsg);

	TimeUtil::millisleep(5500);

	test.check("Expected MyReplyCallback to be summoned", mrb.gotReply());
	test.check("Unexpected event in MyReplyCallback", !mrb.gotTimeout());

	conn.disconnect();
}


	int test_ConfigurableMessageBehaior(Test & test)
{
	test.setDescription("Test Message");

	try
	{
		test_showMsgConfig(test);
		test_configReplySubject(test);
	}
	catch (GmsecException& e)
	{
		test.require(e.what(), false);
	}

	return 0;
}

TEST_DRIVER(test_ConfigurableMessageBehaior)
