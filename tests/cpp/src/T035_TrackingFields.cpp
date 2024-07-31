#include "TestCase.h"

#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/TrackingDetails.h>

#include <string>
#include <vector>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


Message createMessage(MessageFactory& factory, const std::string& subject, bool includeTrackingFields = false);


static const char* trackingOptions[] =
{
	GMSEC_TRACKING_NODE,
	GMSEC_TRACKING_PROCESS_ID,
	GMSEC_TRACKING_USERNAME,
	GMSEC_TRACKING_CONNECTION_ID,
	GMSEC_TRACKING_PUBLISH_TIME,
	GMSEC_TRACKING_UNIQUE_ID,
	GMSEC_TRACKING_MW_INFO,
	GMSEC_TRACKING_ACTIVE_SUBSCRIPTIONS,
	GMSEC_TRACKING_CONNECTION_ENDPOINT,
	GMSEC_TRACKING,
	NULL
};


void runTest(Test& test, unsigned int version)
{
	GMSEC_INFO << "Test Verify Tracking Fields for Specification " << version;

	std::ostringstream oss;
	oss << version;

	Config config1 = test.getConfig();  // make a copy!
	config1.addValue("gmsec-specification-version", oss.str().c_str());

	Config config2 = test.getConfig();  // make a copy!
	config2.addValue("gmsec-specification-version", oss.str().c_str());
	config2.addValue("gmsec-msg-content-validate-all", "true");

	StdUniquePtr<Connection> conn;

	try
	{
		conn.reset(new Connection(config1));

		conn->connect();

		conn->subscribe(test.getSubject().c_str());

		//o Publish message with no tracking fields
		//
		try
		{
			Message msg = createMessage(conn->getMessageFactory(), test.getSubject(), false);

			conn->publish(msg);

			test.check("Successfully published message w/ no tracking fields", true);

			Message* rcvd = conn->receive(10000);

			if (rcvd)
			{
				test.check("Successfully received message", true);

				Message::destroy(rcvd);
			}
			else
			{
				test.check("Failed to received message", false);
			}
		}
		catch (const GmsecException& e)
		{
			test.check(e.what(), false);
		}
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}


	try
	{
		conn.reset(new Connection(config2));

		conn->connect();

		conn->subscribe(test.getSubject().c_str());

		//o Attempt to publish message with tracking fields
		try
		{
			Message msg = createMessage(conn->getMessageFactory(), "", true);

			GMSEC_VERBOSE << "Attempting to publish:\n" << msg.toXML();

			conn->publish(msg);

			test.check("Should not be able to publish message w/ tracking fields", false);
		}
		catch (const GmsecException& e)
		{
			test.check(e.what(), true);

			std::string error = e.what();

			GMSEC_VERBOSE << "Expected Error is:\n" << error.c_str();

			bool hasConnID      = (error.find("CONNECTION-ID") != std::string::npos);
			bool hasMwInfo      = (error.find("MW-INFO") != std::string::npos);
			bool hasNode        = (error.find("NODE") != std::string::npos);
			bool hasProcID      = (error.find("PROCESS-ID") != std::string::npos);
			bool hasPubTime     = (error.find("PUBLISH-TIME") != std::string::npos);
			bool hasUniqID      = (error.find("UNIQUE-ID") != std::string::npos);
			bool hasUserName    = (error.find("USER-NAME") != std::string::npos);
			bool hasConnEndPnt  = (error.find("MW-CONNECTION-ENDPOINT") != std::string::npos);
			bool hasNumSubs     = (error.find("NUM-OF-SUBSCRIPTIONS") != std::string::npos);
			bool hasSubPattern1 = (error.find("SUBSCRIPTION.1.SUBJECT-PATTERN") != std::string::npos);

			test.check("Missing CONNECTION-ID error", hasConnID);
			test.check("Missing MW-INFO error", hasMwInfo);
			test.check("Missing NODE error", hasNode);
			test.check("Missing PROCESS-ID error", hasProcID);
			test.check("Missing PUBLISH-TIME error", hasPubTime);
			test.check("Missing UNIQUE-ID error", hasUniqID);
			test.check("Missing USER-NAME error", hasUserName);

			test.check("Missing MW-CONNECTION-ENDPOINT error", hasConnEndPnt);
			test.check("Missing NUM-OF-SUBSCRIPTIONS error", hasNumSubs);
			test.check("Missing SUBSCRIPTION.1.SUBJECT-PATTERN error", hasSubPattern1);
		}
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}


	try
	{
		conn.reset(new Connection(config1));

		conn->connect();

		conn->subscribe(test.getSubject().c_str());

		//o Attempt to publish message with tracking fields, but allow for such
		Config msgConfig;

		for (size_t i = 0; trackingOptions[i] != NULL; ++i)
		{
			msgConfig.addValue(trackingOptions[i], "false");

			Message msg = createMessage(conn->getMessageFactory(), test.getSubject(), true);

			msg.setConfig(msgConfig);

			try
			{
				GMSEC_VERBOSE << "[" << i << "] Attempting to publish message:\n" << msg.toXML();

				conn->publish(msg);

				Message* rcvd = conn->receive(10000);

				if (rcvd)
				{
					test.check("Successfully received message", true);

					Message::destroy(rcvd);
				}
				else
				{
					test.check("Failed to received message", false);
				}
			}
			catch (const GmsecException& e)
			{
				test.check(e.what(), true);
			}
		}

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}


void test_remove_tracking_fields(Test& test)
{
	GMSEC_INFO << "Testing gmsec-remove-tracking-fields=true";

	Config& config1 = test.getConfig();
	Config  config2 = test.getConfig();  // make a copy
	config2.addValue("gmsec-remove-tracking-fields", "true");
	config2.addValue("gmsec-msg-content-validate-send", "true");

	StdUniquePtr<Connection> conn1;
	StdUniquePtr<Connection> conn2;

	try
	{
		conn1.reset( new Connection(config1) );
		conn2.reset( new Connection(config2) );

		set_standard_fields(conn1->getMessageFactory());
		set_standard_fields(conn2->getMessageFactory());

		conn1->connect();
		conn2->connect();

		// Subscribe
		conn2->subscribe("C2MS.*.*.*.*.*.MSG.HB.>");

		// Create new message
		Message msg1 = conn1->getMessageFactory().createMessage("HB");

		// Publish message (API adds tracking fields)
		conn1->publish(msg1);

		// Received message; attempt to republish it
		Message* msg2 = conn2->receive(5000);

		if (msg2)
		{
			try
			{
				conn2->publish(*msg2);
				test.check("Okay", true);
			}
			catch (const GmsecException& e)
			{
				test.check(e.what(), false);
			}

			Message::destroy(msg2);
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
		test.check(e.what(), false);
	}
}


void verifyToString(Test& test, int m)
{
	std::string setting ;
	if (m == MESSAGE_TRACKINGFIELDS_ON) {
		setting = "ON";
	}
	else if (m == MESSAGE_TRACKINGFIELDS_OFF) {
		setting = "OFF";
	}
	else {
		setting = "UNSET";
	}

	TrackingDetails td;

	td.set(m);
	td.setNode(m);
	td.setProcessId(m);
	td.setUserName(m);
	td.setConnectionId(m);
	td.setPublishTime(m);
	td.setUniqueId(m);
	td.setMwInfo(m);
	td.setActiveSubscriptions(m);
	td.setConnectionEndpoint(m);

	std::string str = td.toString();
	std::vector<std::string> lines = StringUtil::split(str, "\n");

	test.check("Unexpected toString", lines.size() == 10);

	for (size_t i = 0; trackingOptions[i] != NULL; ++i) {
		std::string option = trackingOptions[i];
		if (option.find("TRACKING-") != std::string::npos) {
			option = option.substr(9);
		}
		for (size_t j = 0; j < lines.size(); ++j) {
			if (lines[j].find(option) != std::string::npos) {
				test.check("Unexpected result", lines[j].find(setting) != std::string::npos);
			}
		}
	}
}


void test_to_string(Test& test)
{
	GMSEC_INFO << "Test TrackingDetails::toString()";

	verifyToString(test, MESSAGE_TRACKINGFIELDS_UNSET);
	verifyToString(test, MESSAGE_TRACKINGFIELDS_ON);
	verifyToString(test, MESSAGE_TRACKINGFIELDS_OFF);
}


int test_TrackingFields(Test& test)
{
	test.setDescription("Tracking Fields");

    runTest(test, GMSEC_MSG_SPEC_2019_00);
    runTest(test, GMSEC_MSG_SPEC_CURRENT);

	test_remove_tracking_fields(test);
	test_to_string(test);

    return 0;
}
TEST_DRIVER(test_TrackingFields)


Message createMessage(MessageFactory& factory, const std::string& subject, bool includeTrackingFields)
{
	Message msg = factory.createMessage("HB");

	if (!subject.empty()) {
		msg.setSubject( subject.c_str() );
	}

	msg.setFieldValue("MISSION-ID", "MY-MISSION");
	msg.setFieldValue("CONSTELLATION-ID", "MY-CONSTELLATION-ID");
	msg.setFieldValue("SAT-ID-PHYSICAL", "MY-SAT-ID");
	msg.setFieldValue("SAT-ID-LOGICAL", "MY-SAT-ID");
	msg.setFieldValue("FACILITY", "MY-FACILITY");
	msg.setFieldValue("COMPONENT", "T035");
	msg.setFieldValue("PUB-RATE", "5");
	msg.setFieldValue("DOMAIN1", "MY-DOMAIN-1");
	msg.setFieldValue("DOMAIN2", "MY-DOMAIN-2");

	if (includeTrackingFields)
	{
		msg.setFieldValue("CONNECTION-ID", "1");
		msg.setFieldValue("MW-INFO", "BAD-MW-INFO");
		msg.setFieldValue("NODE", "BAD-NODE");
		msg.setFieldValue("PROCESS-ID", "2");
		msg.setFieldValue("PUBLISH-TIME", "BAD-PUBLISH-TIME");
		msg.setFieldValue("UNIQUE-ID", "BAD-UNIQUE-ID");
		msg.setFieldValue("USER-NAME", "BAD-USER-NAME");
		msg.setFieldValue("MW-CONNECTION-ENDPOINT", "BAD-MW-CONNECTION-ENDPOINT");
		msg.setFieldValue("NUM-OF-SUBSCRIPTIONS", "1");
		msg.setFieldValue("SUBSCRIPTION.1.SUBJECT-PATTERN", "BAD-TOPIC-1");
	}

	return msg;
}
