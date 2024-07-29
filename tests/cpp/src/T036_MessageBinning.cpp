#include "TestCase.h"

#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/SystemUtil.h>

#include <set>
#include <string>
#include <vector>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


void conductTest(Test& test, const Config& config);
void test_CfgMsgBinSize(Test& test);
void test_CfgMsgBinTimeout(Test& test);
void test_CfgMsgBinSubjects(Test& test);
void test_CfgMsgBinExcludeSubjects(Test& test);
void test_CfgMsgBinUsingValidation(Test& test);


int test_MessageBinning(Test& test)
{
    test.setDescription("Message Binning");

	test_CfgMsgBinSize(test);
	test_CfgMsgBinTimeout(test);
	test_CfgMsgBinSubjects(test);
	test_CfgMsgBinExcludeSubjects(test);
	test_CfgMsgBinUsingValidation(test);

	return 0;
}

TEST_DRIVER(test_MessageBinning);


class BurstCallback : public Callback
{
public:
	BurstCallback(Test& test, size_t numExpectedMsgs, int expectedInterval)
		: m_test(test),
		  m_numExpectedMsgs(numExpectedMsgs),
		  m_expectedInterval(expectedInterval),
		  m_msgsReceived(0),
		  m_timeIndex(1),
		  m_firstWave(true),
		  m_allDone(false)
	{
		m_rcvTime[0] = 0;
		m_rcvTime[1] = 0;
	}

	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg)
	{
		// The expectation is that two bursts consisting of N messages will
		// be received.
		//
		// When we receive the first of N messages, we capture the time
		// when it is received.
		//
		// After we have received N messages, we reset our message counter.
		//
		// Upon receipt of the first of N messages from the second burst,
		// we again capture the time.
		//
		// After all messages from the second burst of messages have been
		// received, we then compute the time delta between the two receive
		// times to ensure that we are within our expected interval.

		++m_msgsReceived;

		if (m_msgsReceived == 1)
		{
			GMSEC_VERBOSE << "Begin burst of messages...";
			GMSEC_VERBOSE << "Received Message: " << msg.getSubject();

			m_timeIndex ^= 1;   // ping-pong index between 0 and 1 using XOR operation

			m_rcvTime[m_timeIndex] = TimeUtil::getCurrentTime_s();
		}
		else if (m_msgsReceived == m_numExpectedMsgs)
		{
			GMSEC_VERBOSE << "Received Message: " << msg.getSubject();
			GMSEC_VERBOSE << "End burst of messages...";

			m_msgsReceived = 0;
			m_allDone = (m_timeIndex == 1);
		}
		else
		{
			GMSEC_VERBOSE << "Received Message: " << msg.getSubject();
		}

		if (m_allDone)
		{
			GMSEC_VERBOSE << "Checking time...";

			double delta_raw = m_rcvTime[1] - m_rcvTime[0];
			int    delta_int = static_cast<int>(delta_raw);

			GMSEC_INFO    << "Time delta between receiving bursts of aggregated messages: " << delta_raw << " seconds";
			GMSEC_VERBOSE << "Time delta between receiving bursts of aggregated messages: " << delta_int << " seconds";
			GMSEC_VERBOSE << "Expected interval: " << m_expectedInterval;

			// Allow for up to a +/- 1 second variation for the time-delta.  In theory, we should never see a -1 variation,
			// but this could be caused by issues with TimeUtil::millisleep() or with middleware wrapper (i.e. threading)
			// issues that prevents the timely delivery of messages.
			m_test.check("Delta period is off", (delta_int == (m_expectedInterval-1)) || (delta_int == m_expectedInterval) || (delta_int == (m_expectedInterval+1)));

			m_allDone = false;
		}
	}

private:
	Test&  m_test;
	size_t m_numExpectedMsgs;
	int    m_expectedInterval;
	size_t m_msgsReceived;
	size_t m_timeIndex;
	double m_rcvTime[2];
	bool   m_firstWave;
	bool   m_allDone;
};


class ExpectedMsgCallback : public Callback
{
private:
	typedef std::set<std::string> Topics;

public:
	ExpectedMsgCallback(Test& test, const Topics& excludedSubjects, GMSEC_F64 rcvTimeThreshold_ms)
		: m_test(test),
		  m_excludedSubjects(excludedSubjects),
		  m_rcvTimeThreshold(rcvTimeThreshold_ms/1000),
		  m_numExpectedMsgs(0),
		  m_numExcludedMsgs(0)
	{
	}

	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg)
	{
		GMSEC_VERBOSE << "ExpectedMsgCallback::onMessage -- Received: " << msg.getSubject();

		GMSEC_F64 now      = TimeUtil::getCurrentTime_s();
		GMSEC_F64 sendTime = msg.getF64Value("SEND-TIME");
		GMSEC_F64 delta    = now - sendTime;
		bool      excluded = (m_excludedSubjects.find(msg.getSubject()) != m_excludedSubjects.end());

		if (excluded)
		{
			++m_numExcludedMsgs;

			m_test.check("Excluded Message arrived significantly late", delta <= m_rcvTimeThreshold);

			GMSEC_VERBOSE << "Excluded -- delta is: " << delta << "; threshold is: " << m_rcvTimeThreshold;
		}
		else
		{
			++m_numExpectedMsgs;

			m_test.check("Expected Message arrived significantly early", delta > m_rcvTimeThreshold);

			GMSEC_VERBOSE << "Expected -- delta is: " << delta << "; threshold is: " << m_rcvTimeThreshold;
		}
	}

	size_t getNumExcludedMsgs() const
	{
		return m_numExcludedMsgs;
	}

	size_t getNumExpectedMsgs() const
	{
		return m_numExpectedMsgs;
	}

private:
	Test&       m_test;
	Topics      m_excludedSubjects;
	GMSEC_F64   m_rcvTimeThreshold;
	size_t      m_numExpectedMsgs;
	size_t      m_numExcludedMsgs;
};


class ExcludeMsgCallback : public Callback
{
public:
	ExcludeMsgCallback(Test& test, const std::string& excludedSubject, GMSEC_F64 rcvTimeThreshold_ms)
		: m_test(test),
		  m_excludedSubject(excludedSubject),
		  m_rcvTimeThreshold(rcvTimeThreshold_ms/1000),
		  m_numExpectedMsgs(0),
		  m_numExcludedMsgs(0)
	{
	}

	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg)
	{
		GMSEC_VERBOSE << "ExcludeMsgCallback::onMessage -- Received: " << msg.getSubject();

		GMSEC_F64 now      = TimeUtil::getCurrentTime_s();
		GMSEC_F64 sendTime = msg.getF64Value("SEND-TIME");
		GMSEC_F64 delta    = now - sendTime;

		if (m_excludedSubject == msg.getSubject())
		{
			++m_numExcludedMsgs;

			m_test.check("Excluded Message arrived significantly late", delta <= m_rcvTimeThreshold);

			GMSEC_VERBOSE << "Excluded -- delta is: " << delta << "; threshold is: " << m_rcvTimeThreshold;
		}
		else
		{
			++m_numExpectedMsgs;

			m_test.check("Expected Message arrived significantly early", delta > m_rcvTimeThreshold);

			GMSEC_VERBOSE << "Expected -- delta is: " << delta << "; threshold is: " << m_rcvTimeThreshold;
		}
	}

	size_t getNumExcludedMsgs() const
	{
		return m_numExcludedMsgs;
	}

	size_t getNumExpectedMsgs() const
	{
		return m_numExpectedMsgs;
	}

private:
	Test&       m_test;
	std::string m_excludedSubject;
	GMSEC_F64   m_rcvTimeThreshold;
	size_t      m_numExpectedMsgs;
	size_t      m_numExcludedMsgs;
};


void conductTest(Test& test, const Config& config)
{
	size_t      expectedMsgs     = config.getIntegerValue("expected-msgs");
	int         expectedInterval = config.getIntegerValue("expected-interval");
	const char* subject          = config.getValue("subject");

	StdUniquePtr<Connection> conn;
	BurstCallback bcb(test, expectedMsgs, expectedInterval);

	try
	{
		conn.reset( new Connection(config) );

		conn->connect();

		conn->subscribe(subject, &bcb);

		conn->startAutoDispatch();

		// Allow for time for the auto-dispatch thread to run
		TimeUtil::millisleep(2000);

		Message msg;
		msg.setSubject(subject);

		for (int i = 0; i < 60; ++i)
		{
			msg.addField("COUNT", static_cast<GMSEC_U32>(i+1));

			conn->publish(msg);

			GMSEC_VERBOSE << "Published Message: " << msg.getSubject();

			// delay for 1 second, and then some;
			// this assists testing by ensuring that
			// at most X messages are binned in a
			// defined period (e.g. no more than
			// 5 messages in a 5 second window).
			TimeUtil::millisleep(1050);
		}
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << "Exception: " << e.what();
		test.require(e.what(), false);
	}

	// Allow time for bins to be emptied of messages
	TimeUtil::millisleep((expectedInterval * 1000) + ((expectedInterval / 2) * 1000));

	if (conn.get())
	{
		conn->stopAutoDispatch(true);
		conn->disconnect();
	}
}


void test_CfgMsgBinSize(Test& test)
{
	GMSEC_INFO << "Test Configurable Message Bin: Using Size";

	Config config = test.getConfig();
	config.addValue("gmsec-use-msg-bins",    "true");
	config.addValue("gmsec-msg-bin-size",    "5");
	config.addValue("gmsec-msg-bin-timeout", "60000");
	config.addValue("expected-msgs",         "5");
	config.addValue("expected-interval",     "5");
	config.addValue("subject",               test.getSubject("BIN-SIZE").c_str());

	conductTest(test, config);
}


void test_CfgMsgBinTimeout(Test& test)
{
	GMSEC_INFO << "Test Configurable Message Bin: Using timeout";

	std::string subject = test.getSubject();

	Config config = test.getConfig();
	config.addValue("gmsec-use-msg-bins",    "true");
	config.addValue("gmsec-msg-bin-size",    "5000");
	config.addValue("gmsec-msg-bin-timeout", "10000");
	config.addValue("expected-msgs",         "10");
	config.addValue("expected-interval",     "10");
	config.addValue("subject",               test.getSubject("BIN-TIMEOUT").c_str());

	conductTest(test, config);
}


void test_CfgMsgBinSubjects(Test& test)
{
	GMSEC_INFO << "Test Configurable Message Bin: Only Specific Messages [JIRA 4699]";

	std::vector<std::string> subscribeTo;

	std::string mwID = test.getConfig().getValue("mw-id", "unknown");

	if (mwID.find("dds") != std::string::npos)
	{
		subscribeTo.push_back(test.getSubject("FOOBAR.BINNED1.PUBLISH"));
		subscribeTo.push_back(test.getSubject("FOOBAR.BINNED2.PUBLISH"));
		subscribeTo.push_back(test.getSubject("FOOBAR.NOT-BINNED.PUBLISH"));
		subscribeTo.push_back(test.getSubject("FOOBAR.NOT-BINNED.OTHER"));
	}
	else
	{
		subscribeTo.push_back(test.getSubject("FOOBAR.>"));
	}

	std::string binTopic1 = test.getSubject("FOOBAR.*.PUBLISH");
	std::string msgTopic1 = test.getSubject("FOOBAR.BINNED1.PUBLISH");
	std::string msgTopic2 = test.getSubject("FOOBAR.BINNED2.PUBLISH");
	std::string msgTopic3 = test.getSubject("FOOBAR.NOT-BINNED.PUBLISH");
	std::string msgTopic4 = test.getSubject("FOOBAR.NOT-BINNED.OTHER");

	Config config = test.getConfig();
	config.addValue("gmsec-use-msg-bins",    "true");
	config.addValue("gmsec-msg-bin-size",    "100");
	config.addValue("gmsec-msg-bin-timeout", "15000");

	config.addValue("gmsec-msg-bin-subject-1", binTopic1.c_str());
	config.addValue("gmsec-msg-bin-exclude-subject-1", msgTopic3.c_str());

	std::set<std::string> excludedSubjects;
	excludedSubjects.insert(msgTopic3);
	excludedSubjects.insert(msgTopic4);

	ExpectedMsgCallback ecb(test, excludedSubjects, 2000);

	StdUniquePtr<Connection> conn;

	try
	{
		conn.reset( new Connection(config) );

		conn->connect();

		for (std::vector<std::string>::iterator it = subscribeTo.begin(); it != subscribeTo.end(); ++it)
		{
			conn->subscribe((*it).c_str(), &ecb);
		}

		conn->startAutoDispatch();

		Message expected1, expected2, excluded1, excluded2;

		expected1.setSubject( msgTopic1.c_str() );
		expected2.setSubject( msgTopic2.c_str() );
		excluded1.setSubject( msgTopic3.c_str() );
		excluded2.setSubject( msgTopic4.c_str() );

		const size_t numMsgsToPublish = 10;

		for (size_t i = 0; i < numMsgsToPublish; ++i)
		{
			F64Field sendTime("SEND-TIME", static_cast<GMSEC_F64>(TimeUtil::getCurrentTime_s()));

			expected1.addField(sendTime);
			expected2.addField(sendTime);
			excluded1.addField(sendTime);
			excluded2.addField(sendTime);

			conn->publish(expected1);   // message will be binned (verified in callback)
			conn->publish(expected2);   // message will be binned (verified in callback)
			conn->publish(excluded1);   // message will NOT be binned (verified in callback)
			conn->publish(excluded2);   // message will NOT be binned (verified in callback)

			TimeUtil::millisleep(1000);
		}

		TimeUtil::millisleep(20000);

		conn->disconnect();

		test.check("Failed to receive all expected messages", ecb.getNumExpectedMsgs() == numMsgsToPublish*2);
		test.check("Failed to receive all excluded messages", ecb.getNumExcludedMsgs() == numMsgsToPublish*2);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_CfgMsgBinExcludeSubjects(Test& test)
{
	GMSEC_INFO << "Test Configurable Message Bin: Exclude Specific Messages [JIRA 4509]";

	std::vector<std::string> subscribeTo;

	std::string mwID = test.getConfig().getValue("mw-id", "unknown");

	if (mwID.find("dds") != std::string::npos)
	{
		subscribeTo.push_back(test.getSubject("FOOBAR.EXPECTED1.PUBLISH"));
		subscribeTo.push_back(test.getSubject("FOOBAR.EXPECTED2.PUBLISH"));
		subscribeTo.push_back(test.getSubject("FOOBAR.EXCLUDED.PUBLISH"));
	}
	else
	{
		subscribeTo.push_back(test.getSubject("FOOBAR.>"));
	}

	std::string msgTopic1 = test.getSubject("FOOBAR.EXPECTED1.PUBLISH");
	std::string msgTopic2 = test.getSubject("FOOBAR.EXPECTED2.PUBLISH");
	std::string msgTopic3 = test.getSubject("FOOBAR.EXCLUDED.PUBLISH");

	Config config = test.getConfig();
	config.addValue("gmsec-use-msg-bins",    "true");
	config.addValue("gmsec-msg-bin-size",    "100");
	config.addValue("gmsec-msg-bin-timeout", "15000");

	config.addValue("gmsec-msg-bin-exclude-subject-1", msgTopic3.c_str());

	ExcludeMsgCallback ecb(test, msgTopic3, 2000);

	StdUniquePtr<Connection> conn;

	try
	{
		conn.reset( new Connection(config) );

		conn->connect();

		for (std::vector<std::string>::iterator it = subscribeTo.begin(); it != subscribeTo.end(); ++it)
		{
			conn->subscribe((*it).c_str(), &ecb);
		}

		conn->startAutoDispatch();

		Message expected1, expected2, excluded1;

		expected1.setSubject( msgTopic1.c_str() );
		expected2.setSubject( msgTopic2.c_str() );
		excluded1.setSubject( msgTopic3.c_str() );

		const size_t numMsgsToPublish = 10;

		for (size_t i = 0; i < numMsgsToPublish; ++i)
		{
			F64Field sendTime("SEND-TIME", static_cast<GMSEC_F64>(TimeUtil::getCurrentTime_s()));

			expected1.addField(sendTime);
			expected2.addField(sendTime);
			excluded1.addField(sendTime);

			conn->publish(expected1);   // message will be binned (verified in callback)
			conn->publish(expected2);   // message will be binned (verified in callback)
			conn->publish(excluded1);   // message will NOT be binned (verified in callback)

			TimeUtil::millisleep(1000);
		}

		TimeUtil::millisleep(10000);

		conn->disconnect();

		test.check("Failed to receive all expected messages", ecb.getNumExpectedMsgs() == numMsgsToPublish*2);
		test.check("Failed to receive all excluded messages", ecb.getNumExcludedMsgs() == numMsgsToPublish);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_CfgMsgBinUsingValidation(Test& test)
{
	GMSEC_INFO << "Test Configurable Message Bin: Message Validation Enabled";

	Config pubConfig = test.getConfig();  // make a copy!
	pubConfig.addValue("gmsec-use-msg-bins", "true");
	pubConfig.addValue("gmsec-msg-bin-size", "10");
	pubConfig.addValue("gmsec-msg-bin-timeout", "5000");
	pubConfig.addValue("gmsec-msg-content-validate-send", "true");

	Config subConfig = test.getConfig();  // make a copy!
	subConfig.addValue("gmsec-msg-content-validate-recv", "true");

	std::string node;
	SystemUtil::getHostName(node);
	node = StringUtil::stringToUpper(node);

	StdUniquePtr<Connection> pubConn;
	StdUniquePtr<Connection> subConn;

	try
	{
		pubConn.reset( new Connection(pubConfig) );
		subConn.reset( new Connection(subConfig) );

		set_standard_fields( pubConn->getMessageFactory() );

		pubConn->connect();
		subConn->connect();

		Message msg = pubConn->getMessageFactory().createMessage("HB");

		// subscribe to the message's subject
		subConn->subscribe( msg.getSubject() );

		// publish as quickly as possible
		for (int i = 0; i < 5; ++i)
		{
			msg.setFieldValue("COUNTER", static_cast<GMSEC_I32>(i+1));

			pubConn->publish(msg);
		}

		for (int i = 0; i < 5; ++i)
		{
			Message* rcv = subConn->receive( pubConfig.getIntegerValue("gmsec-msg-bin-timeout") + 1000 );

			if (rcv)
			{
				if (node != rcv->getStringValue("NODE"))
				{
					// ignore messages not found this node (i.e., hostname)
					--i;
					continue;
				}

				GMSEC_VERBOSE << "COUNTER is: " << rcv->getI32Value("COUNTER");

				test.check("Unexpected COUNTER value", rcv->getI32Value("COUNTER") == (i+1));

				Message::destroy(rcv);
			}
			else
			{
				test.check("Failed to receive message", false);
			}
		}

		pubConn->disconnect();
		subConn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}
