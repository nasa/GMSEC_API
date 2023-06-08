#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void assertMessage(Test& test, const Message& msg)
{
	test.check("Message is missing PUB-RATE", msg.hasField("PUB-RATE"));
	const Field* field = msg.getField("PUB-RATE");
	test.check("Message is missing PUB-RATE", NULL != field);
	test.check("PUB-RATE has unexpected field value", 30 == field->getI32Value());
	test.check("PUB-RATE has unexpected field type", Field::Type::U16 == field->getType());

	test.check("Message has unexpected schema ID", std::string("MSG.HB") == msg.getSchemaID());

	test.check("Message is missing DOMAIN1", msg.hasField("DOMAIN1"));
	const Field* dom1 = msg.getField("DOMAIN1");
	test.check("DOMAIN1 has unexpected field value", std::string("MY-DOMAIN-1") == dom1->getStringValue());

	test.check("Message is missing DOMAIN2", msg.hasField("DOMAIN2"));
	const Field* dom2 = msg.getField("DOMAIN2");
	test.check("DOMAIN2 has unexpected field value", std::string("MY-DOMAIN-2") == dom2->getStringValue());
}


void test_constructor(Test& test)
{
	GMSEC_INFO << "Test constructor";

	Message msg;

	test.check("Message has unexpected schema ID", std::string("") == msg.getSchemaID());
	test.check("Message has unexpected field count", 0 == msg.getFieldCount());
	test.check("Message has unexpected kind", Message::Kind::PUBLISH == msg.getKind());
	test.check("Message has NULL subject", NULL != msg.getSubject());
	test.check("Message as XML is NULL", NULL != msg.toXML());
	test.check("Message as JSON is NULL", NULL != msg.toJSON());
}


void test_copy_constructor(Test& test)
{
	GMSEC_INFO << "Test copy-constructor";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");

		msg1.setFieldValue("PUB-RATE", static_cast<GMSEC_U32>(30));

		Message msg2 = msg1;   // invoke copy-constructor
		assertMessage(test, msg2);

		test.check("Unexpected schema ID", strcompare(msg1.getSchemaID(), msg2.getSchemaID()) == 0);
		test.check("Unexpected version", msg1.getVersion() == msg2.getVersion());
		test.check("Unexpected schema level", msg1.getSchemaLevel() == msg2.getSchemaLevel());
		test.check("Message has unexpected XML content", std::string(msg1.toXML()) == std::string(msg2.toXML()));
		test.check("Message has unexpected JSON content", std::string(msg1.toJSON()) == std::string(msg2.toJSON()));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_operator_equal(Test& test)
{
	GMSEC_INFO << "Test operator=()";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = msgFactory.createMessage();

		msg1.setFieldValue("PUB-RATE", static_cast<GMSEC_U32>(30));

		msg2 = msg1;   // invoke operator=() method
		assertMessage(test, msg2);

		test.check("Message has unexpected XML content", std::string(msg1.toXML()) == std::string(msg2.toXML()));
		test.check("Message has unexpected JSON content", std::string(msg1.toJSON()) == std::string(msg2.toJSON()));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_schema_id(Test& test)
{
	GMSEC_INFO << "Test getSchemaID()";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = msgFactory.createMessage("MVAL");
		Message msg3 = msgFactory.createMessage("TLMFRAME");

		test.check("Message 1 has unexpected schema ID", std::string("MSG.HB") == msg1.getSchemaID());
		test.check("Message 2 has unexpected schema ID", std::string("MSG.MVAL") == msg2.getSchemaID());
		test.check("Message 3 has unexpected schema ID", std::string("MSG.TLMFRAME") == msg3.getSchemaID());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_version(Test& test)
{
	GMSEC_INFO << "Test getVersion()";

	try
	{
		MessageFactory msgFactory1;
		MessageFactory msgFactory2(Config("gmsec-specification-version=201900", DataType::KEY_VALUE_DATA));

		Message msg1 = msgFactory1.createMessage("HB");
		Message msg2 = msgFactory2.createMessage("HB");

		test.check("Message 1 has unexpected version", GMSEC_MSG_SPEC_CURRENT == msg1.getVersion());
		test.check("Message 2 has unexpected version", GMSEC_MSG_SPEC_2019_00 == msg2.getVersion());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_schema_level(Test& test)
{
	GMSEC_INFO << "Test getSchemaLevel()";

	try
	{
		MessageFactory msgFactory1;
		MessageFactory msgFactory2(Config("gmsec-schema-level=2", DataType::KEY_VALUE_DATA));

		Message msg1 = msgFactory1.createMessage("HB");
		Message msg2 = msgFactory2.createMessage("HB");

		test.check("Message 1 has unexpected schema level", Specification::SchemaLevel::LEVEL_0 == msg1.getSchemaLevel());
		test.check("Message 2 has unexpected schema level", Specification::SchemaLevel::LEVEL_2 == msg2.getSchemaLevel());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_is_compliant(Test& test)
{
	GMSEC_INFO << "Test isCompliant()";

	try
	{
		MessageFactory msgFactory1;
		MessageFactory msgFactory2;

		set_standard_fields(msgFactory1);

		//o Check field compliancy
		//
		Message msg1 = msgFactory1.createMessage("HB");
		Message msg2 = msgFactory2.createMessage("HB");      // this message lacks standard fields
		Message msg3;                                        // homemade message
		msg3.setSubject("GMSEC.FOO.BAR");

		msg2.addField("DOMAIN1", static_cast<GMSEC_U32>(1));  // illegal field type

		Status status1 = msg1.isCompliant();
		Status status2 = msg2.isCompliant();
		Status status3 = msg3.isCompliant();

		test.check("Message 1 should be compliant", false == status1.hasError());

		test.check("Message 2 should not be compliant", true  == status2.hasError());
		test.check("Message 2 error has unexpected reason", std::string(status2.getReason()).find("Message Validation Failed") != std::string::npos);

		test.check("Message 3 should not be compliant", true  == status3.hasError());
		test.check("Message 3 error has unexpected reason",
		           std::string(status3.getReason()).find("No template available; unable to validate message [GMSEC.FOO.BAR]") != std::string::npos);

		//o Check header field compliancy
		//
		// Nominal test
		Message msg4 = msgFactory1.createMessage("HB");
		msg4.addField(U16Field("PUB-RATE", 30));               // not a header field
		msg4.addField(StringField("CLASS", "FOOBAR", true));   // is a header field

		Status status4 = msg4.isCompliant();
		test.check(status4.get(), status4.hasError() == false);

		// Off-nominal test
		msg4.addField(U16Field("PUB-RATE", 30, true));         // illegal header field designation
		msg4.addField(StringField("CLASS", "FOOBAR"));         // should be a header field

		status4 = msg4.isCompliant();
		test.check("Expected error(s)", status4.hasError());
		test.check("Unexpected error 1", std::string(status4.get()).find("PUB-RATE is improperly identified as a header field") != std::string::npos);
		test.check("Unexpected error 2", std::string(status4.get()).find("CLASS is not identified as a header field") != std::string::npos);
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
	GMSEC_INFO << "Test registerMessageValidator()";

	bool nominal = true;

	MyValidator val1( nominal );
	MyValidator val2( !nominal );
	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		msg1.registerMessageValidator(&val1);
		Status status1 = msg1.isCompliant();
		test.check("Message 1 should be compliant", status1.hasError() == false);

		msg1.clearField("DOMAIN1");   // remove required field
		status1 = msg1.isCompliant();
		test.check("Message 1 should be compliant", status1.hasError());
		test.check("Message 1 has unexpected error", std::string(status1.getReason()).find("Message Validation Failed") != std::string::npos);

		Message msg2 = msgFactory.createMessage("HB");
		msg2.registerMessageValidator(&val2);
		Status status2 = msg2.isCompliant();
		test.check("Message 2 should not be compliant", status2.hasError());
		test.check("Message 2 has unexpected error", std::string(status2.getReason()).find("MyValidator is not happy with the message") != std::string::npos);

		// Create a copy of the message, and set a new configuration.
		// Verify custom message validator still works.
		Message msg3 = Message(msg2);
		msg3.setConfig(Config("tracking=false", DataType::KEY_VALUE_DATA));
		Status status3 = msg3.isCompliant();
		test.check("Message 3 should not be compliant", status3.hasError());
		test.check("Message 3 has unexpected error", std::string(status3.getReason()).find("MyValidator is not happy with the message") != std::string::npos);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal case
	try
	{
		Message msg;
		msg.registerMessageValidator((MessageValidator*) NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("The MessageValidator cannot be null") != std::string::npos);
	}
}


void test_set_field_value(Test& test)
{
	GMSEC_INFO << "Test setFieldValue()";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg = msgFactory.createMessage("HB");

		//o Test with string value
		msg.setFieldValue("PUB-RATE", "5");
		test.check("PUB-RATE has unexpected field type", Field::Type::U16 == msg.getField("PUB-RATE")->getType());
		test.check("PUB-RATE has unexpected field value", static_cast<GMSEC_I32>(5) == msg.getI32Value("PUB-RATE"));

		//o Test with GMSEC_I16 value
		msg.setFieldValue("PUB-RATE", static_cast<GMSEC_I16>(10));
		test.check("PUB-RATE has unexpected field type", Field::Type::U16 == msg.getField("PUB-RATE")->getType());
		test.check("PUB-RATE has unexpected field value", static_cast<GMSEC_I32>(10) == msg.getI32Value("PUB-RATE"));

		//o Test with GMSEC_U16 value
		msg.setFieldValue("PUB-RATE", static_cast<GMSEC_U16>(10));
		test.check("PUB-RATE has unexpected field type", Field::Type::U16 == msg.getField("PUB-RATE")->getType());
		test.check("PUB-RATE has unexpected field value", static_cast<GMSEC_I32>(10) == msg.getI32Value("PUB-RATE"));

		//o Test with GMSEC_I32 value
		msg.setFieldValue("PUB-RATE", static_cast<GMSEC_I32>(10));
		test.check("PUB-RATE has unexpected field type", Field::Type::U16 == msg.getField("PUB-RATE")->getType());
		test.check("PUB-RATE has unexpected field value", static_cast<GMSEC_I32>(10) == msg.getI32Value("PUB-RATE"));

		//o Test with GMSEC_U32 value
		msg.setFieldValue("PUB-RATE", static_cast<GMSEC_U32>(20));
		test.check("PUB-RATE has unexpected field type", Field::Type::U16 == msg.getField("PUB-RATE")->getType());
		test.check("PUB-RATE has unexpected field value", static_cast<GMSEC_I32>(20) == msg.getI32Value("PUB-RATE"));

		//o Test with GMSEC_I64 value
		msg.setFieldValue("PUB-RATE", static_cast<GMSEC_I64>(30));
		test.check("PUB-RATE has unexpected field type", Field::Type::U16 == msg.getField("PUB-RATE")->getType());
		test.check("PUB-RATE has unexpected field value", static_cast<GMSEC_I32>(30) == msg.getI32Value("PUB-RATE"));

		msg.setFieldValue("MISSION-ID", static_cast<GMSEC_I64>(1234));
		test.check("MISSION-ID has unexpected field type", Field::Type::STRING == msg.getField("MISSION-ID")->getType());
		test.check("MISSION-ID has unexpected field value", static_cast<GMSEC_I32>(1234) == msg.getI32Value("MISSION-ID"));

		//o Test with GMSEC_F64 value
		msg.setFieldValue("PUB-RATE", 40.123);
		test.check("PUB-RATE has unexpected field type", Field::Type::U16 == msg.getField("PUB-RATE")->getType());
		test.check("PUB-RATE has unexpected field value", static_cast<GMSEC_I32>(40) == msg.getI32Value("PUB-RATE"));

		msg.setFieldValue("MISSION-ID", static_cast<GMSEC_F64>(1234));
		test.check("MISSION-ID has unexpected field type", Field::Type::STRING == msg.getField("MISSION-ID")->getType());
		test.check("MISSION-ID has unexpected field value", static_cast<GMSEC_I32>(1234) == msg.getI32Value("MISSION-ID"));

		//o Verify result returned is 'true' when replacing a field
		test.check("Expected return value of true", msg.setFieldValue("PUB-RATE", static_cast<GMSEC_I32>(10)) == true);

		//o Verify result returned is 'false' when adding a new field
		test.check("Expected return value of false", msg.setFieldValue("PUBLISH-RATE", static_cast<GMSEC_I32>(10)) == false);


		// Off-nominal tests
		try {
			msg.setFieldValue(NULL, "1");
			test.check("Message::setFieldValue() accepted NULL field name", false);
		}
		catch (...) {
			test.check("Okay", true);
		}

		try {
			msg.setFieldValue("FOO-BAR", static_cast<const char*>(NULL));
			test.check("Message::setFieldValue() accepted NULL field value", false);
		}
		catch (...) {
			test.check("Okay", true);
		}

		try {
			msg.setFieldValue(NULL, static_cast<GMSEC_I64>(1));
			test.check("Message::setFieldValue() accepted NULL field name", false);
		}
		catch (...) {
			test.check("Okay", true);
		}

		try {
			msg.setFieldValue(NULL, static_cast<GMSEC_F64>(1));
			test.check("Message::setFieldValue() accepted NULL field name", false);
		}
		catch (...) {
			test.check("Okay", true);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_set_config(Test& test)
{
	GMSEC_INFO << "Test setConfig()";

	try
	{
		Config config("tracking=false", DataType::KEY_VALUE_DATA);

		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg = msgFactory.createMessage("TLMPROC");

		msg.setConfig(config);

		test.check("Message has unexpected Config content", std::string(config.toXML()) == std::string(msg.getConfig().toXML()));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_set_subject(Test& test)
{
	GMSEC_INFO << "Test setSubject()";

	// Nominal test
	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg = msgFactory.createMessage("TLMPROC");

		msg.setSubject("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY");

		test.check("Message has unexpected subject", std::string("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY") == msg.getSubject());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{
		Message msg;
		msg.setSubject(NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Subject cannot be NULL, nor an empty string") != std::string::npos);
	}

	try
	{
		Message msg;
		msg.setSubject("");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Subject cannot be NULL, nor an empty string") != std::string::npos);
	}

	try
	{
		Message msg;
		msg.setSubject("Subject.Cannot.Contain.Lowercase.Letters");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Invalid message subject") != std::string::npos);
	}

	try
	{
		Message msg;
		msg.setSubject("Hello%World");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Invalid message subject") != std::string::npos);
	}
}


void test_get_subject(Test& test)
{
	GMSEC_INFO << "Test getSubject()";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg = msgFactory.createMessage("TLMPROC");
		msg.addField("STREAM-MODE", "RT");  // add ME2 (stream mode)

		std::string subject = msg.getSubject();
		subject += ".2";   // add ME2 (virtual channel ID)
		subject += ".1";   // add ME3 (AP ID)

		msg.setSubject(subject.c_str());

		test.check("Message has unexpected subject", subject == msg.getSubject());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_set_kind(Test& test)
{
	GMSEC_INFO << "Test setKind()";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg = msgFactory.createMessage("HB");
		test.check("Expected PUBLISH-kind", msg.getKind() == Message::Kind::PUBLISH);

		// Set to REQUEST-kind, and verify
		msg.setKind(Message::Kind::REQUEST);
		test.check("Expected REQUEST-kind", msg.getKind() == Message::Kind::REQUEST);

		// Set to REPLY-kind, and verify
		msg.setKind(Message::Kind::REPLY);
		test.check("Expected REPLY-kind", msg.getKind() == Message::Kind::REPLY);

		// Set to PUBLISH-kind, and verify
		msg.setKind(Message::Kind::PUBLISH);
		test.check("Expected PUBLISH-kind", msg.getKind() == Message::Kind::PUBLISH);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_kind(Test& test)
{
	GMSEC_INFO << "Test getKind()";

	try
	{
		MessageFactory msgFactory;

		set_standard_fields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = msgFactory.createMessage("REQ.DIR");
		Message msg3 = msgFactory.createMessage("RESP.DIR");
		Message msg4 = msgFactory.createMessage();
		Message msg5;

		test.check("Message 1 has unexpected kind", Message::Kind::PUBLISH == msg1.getKind());
		test.check("Message 2 has unexpected kind", Message::Kind::REQUEST == msg2.getKind());
		test.check("Message 3 has unexpected kind", Message::Kind::REPLY   == msg3.getKind());
		test.check("Message 4 has unexpected kind", Message::Kind::PUBLISH == msg4.getKind());
		test.check("Message 5 has unexpected kind", Message::Kind::PUBLISH == msg5.getKind());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_add_field(Test& test)
{
	GMSEC_INFO << "Test addField()";

	try
	{
		StringField field("SPECIFICATION", "C2MS");

		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("LOG");

		unsigned char blob[] = { 0x00, 0x02, 0x04, 0x08, 0xFF };
		size_t blobLen = sizeof(blob)/sizeof(unsigned char);

		test.check("Expected notice field is being replaced", true == msg.addField(field));
		test.check("Expected new field to be added", false == msg.addField("BINARY", blob, blobLen));
		test.check("Expected new field to be added", false == msg.addField("BOOLEAN", true));
		test.check("Expected new field to be added", false == msg.addField("CHAR", 'b'));
		test.check("Expected new field to be added", true == msg.addField("CHAR", 'c'));
		test.check("Expected new field to be added", false == msg.addField("FLOAT", (GMSEC_F32) 1.2));
		test.check("Expected new field to be added", false == msg.addField("DOUBLE", (GMSEC_F64) 2.4));
		test.check("Expected new field to be added", false == msg.addField("BYTE", (GMSEC_I8) 5));
		test.check("Expected new field to be added", false == msg.addField("SHORT", (GMSEC_I16) 55));
		test.check("Expected new field to be added", false == msg.addField("INT", (GMSEC_I32) 555));
		test.check("Expected new field to be added", false == msg.addField("LONG", (GMSEC_I64) 5555));
		test.check("Expected new field to be added", false == msg.addField("STRING", "The old fox"));
		test.check("Expected new field to be added", false == msg.addField("UBYTE", (GMSEC_U8) 5));
		test.check("Expected new field to be added", false == msg.addField("USHORT", (GMSEC_U16) 55));
		test.check("Expected new field to be added", false == msg.addField("UINT", (GMSEC_U32) 555));
		test.check("Expected new field to be added", false == msg.addField("ULONG", (GMSEC_U64) 5555));

		test.check("Expected to find field 1", true == msg.hasField("SPECIFICATION"));
		test.check("Unexpected field type 1", Field::Type::STRING == msg.getField("SPECIFICATION")->getType());
		test.check("Unexpected field value 1", std::string("C2MS") == msg.getStringValue("SPECIFICATION"));

		test.check("Expected to find field 2", true == msg.hasField("BINARY"));
		test.check("Unexpected field type 2", Field::Type::BINARY == msg.getField("BINARY")->getType());
		test.check("Unexpected field length 2", blobLen == ((BinaryField*)msg.getField("BINARY"))->getLength());

		const GMSEC_U8* fieldBlob = dynamic_cast<const BinaryField*>(msg.getField("BINARY"))->getValue();

		for (size_t i = 0; i < blobLen; ++i)
		{
			bool same = (blob[i] == fieldBlob[i]);

			if (!same)
			{
				std::ostringstream oss;
				oss << "Blob data not the same at position: " << i;
				test.check(oss.str().c_str(), false);
			}
		}

		test.check("Expected to find field 3", true == msg.hasField("BOOLEAN"));
		test.check("Unexpected field type 3", Field::Type::BOOL == msg.getField("BOOLEAN")->getType());
		test.check("Unexpected field value 3", true == ((BooleanField*)msg.getField("BOOLEAN"))->getValue());

		test.check("Expected to find field 4", true == msg.hasField("CHAR"));
		test.check("Unexpected field type 4", Field::Type::CHAR == msg.getField("CHAR")->getType());
		test.check("Unexpected field value 4", 'c'== ((CharField*)msg.getField("CHAR"))->getValue());

		test.check("Expected to find field 5", true == msg.hasField("FLOAT"));
		test.check("Unexpected field type 5", Field::Type::F32 == msg.getField("FLOAT")->getType());
		test.check("Unexpected field value 5", (GMSEC_F32) 1.2 == ((F32Field*)msg.getField("FLOAT"))->getValue());

		test.check("Expected to find field 6", true == msg.hasField("DOUBLE"));
		test.check("Unexpected field type 6", Field::Type::F64 == msg.getField("DOUBLE")->getType());
		test.check("Unexpected field value 6", (GMSEC_F64) 2.4 == ((F64Field*)msg.getField("DOUBLE"))->getValue());

		test.check("Expected to find field 7", true == msg.hasField("BYTE"));
		test.check("Unexpected field type 7", Field::Type::I8 == msg.getField("BYTE")->getType());
		test.check("Unexpected field value 7", 5 == ((I8Field*)msg.getField("BYTE"))->getValue());

		test.check("Expected to find field 8", true == msg.hasField("SHORT"));
		test.check("Unexpected field type 8", Field::Type::I16 == msg.getField("SHORT")->getType());
		test.check("Unexpected field value 8", 55 == ((I16Field*)msg.getField("SHORT"))->getValue());

		test.check("Expected to find field 9", true == msg.hasField("INT"));
		test.check("Unexpected field type 9", Field::Type::I32 == msg.getField("INT")->getType());
		test.check("Unexpected field value 9", 555 == ((I32Field*)msg.getField("INT"))->getValue());

		test.check("Expected to find field 10", true == msg.hasField("LONG"));
		test.check("Unexpected field type 10", Field::Type::I64 == msg.getField("LONG")->getType());
		test.check("Unexpected field value 10", 5555 == ((I64Field*)msg.getField("LONG"))->getValue());

		test.check("Expected to find field 11", true == msg.hasField("STRING"));
		test.check("Unexpected field type 11", Field::Type::STRING == msg.getField("STRING")->getType());
		test.check("Unexpected field value 11", std::string("The old fox") == ((StringField*)msg.getField("STRING"))->getValue());

		test.check("Expected to find field 12", true == msg.hasField("UBYTE"));
		test.check("Unexpected field type 12", Field::Type::U8 == msg.getField("UBYTE")->getType());
		test.check("Unexpected field value 12", 5 == ((U8Field*)msg.getField("UBYTE"))->getValue());

		test.check("Expected to find field 13", true == msg.hasField("USHORT"));
		test.check("Unexpected field type 13", Field::Type::U16 == msg.getField("USHORT")->getType());
		test.check("Unexpected field value 13", 55 == ((U16Field*)msg.getField("USHORT"))->getValue());

		test.check("Expected to find field 14", true == msg.hasField("UINT"));
		test.check("Unexpected field type 14", Field::Type::U32 == msg.getField("UINT")->getType());
		test.check("Unexpected field value 14", 555 == ((U32Field*)msg.getField("UINT"))->getValue());

		test.check("Expected to find field 15", true == msg.hasField("ULONG"));
		test.check("Unexpected field type 15", Field::Type::U64 == msg.getField("ULONG")->getType());
		test.check("Unexpected field value 15", 5555 == ((U64Field*)msg.getField("ULONG"))->getValue());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_add_fields(Test& test)
{
	GMSEC_INFO << "Test addFields()";

	try
	{
		StringField one("ONE", "uno");
		StringField two("TWO", "dos");
		StringField three("THREE", "tres");

		List<Field*> fields;
		fields.push_back(&one);
		fields.push_back(&two);
		fields.push_back(&three);

		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("LOG");

		test.check("Expected only new fields to be added", false == msg.addFields(fields));

		test.check("Expected to have field ONE", true == msg.hasField("ONE"));
		test.check("Unexpected field type for ONE", Field::Type::STRING == msg.getField("ONE")->getType());
		test.check("Unexpected field value for ONE", std::string("uno") == ((StringField*)msg.getField("ONE"))->getValue());

		test.check("Expected to have field TWO", true == msg.hasField("TWO"));
		test.check("Unexpected field type for TWO", Field::Type::STRING == msg.getField("TWO")->getType());
		test.check("Unexpected field value for TWO", std::string("dos") == ((StringField*)msg.getField("TWO"))->getValue());

		test.check("Expected to have field THREE", true == msg.hasField("THREE"));
		test.check("Unexpected field type for THREE", Field::Type::STRING == msg.getField("THREE")->getType());
		test.check("Unexpected field value for THREE", std::string("tres") == ((StringField*)msg.getField("THREE"))->getValue());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_clear_fields(Test& test)
{
	GMSEC_INFO << "Test clearFields()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		test.check("Message missing CONTENT-VERSION", true == msg.hasField("CONTENT-VERSION"));
		test.check("Message missing HEADER-VERSION", true == msg.hasField("HEADER-VERSION"));
		test.check("Message missing MESSAGE-SUBTYPE", true == msg.hasField("MESSAGE-SUBTYPE"));
		test.check("Message missing MESSAGE-TYPE", true == msg.hasField("MESSAGE-TYPE"));
		test.check("Message missing SPECIFICATION", true == msg.hasField("SPECIFICATION"));

		msg.clearFields();

		test.check("Message has CONTENT-VERSION", false == msg.hasField("CONTENT-VERSION"));
		test.check("Message has HEADER-VERSION", false == msg.hasField("HEADER-VERSION"));
		test.check("Message has MESSAGE-SUBTYPE", false == msg.hasField("MESSAGE-SUBTYPE"));
		test.check("Message has MESSAGE-TYPE", false == msg.hasField("MESSAGE-TYPE"));
		test.check("Message has SPECIFICATION", false == msg.hasField("SPECIFICATION"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_clear_field(Test& test)
{
	GMSEC_INFO << "Test clearField()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		test.check("Message should have CONTENT-VERSION", true == msg.hasField("CONTENT-VERSION"));

		test.check("Expected result to indicate field cleared", true == msg.clearField("CONTENT-VERSION"));
		test.check("Expected result to indicate field not cleared", false == msg.clearField("FOO-BAR"));
		test.check("Expected result to indicate field not cleared", false == msg.clearField("foo-bar"));

		test.check("Message should not have CONTENT-VERSION", false == msg.hasField("CONTENT-VERSION"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_string_value(Test& test)
{
	GMSEC_INFO << "Test getStringValue()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("MESSAGE-TYPE has unexpected value", std::string("MSG") == msg.getStringValue("MESSAGE-TYPE"));
		test.check("PUB-RATE has unexpected value", std::string("30")  == msg.getStringValue("PUB-RATE"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_boolean_value(Test& test)
{
	GMSEC_INFO << "Test getBooleanValue()";

	try
	{
		Message msg;

		msg.addField("FIELD-1", static_cast<GMSEC_U32>(0));
		msg.addField("FIELD-2", static_cast<GMSEC_U32>(1));
		msg.addField("FIELD-3", "10000");
		msg.addField("FIELD-4", static_cast<GMSEC_U32>(10000));
		msg.addField("FIELD-5", true);
		msg.addField("FIELD-6", false);
		msg.addField("FIELD-7", 2019.0);
		msg.addField("FIELD-8", "true");
		msg.addField("FIELD-9", "FALSE");
		msg.addField("FIELD-0", "foo bar");

		// Nominal tests
		test.check("1 Expected boolean value of false", msg.getBooleanValue("FIELD-1") == false);
		test.check("2 Expected boolean value of true",  msg.getBooleanValue("FIELD-2"));
		test.check("3 Expected boolean value of true",  msg.getBooleanValue("FIELD-3"));
		test.check("4 Expected boolean value of true",  msg.getBooleanValue("FIELD-4"));
		test.check("5 Expected boolean value of true",  msg.getBooleanValue("FIELD-5"));
		test.check("6 Expected boolean value of false", msg.getBooleanValue("FIELD-6") == false);
		test.check("7 Expected boolean value of true",  msg.getBooleanValue("FIELD-7"));
		test.check("8 Expected boolean value of true",  msg.getBooleanValue("FIELD-8"));
		test.check("9 Expected boolean value of false", msg.getBooleanValue("FIELD-9") == false);

		// Off-nominal tests

		// Field cannot be converted
		try {
			msg.getBooleanValue("FIELD-0");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be represented as a boolean") != std::string::npos);
		}

		// Unknown field
		try {
			msg.getBooleanValue("BOGUS");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Message does not contain field with name: BOGUS") != std::string::npos);
		}

		// NULL field name
		try {
			msg.getBooleanValue(NULL);
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
		}

		// empty-string field name
		try {
			msg.getBooleanValue("");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_i16_value(Test& test)
{
	GMSEC_INFO << "Test getI16Value()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("PUB-RATE has unexpected value", static_cast<GMSEC_I16>(30) == msg.getI16Value("PUB-RATE"));

		// Off-nominal tests
		try {
			msg.getI16Value("MESSAGE-TYPE");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I16") != std::string::npos);
		}

		try {
			msg.getI16Value("FOO-BAR");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Message does not contain field with name") != std::string::npos);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_i32_value(Test& test)
{
	GMSEC_INFO << "Test getI32Value()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("PUB-RATE has unexpected value", 30 == msg.getI32Value("PUB-RATE"));

		// Off-nominal tests
		try {
			msg.getI32Value("MESSAGE-TYPE");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I32") != std::string::npos);
		}

		try {
			msg.getI32Value("FOO-BAR");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Message does not contain field with name") != std::string::npos);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_i64_value(Test& test)
{
	GMSEC_INFO << "Test getI64Value()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("PUB-RATE has unexpected value", static_cast<GMSEC_I64>(30) == msg.getI64Value("PUB-RATE"));

		// Off-nominal tests
		try {
			msg.getI64Value("MESSAGE-TYPE");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I64") != std::string::npos);
		}

		try {
			msg.getI64Value("FOO-BAR");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Message does not contain field with name") != std::string::npos);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_u16_value(Test& test)
{
	GMSEC_INFO << "Test getU16Value()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("PUB-RATE has unexpected value", static_cast<GMSEC_U16>(30) == msg.getU16Value("PUB-RATE"));

		// Off-nominal tests
		try {
			msg.getU16Value("MESSAGE-TYPE");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U16") != std::string::npos);
		}

		try {
			msg.getU16Value("FOO-BAR");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Message does not contain field with name") != std::string::npos);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_u32_value(Test& test)
{
	GMSEC_INFO << "Test getU32Value()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("PUB-RATE has unexpected value", static_cast<GMSEC_U32>(30) == msg.getU32Value("PUB-RATE"));

		// Off-nominal tests
		try {
			msg.getU32Value("MESSAGE-TYPE");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U32") != std::string::npos);
		}

		try {
			msg.getU32Value("FOO-BAR");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Message does not contain field with name") != std::string::npos);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_u64_value(Test& test)
{
	GMSEC_INFO << "Test getU64Value()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("PUB-RATE has unexpected value", static_cast<GMSEC_U64>(30) == msg.getU64Value("PUB-RATE"));

		// Off-nominal tests
		try {
			msg.getU64Value("MESSAGE-TYPE");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U64") != std::string::npos);
		}

		try {
			msg.getU64Value("FOO-BAR");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Message does not contain field with name") != std::string::npos);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_f64_value(Test& test)
{
	GMSEC_INFO << "Test getF64Value()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("PUB-RATE has unexpected value", static_cast<GMSEC_F64>(30) == msg.getF64Value("PUB-RATE"));

		// Off-nominal tests
		try {
			msg.getF64Value("MESSAGE-TYPE");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_F64") != std::string::npos);
		}

		try {
			msg.getF64Value("FOO-BAR");
			test.check("An exception was expected", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Message does not contain field with name") != std::string::npos);
		}
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_has_field(Test& test)
{
	GMSEC_INFO << "Test hasField()";

	try
	{
		MessageFactory msgFactory;

		Message msg = msgFactory.createMessage("HB");

		msg.setFieldValue("PUB-RATE", "30");

		test.check("Message should have PUB-RATE", true == msg.hasField("PUB-RATE"));
		test.check("Message should not have PUBLISH-RATE", false == msg.hasField("PUBLISH-RATE"));
		test.check("Message should not have null field", false == msg.hasField(NULL));
		test.check("Message should not have empty string field", false == msg.hasField(""));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_get_field(Test& test)
{
	GMSEC_INFO << "Test getField()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("HB");

	// Use valid field name
	test.check("Message should have MESSAGE-TYPE", NULL != msg.getField("MESSAGE-TYPE"));

	// Use bogus field name
	test.check("Message should not have MSG-TYPE", NULL == msg.getField("MSG-TYPE"));

	// Use NULL field name
	try {
		msg.getField(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getField("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_field_type(Test& test)
{
	GMSEC_INFO << "Test getFieldType()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("HB");

	// Use valid field name
	test.check("MESSAGE-TYPE has unexpected field type", Field::Type::STRING == msg.getFieldType("MESSAGE-TYPE"));
	test.check("CONTENT-VERSION has unexpected field type", Field::Type::F32 == msg.getFieldType("CONTENT-VERSION"));

	// Use bogus field name
	try {
		msg.getFieldType("MSG-TYPE");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain field with name: MSG-TYPE") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getFieldType(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getFieldType("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_binary_field(Test& test)
{
	GMSEC_INFO << "Test getBinaryField()";

	const size_t dataSize = 256;
	unsigned char data[dataSize];
	for (size_t i = 0; i < dataSize; ++i)
	{
		data[i] = i & 0xFF;
	}

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("MSG.TLMPKT");

	msg.addField("DATA", data, dataSize);

	// Use valid field name
	try {
		msg.getBinaryField("DATA");
		test.check("DATA found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-binary field name
	try {
		msg.getBinaryField("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain BinaryField with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getBinaryField(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getBinaryField("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_boolean_field(Test& test)
{
	GMSEC_INFO << "Test getBooleanField()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("RESPONSE", true);

	// Use valid field name
	try {
		msg.getBooleanField("RESPONSE");
		test.check("RESPONSE found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-boolean field name
	try {
		msg.getBooleanField("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain BooleanField with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getBooleanField(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getBooleanField("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_char_field(Test& test)
{
	GMSEC_INFO << "Test getCharField()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("CHAR-FIELD", 'c');

	// Use valid field name
	try {
		msg.getCharField("CHAR-FIELD");
		test.check("CHAR-FIELD found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-char field name
	try {
		msg.getCharField("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain CharField with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getCharField(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getCharField("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_f32_field(Test& test)
{
	GMSEC_INFO << "Test getF32Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	// Use valid field name
	try {
		msg.getF32Field("CONTENT-VERSION");
		test.check("CONTENT-VERSION was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-F32 field name
	try {
		msg.getF32Field("MESSAGE-TYPE");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain F32Field with name: MESSAGE-TYPE") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getF32Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getF32Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_f64_field(Test& test)
{
	GMSEC_INFO << "Test getF64Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("F64-FIELD", static_cast<GMSEC_F64>(3.14));

	// Use valid field name
	try {
		msg.getF64Field("F64-FIELD");
		test.check("F64-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-F64 field name
	try {
		msg.getF64Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain F64Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getF64Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getF64Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_i8_field(Test& test)
{
	GMSEC_INFO << "Test getI8Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("I8-FIELD", static_cast<GMSEC_I8>(8));

	// Use valid field name
	try {
		msg.getI8Field("I8-FIELD");
		test.check("I8-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-I8 field name
	try {
		msg.getI8Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain I8Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getI8Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getI8Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_i16_field(Test& test)
{
	GMSEC_INFO << "Test getI16Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("I16-FIELD", static_cast<GMSEC_I16>(16));

	// Use valid field name
	try {
		msg.getI16Field("I16-FIELD");
		test.check("I16-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-I16 field name
	try {
		msg.getI16Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain I16Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getI16Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getI16Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_i32_field(Test& test)
{
	GMSEC_INFO << "Test getI32Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("I32-FIELD", static_cast<GMSEC_I32>(32));

	// Use valid field name
	try {
		msg.getI32Field("I32-FIELD");
		test.check("I32-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-I32 field name
	try {
		msg.getI32Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain I32Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getI32Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getI32Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_i64_field(Test& test)
{
	GMSEC_INFO << "Test getI64Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("I64-FIELD", static_cast<GMSEC_I64>(64));

	// Use valid field name
	try {
		msg.getI64Field("I64-FIELD");
		test.check("I64-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-I64 field name
	try {
		msg.getI64Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain I64Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getI64Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getI64Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_u8_field(Test& test)
{
	GMSEC_INFO << "Test getU8Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("U8-FIELD", static_cast<GMSEC_U8>(8));

	// Use valid field name
	try {
		msg.getU8Field("U8-FIELD");
		test.check("U8-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-U8 field name
	try {
		msg.getU8Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain U8Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getU8Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getU8Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_u16_field(Test& test)
{
	GMSEC_INFO << "Test getU16Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("U16-FIELD", static_cast<GMSEC_U16>(16));

	// Use valid field name
	try {
		msg.getU16Field("U16-FIELD");
		test.check("U16-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-U16 field name
	try {
		msg.getU16Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain U16Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getU16Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getU16Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_u32_field(Test& test)
{
	GMSEC_INFO << "Test getU32Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("U32-FIELD", static_cast<GMSEC_U32>(32));

	// Use valid field name
	try {
		msg.getU32Field("U32-FIELD");
		test.check("U32-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-U32 field name
	try {
		msg.getU32Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain U32Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getU32Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getU32Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_u64_field(Test& test)
{
	GMSEC_INFO << "Test getU64Field()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	msg.addField("U64-FIELD", static_cast<GMSEC_U64>(64));

	// Use valid field name
	try {
		msg.getU64Field("U64-FIELD");
		test.check("U64-FIELD was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-U64 field name
	try {
		msg.getU64Field("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain U64Field with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getU64Field(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getU64Field("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_string_field(Test& test)
{
	GMSEC_INFO << "Test getStringField()";

	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("REQ.DIR");

	// Use valid field name
	try {
		msg.getStringField("MESSAGE-TYPE");
		test.check("MESSAGE-TYPE was found", true);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	// Use non-String field name
	try {
		msg.getStringField("CONTENT-VERSION");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Message does not contain StringField with name: CONTENT-VERSION") != std::string::npos);
	}

	// Use NULL field name
	try {
		msg.getStringField(NULL);
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}

	// Use empty field name
	try {
		msg.getStringField("");
		test.check("An exception was expected", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("Field name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_get_field_count(Test& test)
{
	GMSEC_INFO << "Test getFieldCount()";

	MessageFactory msgFactory;

	set_standard_fields(msgFactory);

	Message msg = msgFactory.createMessage("HB");

	test.check("Message has unexpected field count", 13 == msg.getFieldCount());

	msg.setFieldValue("PUB-RATE", "30");

	test.check("Message has unexpected field count", 14 == msg.getFieldCount());

	msg.clearField("PUB-RATE");

	test.check("Message has unexpected field count", 13 == msg.getFieldCount());

	msg.clearFields();

	test.check("Message has unexpected field count", 0 == msg.getFieldCount());
}


void test_copy_fields(Test& test)
{
	GMSEC_INFO << "Test copyFields()";

	MessageFactory msgFactory;

	set_standard_fields(msgFactory);

	Message msg1 = msgFactory.createMessage("HB");
	Message msg2 = msg1;

	msg1.setFieldValue("PUB-RATE", "30");

	test.check("Message 2 should not have PUB-RATE", false == msg2.hasField("PUB-RATE"));

	msg1.copyFields(msg2);

	test.check("Message 2 should have PUB-RATE", true == msg2.hasField("PUB-RATE"));
}


void test_to_xml(Test& test)
{
	GMSEC_INFO << "Test toXML()";

	MessageFactory msgFactory;

	set_standard_fields(msgFactory);

	Message msg1 = msgFactory.createMessage("HB");
	Message msg2 = msg1;

	test.check("Message 1 has NULL XML content", NULL != msg1.toXML());
	test.check("Message 2 has NULL XML content", NULL != msg2.toXML());
	test.check("Messages 1 and 2 do not have same XML content", std::string(msg1.toXML()) == msg2.toXML());

	Message msg3 = msgFactory.fromData(msg1.toXML(), DataType::XML_DATA);
	test.check("Messages 1 and 3 do not have same XML content", std::string(msg1.toXML()) == msg3.toXML());

	Message msg4 = msgFactory.createMessage("REQ.DIR");
	msg4.setConfig( Config("gmsec-show-msg-config=true", DataType::KEY_VALUE_DATA) );
	test.check("Message 4 has NULL XML content", NULL != msg4.toXML());

	Message msg5 = msgFactory.createMessage("RESP.DIR");
	msg5.setConfig( Config("gmsec-show-msg-config=false", DataType::KEY_VALUE_DATA) );
	test.check("Message 5 has NULL XML content", NULL != msg5.toXML());

	try {
		msg1.setConfig( Config("gmsec-show-msg-config=foobar", DataType::KEY_VALUE_DATA) );
		test.check("Expected an exception because of bad value for gmsec-show-msg-config", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), true);
	}
}


void test_to_json(Test& test)
{
	GMSEC_INFO << "Test toJSON()";

	MessageFactory msgFactory;

	set_standard_fields(msgFactory);

	Message msg1 = msgFactory.createMessage("HB");
	Message msg2 = msg1;

	test.check("Message 1 has NULL JSON content", NULL != msg1.toJSON());
	test.check("Message 2 has NULL JSON content", NULL != msg2.toJSON());
	test.check("Messages 1 and 2 do not have same JSON content", std::string(msg1.toJSON()) == msg2.toJSON());

	Message msg3 = msgFactory.fromData(msg1.toJSON(), DataType::JSON_DATA);
	test.check("Messages 1 and 3 do not have same JSON content", std::string(msg1.toJSON()) == msg3.toJSON());

	Message msg4 = msgFactory.createMessage("REQ.DIR");
	msg4.setConfig( Config("gmsec-show-msg-config=true", DataType::KEY_VALUE_DATA) );
	test.check("Message 4 has NULL XML content", NULL != msg4.toJSON());

	Message msg5 = msgFactory.createMessage("RESP.DIR");
	msg5.setConfig( Config("gmsec-show-msg-config=false", DataType::KEY_VALUE_DATA) );
	test.check("Message 5 has NULL XML content", NULL != msg5.toJSON());
}


void test_get_size(Test& test)
{
	GMSEC_INFO << "Test getSize()";

	MessageFactory msgFactory;

	Message msg1 = msgFactory.createMessage("HB");
	Message msg2 = msg1;

	test.check("Message 1 has unexpected size", 0 < msg1.getSize());
	test.check("Message 2 has unexpected size", 0 < msg2.getSize());
	test.check("Messages should have the same size", msg1.getSize() == msg2.getSize());
}


void test_get_field_iterator(Test& test)
{
	MessageFactory msgFactory;

	Message msg = msgFactory.createMessage("HB");

	(void) msg.getFieldIterator();
	test.check("Got the field iterator", true);
}


int test_Message(Test& test)
{
	test.setDescription("Test Message");

	try
	{
		test_constructor(test);
		test_copy_constructor(test);
		test_operator_equal(test);
		test_get_schema_id(test);
		test_get_version(test);
		test_get_schema_level(test);
		test_is_compliant(test);
		test_register_message_validator(test);
		test_set_field_value(test);
		test_set_config(test);
		test_set_subject(test);
		test_get_subject(test);
		test_set_kind(test);
		test_get_kind(test);
		test_add_field(test);
		test_add_fields(test);
		test_clear_fields(test);
		test_clear_field(test);
		test_get_string_value(test);
		test_get_boolean_value(test);
		test_get_i16_value(test);
		test_get_i32_value(test);
		test_get_i64_value(test);
		test_get_u16_value(test);
		test_get_u32_value(test);
		test_get_u64_value(test);
		test_get_f64_value(test);
		test_has_field(test);
		test_get_field(test);
		test_get_field_type(test);
		test_get_binary_field(test);
		test_get_boolean_field(test);
		test_get_char_field(test);
		test_get_f32_field(test);
		test_get_f64_field(test);
		test_get_i8_field(test);
		test_get_i16_field(test);
		test_get_i32_field(test);
		test_get_i64_field(test);
		test_get_u8_field(test);
		test_get_u16_field(test);
		test_get_u32_field(test);
		test_get_u64_field(test);
		test_get_string_field(test);
		test_get_field_count(test);
		test_copy_fields(test);
		test_to_xml(test);
		test_to_json(test);
		test_get_size(test);
		test_get_field_iterator(test);
	}
	catch (GmsecException& e)
	{
		test.require(e.what(), false);
	}

	return 0;
}

TEST_DRIVER(test_Message)
