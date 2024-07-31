#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void verify_heartbeat_message(Test& test, const Config& config, GMSEC_U16 expectedPubRate)
{
	std::stringstream topic;
	topic << "C2MS.*.*.*.*.*.MSG.HB." << test.getUniqueComponent() << ".+";

	try
	{
		Connection conn(config);
		conn.connect();
		conn.subscribe(topic.str().c_str());

		double t1 = 0, t2;

		for (int i = 0; i < 7; ++i)
		{
			Message* hbmsg = conn.receive(5000);

			// ignore the first few incoming messages (if any)
			if (i < 3)
			{
				if (hbmsg != NULL) Message::destroy(hbmsg);
				continue;
			}

			if (hbmsg)
			{
				if (t1 == 0)
				{
					t1 = TimeUtil::getCurrentTime_s();
				}
				else
				{
					t2 = TimeUtil::getCurrentTime_s();

					double delta = t2 - t1;
					if (delta < expectedPubRate)
					{
						delta = ((t2 - t1) * 10.0 + 0.5) / 10.0;
					}

					GMSEC_INFO << "Expected rate is: " << expectedPubRate << ", delta is: " << delta;

					test.check("Unexpected publish rate", expectedPubRate == static_cast<GMSEC_U16>(delta));

					t1 = t2;
				}

				test.check("Unexpected MESSAGE-TYPE", std::string("MSG") == hbmsg->getStringValue("MESSAGE-TYPE"));
				test.check("Unexpected MESSAGE-SUBTYPE", std::string("HB") == hbmsg->getStringValue("MESSAGE-SUBTYPE"));
				test.check("Unexpected PUB-RATE", expectedPubRate == static_cast<GMSEC_U16>(hbmsg->getI32Value("PUB-RATE")));

				if (hbmsg->hasField("CUSTOM-FIELD"))
				{
					// Verify the CUSTOM-FIELD has the correct field type
					test.check("Unexpected CUSTOM-FIELD type", hbmsg->getField("CUSTOM-FIELD")->getType() == Field::Type::I16);
				}

				Message::destroy(hbmsg);
			}
			else
			{
				test.check("Failed to received Heartbeat Message", false);
			}
		}

		conn.disconnect();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << "verify_heartbeat_message: " << e.what();
	}
}


void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Test constructor 1...";

	Config config(test.getConfig());  // make a copy!
	config.addValue("gmsec-msg-content-validate", "true");

	HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(5));

	hbgen.setField("MISSION-ID", "MY-MISSION");
	hbgen.setField("SAT-ID-PHYSICAL", "MY-SAT-ID");
	hbgen.setField("COMPONENT", "HB-GEN");
	hbgen.setField("BOGUS", "FOOBAR");

	test.check("HeartbeatGenerator should not be running", false == hbgen.isRunning());

	// Off-nominal test(s)
	try
	{
		// Bogus middleware
		config.addValue("mw-id", "bogus-mw");
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(5));
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Unable to load") != std::string::npos);
	}
}


void test_constructor_2(Test& test)
{
	GMSEC_INFO << "Test constructor 2...";

	Config config(test.getConfig());  // make a copy!
	config.addValue("gmsec-msg-content-validate", "true");

	List<Field*> standardFields;
	List<Field*> emptyFieldList;

	get_standard_fields(standardFields);

	// Nominal test -- no extra fields
	HeartbeatGenerator hbgen1(config, static_cast<GMSEC_U16>(5), emptyFieldList);
	hbgen1.setField("MISSION-ID", "MY-MISSION");
	hbgen1.setField("SAT-ID-PHYSICAL", "MY-SAT-ID");
	hbgen1.setField("COMPONENT", "HB-GEN");
	hbgen1.setField("BOGUS", "FOOBAR");
	test.check("HeartbeatGenerator should not be running", false == hbgen1.isRunning());

	// Nominal test -- with extra fields
	HeartbeatGenerator hbgen2(config, static_cast<GMSEC_U16>(5), standardFields);
	hbgen2.setField("BOGUS", "FOOBAR");
	test.check("HeartbeatGenerator should not be running", false == hbgen2.isRunning());

	// Off-nominal test(s)
	try
	{
		// Bogus middleware
		config.addValue("mw-id", "bogus-mw");
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(5));
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Unable to load") != std::string::npos);
	}

	// Cleanup
	destroy_standard_fields(standardFields);
}


void test_start(Test& test)
{
	GMSEC_INFO << "Test start()...";

	Config config = test.getConfig();  // make a copy!

	List<Field*> fields;
	get_standard_fields(fields);

	// Add custom field
	fields.push_back( new I16Field("CUSTOM-FIELD", static_cast<GMSEC_I16>(3)) );

	try
	{   // Have all standard fields, and custom field, and message validation is not enabled
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), fields);

		/* To ensure this heartbeat message is unique, we set the COMPONENT field */
		hbgen.setField("COMPONENT", test.getUniqueComponent().c_str());

		hbgen.start();

		test.check("HeartbeatGenerator should be running", hbgen.isRunning());

		verify_heartbeat_message(test, config, static_cast<GMSEC_U16>(1));

		hbgen.stop();

		// Allow time for the HB-gen thread to stop
		TimeUtil::millisleep(2000);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal cases
	GMSEC_INFO << "Performing off-nominal tests...";

	config.addValue("gmsec-msg-content-validate", "true");

	//o Add bogus field using a U16Field
	try
	{
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), fields);
		hbgen.setField( U16Field("BOGUS-FIELD", static_cast<GMSEC_U16>(2)) );
		hbgen.start();
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message Validation Failed") != std::string::npos);
	}

	//o Add bogus field using a GMSEC_I64 value
	try
	{
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), fields);
		hbgen.setField("BOGUS-FIELD", static_cast<GMSEC_I64>(2));
		hbgen.start();
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message Validation Failed") != std::string::npos);
	}

	//o Add bogus field using a GMSEC_F64 value
	try
	{
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), fields);
		hbgen.setField("BOGUS-FIELD", static_cast<GMSEC_F64>(2));
		hbgen.start();
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message Validation Failed") != std::string::npos);
	}

	//o Add bogus field using a String value
	try
	{
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), fields);
		hbgen.setField("BOGUS-FIELD", "2");
		hbgen.start();
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message Validation Failed") != std::string::npos);
	}

	//o Add legit field but with illegal value
	try
	{
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), fields);
		hbgen.setField( F32Field("CONTENT-VERSION", static_cast<GMSEC_F32>(1776)) );
		hbgen.start();
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message Validation Failed") != std::string::npos);
	}

	destroy_standard_fields(fields);
}


void test_stop(Test& test)
{
	GMSEC_INFO << "Test stop()...";

	Config& config = test.getConfig();

	List<Field*> standardFields;
	get_standard_fields(standardFields);

	try
	{
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), standardFields);

		/* To ensure this heartbeat message is unique, we set the COMPONENT field */
		hbgen.setField("COMPONENT", test.getUniqueComponent().c_str());

		hbgen.start();

		test.check("HeartbeatGenerator should be running", hbgen.isRunning());

		hbgen.stop();

		test.check("HeartbeatGenerator should have stopped", false == hbgen.isRunning());

		// Allow time for the HB-gen thread to stop
		TimeUtil::millisleep(2000);

		std::stringstream topic;
		topic << "C2MS.*.*.*.*.*.MSG.HB." << test.getUniqueComponent();

		Connection conn(config);
		conn.connect();
		conn.subscribe(topic.str().c_str());

		for (int i = 0; i < 2; ++i)
		{
			Message *hbmsg = conn.receive(5000);

			if (hbmsg)
			{
				test.check("Unexpectedly received a Heartbeat Message", false);

				Message::destroy(hbmsg);
			}
		}

		conn.disconnect();
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	destroy_standard_fields(standardFields);
}


void test_change_publish_rate(Test& test)
{
	GMSEC_INFO << "Test changePublishRate()...";

	Config& config = test.getConfig();

	List<Field*> standardFields;
	get_standard_fields(standardFields);

	try
	{
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), standardFields);

		/* To ensure this heartbeat message is unique, we set the COMPONENT field */
		hbgen.setField("COMPONENT", test.getUniqueComponent().c_str());

		hbgen.start();

		// Verify 2-second publishing of heartbeat messages
		hbgen.changePublishRate(static_cast<GMSEC_U16>(2));

		verify_heartbeat_message(test, config, static_cast<GMSEC_U16>(2));

		// Verify only one heartbeat is published after changing HB-generator's pub-rate to 0.
		std::stringstream topic;
		topic << "C2MS.*.*.*.*.*.MSG.HB." << test.getUniqueComponent();

		Connection conn(config);
		conn.connect();
		conn.subscribe(topic.str().c_str());

		TimeUtil::millisleep(1000);

		hbgen.changePublishRate(static_cast<GMSEC_U16>(0));

		TimeUtil::millisleep(1000);

		bool lastMessage = false;

		for (int i = 0; i < 3 && !lastMessage; ++i)
		{
			Message* hbmsg = conn.receive(5000);

			if (!hbmsg)
			{
				test.check("Failed to receive Heartbeat Message", false);
			}
			else if (hbmsg->getU32Value("PUB-RATE") != 0)
			{
				// ignore message
				Message::destroy(hbmsg);
			}
			else
			{
				// got final message
				lastMessage = true;
				Message::destroy(hbmsg);
			}
		}

		test.check("Fail to receive message with PUB-RATE of 0", lastMessage);

		conn.disconnect();

		hbgen.stop();

		// Allow time for the HB-gen thread to stop
		TimeUtil::millisleep(2000);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	destroy_standard_fields(standardFields);
}


void test_set_field(Test& test)
{
	GMSEC_INFO << "Test setField()...";

	Config config = test.getConfig();   // make a copy

	List<Field*> standardFields;
	get_standard_fields(standardFields);

	try
	{
		HeartbeatGenerator hbgen(config, static_cast<GMSEC_U16>(1), standardFields);

		/* To ensure this heartbeat message is unique, we set the COMPONENT field */
		hbgen.setField("COMPONENT", test.getUniqueComponent().c_str());

		hbgen.start();

		TimeUtil::millisleep(2000);

		// Use a Field
		hbgen.setField(U16Field("PUB-RATE", static_cast<GMSEC_U16>(2)));
		verify_heartbeat_message(test, config, static_cast<GMSEC_U16>(2));

		// Use Field Name and GMSEC_I64 Value
		hbgen.setField("PUB-RATE", static_cast<GMSEC_I64>(3));
		verify_heartbeat_message(test, config, static_cast<GMSEC_U16>(3));

		// Use Field Name and GMSEC_F64 Value
		hbgen.setField("PUB-RATE", static_cast<GMSEC_F64>(2));
		verify_heartbeat_message(test, config, static_cast<GMSEC_U16>(2));

		// Use Field Name and String Value
		hbgen.setField("PUB-RATE", "3");
		verify_heartbeat_message(test, config, static_cast<GMSEC_U16>(3));

		hbgen.stop();

		// Allow time for the HB-gen thread to stop
		TimeUtil::millisleep(2000);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	destroy_standard_fields(standardFields);
}


int test_HeartbeatGenerator(Test& test)
{
	test.setDescription("Test HeartbeatGenerator");

	try
	{
		test_constructor_1(test);
		test_constructor_2(test);
		test_start(test);
		test_stop(test);
		test_change_publish_rate(test);
		test_set_field(test);
	}
	catch (GmsecException& e)
	{
		test.require(e.what(), false);
	}

	return 0;
}

TEST_DRIVER(test_HeartbeatGenerator)
