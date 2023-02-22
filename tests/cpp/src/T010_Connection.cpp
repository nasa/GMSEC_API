#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class MyEventCallback : public EventCallback
{
public:
	MyEventCallback() : m_called(false), m_event(Connection::Event::ALL_EVENTS) {}

	void onEvent(Connection& conn, const Status& status, Connection::Event event)
	{
		m_called = true;
		m_event  = event;
	}

	bool wasCalled() const { return m_called; }

	Connection::Event getEvent() const { return m_event; }

	void reset()
	{
		m_called = false;
		m_event  = Connection::Event::ALL_EVENTS;
	}

private:
	bool m_called;
	Connection::Event m_event;
};


class MyCallback : public Callback
{
public:
	MyCallback() : m_called(false) {}

	void onMessage(Connection& conn, const Message& msg) { m_called = true; }

	bool wasCalled() const { return m_called; }

	void reset() { m_called = false; }

private:
	bool m_called;
};


class MyRequestCallback : public Callback
{
public:
	MyRequestCallback(Test& test) : m_test(test), m_provideResponse(true) {}

	void onMessage(Connection& conn, const Message& msg)
	{
		try
		{
			if (msg.getKind() != Message::Kind::REQUEST)
			{
				m_test.check("Received unexpected message kind", false);
				return;
			}

			if (m_provideResponse)
			{
				Message repMsg = conn.getMessageFactory().createMessage("RESP.DIR");
				repMsg.setSubject( m_test.getSubject("RESP.DIR.MY-COMPONENT").c_str() );

				repMsg.addField("RESPONSE-STATUS", static_cast<GMSEC_I16>(Message::ResponseStatus::SUCCESSFUL_COMPLETION));

				if (msg.hasField("COMPONENT"))
				{
					repMsg.addField("DESTINATION-COMPONENT", msg.getStringValue("COMPONENT"));
				}
				if (msg.hasField("DESTINATION-COMPONENT"))
				{
					repMsg.addField("COMPONENT", msg.getStringValue("DESTINATION-COMPONENT"));
				}
				else
				{
					repMsg.addField("COMPONENT", "RESPONDER");
				}
				if (msg.hasField("REQUEST-ID"))
				{
					repMsg.addField(*msg.getField("REQUEST-ID"));
				}

				conn.reply(msg, repMsg);
			}
			else
			{
				m_provideResponse = true;
			}
		}
		catch (const GmsecException& e)
		{
			m_test.check(e.what(), false);
		}
	}

	void provideResponse(bool flag) { m_provideResponse = flag; }

private:
	Test& m_test;
	bool  m_provideResponse;
};


class MyReplyCallback : public ReplyCallback
{
public:
	MyReplyCallback() : m_gotReply(false), m_gotTimeout(false) {}

	void onReply(Connection& conn, const Message& request, const Message& reply) { m_gotReply = true; }

	void onEvent(Connection& conn, const Status& status, gmsec::api5::Connection::Event event)
	{ m_gotTimeout = (event == Connection::Event::REQUEST_TIMEOUT_EVENT); }

	bool gotReply() const { return m_gotReply; }

	bool gotTimeout() const { return m_gotTimeout; }

	void reset() { m_gotReply = false; m_gotTimeout = false; }

private:
	bool m_gotReply;
	bool m_gotTimeout;
};


void test_constructor(Test& test)
{
	GMSEC_INFO << "Test constructor";

	// Nominal test
	try
	{
		Connection conn(test.getConfig());
		test.check("Okay", true);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Nominal test w/ MessageFactory
	try
	{
		MessageFactory factory(test.getConfig());

		Connection conn(test.getConfig(), factory);
		test.check("Okay", true);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-Nominal test
	try
	{   // missing mw-id
		Config config;
		Connection conn(config);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("mw-id in Config is not specified") != std::string::npos);
	}

	try
	{   //bogus middleware
		Config config("mw-id=bogus", DataType::KEY_VALUE_DATA);
		Connection conn(config);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Unable to load") != std::string::npos);
	}
}


void test_get_api_version(Test& test)
{
	GMSEC_INFO << "Test getAPIVersion()";

	test.check("Expected to get an API version string", NULL != Connection::getAPIVersion());
	test.check("Unexpected version number", strcontains(Connection::getAPIVersion(), GMSEC_VERSION_NUMBER));
}


void test_connect(Test& test)
{
	GMSEC_INFO << "Test connect()";

	// Nominal test
	try
	{
		Connection conn(test.getConfig());
		conn.connect();
		test.check("Okay", true);
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}

	// Off-nominal test
	try
	{
		Config config("mw-id=bolt server=gs580s-gc764mw1:1234", DataType::KEY_VALUE_DATA);
		Connection conn(config);
		conn.connect();
		test.check("Expected error to indicate unable to connect", false);
	}
	catch (const GmsecException& e)
	{
		bool result = (e.getErrorClass() == StatusClass::MIDDLEWARE_ERROR) &&
		              (e.getErrorCode()  == StatusCode::CUSTOM_ERROR_CODE) &&
		              (e.getCustomCode() == 7);

		test.check(e.what(), result);
	}
}


void test_disconnect(Test& test)
{
	GMSEC_INFO << "Test disconnect()";

	try
	{
		Connection conn(test.getConfig());
		conn.connect();
		conn.disconnect();
		test.check("Okay", true);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_library_name(Test& test)
{
	GMSEC_INFO << "Test getLibraryName()";

	try
	{
		Connection conn(test.getConfig());

		std::string mw  = test.getConfig().getValue("mw-id", "unknown");
		std::string lib = std::string("gmsec_") + mw;

		test.check("Unexpected library name", std::string(conn.getLibraryName()).find(lib) != std::string::npos);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_library_version(Test& test)
{
	GMSEC_INFO << "Test getLibraryVersion()";

	try
	{
		Connection conn(test.getConfig());
		test.check("Expected a library version string", NULL != conn.getLibraryVersion());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_message_factory(Test& test)
{
	GMSEC_INFO << "Test getMessageFactory()";

	try
	{
		Connection conn(test.getConfig());
		test.check("Unexpected specification version", GMSEC_MSG_SPEC_CURRENT == conn.getMessageFactory().getSpecification().getVersion());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_register_event_callback(Test& test)
{
	GMSEC_INFO << "Test registerEventCallback()";

	std::string mw = test.getConfig().getValue("mw-id", "unknown");

	if (mw == "unknown" || mw == "mb" || mw == "generic_jms" || mw == "kafka" || mw == "zeromq413" || mw == "zeromq432")
	{
		// Test is being skipped because of lame middleware wrapper
		// which does not support event notification upon connect
		// and/or disconnect to/from the GMSEC Bus.
		return;
	}

	MyEventCallback mec;

	// Nominal case
	try
	{
		Connection conn(test.getConfig());
		conn.registerEventCallback(Connection::Event::ALL_EVENTS, &mec);

		conn.connect();
		test.check("Expected event callback to be summoned", true == mec.wasCalled());
		test.check("Event callback received wrong event", Connection::Event::CONNECTION_SUCCESSFUL_EVENT == mec.getEvent());

		mec.reset();

		conn.disconnect();
		test.check("Expected event callback to be summoned", true == mec.wasCalled());
		test.check("Event callback received wrong event", Connection::Event::CONNECTION_BROKEN_EVENT == mec.getEvent());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal case
	try
	{
		Connection conn(test.getConfig());
		conn.registerEventCallback(Connection::Event::ALL_EVENTS, (EventCallback*) NULL);
		test.check("Expected exception to be raised", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("EventCallback cannot be NULL") != std::string::npos);
	}
}


void test_subscribe(Test& test)
{
	GMSEC_INFO << "Test subscribe() w/ topic";

	// Nominal test
	try
	{
		std::string topic1   = test.getSubject("*.BAR");
		std::string topic2   = test.getSubject("FOO.BAZ");
		std::string subject1 = test.getSubject("GIN.BAR");
		std::string subject2 = test.getSubject("FOO.BAZ");
		std::string subject3 = test.getSubject("FOO.GAZ");

		Connection conn(test.getConfig());
		conn.connect();

		(void) conn.subscribe( topic1.c_str() );
		(void) conn.subscribe( topic2.c_str() );

		Message msg1 = conn.getMessageFactory().createMessage("LOG");
		Message msg2 = conn.getMessageFactory().createMessage("LOG");
		Message msg3 = conn.getMessageFactory().createMessage("LOG");

		msg1.setSubject( subject1.c_str() );
		msg2.setSubject( subject2.c_str() );
		msg3.setSubject( subject3.c_str() );

		conn.publish(msg1);

		Message* msg = conn.receive(10000);
		test.require("1. Failed to receive message", NULL != msg);
		test.check("1. Unexpected message subject", subject1 == msg->getSubject());
		Message::destroy(msg);

		conn.publish(msg2);

		msg = conn.receive(10000);
		test.require("2. Failed to receive message", NULL != msg);
		test.check("2. Unexpected message subject", subject2 == msg->getSubject());
		Message::destroy(msg);

		conn.publish(msg3);

		msg = conn.receive(5000);
		test.check("Was not expecting a message", NULL == msg);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{   // duplicate subscription
		Connection conn(test.getConfig());
		conn.connect();
		(void) conn.subscribe("GMSEC.>");
		(void) conn.subscribe("GMSEC.>");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Duplicate subscription") != std::string::npos);
	}

	try
	{   // subscribe() before connect()
		Connection conn(test.getConfig());
		(void) conn.subscribe("GMSEC.>");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}

	try
	{   // NULL subscription subject
		Connection conn(test.getConfig());
		conn.connect();
		(void) conn.subscribe(NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Subject is NULL or is empty string") != std::string::npos);
	}

	try
	{   // illegal subscription subject
		Connection conn(test.getConfig());
		conn.connect();
		(void) conn.subscribe("GMSEC.MY MISSION.*.*.>");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Subject is invalid") != std::string::npos);
	}
}


void test_subscribe_with_callback(Test& test)
{
	GMSEC_INFO << "Test subscribe() w/ callback";

	MyCallback      mcb;
	MyEventCallback ecb;

	std::string topic1   = test.getSubject("*.BAR");
	std::string subject1 = test.getSubject("GIN.BAR");

	StdUniquePtr<Connection> conn;

	// Nominal test (verify Callback is summoned)
	try
	{
		conn.reset(new Connection(test.getConfig()));

		conn->connect();

		conn->registerEventCallback(Connection::Event::ALL_EVENTS, &ecb);

		conn->subscribe(topic1.c_str(), &mcb);

		conn->startAutoDispatch();

		set_standard_fields(conn->getMessageFactory());

		Message msg = conn->getMessageFactory().createMessage("HB");
		msg.setSubject( subject1.c_str() );

		conn->publish(msg);

		TimeUtil::millisleep(5000);

		test.check("MyCallback was not summoned", mcb.wasCalled());
		test.check("MyEventCallback received incorrect event", Connection::Event::ALL_EVENTS == ecb.getEvent());

		mcb.reset();
		ecb.reset();

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Nominal test (verify Callback is not summoned because of illegal message)
	try
	{
		Config config = test.getConfig();   // make a copy
		config.addValue("gmsec-msg-content-validate-recv", "true");

		conn.reset(new Connection(config));

		conn->connect();

		conn->registerEventCallback(Connection::Event::ALL_EVENTS, &ecb);

		conn->subscribe(topic1.c_str(), &mcb);

		conn->startAutoDispatch();

		set_standard_fields(conn->getMessageFactory());

		Message msg = conn->getMessageFactory().createMessage("HB");
		msg.setSubject( subject1.c_str() );

		conn->publish(msg);

		TimeUtil::millisleep(5000);

		test.check("MyCallback unexpectedly summoned", false == mcb.wasCalled());
		test.check("MyEventCallback received incorrect event", Connection::Event::INVALID_MESSAGE_EVENT == ecb.getEvent());

		mcb.reset();
		ecb.reset();

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	try
	{   // subscriptions using same subject and unique callbacks
		MyCallback mcb2;
		Connection conn(test.getConfig());
		conn.connect();
		(void) conn.subscribe("NIGHTRUN.>", &mcb);
		(void) conn.subscribe("NIGHTRUN.>", &mcb2);
		test.check("Okay; unique subscriptions", true);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{   // duplicate subscription using the same callback
		Connection conn(test.getConfig());
		conn.connect();
		(void) conn.subscribe("NIGHTRUN.>", &mcb);
		(void) conn.subscribe("NIGHTRUN.>", &mcb);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Duplicate subscription") != std::string::npos);
	}
}


void test_unsubscribe(Test& test)
{
	GMSEC_INFO << "Test unsubscribe()";

	MyCallback mcb;

	// Nominal tests
	try
	{
		Connection conn(test.getConfig());
		conn.connect();
		SubscriptionInfo* info = conn.subscribe( test.getSubject("FOO.BAR").c_str() );
		conn.unsubscribe(info);

		Message hb = conn.getMessageFactory().createMessage("HB");
		hb.setSubject( test.getSubject("FOO.BAR").c_str() );

		conn.publish(hb);

		Message* msg = conn.receive(5000);

		test.check("Received unexpected message", NULL == msg);
		test.check("SubscriptionInfo object not nullified", NULL == info);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{   // Attempt to unsubscribe using NULL object
		Connection conn(test.getConfig());
		conn.connect();
		SubscriptionInfo* info = NULL;
		conn.unsubscribe(info);
		test.check("Expected an exception to be raised", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Cannot unsubscribe using NULL SubscriptionInfo object") != std::string::npos);
	}

	try
	{   // Attempt to unsubscribe using wrong Connection object
		Connection conn1(test.getConfig());
		Connection conn2(test.getConfig());

		conn1.connect();
		conn2.connect();

		SubscriptionInfo* info1 = conn1.subscribe("GMSEC.FOO.BAR");

		conn2.unsubscribe(info1);

		test.check("Expected an exception to be raised", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("SubscriptionInfo object is not associated with this Connection") != std::string::npos);
	}
}


void test_start_auto_dispatch(Test& test)
{
	GMSEC_INFO << "Test startAutoDispatch()";

	// startAutoDispatch already tested within test_subscribe_callback()

	// Off-nominal test
	try
	{   // Auto-dispatch cannot start until Connection is initialized
		Connection conn(test.getConfig());
		conn.startAutoDispatch();
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}
}


void test_stop_auto_dispatch(Test& test)
{
	GMSEC_INFO << "Test stopAutoDispatch()";

	// stopAutoDispatcher already tested within test_subscribe_callback()

	// Off-nominal test
	try
	{   // Auto-dispatch cannot be stopped until Connection is initialized
		Connection conn(test.getConfig());
		conn.stopAutoDispatch();
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}
}


void test_publish(Test& test)
{
	GMSEC_INFO << "Test publish()";

	// Nominal test
	try
	{
		std::string topic1   = test.getSubject("*.BAR");
		std::string subject1 = test.getSubject("FOO.BAR");

		Connection conn(test.getConfig());

		conn.connect();
		conn.subscribe( topic1.c_str() );

		set_standard_fields(conn.getMessageFactory());

		Message msg = conn.getMessageFactory().createMessage("HB");
		msg.setSubject( subject1.c_str() );

		conn.publish(msg);

		Message* rcvd = conn.receive(5000);

		if (rcvd)
		{
			test.check("Message has no subject", NULL != rcvd->getSubject());
			test.check("Unexpected message kind", Message::Kind::PUBLISH == rcvd->getKind());

			for (size_t i = 0; i < sizeof(testStandardFields) / sizeof(FieldInfo); ++i)
			{
				test.check("Message standard field has incorrect value", std::string(testStandardFields[i].value) == rcvd->getStringValue(testStandardFields[i].name));
			}

			Message::destroy(rcvd);
		}
		else
		{
			test.check("Failed to receive published message", false);
		}

		conn.disconnect();
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	Config config = test.getConfig();
	config.addValue("gmsec-msg-content-validate", "true");

	// Off-nominal tests
	try
	{   // Message validation fails
		Connection conn(config);
		Message msg = conn.getMessageFactory().createMessage("HB");
		conn.connect();
		conn.publish(msg);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message Validation Failed") != std::string::npos);
	}

	try
	{   // Connection not initialized
		Connection conn(config);
		Message msg = conn.getMessageFactory().createMessage("HB");
		conn.publish(msg);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}

	try
	{   // Non-PUBLISH kind message
		Connection conn(config);
		Message msg = conn.getMessageFactory().createMessage("REQ.DIR");
		conn.connect();
		conn.publish(msg);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Cannot publish message with non-PUBLISH message kind") != std::string::npos);
	}
}


void test_publish_with_mwconfig(Test& test)
{
	GMSEC_INFO << "Test publish() with mwConfig";

	Config&     config  = test.getConfig();
	std::string subject = test.getSubject("MWCONFIG");

	StdUniquePtr<Connection> conn;

	//o Just test the interface; a functional test is performed by T105_ActiveMQ_DurableMsgs.cpp
	try
	{
		conn.reset( new Connection(config) );

		set_standard_fields( conn->getMessageFactory() );

		Config  mwConfig;
		Message msg = conn->getMessageFactory().createMessage( "HB" );

		msg.setSubject( subject.c_str() );

		conn->connect();

		conn->subscribe( subject.c_str() );

		conn->publish( msg, mwConfig );

		Message* rcv = conn->receive( 5000 );

		if (rcv)
		{
			test.check("Received a message", true);

			Message::destroy(rcv);
		}
		else
		{
			test.check("Failed to receive a message", false);
		}

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_request_with_callback(Test& test)
{
	GMSEC_INFO << "Test request() with callback";

	Config& config = test.getConfig();

	MyRequestCallback reqCallback(test);
	MyReplyCallback   repCallback;

	// Nominal case
	try
	{
		Connection conn(config);

		conn.connect();

		set_standard_fields(conn.getMessageFactory());

		Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
		reqMsg.setSubject( test.getSubject("REQ.DIR.MY-COMPONENT").c_str() );
		reqMsg.addField("DIRECTIVE-STRING", "Do something!");
		reqMsg.addField("DESTINATION-COMPONENT", "RESPONDER");

		// subscribe to receive the response message
		conn.subscribe( test.getSubject("RESP.DIR.MY-COMPONENT.+").c_str() );

		// subscribe to process the request message
		conn.subscribe(reqMsg.getSubject(), &reqCallback);
		conn.startAutoDispatch();

		//o Verify response is returned
		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(0));
		conn.request(reqMsg, 5000, &repCallback, GMSEC_REQUEST_REPUBLISH_NEVER);
		TimeUtil::millisleep(5500);
		test.check("Expected MyReplyCallback to be summoned", repCallback.gotReply());
		test.check("Unexpected event in MyReplyCallback", false == repCallback.gotTimeout());
		repCallback.reset();

		//o Verify NULL is returned on timeout
		reqCallback.provideResponse(false);

		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(1));
		conn.request(reqMsg, 5000, &repCallback, GMSEC_REQUEST_REPUBLISH_NEVER);
		TimeUtil::millisleep(5500);
		test.check("Expected MyReplyCallback not to be summoned", false == repCallback.gotReply());
		test.check("Expected event in MyReplyCallback", repCallback.gotTimeout());

		//o Verify request is delivered multiple times until response delivered
		reqCallback.provideResponse(false);

		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(2));
		conn.request(reqMsg, 5000, &repCallback, 5000);
		TimeUtil::millisleep(5500*2);
		test.check("Expected MyReplyCallback to be summoned", repCallback.gotReply());
		test.check("Expected event in MyReplyCallback", repCallback.gotTimeout());

		//o Verify request is delivered multiple times until response delivered
		reqCallback.provideResponse(false);

		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(3));
		conn.request(reqMsg, -1, &repCallback, 5000);
		TimeUtil::millisleep(5500*2);
		test.check("Expected MyReplyCallback to be summoned", repCallback.gotReply());
		test.check("Expected event in MyReplyCallback", repCallback.gotTimeout());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{   // Connection not initialized
		Connection conn(config);
		Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
		conn.request(reqMsg, 5000, &repCallback, GMSEC_REQUEST_REPUBLISH_NEVER);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}

	try
	{   // Request using non-REQUEST kind message
		Connection conn(config);
		conn.connect();
		Message badMsg = conn.getMessageFactory().createMessage("RESP.DIR");
		conn.request(badMsg, 5000, &repCallback, GMSEC_REQUEST_REPUBLISH_NEVER);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Cannot issue request with non-REQUEST kind message") != std::string::npos);
	}
}


void test_request(Test& test)
{
	GMSEC_INFO << "Test request()";

	Config& config = test.getConfig();

	MyRequestCallback reqCallback(test);

	// Nominal case
	try
	{
		Connection conn(config);

		conn.connect();

		set_standard_fields(conn.getMessageFactory());

		Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
		reqMsg.setSubject( test.getSubject("REQ.DIR.MY-COMPONENT").c_str() );
		reqMsg.addField("DIRECTIVE-STRING", "Do something!");
		reqMsg.addField("DESTINATION-COMPONENT", "RESPONDER");

		// subscribe to receive the response message
		conn.subscribe( test.getSubject("RESP.DIR.MY-COMPONENT.+").c_str() );

		// subscribe to process the request message
		conn.subscribe(reqMsg.getSubject(), &reqCallback);
		conn.startAutoDispatch();

		//o Verify response is returned
		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(0));
		Message* repMsg = conn.request(reqMsg, 5000, GMSEC_REQUEST_REPUBLISH_NEVER);
		test.require("Reply message not received", NULL != repMsg);
		test.check("Unexpected message kind", Message::Kind::REPLY == repMsg->getKind());
		Message::destroy(repMsg);

		//o Verify NULL is returned on timeout
		reqCallback.provideResponse(false);

		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(1));
		repMsg = conn.request(reqMsg, 5000, GMSEC_REQUEST_REPUBLISH_NEVER);
		test.check("Received unexpected reply message", NULL == repMsg);

		//o Verify request is delivered multiple times until response delivered
		reqCallback.provideResponse(false);

		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(2));
		repMsg = conn.request(reqMsg, 5000, 1000);
		test.require("Reply message not received", NULL != repMsg);
		test.check("Unexpected message kind", Message::Kind::REPLY == repMsg->getKind());
		Message::destroy(repMsg);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{   // Connection not initialized
		Connection conn(config);
		Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
		(void) conn.request(reqMsg, 5000, GMSEC_REQUEST_REPUBLISH_NEVER);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}

	try
	{   // Request using non-REQUEST kind message
		Connection conn(config);
		conn.connect();
		Message badMsg = conn.getMessageFactory().createMessage("RESP.DIR");
		(void) conn.request(badMsg, 5000, GMSEC_REQUEST_REPUBLISH_NEVER);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Cannot issue request with non-REQUEST kind message") != std::string::npos);
	}
}


void test_reply(Test& test)
{
	GMSEC_INFO << "Test reply()";

	// Nominal tests for Connection::reply() tested earlier

	Config& config = test.getConfig();

	// Off-nominal tests
	try
	{   // Connection not initialized
		Connection conn(config);
		Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
		Message repMsg = conn.getMessageFactory().createMessage("RESP.DIR");
		conn.reply(reqMsg, repMsg);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}

	try
	{   // Bad reply message
		Connection conn(config);
		conn.connect();
		Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
		Message badMsg = conn.getMessageFactory().createMessage("REQ.DIR");
		conn.reply(reqMsg, badMsg);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Cannot issue reply with non-REPLY kind message") != std::string::npos);
	}
}


void test_dispatch(Test& test)
{
	GMSEC_INFO << "Test dispatch()";

	MyCallback mcb1;
	MyCallback mcb2;
	MyCallback mcb3;

	try
	{
		Connection conn(test.getConfig());
		conn.connect();

		set_standard_fields(conn.getMessageFactory());

		Message msg = conn.getMessageFactory().createMessage("HB");
		msg.setSubject( test.getSubject().c_str() );

		conn.subscribe( msg.getSubject(), &mcb1 );
		conn.subscribe( msg.getSubject(), &mcb2 );
		conn.subscribe( test.getSubject("FOO.BAR").c_str(),  &mcb3 );

		conn.publish(msg);

		Message* rcvd = conn.receive(5000);

		test.require("Failed to receive a message", NULL != rcvd);

		conn.dispatch(*rcvd);

		TimeUtil::millisleep(1000);

		test.check("Callback 1 was not called", mcb1.wasCalled());
		test.check("Callback 2 was not called", mcb2.wasCalled());
		test.check("Callback 3 was called", false == mcb3.wasCalled());

		Message::destroy(rcvd);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_receive(Test& test)
{
	GMSEC_INFO << "Test receive()";

	// Nominal tests
	try
	{
		Connection conn(test.getConfig());

		set_standard_fields(conn.getMessageFactory());

		conn.connect();

		//o Timeout occurs (no subscription)
		Message* rcvd = conn.receive(10);
		test.check("Received unexpected message", NULL == rcvd);

		//o Receive message
		Message msg = conn.getMessageFactory().createMessage("HB");
		msg.setSubject( test.getSubject().c_str() );

		conn.subscribe( msg.getSubject() );
		conn.publish(msg);

		rcvd = conn.receive(5000);
		test.check("Failed to receive message", NULL != rcvd);
		Message::destroy(rcvd);
		test.check("Failed to receive message", NULL == rcvd);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{   // Connection not initialized
		Connection conn(test.getConfig());
		(void) conn.receive(10);
		test.check("Expected exception to be raised", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}

	try
	{   // Auto-dispatcher enabled
		Connection conn(test.getConfig());
		conn.connect();
		conn.startAutoDispatch();
		(void) conn.receive(10);
		test.check("Expected exception to be raised", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("This method cannot be called when the Auto-Dispatcher is in use") != std::string::npos);
	}
}


void test_exclude_subject(Test& test)
{
	GMSEC_INFO << "Test excludeSubject()";

	try
	{
		Connection conn(test.getConfig());
		conn.connect();

		set_standard_fields(conn.getMessageFactory());

		std::string subject1 = test.getSubject("HB");
		std::string subject2 = test.getSubject("LOG");
		std::string topic1   = test.getSubject(">");

		Message msg1 = conn.getMessageFactory().createMessage("HB");
		Message msg2 = conn.getMessageFactory().createMessage("LOG");

		msg1.setSubject(subject1.c_str());
		msg2.setSubject(subject2.c_str());

		msg2.addField("SUBCLASS", "NO-CLASS");
		msg2.addField("OCCURRENCE-TYPE", "FREQUENTLY");
		msg2.addField("SEVERITY", static_cast<short>(1));

		conn.subscribe(topic1.c_str());
		conn.excludeSubject(subject2.c_str());

		conn.publish(msg1);
		Message* rcvd = conn.receive(5000);
		test.check("Failed to receive a message", NULL != rcvd);
		Message::destroy(rcvd);

		conn.publish(msg2);
		rcvd = conn.receive(5000);
		test.check("Received unexpected message", NULL == rcvd);

		conn.removeExcludedSubject(subject2.c_str());

		conn.publish(msg2);
		rcvd = conn.receive(5000);
		test.check("Failed to receive a message", NULL != rcvd);
		Message::destroy(rcvd);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{   // Connection not initialized
		Connection conn(test.getConfig());
		conn.excludeSubject("GMSEC.FOO.BAR");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}

	try
	{   // Invalid subject
		Connection conn(test.getConfig());
		conn.connect();
		conn.excludeSubject("GMSEC FOO BAR");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Subject is invalid") != std::string::npos);
	}
}


void test_remove_excluded_subject(Test& test)
{
	GMSEC_INFO << "Test removeExcludedSubject()";

	// Nominal test for removeExcludedSubject() are conducted within test_exclude_subject()

	// Off-nominal tests
	try
	{   // Connection not initialized
		Connection conn(test.getConfig());
		conn.removeExcludedSubject("GMSEC.FOO.BAR");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Connection has not been initialized") != std::string::npos);
	}

	try
	{   // Invalid subject
		Connection conn(test.getConfig());
		conn.connect();
		conn.removeExcludedSubject("GMSEC FOO BAR");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Subject is invalid") != std::string::npos);
	}
}


void test_get_set_name(Test& test)
{
	GMSEC_INFO << "Test getName() and setName()";

	try
	{
		Connection conn(test.getConfig());
		conn.connect();
		test.check("Expected a connection name to be non-NULL", NULL != conn.getName());

		conn.setName("FOOBAR");

		test.check("Unexpected connection name", std::string("FOOBAR") == conn.getName());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_id(Test& test)
{
	GMSEC_INFO << "Test getID()";

	try
	{
		Connection conn(test.getConfig());
		conn.connect();
		test.check("Expected connection ID to be non-NULL", NULL != conn.getID());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_mw_info(Test& test)
{
	GMSEC_INFO << "Test getMWInfo";

	Config& config = test.getConfig();

	try
	{
		Connection conn(config);
		test.check("Unexpected mw-info string", std::string(conn.getMWInfo()).find(config.getValue("mw-id")) != std::string::npos);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_connection_endpoint(Test& test)
{
	GMSEC_INFO << "Test getConnectionEndpoint()";

	try
	{
		Connection conn(test.getConfig());
		conn.connect();
		test.check("Unexpected connection endpoint", conn.getConnectionEndpoint() != NULL);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_publish_queue_message_count(Test& test)
{
	GMSEC_INFO << "Test getPublishQueueMessageCount()";

	Config config = test.getConfig();   // make a copy
	config.addValue("gmsec-async-publish", "true");
	config.addValue("gmsec-async-publish-queue-depth", "20");
	config.addValue("gmsec-async-publish-teardown-wait", "2000");

	try
	{
		Connection conn(config);

		conn.connect();

		set_standard_fields(conn.getMessageFactory());

		Message msg = conn.getMessageFactory().createMessage("LOG");
		msg.addField("SUBCLASS", "NO-CLASS");
		msg.addField("OCCURRENCE-TYPE", "FREQUENTLY");
		msg.addField("SEVERITY", static_cast<short>(1));

		for (int i = 0; i < 10; ++i)
		{
			//DMW GMSEC_U64 count = conn.getPublishQueueMessageCount();
			//DMW test.check("Unexpected message count", count <= static_cast<GMSEC_U64>(1));
			conn.publish(msg);
			test.check("Unexpected message count", conn.getPublishQueueMessageCount() >= static_cast<GMSEC_U64>(0));
			//DMW TimeUtil::millisleep(500);
		}
		test.check("Unexpected message count", conn.getPublishQueueMessageCount() >= static_cast<GMSEC_U64>(0));
		//DMW TimeUtil::millisleep(500);
		TimeUtil::millisleep(2000);
		//DMW test.check("Expected message count to be 0 (zero)", static_cast<GMSEC_U64>(0) == conn.getPublishQueueMessageCount());
		test.check("Expected message count to be 0 (zero)", conn.getPublishQueueMessageCount() == static_cast<GMSEC_U64>(0));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


int test_Connection(Test& test)
{
	test.setDescription("Connection");

	test_constructor(test);
	test_get_api_version(test);
	test_connect(test);
	test_disconnect(test);
	test_get_library_name(test);
	test_get_library_version(test);
	test_get_message_factory(test);
	test_register_event_callback(test);
	test_subscribe(test);
	test_subscribe_with_callback(test);    // also calls on startAutoDispatch/stopAutoDispatch
	test_unsubscribe(test);
	test_start_auto_dispatch(test);
	test_stop_auto_dispatch(test);
	test_publish(test);                    // also calls on subscribe() and receive()
	test_publish_with_mwconfig(test);
	test_request_with_callback(test);      // also calls on reply()
	test_request(test);                    // also calls on reply()
	test_reply(test);
	test_dispatch(test);
	test_receive(test);
	test_exclude_subject(test);            // also tests removeExcludedSubject()
	test_remove_excluded_subject(test);
	test_get_set_name(test);
	test_get_id(test);
	test_get_mw_info(test);
	test_get_connection_endpoint(test);
	test_get_publish_queue_message_count(test);

	return 0;
}

TEST_DRIVER(test_Connection)
