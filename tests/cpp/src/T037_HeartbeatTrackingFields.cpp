#include "TestCase.h"

#include <string>
#include <sstream>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


/**
	+--------+------------+---------------------------------+--------------+------------------+
	|  Test  |  tracking  |  tracking-active-subscriptions  |  HB Message  |  ActiveSubFld?   |
	+--------+------------+---------------------------------+--------------+------------------+
	|  1     |  UNSET     |  UNSET                          |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  2     |  UNSET     |  UNSET                          |  Yes         |  Yes             |
	+--------+------------+---------------------------------+--------------+------------------+
	|  3     |  UNSET     |  OFF                            |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  4     |  UNSET     |  OFF                            |  Yes         |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  5     |  UNSET     |  ON                             |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  6     |  UNSET     |  ON                             |  Yes         |  Yes             |
	+--------+------------+---------------------------------+--------------+------------------+
	|  7     |  OFF       |  UNSET                          |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  8     |  OFF       |  UNSET                          |  Yes         |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  9     |  OFF       |  OFF                            |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  10    |  OFF       |  OFF                            |  Yes         |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  11    |  OFF       |  ON                             |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  12    |  OFF       |  ON                             |  Yes         |  Yes             |
	+--------+------------+---------------------------------+--------------+------------------+
	|  13    |  ON        |  UNSET                          |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  14    |  ON        |  UNSET                          |  Yes         |  Yes             |
	+--------+------------+---------------------------------+--------------+------------------+
	|  15    |  ON        |  OFF                            |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  16    |  ON        |  OFF                            |  Yes         |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  17    |  ON        |  ON                             |  No          |  No              |
	+--------+------------+---------------------------------+--------------+------------------+
	|  18    |  ON        |  ON                             |  Yes         |  Yes             |
	+--------+------------+---------------------------------+--------------+------------------+
*/


enum State { UNSET, OFF, ON };

void runTest(int testNum, Test& test, State trackingState, State activeSubState, bool makeHeartbeatMsg, bool expectActiveSubsField);

void doTest(int testNum, Test& test, const Config& config, bool makeHeartbeatMsg, bool expectActiveSubs);


static const bool USE_HB_MSG           = true;
static const bool HAS_ACTIVE_SUB_FIELD = true;


int test_HeartbeatMsgTrackingFields(Test& test)
{
    test.setDescription("Heartbeat Tracking Fields");

	// This is an extensive unit test. Will need to speed up acquisition
	// of messages when working with the IBM MQ m/w.
	//
	const char* mwID = test.getConfig().getValue("mw-id");

	if (mwID && strcontains(mwID, "ibmmq"))
	{
		test.getConfig().addValue(IBM_MESSAGE_WAIT_PERIOD, "100");
	}

	runTest( 1, test, UNSET, UNSET, !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest( 2, test, UNSET, UNSET,  USE_HB_MSG,  HAS_ACTIVE_SUB_FIELD);
	runTest( 3, test, UNSET, OFF,   !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest( 4, test, UNSET, OFF,    USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest( 5, test, UNSET, ON,    !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest( 6, test, UNSET, ON,     USE_HB_MSG,  HAS_ACTIVE_SUB_FIELD);

	runTest( 7, test, OFF,   UNSET, !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest( 8, test, OFF,   UNSET,  USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest( 9, test, OFF,   OFF,   !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest(10, test, OFF,   OFF,    USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest(11, test, OFF,   ON,    !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest(12, test, OFF,   ON,     USE_HB_MSG,  HAS_ACTIVE_SUB_FIELD);

	runTest(13, test, ON,    UNSET, !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest(14, test, ON,    UNSET,  USE_HB_MSG,  HAS_ACTIVE_SUB_FIELD);
	runTest(15, test, ON,    OFF,   !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest(16, test, ON,    OFF,    USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest(17, test, ON,    ON,    !USE_HB_MSG, !HAS_ACTIVE_SUB_FIELD);
	runTest(18, test, ON,    ON,     USE_HB_MSG,  HAS_ACTIVE_SUB_FIELD);

	return 0;
}
TEST_DRIVER(test_HeartbeatMsgTrackingFields);


void runTest(int testNum, Test& test, State trackingState, State activeSubState, bool makeHeartbeatMsg, bool expectActiveSubsField)
{
	GMSEC_INFO << "Conducting Test #" << testNum;

	Config config = test.getConfig();   // make a copy!!

	switch (trackingState)
	{
		case UNSET:
			break;
		case OFF:
			config.addValue("tracking", "false");
			break;
		case ON:
			config.addValue("tracking", "true");
			break;
	}
	switch (activeSubState)
	{
		case UNSET:
			break;
		case OFF:
			config.addValue("tracking-active-subscriptions", "false");
			break;
		case ON:
			config.addValue("tracking-active-subscriptions", "true");
			break;
	}

	doTest(testNum, test, config, makeHeartbeatMsg, expectActiveSubsField);
}


void doTest(int testNum, Test& test, const Config& config, bool makeHeartbeatMsg, bool expectActiveSubsField)
{
	std::ostringstream oss;
	oss << testNum << "-";

	std::string subject1 = test.getSubject(oss.str() + "A");
	std::string subject2 = test.getSubject(oss.str() + "B");
	std::string subject3 = test.getSubject(oss.str() + "C");
	std::string subject4 = test.getSubject(oss.str() + "D");

	Message msg;
	msg.setSubject( subject1.c_str() );

	if (makeHeartbeatMsg)
	{
		msg.addField("MESSAGE-TYPE",    "MSG");
		msg.addField("MESSAGE-SUBTYPE", "HB");
	}

	StdUniquePtr<Connection> conn1;
	StdUniquePtr<Connection> conn2;

	try
	{
		conn1.reset( new Connection(config) );
		conn2.reset( new Connection(config) );

		conn1->connect();
		conn2->connect();

		conn1->subscribe(subject1.c_str());
		conn1->subscribe(subject2.c_str());

		SubscriptionInfo* info1 = conn2->subscribe(subject1.c_str());
		conn2->subscribe(subject2.c_str());
		conn2->subscribe(subject3.c_str());
		SubscriptionInfo* info4 = conn2->subscribe(subject4.c_str());

		conn2->unsubscribe(info1);
		conn2->unsubscribe(info4);

		conn1->publish(msg);

		Message* rcvd = conn1->receive(4000);
		if (rcvd)
		{
			GMSEC_DEBUG << "Received:\n" << rcvd->toXML();

			if (expectActiveSubsField)
			{
				try
				{
					GMSEC_U16 numActiveSubs = static_cast<GMSEC_U16>( rcvd->getU32Value("NUM-OF-SUBSCRIPTIONS") );

					test.check("Did NOT received expected number of active subscriptions", numActiveSubs == 3);

					for (GMSEC_U16 n = 0; n < numActiveSubs; ++n)
					{
						std::ostringstream fieldName;
						fieldName << "SUBSCRIPTION." << (n+1) << ".SUBJECT-PATTERN";

						const char* pattern = rcvd->getStringValue(fieldName.str().c_str());

						GMSEC_DEBUG << "Got subscription pattern: " << pattern;

						if ((subject1 != pattern) && (subject2 != pattern) && (subject3 != pattern))
						{
							test.check("Did NOT received expected active subscriptions", false);
						}
					}
				}
				catch (const GmsecException& e)
				{
					test.check(e.what(), false);
				}
			}
			else
			{
				GMSEC_DEBUG << "No active subscriptions expected";
				const Field* numSubs = rcvd->getField("NUM-OF-SUBSCRIPTIONS");
				test.check("Received active subscriptions when none expected", numSubs == NULL);
			}

			Message::destroy(rcvd);
		}
		else
		{
			test.check("Failed to receive message", false);
		}

		conn1->disconnect();
		conn2->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}
