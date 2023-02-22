#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Test basic constructor (no args)...";

	try
	{
		MessageFactory msgFactory;

		test.check("Unexpected specification version", GMSEC_MSG_SPEC_CURRENT == msgFactory.getSpecification().getVersion());
		test.check("Unexpected specification schema level", Specification::SchemaLevel::LEVEL_0 == msgFactory.getSpecification().getSchemaLevel());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_constructor_2(Test& test)
{
	GMSEC_INFO << "Test constructor w/ Config...";

	try
	{
		Config config( "gmsec-schema-level=2", DataType::KEY_VALUE_DATA );

		MessageFactory msgFactory(config);

		test.check("Unexpected specification version", GMSEC_MSG_SPEC_CURRENT == msgFactory.getSpecification().getVersion());
		test.check("Unexpected specification schema level", Specification::SchemaLevel::LEVEL_2 == msgFactory.getSpecification().getSchemaLevel());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_set_standard_fields(Test& test)
{
	GMSEC_INFO << "Test setStandardFields...";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg = msgFactory.createMessage();

		verify_standard_fields(test, msg);

		msgFactory.clearStandardFields();
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_clear_standard_fields(Test& test)
{
	GMSEC_INFO << "Test clearStandardFields...";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg1 = msgFactory.createMessage();

		verify_standard_fields(test, msg1);

		msgFactory.clearStandardFields();

		Message msg2 = msgFactory.createMessage();

		test.check("Expected message field count to be 0 (zero)", 0 == msg2.getFieldCount());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_set_message_config(Test& test)
{
	GMSEC_INFO << "Test setMessageConfig...";

	// Nominal case
	try
	{
		Config         msgConfig("tracking=false", DataType::KEY_VALUE_DATA);
		MessageFactory msgFactory;

		msgFactory.setMessageConfig(msgConfig);

		Message msg = msgFactory.createMessage();

		test.check("Unexpected message config", std::string(msg.getConfig().toXML()) == msgConfig.toXML());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_create_message_1(Test& test)
{
	GMSEC_INFO << "Test createMessage (no args)...";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage();

		test.check("Expected no fields in message", 0 == msg.getFieldCount());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_create_message_2(Test& test)
{
	GMSEC_INFO << "Test createMessage...";

	// Nominal case (valid schema ID given)
	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		test.check("Unexpected MESSAGE-TYPE", std::string("MSG") == msg.getStringValue("MESSAGE-TYPE"));
		test.check("Unexpected MESSAGE-SUBTYPE", std::string("HB") == msg.getStringValue("MESSAGE-SUBTYPE"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-Nominal case (invalid schema ID given)
	try
	{
		MessageFactory msgFactory;

		(void) msgFactory.createMessage("BOGUS");

		test.check("Expected exception when using BOGUS schema ID", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("SchemaID \"BOGUS\" could not be found in list of available schema") != std::string::npos);
	}

	// Off-Nominal case (empty schema ID given)
	try
	{
		MessageFactory msgFactory;

		(void) msgFactory.createMessage("");

		test.check("Expected exception when using empty schema ID", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("SchemaID cannot be NULL, nor be an empty string") != std::string::npos);
	}

	// Off-Nominal case (NULL schema ID given)
	try
	{
		MessageFactory msgFactory;

		(void) msgFactory.createMessage(NULL);

		test.check("Expected exception when using NULL schema ID", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("SchemaID cannot be NULL, nor be an empty string") != std::string::npos);
	}
}


void test_new_and_destroy_message(Test& test)
{
	GMSEC_INFO << "Test new() and destroy()...";

	try
	{
		Message* msg = new Message();

		test.check("Expected to allocate a message", NULL != msg);

		Message::destroy(msg);

		test.check("Expected message to be destroyed", NULL == msg);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


Message from_data(Test& test, MessageFactory& msgFactory, const char* data, DataType type, const char* expectedError)
{
	Message msg;
	try
	{
		msg = msgFactory.fromData(data, type);

		if (expectedError != NULL)
		{
			std::ostringstream oss;
			oss << "Expected an errror: " << expectedError;
			test.check(oss.str().c_str(), false);
		}
	}
	catch (const GmsecException& e)
	{
		if (expectedError != NULL)
		{
			test.check(e.what(), std::string(e.what()).find(expectedError) != std::string::npos);
		}
		else
		{
			test.check(e.what(), false);
		}
	}

	return msg;
}


void test_from_data(Test& test)
{
	GMSEC_INFO << "Test fromData()...";

	// Nominal tests
	try
	{
		std::string xml =
			"<MESSAGE SUBJECT=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" KIND=\"PUBLISH\">\n"
				"\t<FIELD NAME=\"BIN-FIELD\" TYPE=\"BIN\">01020304</FIELD>\n"
				"\t<FIELD NAME=\"BOOL-FIELD\" TYPE=\"BOOL\">TRUE</FIELD>\n"
				"\t<FIELD NAME=\"CHAR-FIELD\" TYPE=\"CHAR\">c</FIELD>\n"
				"\t<FIELD NAME=\"COMPONENT\" TYPE=\"STRING\">GMPUB</FIELD>\n"
				"\t<FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">CNS1</FIELD>\n"
				"\t<FIELD NAME=\"CONTENT-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n"
				"\t<FIELD NAME=\"COUNTER\" TYPE=\"U16\">1</FIELD>\n"
				"\t<FIELD NAME=\"DOMAIN1\" TYPE=\"STRING\">DOM1</FIELD>\n"
				"\t<FIELD NAME=\"DOMAIN2\" TYPE=\"STRING\">DOM2</FIELD>\n"
				"\t<FIELD NAME=\"F64-FIELD\" TYPE=\"F64\" BITS=\"3FF0000000000000\">1</FIELD>\n"
				"\t<FIELD NAME=\"FACILITY\" TYPE=\"STRING\">MY-FACILITY</FIELD>\n"
				"\t<FIELD NAME=\"HEADER-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n"
				"\t<FIELD NAME=\"I32-FIELD\" TYPE=\"I32\">1</FIELD>\n"
				"\t<FIELD NAME=\"I64-FIELD\" TYPE=\"I64\">1</FIELD>\n"
				"\t<FIELD NAME=\"I8-FIELD\" TYPE=\"I8\">1</FIELD>\n"
				"\t<FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n"
				"\t<FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n"
				"\t<FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MSSN</FIELD>\n"
				"\t<FIELD NAME=\"PUB-RATE\" TYPE=\"U16\">1</FIELD>\n"
				"\t<FIELD NAME=\"SAT-ID-LOGICAL\" TYPE=\"STRING\">SAT1</FIELD>\n"
				"\t<FIELD NAME=\"SAT-ID-PHYSICAL\" TYPE=\"STRING\">SAT1</FIELD>\n"
				"\t<FIELD NAME=\"SPECIFICATION\" TYPE=\"STRING\">GMSEC</FIELD>\n"
				"\t<FIELD NAME=\"STR-FIELD\" TYPE=\"STRING\"></FIELD>\n"
				"\t<FIELD NAME=\"U16-FIELD\" TYPE=\"U16\">1</FIELD>\n"
				"\t<FIELD NAME=\"U32-FIELD\" TYPE=\"U32\">1</FIELD>\n"
				"\t<FIELD NAME=\"U64-FIELD\" TYPE=\"U64\">1</FIELD>\n"
				"\t<FIELD NAME=\"U8-FIELD\" TYPE=\"U8\">1</FIELD>\n"
			"</MESSAGE>";

		std::string xml_lower =
			"<message subject=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" kind=\"publish\">\n"
				"\t<field name=\"BIN-FIELD\" type=\"BIN\">01020304</field>\n"
				"\t<field name=\"BOOL-FIELD\" type=\"BOOL\">TRUE</field>\n"
				"\t<field name=\"CHAR-FIELD\" type=\"CHAR\">c</field>\n"
				"\t<field name=\"COMPONENT\" type=\"STRING\">GMPUB</field>\n"
				"\t<field name=\"CONSTELLATION-ID\" type=\"STRING\">CNS1</field>\n"
				"\t<field name=\"CONTENT-VERSION\" type=\"F32\" bits=\"44FC6000\">2019</field>\n"
				"\t<field name=\"COUNTER\" type=\"U16\">1</field>\n"
				"\t<field name=\"DOMAIN1\" type=\"STRING\">DOM1</field>\n"
				"\t<field name=\"DOMAIN2\" type=\"STRING\">DOM2</field>\n"
				"\t<field name=\"F64-FIELD\" type=\"F64\" BITS=\"3FF0000000000000\">1</field>\n"
				"\t<field name=\"FACILITY\" type=\"STRING\">MY-FACILITY</field>\n"
				"\t<field name=\"HEADER-VERSION\" type=\"F32\" bits=\"44FC6000\">2019</field>\n"
				"\t<field name=\"I32-FIELD\" type=\"I32\">1</field>\n"
				"\t<field name=\"I64-FIELD\" type=\"I64\">1</field>\n"
				"\t<field name=\"I8-FIELD\" type=\"I8\">1</field>\n"
				"\t<field name=\"MESSAGE-SUBTYPE\" type=\"STRING\">HB</field>\n"
				"\t<field name=\"MESSAGE-TYPE\" type=\"STRING\">MSG</field>\n"
				"\t<field name=\"MISSION-ID\" type=\"STRING\">MSSN</field>\n"
				"\t<field name=\"PUB-RATE\" type=\"U16\">1</field>\n"
				"\t<field name=\"SAT-ID-LOGICAL\" type=\"STRING\">SAT1</field>\n"
				"\t<field name=\"SAT-ID-PHYSICAL\" type=\"STRING\">SAT1</field>\n"
				"\t<field name=\"SPECIFICATION\" type=\"STRING\">GMSEC</field>\n"
				"\t<field name=\"STR-FIELD\" type=\"STRING\"></field>\n"
				"\t<field name=\"U16-FIELD\" type=\"U16\">1</field>\n"
				"\t<field name=\"U32-FIELD\" type=\"U32\">1</field>\n"
				"\t<field name=\"U64-FIELD\" type=\"U64\">1</field>\n"
				"\t<field name=\"U8-FIELD\" type=\"U8\">1</field>\n"
			"</message>";

		std::string json =
			"{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"KIND\":\"PUBLISH\","
			"\"FIELD\":["
			"{\"NAME\":\"COMPONENT\",\"TYPE\":\"STRING\",\"VALUE\":\"GMPUB\"},"
			"{\"NAME\":\"CONSTELLATION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"CNS1\"},"
			"{\"NAME\":\"CONTENT-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},"
			"{\"NAME\":\"COUNTER\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},"
			"{\"NAME\":\"DOMAIN1\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM1\"},"
			"{\"NAME\":\"DOMAIN2\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM2\"},"
			"{\"NAME\":\"FACILITY\",\"TYPE\":\"STRING\",\"VALUE\":\"MY-FACILITY\"},"
			"{\"NAME\":\"HEADER-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},"
			"{\"NAME\":\"MESSAGE-SUBTYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"HB\"},"
			"{\"NAME\":\"MESSAGE-TYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"MSG\"},"
			"{\"NAME\":\"MISSION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"MSSN\"},"
			"{\"NAME\":\"PUB-RATE\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},"
			"{\"NAME\":\"SAT-ID-LOGICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},"
			"{\"NAME\":\"SAT-ID-PHYSICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},"
			"{\"NAME\":\"SPECIFICATION\",\"TYPE\":\"STRING\",\"VALUE\":\"GMSEC\"}"
			"]}}";

		std::string json_lower =
			"{\"message\":{\"subject\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"kind\":\"publish\","
			"\"field\":["
			"{\"name\":\"COMPONENT\",\"type\":\"string\",\"value\":\"GMPUB\"},"
			"{\"name\":\"CONSTELLATION-ID\",\"type\":\"string\",\"value\":\"CNS1\"},"
			"{\"name\":\"CONTENT-VERSION\",\"type\":\"f32\",\"bits\":\"44FC6000\",\"value\":\"2019\"},"
			"{\"name\":\"COUNTER\",\"type\":\"u16\",\"value\":\"1\"},"
			"{\"name\":\"DOMAIN1\",\"type\":\"string\",\"value\":\"DOM1\"},"
			"{\"name\":\"DOMAIN2\",\"type\":\"string\",\"value\":\"DOM2\"},"
			"{\"name\":\"FACILITY\",\"type\":\"string\",\"value\":\"MY-FACILITY\"},"
			"{\"name\":\"HEADER-VERSION\",\"type\":\"f32\",\"bits\":\"44FC6000\",\"value\":\"2019\"},"
			"{\"name\":\"MESSAGE-SUBTYPE\",\"type\":\"string\",\"value\":\"HB\"},"
			"{\"name\":\"MESSAGE-TYPE\",\"type\":\"string\",\"value\":\"MSG\"},"
			"{\"name\":\"MISSION-ID\",\"type\":\"string\",\"value\":\"MSSN\"},"
			"{\"name\":\"PUB-RATE\",\"type\":\"u16\",\"value\":\"1\"},"
			"{\"name\":\"SAT-ID-LOGICAL\",\"type\":\"string\",\"value\":\"SAT1\"},"
			"{\"name\":\"SAT-ID-PHYSICAL\",\"type\":\"string\",\"value\":\"SAT1\"},"
			"{\"name\":\"SPECIFICATION\",\"type\":\"string\",\"value\":\"GMSEC\"}"
			"]}}";

		MessageFactory msgFactory( Config("gmsec-schema-level=2", DataType::KEY_VALUE_DATA) );

		Message msg1 = from_data(test, msgFactory, xml.c_str(), DataType::XML_DATA, NULL);
		Message msg2 = from_data(test, msgFactory, json.c_str(), DataType::JSON_DATA, NULL);
		Message msg3 = from_data(test, msgFactory, xml_lower.c_str(), DataType::XML_DATA, NULL);
		Message msg4 = from_data(test, msgFactory, json_lower.c_str(), DataType::JSON_DATA, NULL);

		test.check("Unexpected XML", xml == msg1.toXML());
		test.check("Unexpected JSON", json == msg2.toJSON());
		test.check("Unexpected XML", xml == msg3.toXML());
		test.check("Unexpected JSON", json == msg4.toJSON());

		test.check("Message 1 missing CONTENT-VERSION", msg1.hasField("CONTENT-VERSION"));
		test.check("Message 1 missing HEADER-VERSION", msg1.hasField("HEADER-VERSION"));
		test.check("Message 1 missing MESSAGE-SUBTYPE", msg1.hasField("MESSAGE-SUBTYPE"));
		test.check("Message 1 missing MESSAGE-TYPE", msg1.hasField("MESSAGE-TYPE"));
		test.check("Message 1 missing PUB-RATE", msg1.hasField("PUB-RATE"));

		test.check("Message 2 missing CONTENT-VERSION", msg2.hasField("CONTENT-VERSION"));
		test.check("Message 2 missing HEADER-VERSION", msg2.hasField("HEADER-VERSION"));
		test.check("Message 2 missing MESSAGE-SUBTYPE", msg2.hasField("MESSAGE-SUBTYPE"));
		test.check("Message 2 missing MESSAGE-TYPE", msg2.hasField("MESSAGE-TYPE"));
		test.check("Message 2 missing PUB-RATE", msg2.hasField("PUB-RATE"));

		test.check("Message 3 missing CONTENT-VERSION", msg3.hasField("CONTENT-VERSION"));
		test.check("Message 3 missing HEADER-VERSION", msg3.hasField("HEADER-VERSION"));
		test.check("Message 3 missing MESSAGE-SUBTYPE", msg3.hasField("MESSAGE-SUBTYPE"));
		test.check("Message 3 missing MESSAGE-TYPE", msg3.hasField("MESSAGE-TYPE"));
		test.check("Message 3 missing PUB-RATE", msg3.hasField("PUB-RATE"));

		test.check("Message 4 missing CONTENT-VERSION", msg4.hasField("CONTENT-VERSION"));
		test.check("Message 4 missing HEADER-VERSION", msg4.hasField("HEADER-VERSION"));
		test.check("Message 4 missing MESSAGE-SUBTYPE", msg4.hasField("MESSAGE-SUBTYPE"));
		test.check("Message 4 missing MESSAGE-TYPE", msg4.hasField("MESSAGE-TYPE"));
		test.check("Message 4 missing PUB-RATE", msg4.hasField("PUB-RATE"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Nominal tests (special cases for legacy style messages)
	try
	{
		const char* legacy = "<MESSAGE SUBJECT=\"GMSEC.MISSION.SAT.MSG.HB.COMP.C2CX\" KIND=\"PUBLISH\">"
		                     "    <FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>"
		                     "    <FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">C2CX</FIELD>"
		                     "    <FIELD NAME=\"C2CX-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>"
		                     "</MESSAGE>";

		MessageFactory msgFactory1;
		MessageFactory msgFactory2( Config("gmsec-msg-content-validate=true", DataType::KEY_VALUE_DATA) );

		// Message validation disabled
		Message msg1 = from_data(test, msgFactory1, legacy, DataType::XML_DATA, NULL);

		// Message validation enabled
		Message msg2 = from_data(test, msgFactory2, legacy, DataType::XML_DATA, "SchemaID \"MSG.C2CX\" could not be found in list of available schema.");
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{
		MessageFactory msgFactory;

		const char* valid_msg           = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\"/>";
		const char* invalid_msg_subject = "<MESSAGE SUBJECT=\"GMSEC FOO BAR\"/>";
		const char* invalid_xml         = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\">";
		const char* invalid_json        = "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.FOO.BAR\"";
		const char* empty_str           = "";
		const char* null_str            = NULL;
		const char* invalid_fld_name    = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\"><FIELD NAME=\"\" TYPE=\"I8\">1</FIELD></MESSAGE>";
		const char* invalid_fld_type    = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\"><FIELD NAME=\"BAZ\" TYPE=\"\">1</FIELD></MESSAGE>";
		const char* unknown_fld_type    = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\"><FIELD NAME=\"BAZ\" TYPE=\"BAZ\">1</FIELD></MESSAGE>";
		const char* invalid_value       = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\"><FIELD NAME=\"BAZ\" TYPE=\"I16\">BAZ</FIELD></MESSAGE>";

		(void) from_data(test, msgFactory, invalid_msg_subject, DataType::XML_DATA, "Invalid message subject");
		(void) from_data(test, msgFactory, invalid_xml, DataType::XML_DATA, "Mismatched element");
		(void) from_data(test, msgFactory, invalid_json, DataType::JSON_DATA, "Invalid JSON message format -- invalid JSON string data");
		(void) from_data(test, msgFactory, empty_str, DataType::XML_DATA, "Data string cannot be NULL, nor empty");
		(void) from_data(test, msgFactory, null_str, DataType::JSON_DATA, "Data string cannot be NULL, nor empty");
		(void) from_data(test, msgFactory, invalid_fld_name, DataType::XML_DATA, "Field name was not specified");
		(void) from_data(test, msgFactory, invalid_fld_type, DataType::XML_DATA, "Field type was not specified");
		(void) from_data(test, msgFactory, unknown_fld_type, DataType::XML_DATA, "Field BAZ has illegal type ['BAZ']");
		(void) from_data(test, msgFactory, invalid_value, DataType::XML_DATA, "Field BAZ has illegal value ['BAZ']");
		(void) from_data(test, msgFactory, valid_msg, DataType::KEY_VALUE_DATA, "Cannot generate Message from key=value pair(s)");
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_specification(Test& test)
{
	GMSEC_INFO << "Test getSpecification()...";

	try
	{
		MessageFactory msgFactory1;
		MessageFactory msgFactory2( Config("gmsec-specification-version=201900", DataType::KEY_VALUE_DATA) );

		test.check("Unexpected specification version", GMSEC_MSG_SPEC_CURRENT == msgFactory1.getSpecification().getVersion());
		test.check("Unexpected specification version", GMSEC_MSG_SPEC_2019_00 == msgFactory2.getSpecification().getVersion());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


class MyValidator : public MessageValidator
{
public:
	MyValidator(bool nominal) : m_nominal(nominal), m_used(false) {}

	bool wasUsed() const { return m_used; }

	void reset() { m_used = false; }

	Status validateMessage(const Message& msg)
	{
		Status status;

		if (!m_nominal)
		{
			status.set(MSG_ERROR, MESSAGE_FAILED_VALIDATION, "MyValidator is not happy with the message");
		}

		return status;
	}

private:
	bool m_nominal;
	bool m_used;
};


void test_register_message_validator(Test& test)
{
	GMSEC_INFO << "Test registerMessageValidator()...";

	bool nominal = true;

	MyValidator val1(nominal);
	MyValidator val2(!nominal);

	try
	{
		MessageFactory msgFactory1;

		set_standard_fields(msgFactory1);
		msgFactory1.registerMessageValidator(&val1);
		Message msg1 = msgFactory1.createMessage("HB");
		Status status1 = msg1.isCompliant();
		test.check("Expected message to be compliant", false == status1.hasError());

		MessageFactory msgFactory2;
		msgFactory2.registerMessageValidator(&val2);
		Message msg2 = msgFactory2.createMessage("HB");
		Status status2 = msg2.isCompliant();
		test.check("Expected message to be non-compliant", true  == status2.hasError());
		test.check(status2.getReason(), std::string(status2.getReason()).find("MyValidator is not happy with the message") != std::string::npos);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal case
	try
	{
		MessageFactory msgFactory1;
		msgFactory1.registerMessageValidator((MessageValidator*) NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("The MessageValidator cannot be null") != std::string::npos);
	}
}


void test_thread_safeness(Test& test);


int test_MessageFactory(Test& test)
{
	test.setDescription("Test MessageFactory");

	test_constructor_1(test);
	test_constructor_2(test);
	test_set_standard_fields(test);
	test_clear_standard_fields(test);
	test_set_message_config(test);
	test_create_message_1(test);
	test_create_message_2(test);
	test_new_and_destroy_message(test);
	test_from_data(test);
	test_get_specification(test);
	test_register_message_validator(test);

	test_thread_safeness(test);

	return 0;
}


class MyThread
{
public:
	static void runThread(StdSharedPtr<MyThread> mt) {
		mt->run();
	}

	MyThread(Test& test, StdSharedPtr<MessageFactory> msgFactory) : test(test), msgFactory(msgFactory) {
	}

	~MyThread() {
	}

	void run() {
		try {
			for (int i = 0; i < 1000; ++i) {
				Message msg = msgFactory->createMessage( ((i % 2) == 0 ? "HB" : "LOG") );
				TimeUtil::millisleep(5);
			}
			test.check("Okay, all done", true);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), false);
		}
		catch (const std::exception& e) {
			test.check(e.what(), false);
		}
	}

private:
	Test& test;
	StdSharedPtr<MessageFactory> msgFactory;
};


void test_thread_safeness(Test& test)
{
	GMSEC_INFO << "Test thread-safeness";

	const Config& config = test.getConfig();

	try
	{
		StdSharedPtr<MessageFactory> msgFactory( new MessageFactory(config) );

		StdSharedPtr<MyThread> mt( new MyThread(test, msgFactory) );

		set_standard_fields( *msgFactory.get() );

		StdUniquePtr<StdThread> thread1( new StdThread(&MyThread::runThread, mt) );
		StdUniquePtr<StdThread> thread2( new StdThread(&MyThread::runThread, mt) );
		StdUniquePtr<StdThread> thread3( new StdThread(&MyThread::runThread, mt) );

		thread1->start();
		thread2->start();
		thread3->start();

		thread1->join();
		thread2->join();
		thread3->join();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << "Exception: "  << e.what();
	}
}

TEST_DRIVER(test_MessageFactory)
