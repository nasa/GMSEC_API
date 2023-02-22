#include "TestCase.h"

#include <gmsec5_cpp.h>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


const char*     CONNECTION_CLIENT_ID         = "mw-connection-client-id";
const char*     DURABLE_SUBSCRIBE            = "mw-durable-subscribe";
const char*     DURABLE_CLIENT_ID            = "mw-durable-client-id";
const char*     DURABLE_MESSAGE_SELECTOR     = "mw-durable-message-selector";
const char*     DURABLE_PUBLISH              = "mw-durable-publish";
const char*     DURABLE_PRIORITY             = "mw-durable-priority";
const char*     DURABLE_TIME_TO_LIVE         = "mw-durable-time-to-live";
const char*     DURABLE_MESSAGE_LIFETIME     = "30000";
const int       DURABLE_SUBSCRIPTION_TIMEOUT = 60000;
const int       DURABLE_MESSAGE_LIFETIME_INT = 30000;
const GMSEC_U32 getnextmsg_timeout           = 4000;


class DurableSubscriberCallback : public Callback
{
public:
	DurableSubscriberCallback() : messageCount(0)
	{
	}

	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg)
	{
		++messageCount;
	}

	int getMessageCount() const
	{
		return messageCount;
	}

private:
	int messageCount;
};


class ActiveMQDurability
{
public:
	static int test_ActiveMQDurability(Test &test)
	{
		test.setDescription("ActiveMQ Wrapper Durable Subscribe and Publish Functionality");

		const char* mwValue = test.getConfig().getValue("MW-ID");

		if (mwValue == 0)
		{
			test.excuse("Unable to deduce middleware type.");
			return 0;
		}
		else if ((std::string(mwValue).substr(0,8) != "activemq") && (std::string(mwValue).substr(0,7) != "artemis"))
		{
			test.excuse("Test is being excused for all middlewares except ActiveMQ and Artemis");
			return 0;
		}

		// Commence testing
		ActiveMQDurability tester(test);
		tester.run();

		return 0;
	}

protected:
	ActiveMQDurability(Test &test);

	void run();
// Setup and Teardown Methods
	void setupConnections();
	void setupSubConnection();
	void setupPubConnection();

	void teardownConnections();
	void teardownSubConnection();
	void teardownPubConnection();

	void setupConfigs(const char* timeToLive);
	void teardownConfigs();

	void setupSubject(const int testNo);
	void resetSubject();
// Nominal Test Cases
	void testBasicSubPub();
	void testDurableSubUnsub();
	void testSubscriptionCleanup();
	void testMessageTimeToLive();
	void testDurableSubDisconnect();
// Off-Nominal Test Cases

private:
	std::string createSubject(const int testNo);
	
	Test&       m_test;
	Connection* m_subConnection;
	Connection* m_pubConnection;
	Config      m_subConfig;
	Config      m_pubConfig;
	std::string m_subject;
};


ActiveMQDurability::ActiveMQDurability(Test& test)
	: m_test(test),
	  m_subConnection(0),
	  m_pubConnection(0),
	  m_subConfig(Config()),
	  m_pubConfig(Config()),
	  m_subject("")
{
}


TEST_DRIVER(ActiveMQDurability::test_ActiveMQDurability)


void ActiveMQDurability::run()
{
//o Nominal Tests
	testBasicSubPub();
	testDurableSubUnsub();
	testDurableSubDisconnect();
	testSubscriptionCleanup();
	testMessageTimeToLive();

//o Off-Nominal Tests
	// TODO
}


void
ActiveMQDurability::testBasicSubPub()
{
	GMSEC_INFO << "Testing the ability to use a durable subscription to receive a message without an unsubscribe/disconnect";

	try
	{
		setupSubject(1);
		setupConnections();
		setupConfigs(DURABLE_MESSAGE_LIFETIME);

		// Create the message
		Message msg;
		msg.setSubject(m_subject.c_str());

		// Create the durable subscription
		m_subConnection->subscribe(m_subject.c_str(), m_subConfig);

		// Publish message using the durable publisher config
		m_pubConnection->publish(msg, m_pubConfig);

		// Receive the message
		Message* rcvMsg = m_subConnection->receive(getnextmsg_timeout);
		m_test.check("Did not receive a message!", rcvMsg != NULL);

		if (rcvMsg)
		{
			Message::destroy(rcvMsg);
		}

		resetSubject();
		teardownConfigs();
		teardownConnections();
	}
	catch (const GmsecException& e)
	{
		m_test.require(e.what(), false);
	}
}


void ActiveMQDurability::testDurableSubUnsub()
{
	GMSEC_INFO << "Testing the ability to use a durable subscription to receive a message while unsubscribed";

	try
	{
		setupSubject(2);
		setupConnections();
		setupConfigs(DURABLE_MESSAGE_LIFETIME);

		// Create the message
		Message msg;
		msg.setSubject(m_subject.c_str());

		// Create the durable subscription
		SubscriptionInfo* info = m_subConnection->subscribe(m_subject.c_str(), m_subConfig);

		// Unsubscribe
		m_subConnection->unsubscribe(info);

		// Publish message using the durable publisher config
		m_pubConnection->publish(msg, m_pubConfig);

		// Create the durable subscription
		info = m_subConnection->subscribe(m_subject.c_str(), m_subConfig);

		// Receive the message
		Message* rcvMsg = m_subConnection->receive(getnextmsg_timeout);
		m_test.check("Did not receive a message!", rcvMsg != NULL);

		if (rcvMsg)
		{
			Message::destroy(rcvMsg);
		}

		// Unsubscribe
		m_subConnection->unsubscribe(info);

		resetSubject();
		teardownConfigs();
		teardownConnections();
	}
	catch (const GmsecException& e)
	{
		m_test.require(e.what(), false);
	}
}


void ActiveMQDurability::testDurableSubDisconnect()
{
	GMSEC_INFO << "Testing the ability to use a durable subscription to receive messages while disconnected";

	try
	{
		setupSubject(3);
		setupConnections();
		setupConfigs(DURABLE_MESSAGE_LIFETIME);

		// Create the message
		Message msg;
		msg.setSubject(m_subject.c_str());

		// Create the callback for this test
		DurableSubscriberCallback cb;

		// Create the durable subscription
		m_subConnection->subscribe(m_subject.c_str(), m_subConfig, &cb);
		m_subConnection->startAutoDispatch();

		int pubIterations = 10;

		for (int i = 0; i < pubIterations; i++)
		{
			// Publish message using the durable publisher config
			m_pubConnection->publish(msg, m_pubConfig);

			if (i == 4)
			{
				teardownSubConnection();
			}
			else if (i == 8)
			{
				setupSubConnection();
				m_subConnection->subscribe(m_subject.c_str(), m_subConfig, &cb);
				m_subConnection->startAutoDispatch();
			}
		}

		// Check to see if the same number of messages published were received
		m_test.check("Durable subscriber did not receive all messages!", cb.getMessageCount() != pubIterations);

		resetSubject();
		teardownConfigs();
		teardownConnections();
	}
	catch (const GmsecException& e)
	{
		m_test.require(e.what(), false);
	}
}


void ActiveMQDurability::testSubscriptionCleanup()
{
	GMSEC_INFO << "Testing to ensure that durable subscriptions (and corresponding messages) are "
	              "cleaned up on the server when nobody is subscribed to the topic for a period of time";

	try
	{
		setupSubject(4);
		setupConnections();
		setupConfigs(DURABLE_MESSAGE_LIFETIME);

		// Create the message
		Message msg;
		msg.setSubject(m_subject.c_str());

		// Create the durable subscription
		SubscriptionInfo* info = m_subConnection->subscribe(m_subject.c_str(), m_subConfig);

		// Unsubscribe
		m_subConnection->unsubscribe(info);

		// Publish message using the durable publisher config
		m_pubConnection->publish(msg, m_pubConfig);

		TimeUtil::millisleep(DURABLE_SUBSCRIPTION_TIMEOUT + 2000);

		// Create the durable subscription
		m_subConnection->subscribe(m_subject.c_str(), m_subConfig);

		// Attempt to receive a non-existent message
		Message* rcvMsg = m_subConnection->receive(getnextmsg_timeout);
		m_test.check("Erroneously received a message!", rcvMsg == NULL);

		if (rcvMsg)
		{
			Message::destroy(rcvMsg);
		}

		resetSubject();
		teardownConfigs();
		teardownConnections();
	}
	catch (const GmsecException& e)
	{
		m_test.require(e.what(), false);
	}
}


void ActiveMQDurability::testMessageTimeToLive()
{
	GMSEC_INFO << "Test to ensure that durable messages are cleaned up before the end "
	              "of their configured lifetime if a consumer does not retrieve them";

	try
	{
		setupSubject(5);
		setupConnections();
		setupConfigs(DURABLE_MESSAGE_LIFETIME);

		// Create the message
		Message msg;
		msg.setSubject(m_subject.c_str());

		// Create the durable subscription
		SubscriptionInfo* info = m_subConnection->subscribe(m_subject.c_str(), m_subConfig);

		// Unsubscribe
		m_subConnection->unsubscribe(info);

		// Publish message using the durable publisher config
		m_pubConnection->publish(msg, m_pubConfig);

		TimeUtil::millisleep(DURABLE_MESSAGE_LIFETIME_INT + 2000);

		// Create the durable subscription
		m_subConnection->subscribe(m_subject.c_str(), m_subConfig);

		// Attempt to receive a non-existent message
		Message* rcvMsg = m_subConnection->receive(getnextmsg_timeout);
		m_test.check("Erroneously received a message!", rcvMsg == NULL);

		if (rcvMsg)
		{
			Message::destroy(rcvMsg);
		}

		resetSubject();
		teardownConfigs();
		teardownConnections();
	}
	catch (const GmsecException& e)
	{
		m_test.require(e.what(), false);
	}
}


void ActiveMQDurability::setupConnections()
{
	setupSubConnection();
	setupPubConnection();
}


void ActiveMQDurability::setupSubConnection()
{
	Config config = m_test.getConfig();
	config.addValue(CONNECTION_CLIENT_ID, m_subject.c_str());

	m_subConnection = new Connection(config);
	m_subConnection->connect();
}


void ActiveMQDurability::setupPubConnection()
{
	Config config = m_test.getConfig();

	m_pubConnection = new Connection(config);
	m_pubConnection->connect();
}


void ActiveMQDurability::teardownConnections()
{
	teardownSubConnection();
	teardownPubConnection();
}


void ActiveMQDurability::teardownSubConnection()
{
	m_subConnection->disconnect();

	delete m_subConnection;
}


void ActiveMQDurability::teardownPubConnection()
{
	m_pubConnection->disconnect();

	delete m_pubConnection;
}


void ActiveMQDurability::setupConfigs(const char* timeToLive)
{
	// Create the durable subscriber config object
	m_subConfig.addValue(DURABLE_SUBSCRIBE, "true");
	m_subConfig.addValue(DURABLE_CLIENT_ID, "nightrun");
	m_subConfig.addValue(DURABLE_MESSAGE_SELECTOR, "");

	// Create the durable publisher config object
	m_pubConfig.addValue(DURABLE_PUBLISH, "true");
	m_pubConfig.addValue(DURABLE_PRIORITY, "4");
	m_pubConfig.addValue(DURABLE_TIME_TO_LIVE, timeToLive);
}


void ActiveMQDurability::teardownConfigs()
{
	// Create the durable subscriber config object
	m_subConfig.clearValue(DURABLE_SUBSCRIBE);
	m_subConfig.clearValue(DURABLE_CLIENT_ID);
	m_subConfig.clearValue(DURABLE_MESSAGE_SELECTOR);

	// Create the durable publisher config object
	m_pubConfig.clearValue(DURABLE_PUBLISH);
	m_pubConfig.clearValue(DURABLE_PRIORITY);
	m_pubConfig.clearValue(DURABLE_TIME_TO_LIVE);
}


void ActiveMQDurability::setupSubject(int testNo)
{
	m_subject = createSubject(testNo);
}


void ActiveMQDurability::resetSubject()
{
	m_subject = "";
}


std::string ActiveMQDurability::createSubject(int testNo)
{
	std::stringstream ss;
	ss << testNo;
	return m_test.getSubject(ss.str().c_str());
}
