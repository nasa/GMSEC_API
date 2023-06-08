#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void verify_resource_message(Test& test, const Config& config, GMSEC_U16 expectedPubRate)
{
	std::stringstream topic;
	topic << "C2MS.*.*.*.*.*.MSG.RSRC." << test.getUniqueComponent() << ".+";

	try
	{
		Connection conn(config);
		conn.connect();
		conn.subscribe(topic.str().c_str());

		double t1 = 0, t2;

		for (int i = 0; i < 7; ++i)
		{
			Message* rsrc_msg = conn.receive(5000);

			// ignore the first few incoming messages (if any)
			if (i < 3)
			{
				if (rsrc_msg != NULL) Message::destroy(rsrc_msg);
				continue;
			}

			if (rsrc_msg)
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

				test.check("Unexpected MESSAGE-TYPE", std::string("MSG") == rsrc_msg->getStringValue("MESSAGE-TYPE"));
				test.check("Unexpected MESSAGE-SUBTYPE", std::string("RSRC") == rsrc_msg->getStringValue("MESSAGE-SUBTYPE"));
				test.check("Unexpected PUB-RATE", expectedPubRate == static_cast<GMSEC_U16>(rsrc_msg->getI32Value("PUB-RATE")));

				if (rsrc_msg->hasField("CUSTOM-FIELD"))
				{
					// Verify the CUSTOM-FIELD has the correct field type
					test.check("Unexpected CUSTOM-FIELD type", rsrc_msg->getField("CUSTOM-FIELD")->getType() == Field::Type::I16);
				}

				Message::destroy(rsrc_msg);
			}
			else
			{
				test.check("Failed to received Resource Message", false);
			}
		}

		conn.disconnect();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << "verify_resource_message: " << e.what();
	}
}


void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Test constructor 1...";

	Config config(test.getConfig());  // make a copy
	config.addValue("gmsec-msg-content-validate", "true");

	ResourceGenerator rsrc_gen(config, static_cast<GMSEC_U16>(5), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10));

	rsrc_gen.setField( StringField("MISSION-ID", "MY-MISSION") );
	rsrc_gen.setField( StringField("SAT-ID-PHYSICAL", "MY-SAT-ID") );
	rsrc_gen.setField( StringField("COMPONENT", "HB-GEN") );
	rsrc_gen.setField( StringField("BOGUS", "FOOBAR") );

	test.check("ResourceGenerator should not be running", false == rsrc_gen.isRunning());

	// Off-nominal test(s)
	try
	{
		// Bogus middleware
		config.addValue("mw-id", "bogus-mw");
		(void) ResourceGenerator(config, static_cast<GMSEC_U16>(5), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10));
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

	Config config(test.getConfig());  // make a copy
	config.addValue("gmsec-msg-content-validate", "true");

	List<Field*> standardFields;
	List<Field*> emptyFieldList;

	get_standard_fields(standardFields);

	// Nominal test - no extra fields
	ResourceGenerator rsrc_gen1(config, static_cast<GMSEC_U16>(5), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10), emptyFieldList);
	rsrc_gen1.setField( StringField("MISSION-ID", "MY-MISSION") );
	rsrc_gen1.setField( StringField("SAT-ID-PHYSICAL", "MY-SAT-ID") );
	rsrc_gen1.setField( StringField("COMPONENT", "HB-GEN") );
	rsrc_gen1.setField( StringField("BOGUS", "FOOBAR") );
	test.check("ResourceGenerator should not be running", false == rsrc_gen1.isRunning());

	// Nominal test - standard fields provided
	ResourceGenerator rsrc_gen2(config, static_cast<GMSEC_U16>(5), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10), standardFields);
	rsrc_gen2.setField( StringField("BOGUS", "FOOBAR") );
	test.check("ResourceGenerator should not be running", false == rsrc_gen2.isRunning());

	// Off-nominal test(s)
	GMSEC_INFO << "Off-nominal cases...";
	try
	{
		// Bogus middleware
		config.addValue("mw-id", "bogus-mw");
		ResourceGenerator(config, static_cast<GMSEC_U16>(5), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10), standardFields);
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

	Config config = test.getConfig();   // make a copy!

	List<Field*> fields;
	get_standard_fields(fields);

	try
	{
		ResourceGenerator rsrc_gen(config, static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10), fields);

        /* To ensure this resource message is unique, we set the COMPONENT field */
        rsrc_gen.setField( StringField("COMPONENT", test.getUniqueComponent().c_str(), true) );
		rsrc_gen.setField( I16Field("CUSTOM-FIELD", static_cast<GMSEC_I16>(3)) );

		rsrc_gen.start();

		test.check("ResourceGenerator should be running", rsrc_gen.isRunning());

		verify_resource_message(test, config, static_cast<GMSEC_U16>(1));

		rsrc_gen.stop();

		// Allow time for the RSRC-gen thread to stop
		TimeUtil::millisleep(2000);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal test(s)
	GMSEC_INFO << "Off-nominal cases...";
	config.addValue("gmsec-msg-content-validate", "true");
	try
	{
		//o Add bogus field using a String value
		ResourceGenerator rsrc_gen(config, static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10), fields);
		rsrc_gen.setField( StringField("BOGUS-FIELD", "2") );
		rsrc_gen.start();
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message Validation Failed") != std::string::npos);
	}

	try
	{
		//o Add legit field but with illegal value
		ResourceGenerator rsrc_gen(config, static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10), fields);
		rsrc_gen.setField( F32Field("CONTENT-VERSION", static_cast<GMSEC_F32>(1776)) );
		rsrc_gen.start();
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message Validation Failed") != std::string::npos);
	}

	// Cleanup
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
		ResourceGenerator rsrc_gen(config, static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10), standardFields);

        /* To ensure this resource message is unique, we set the COMPONENT field */
        rsrc_gen.setField( StringField("COMPONENT", test.getUniqueComponent().c_str()) );

		rsrc_gen.start();

		test.check("ResourceGenerator should be running", rsrc_gen.isRunning());

		rsrc_gen.stop();

		test.check("ResourceGenerator should have stopped", false == rsrc_gen.isRunning());

		// Allow time for the RSRC-gen thread to stop
		TimeUtil::millisleep(2000);

		std::stringstream topic;
		topic << "C2MS.*.*.*.*.*.MSG.RSRC." << test.getUniqueComponent();

		Connection conn(config);
		conn.connect();
		conn.subscribe(topic.str().c_str());

		for (int i = 0; i < 3; ++i)
		{
			Message *rsrc_msg = conn.receive(5000);

			if (rsrc_msg)
			{
				test.check("Unexpectedly received a Resource Message", false);

				Message::destroy(rsrc_msg);
			}
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Cleanup
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
		ResourceGenerator rsrc_gen(config, static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(1), static_cast<GMSEC_U16>(10), standardFields);

        /* To ensure this resource message is unique, we set the COMPONENT field */
        rsrc_gen.setField( StringField("COMPONENT", test.getUniqueComponent().c_str()) );

		rsrc_gen.setField( StringField("DESTINATION-COMPONENT", "FOO-BAR") );

		rsrc_gen.start();

		TimeUtil::millisleep(2000);

		// Use a Field
		rsrc_gen.setField( U16Field("PUB-RATE", static_cast<GMSEC_U16>(2)) );
		verify_resource_message(test, config, static_cast<GMSEC_U16>(2));

		rsrc_gen.stop();

		// Allow time for the RSRC-gen thread to stop
		TimeUtil::millisleep(2000);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal cases
	GMSEC_INFO << "Off-nominal cases...";

	GMSEC_INFO << "Cleanup...";
	destroy_standard_fields(standardFields);
}


void test_create_resource_message(Test& test)
{
	GMSEC_INFO << "Test createResourceMessage()...";

	Config& config = test.getConfig();

	List<Field*> standardFields;
	get_standard_fields(standardFields);

	try
	{
		MessageFactory factory( config );
		GMSEC_U16      sampleInterval  = static_cast<GMSEC_U16>(1);
		GMSEC_U16      averageInterval = static_cast<GMSEC_U16>(10);

		factory.setStandardFields( standardFields );

		Message rsrcMsg = ResourceGenerator::createResourceMessage( factory, sampleInterval, averageInterval );

		test.check("Unexpected MESSAGE-TYPE", std::string("MSG") == rsrcMsg.getStringValue("MESSAGE-TYPE"));
		test.check("Unexpected MESSAGE-SUBTYPE", std::string("RSRC") == rsrcMsg.getStringValue("MESSAGE-SUBTYPE"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	GMSEC_INFO << "Cleanup...";
	destroy_standard_fields( standardFields );
}


int test_ResourceGenerator(Test& test)
{
	test.setDescription("Test ResourceGenerator");

	test_constructor_1(test);
	test_constructor_2(test);
	test_start(test);
	test_stop(test);
	test_set_field(test);
	test_create_resource_message(test);

	return 0;
}

TEST_DRIVER(test_ResourceGenerator)
