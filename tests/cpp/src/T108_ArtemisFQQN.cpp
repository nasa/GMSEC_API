#include "TestCase.h"

#include <sstream>
#include <string>


/*
 T108_ArtemisFQQN:

 This test employs the use of a Connection object to subscribe to receive messages using a FQQN
 consumer. This consumer will only acknowledge the first 2 messages (out of 10) it receives
 (even though it receives all messages). The consumer's Connection will then be terminated.
 Note this portion of the unit test will result in memory leaks since 8 messages are not
 acknowledged; the leaks are expected and should not be a cause for concern.

 Moments after the first consumer has disconnected, a new consumer will be set up.
 This new consumer will receive all 8 messages that were previously not acknowledged.
 */


using namespace gmsec::api5;
using namespace gmsec::api5::util;


class Subscriber : public Callback
{
public:
	enum class Type { GOOD_SUBSCRIBER, BAD_SUBSCRIBER };

	Subscriber(Type type) : m_type(type), m_msgsRcvd(0) {}

	virtual ~Subscriber() {}

	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg) {
		++m_msgsRcvd;

		if (m_type == Type::BAD_SUBSCRIBER && m_msgsRcvd < 3) {
			// only acknowledge the first two messages; normally a good subscriber would acknowledge all
			msg.acknowledge();
		}
		else if (m_type == Type::GOOD_SUBSCRIBER) {
			msg.acknowledge();
		}
	}

	int numMsgsRcvd() const {
		return m_msgsRcvd;
	}

	void reset() {
		m_msgsRcvd = 0;
	}

private:
	Type m_type;
	int  m_msgsRcvd;
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
	Config      config;
	std::string msgSubject;
};


int test_ArtemisFQQN(Test& test)
{
	test.setDescription("Artemis FQQN - Part 3");

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

	GMSEC_INFO << "Verify once FQQN subscriber stops ACKing msgs, another subscriber receives them";

	// Elements to conjure unique message subjects and subscription topics
	std::stringstream msgSubject;
	msgSubject << "T108.FOO.BAR." << test.getUniqueComponent();

	// Elements to conjure unique message subjects and subscription topics
	std::stringstream id;
	id << (test.getProcessID() & 0xFFFF);

	std::string fqqn_topic = msgSubject.str() + "::MQ-" + id.str();

	GMSEC_INFO << "FQQN Subject: " << fqqn_topic.c_str();

	// Setup config for INDIVIDUAL_ACKNOWLEDGEMENT (4)
	Config config = test.getConfig();
	config.addValue(AMQ_ACK_MODE, "4");

	StdSharedPtr<Publisher>   publisher( new Publisher(config, msgSubject.str().c_str()) );
	StdUniquePtr<StdThread>   thread( new StdThread(&Publisher::runThread, publisher) );
	StdUniquePtr<Connection>  conn;
	Subscriber                fqqn_cb1( Subscriber::Type::BAD_SUBSCRIBER );
	Subscriber                fqqn_cb2( Subscriber::Type::GOOD_SUBSCRIBER );

	//o Use the Publisher to publish messages to an FQQN subscriber that stops acknowledging messages.
	//  Messages that are not acknowledged will be delivered to a second FQQN subscriber.
	try
	{
		// Start up consumer #1
		conn.reset( new Connection(config) );

		conn->connect();

		conn->subscribe( fqqn_topic.c_str(), &fqqn_cb1 );

		conn->startAutoDispatch();

		TimeUtil::millisleep(5000);

		// Start thread to publish messages
		thread->start();
		thread->join();

		// Disconnect consumer #1
		conn->disconnect();


		TimeUtil::millisleep(5000);


		// Start up consumer #2 to receive un-ACKed messages
		conn.reset( new Connection(config) );

		conn->connect();

		conn->subscribe( fqqn_topic.c_str(), &fqqn_cb2 );

		conn->startAutoDispatch();

		TimeUtil::millisleep(5000);   // allow time to receive messages

		// Disconnect consumer #2
		conn->disconnect();


		int fqqn_cb1_rcvd = fqqn_cb1.numMsgsRcvd();
		int fqqn_cb2_rcvd = fqqn_cb2.numMsgsRcvd();

		GMSEC_INFO << "FQQN subscriber 1 received: " << fqqn_cb1_rcvd;
		GMSEC_INFO << "FQQN subscriber 2 received: " << fqqn_cb2_rcvd;

		test.check("Expected FQQN subscriber 1 to have received 10 messages", fqqn_cb1_rcvd == 10);
		test.check("Expected FQQN subscriber 2 to have received 8 messages",  fqqn_cb2_rcvd == 8);
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}

	conn.reset();

	Connection::shutdownAllMiddlewares();

	return 0;
}

TEST_DRIVER(test_ArtemisFQQN)
