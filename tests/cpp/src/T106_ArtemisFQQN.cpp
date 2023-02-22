#include "TestCase.h"

#include <sstream>
#include <string>


/*
 T106_ArtemisFQQN:

 This test employs the use of one regular subscriber, and two FQQN subscribers, all of which rely
 on their own respective Connection object.

 The point of the test is to publish messages using a known message subject, and to verify the
 regular (normal) subscriber receives all messages, and that the FQQN subscribers each receive
 half of those.
 */


using namespace gmsec::api5;
using namespace gmsec::api5::util;


class Subscriber : public Callback
{
public:
	enum class Type { NORMAL, FQQN };

	Subscriber(Test& test, Type type) : m_test(test), m_type(type), m_counter(0), m_msgsRcvd(0) {}

	virtual ~Subscriber() {}

	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg)
	{
		++m_msgsRcvd;

		if (msg.hasField("COUNTER"))
		{
			GMSEC_U16 counter = static_cast<GMSEC_U16>( msg.getU32Value("COUNTER") );

			if (m_counter == 0)
			{
				m_test.check("Got a message", true);
			}
			else
			{
				if (m_type == Type::NORMAL)
				{
					m_test.check("Unexpected counter value", (m_counter + 1) == counter);
				}
				else
				{
					m_test.check("Unexpected counter value", (m_counter + 2) == counter);
				}
			}

			m_counter = counter;
		}
		else
		{
			m_test.check("Message is missing COUNTER field", false);
		}

		msg.acknowledge();
	}

	int numMsgsRcvd() const
	{
		return m_msgsRcvd;
	}

	void reset()
	{
		m_counter  = 0;
		m_msgsRcvd = 0;
	}

private:
	Test&     m_test;
	Type      m_type;
	GMSEC_U16 m_counter;
	int       m_msgsRcvd;
};


class Publisher
{
public:
	static void runThread(StdSharedPtr<Publisher> pub) {
		pub->run();
	}

	Publisher(const Config& config, const char* msgSubject) : config(config), msgSubject(msgSubject) {
	}

	~Publisher() {
	}

	void run() {
		StdUniquePtr<Connection> conn;

		try {
			Message msg;
			msg.setSubject(msgSubject.c_str());

			conn.reset( new Connection(config) );

			conn->connect();

			for (GMSEC_U16 counter = 1; counter < 11; ++counter)
			{
				msg.addField("COUNTER", counter);

				conn->publish(msg);

				/*
				if (counter == static_cast<GMSEC_U16>(1)) {
					GMSEC_INFO << "Published:\n" << msg.toXML();
				}
				else {
					GMSEC_INFO << "Published: " << msg.getSubject();
				}
				*/

				TimeUtil::millisleep(1000);
			}

			conn->disconnect();
		}
		catch (const GmsecException& e) {
			GMSEC_ERROR << "Publisher thread aborting; reason: " << e.what();
		}

		conn.reset();
	}

private:
	const Config& config;
	std::string   msgSubject;
};


int test_ArtemisFQQN(Test& test)
{
	test.setDescription("Artemis FQQN - Part 1");

	// Only run tests with Artemis
	const char* mw = test.getConfig().getValue("mw-id");
	if (!mw)
	{
		test.excuse("Unable to deduce middleware type.");
		return -1;
	}

	if (!strcontains(mw, "artemis"))
	{
		test.excuse("Test is being excused for all middlewares except Apache Artemis");
		return 0;
	}

	GMSEC_INFO << "Verify usage of two FQQN subscribers, alongside a sole (normal) subscriber";

	// Elements to conjure unique message subjects and subscription topics
	std::stringstream msgSubject;
	msgSubject << "T106.FOO.BAR." << test.getUniqueComponent();

	// Elements to conjure unique message subjects and subscription topics
	std::stringstream id;
	id << (test.getProcessID() & 0xFFFF);

	std::string norm_topic = msgSubject.str();
	std::string fqqn_topic = msgSubject.str() + "::MQ-" + id.str();

	GMSEC_INFO << "Norm Subject: " << norm_topic.c_str();
	GMSEC_INFO << "FQQN Subject: " << fqqn_topic.c_str();

	// Setup config for INDIVIDUAL_ACKNOWLEDGEMENT (4)
	Config config = test.getConfig();
	config.addValue(AMQ_ACK_MODE, "4");

	StdSharedPtr<Publisher>   publisher( new Publisher(config, msgSubject.str().c_str()) );
	StdUniquePtr<StdThread>   thread( new StdThread(&Publisher::runThread, publisher) );
	StdUniquePtr<Connection>  conn1, conn2, conn3;
	Subscriber                norm_cb(test, Subscriber::Type::NORMAL);
	Subscriber                fqqn_cb1(test, Subscriber::Type::FQQN);
	Subscriber                fqqn_cb2(test, Subscriber::Type::FQQN);

	//o Use the Publisher to publish messages that both a regular subscriber
	//  and FQQN subscribers receive. The FQQN subscribers will each receive half
	//  of the published messages.
	try
	{
		conn1.reset( new Connection(config) );
		conn2.reset( new Connection(config) );
		conn3.reset( new Connection(config) );

		conn1->connect();
		conn2->connect();
		conn3->connect();

		conn1->subscribe( norm_topic.c_str(), &norm_cb );    // Normal subscriber
		conn2->subscribe( fqqn_topic.c_str(), &fqqn_cb1 );   // FQQN subscriber
		conn3->subscribe( fqqn_topic.c_str(), &fqqn_cb2 );   // FQQN subscriber

		conn1->startAutoDispatch();
		conn2->startAutoDispatch();
		conn3->startAutoDispatch();

		TimeUtil::millisleep(5000);   // give a chance for the auto-dispatcher threads to start

		// Start publishing messages
		thread->start();
		thread->join();

		int norm_cb_rcvd  = norm_cb.numMsgsRcvd();
		int fqqn_cb1_rcvd = fqqn_cb1.numMsgsRcvd();
		int fqqn_cb2_rcvd = fqqn_cb2.numMsgsRcvd();

		test.check("Expected NORMAL subscriber to have received 10 messages", norm_cb_rcvd  == 10);
		test.check("Expected FQQN subscriber 1 to have received 5 messages",  fqqn_cb1_rcvd == 5);
		test.check("Expected FQQN subscriber 2 to have received 5 messages",  fqqn_cb2_rcvd == 5);

		GMSEC_INFO << "NORMAL subscriber received: " << norm_cb_rcvd;
		GMSEC_INFO << "FQQN subscriber 1 received: " << fqqn_cb1_rcvd;
		GMSEC_INFO << "FQQN subscriber 2 received: " << fqqn_cb2_rcvd;

		conn1->disconnect();
		conn2->disconnect();
		conn3->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}

	conn1.reset();
	conn2.reset();
	conn3.reset();

	Connection::shutdownAllMiddlewares();

	return 0;
}

TEST_DRIVER(test_ArtemisFQQN)
