#include "TestCase.h"

#include <string>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


/**
	+--------+------------+---------------------------------+--------------+------------------+
	|  Test  |  tracking  |  tracking-connection-endpoint   |  HB Message  |  ConnEndPntFld?  |
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

void runTest(int testNum, Test& test, State trackingState, State connEndPntState, bool makeHeartbeatMsg, bool expectConnEndPntField);

void doTest(int testNum, Test& test, const Config& config, bool makeHeartbeatMsg, bool expectConnEndPnt);


static const bool USE_HB_MSG            = true;
static const bool HAS_CONN_ENDPNT_FIELD = true;

int test_ConnectionEndpoint(Test& test)
{
    test.setDescription("Connection Endpoint");

	// This is an extensive unit test. Will need to speed up acquisition
	// of messages when working with the IBM MQ m/w.
	//
	const char* mwID = test.getConfig().getValue("mw-id");

	if (mwID && strcontains(mwID, "ibmmq"))
	{
		test.getConfig().addValue(IBM_MESSAGE_WAIT_PERIOD, "100");
	}

	const char* server = test.getConfig().getValue("server");

	if (!server && mwID)
	{
		if (mwID && (strcontains(mwID, "activemq") || strcontains(mwID, "artemis")))
		{
			test.getConfig().addValue("server", "tcp://localhost:61616");
		}
		else if (mwID && strcontains(mwID, "ibmmq"))
		{
			test.getConfig().addValue("server", "localhost");
			test.getConfig().addValue("mw-channel", "GMSEC.TEST.CHANNEL");
		}
		else
		{
			test.getConfig().addValue("server", "localhost");
		}
	}

	runTest( 1, test, UNSET, UNSET, !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest( 2, test, UNSET, UNSET,  USE_HB_MSG,  HAS_CONN_ENDPNT_FIELD);
	runTest( 3, test, UNSET, OFF,   !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest( 4, test, UNSET, OFF,    USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest( 5, test, UNSET, ON,    !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest( 6, test, UNSET, ON,     USE_HB_MSG,  HAS_CONN_ENDPNT_FIELD);

	runTest( 7, test, OFF,   UNSET, !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest( 8, test, OFF,   UNSET,  USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest( 9, test, OFF,   OFF,   !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest(10, test, OFF,   OFF,    USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest(11, test, OFF,   ON,    !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest(12, test, OFF,   ON,     USE_HB_MSG,  HAS_CONN_ENDPNT_FIELD);

	runTest(13, test, ON,    UNSET, !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest(14, test, ON,    UNSET,  USE_HB_MSG,  HAS_CONN_ENDPNT_FIELD);
	runTest(15, test, ON,    OFF,   !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest(16, test, ON,    OFF,    USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest(17, test, ON,    ON,    !USE_HB_MSG, !HAS_CONN_ENDPNT_FIELD);
	runTest(18, test, ON,    ON,     USE_HB_MSG,  HAS_CONN_ENDPNT_FIELD);

	return 0;
}
TEST_DRIVER(test_ConnectionEndpoint);


void runTest(int testNum, Test& test, State trackingState, State connEndPntState, bool makeHeartbeatMsg, bool expectConnEndPntField)
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
	switch (connEndPntState)
	{
		case UNSET:
			break;
		case OFF:
			config.addValue("tracking-connection-endpoint", "false");
			break;
		case ON:
			config.addValue("tracking-connection-endpoint", "true");
			break;
	}

	doTest(testNum, test, config, makeHeartbeatMsg, expectConnEndPntField);
}


void doTest(int testNum, Test& test, const Config& config, bool makeHeartbeatMsg, bool expectConnEndPntField)
{
	std::ostringstream oss;
	oss << testNum << "-";

	std::string subject = test.getSubject(oss.str());

	Message msg;
	msg.setSubject( subject.c_str() );

	if (makeHeartbeatMsg)
	{
		msg.addField("MESSAGE-TYPE",    "MSG");
		msg.addField("MESSAGE-SUBTYPE", "HB");
	}

	StdUniquePtr<Connection> conn;

	try
	{
		conn.reset( new Connection(config) );

		conn->connect();

		conn->subscribe(subject.c_str());

		conn->publish(msg);

		Message* rcvd = conn->receive(4000);

		if (rcvd)
		{
			GMSEC_DEBUG << "Received:\n" << rcvd->toXML();

			if (expectConnEndPntField)
			{
				test.check("MW-CONNECTION-ENDPOINT missing from message", rcvd->hasField("MW-CONNECTION-ENDPOINT"));
			}
			else
			{
				test.check("MW-CONNECTION-ENDPOINT when not expected", rcvd->hasField("MW-CONNECTION-ENDPOINT") == false);
			}

			Message::destroy(rcvd);
		}
		else
		{
			test.check("Failed to receive message", false);
		}

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}
