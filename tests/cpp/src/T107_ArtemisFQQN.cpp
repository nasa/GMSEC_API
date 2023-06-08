#include "TestCase.h"

#include <sstream>
#include <string>


/*
 T107_ArtemisFQQN:

 This test employs the use of two regular subscribers, and four FQQN subscribers. Three Connection
 objects are used, each supporting two subscriptions.

 The point of the test is to publish two sets of messages using known message subjects, and to verify
 the regular (normal) subscribers receives all messages for which they are subscribed, and that the
 FQQN subscribers each receive half of those.
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
	test.setDescription("Artemis FQQN - Part 2");

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

	GMSEC_INFO << "Verify usage of multiple FQQN subscribers w/ distinct message queues";

	// Elements to conjure unique message subjects and subscription topics
	std::stringstream msgSubject1, msgSubject2;
	msgSubject1 << "T107.FOO.BAR." << test.getUniqueComponent();
	msgSubject2 << "T107.FOO.BAZ." << test.getUniqueComponent();

	// Elements to conjure unique message subjects and subscription topics
	std::stringstream id1, id2;
	id1 << (test.getProcessID() & 0xFFFF);
	id2 << (test.getProcessID() & 0x00FF);

	std::string norm_topic1 = msgSubject1.str();
	std::string norm_topic2 = msgSubject2.str();
	std::string fqqn_topic1 = msgSubject1.str() + "::MQ-" + id1.str();
	std::string fqqn_topic2 = msgSubject2.str() + "::MQ-" + id2.str();

	GMSEC_INFO << "Norm Subject 1: " << norm_topic1.c_str();
	GMSEC_INFO << "Norm Subject 2: " << norm_topic2.c_str();
	GMSEC_INFO << "FQQN Subject 1: " << fqqn_topic1.c_str();
	GMSEC_INFO << "FQQN Subject 2: " << fqqn_topic2.c_str();

	// Setup config for INDIVIDUAL_ACKNOWLEDGEMENT (4)
	Config config = test.getConfig();
	config.addValue(AMQ_ACK_MODE, "4");

    StdSharedPtr<Publisher>   publisher1( new Publisher(config, msgSubject1.str().c_str()) );
    StdSharedPtr<Publisher>   publisher2( new Publisher(config, msgSubject2.str().c_str()) );
    StdUniquePtr<StdThread>   thread1( new StdThread(&Publisher::runThread, publisher1) );
    StdUniquePtr<StdThread>   thread2( new StdThread(&Publisher::runThread, publisher2) );

	StdUniquePtr<Connection>  conn1, conn2, conn3;
	Subscriber                norm_cb1(test, Subscriber::Type::NORMAL);
	Subscriber                norm_cb2(test, Subscriber::Type::NORMAL);
	Subscriber                fqqn_cb1(test, Subscriber::Type::FQQN);
	Subscriber                fqqn_cb2(test, Subscriber::Type::FQQN);
	Subscriber                fqqn_cb3(test, Subscriber::Type::FQQN);
	Subscriber                fqqn_cb4(test, Subscriber::Type::FQQN);

	try
	{
		conn1.reset( new Connection(config) );
		conn2.reset( new Connection(config) );
		conn3.reset( new Connection(config) );

		conn1->connect();
		conn2->connect();
		conn3->connect();

		conn1->subscribe( norm_topic1.c_str(), &norm_cb1 );   // Normal subscriber
		conn1->subscribe( norm_topic2.c_str(), &norm_cb2 );   // Normal subscriber

		conn2->subscribe( fqqn_topic1.c_str(), &fqqn_cb1 );   // FQQN subscriber
		conn3->subscribe( fqqn_topic1.c_str(), &fqqn_cb2 );   // FQQN subscriber

		conn2->subscribe( fqqn_topic2.c_str(), &fqqn_cb3 );   // FQQN subscriber
		conn3->subscribe( fqqn_topic2.c_str(), &fqqn_cb4 );   // FQQN subscriber

		conn1->startAutoDispatch();
		conn2->startAutoDispatch();
		conn3->startAutoDispatch();

		TimeUtil::millisleep(5000);   // give a change for the auto-dispatcher threads to start

		// Start HB and RSRC generators to publish messages
		thread1->start();
		thread2->start();
		thread1->join();
		thread2->join();

		int norm_cb1_rcvd = norm_cb1.numMsgsRcvd();
		int norm_cb2_rcvd = norm_cb2.numMsgsRcvd();
		int fqqn_cb1_rcvd = fqqn_cb1.numMsgsRcvd();
		int fqqn_cb2_rcvd = fqqn_cb2.numMsgsRcvd();
		int fqqn_cb3_rcvd = fqqn_cb3.numMsgsRcvd();
		int fqqn_cb4_rcvd = fqqn_cb4.numMsgsRcvd();

		test.check("Expected NORMAL subscriber 1 to have received 10 messages", norm_cb1_rcvd == 10);
		test.check("Expected NORMAL subscriber 2 to have received 10 messages", norm_cb2_rcvd == 10);
		test.check("Expected FQQN subscriber 1 to have received 5 messages",    fqqn_cb1_rcvd == 5);
		test.check("Expected FQQN subscriber 2 to have received 5 messages",    fqqn_cb2_rcvd == 5);
		test.check("Expected FQQN subscriber 3 to have received 5 messages",    fqqn_cb3_rcvd == 5);
		test.check("Expected FQQN subscriber 4 to have received 5 messages",    fqqn_cb4_rcvd == 5);

		GMSEC_INFO << "NORMAL subscriber 1 received: " << norm_cb1_rcvd;
		GMSEC_INFO << "NORMAL subscriber 2 received: " << norm_cb2_rcvd;
		GMSEC_INFO << "FQQN subscriber 1 received: "   << fqqn_cb1_rcvd;
		GMSEC_INFO << "FQQN subscriber 2 received: "   << fqqn_cb2_rcvd;
		GMSEC_INFO << "FQQN subscriber 3 received: "   << fqqn_cb3_rcvd;
		GMSEC_INFO << "FQQN subscriber 4 received: "   << fqqn_cb4_rcvd;

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
